
#include "DockGNUDebugger.h"
#include "pFileManager.h"
#include "ProjectItem.h"
#include "MonkeyCore.h"
#include "pAbstractChild.h"
#include "pEditor.h"

/* debugger */
//

#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>

#include "pMonkeyStudio.h"

DockGNUDebugger::DockGNUDebugger( QWidget* w )
	: pDockWidget( w )
{
	// create tabwidget
/*	// connections
	connect( lwBuildSteps, SIGNAL( itemPressed( QListWidgetItem* ) ), this, SLOT( lwBuildSteps_itemPressed( QListWidgetItem* ) ) );
	connect( leRawCommand, SIGNAL( returnPressed() ), this, SLOT( leRawCommand_returnPressed() ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandError( const pCommand&, QProcess::ProcessError ) ), this, SLOT( commandError( const pCommand&, QProcess::ProcessError ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ), this, SLOT( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandReadyRead( const pCommand&, const QByteArray& ) ), this, SLOT( commandReadyRead( const pCommand&, const QByteArray& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandStarted( const pCommand& ) ), this, SLOT( commandStarted( const pCommand& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandStateChanged( const pCommand&, QProcess::ProcessState ) ), this, SLOT( commandStateChanged( const pCommand&, QProcess::ProcessState ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( commandSkipped( const pCommand& ) ), this, SLOT( commandSkipped( const pCommand& ) ) );
	connect( MonkeyCore::consoleManager(), SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SLOT( appendStep( const pConsoleManager::Step& ) ) );
*/
	// create plugin Manager
	mw = new QMainWindow(w);
	setWidget(mw);
	
	setMinimumHeight( 120 );

	// create parser
	Parser = new GdbParser(this);

	kernelDispatcher = new GdbKernelDispatcher(Parser);
	mw->addDockWidget(Qt::RightDockWidgetArea, kernelDispatcher->getContainer());

	// create backtrace
	backtraceGdb = new GdbBackTrace(Parser);
	mw->tabifyDockWidget(kernelDispatcher->getContainer(),backtraceGdb->getContainer());

	// create registers	
	registersGdb = new GdbRegisters(Parser);
	mw->tabifyDockWidget(backtraceGdb->getContainer(),registersGdb->getContainer());

	// create watch
	watchGdb = new GdbWatch(Parser);
	mw->tabifyDockWidget(registersGdb->getContainer(),watchGdb->getContainer());

	// create bridge
	bridgeEditor = new GdbBridgeEditor(Parser);
	mw->tabifyDockWidget(watchGdb->getContainer(), bridgeEditor->getContainer());
//	mw->tabifyDockWidget(registersGdb->getContainer(), bridgeEditor->getContainer());

	// create breakpoint
	breakpointGdb = new  GdbBreakPoint(Parser);
	mw->tabifyDockWidget(bridgeEditor->getContainer(),breakpointGdb->getContainer());

	// create answer
	answerGdb = new  GdbAnswer(Parser);
	mw->tabifyDockWidget(breakpointGdb->getContainer(),answerGdb->getContainer());

	// create control
	controlGdb = new  GdbControl(Parser);
	mw->setCentralWidget(controlGdb->getContainer());

	// add plugin under manager
	kernelDispatcher->add(breakpointGdb);
	kernelDispatcher->add(watchGdb);
	kernelDispatcher->add(registersGdb);
	kernelDispatcher->add(backtraceGdb);
	kernelDispatcher->add(controlGdb);
	kernelDispatcher->add(bridgeEditor);
	kernelDispatcher->add(answerGdb);


	// generic
	connect(Parser, SIGNAL(done(int, QString)), this , SLOT(onDone(int, QString)));
	connect(Parser, SIGNAL(error(int, QString)), this , SLOT(onError(int, QString)));
	connect(Parser, SIGNAL(info(int, QString)), this , SLOT(onInfo(int, QString)));

	// target
	connect(Parser, SIGNAL(targetLoaded(int, QString)), this , SLOT(onTargetLoaded(int, QString)));
	connect(Parser, SIGNAL(targetExited(int, QString)), this , SLOT(onTargetExited(int, QString)));
	connect(Parser, SIGNAL(targetRunning(int, QString)), this , SLOT(onTargetRunning(int, QString)));
	connect(Parser, SIGNAL(targetStopped(int, QString)), this , SLOT(onTargetStopped(int, QString)));

	
	// connect brigde to Qsci
	connect(bridgeEditor, SIGNAL(breakpoint(QByteArray , int, bool)), this , SLOT(onBreakpoint(QByteArray, int, bool)));
	connect(bridgeEditor, SIGNAL(backtrace(QByteArray , int)), this , SLOT(onBacktrace(QByteArray, int)));
	connect(bridgeEditor, SIGNAL(breakpointMoved(QByteArray , int, int)), this , SLOT(onBreakpointMoved(QByteArray, int, int)));
	connect(bridgeEditor, SIGNAL(breakpointConditionnaled(QByteArray , int,bool)), this , SLOT(onBreakpointConditionnaled(QByteArray, int, bool)));
	connect(bridgeEditor, SIGNAL(breakpointEnabled(QByteArray , int,bool)), this , SLOT(onBreakpointEnabled(QByteArray, int, bool)));


	pConsole =  MonkeyCore::consoleManager();
 
	connect(pConsole, SIGNAL( commandStarted( const pCommand& )), this, SLOT(gdbStarted( const pCommand& )));
	connect(pConsole, SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus )), this, SLOT( gdbFinished( const pCommand&, int, QProcess::ExitStatus )));
	connect(pConsole, SIGNAL( commandReadyRead( const pCommand&, const QByteArray& )), this, SLOT( commandReadyRead( const pCommand&, const QByteArray& )));

	// plugin send
	connect(kernelDispatcher, SIGNAL(sendRawData(GdbCore*, QByteArray)) ,this , SLOT(onSendRawData(GdbCore *, QByteArray) ));

	// start gdb
	Cmd = new pCommand( "gdb", "gdb", QString::null,true, QStringList() << "gdb", QString::null,  false );
	pConsole->addCommand(*Cmd);

	// CRLF
	#ifdef Q_OS_WIN 
		crlf = "\r\n";
	#endif
	#ifdef Q_OS_MAC 
		crlf = "\n";
	#endif
	#ifdef Q_OS_UNIX
		crlf = "\n";
	#endif

	// when a file is opened
	connect (MonkeyCore::fileManager(), SIGNAL(fileOpened( const QString& )), this, SLOT(onFileOpened( const QString& )));
}

// EDITOR
// a new file is open under editor
void DockGNUDebugger::onFileOpened( const QString& file )
{
//	((QTextEdit * )(bridge->widget() ))->append(file);

	if(MonkeyCore::fileManager()->currentChild())
 	{
		// get new file
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			// set margin Qsci sensitiv
			e->setMarginSensitivity(0,true);
			// connect margin clicked
			connect (e, SIGNAL(marginClicked (int, int , Qt::KeyboardModifiers )), this, SLOT(onMarginClicked(int, int,  Qt::KeyboardModifiers)));
	
			// set somes icons on under editor
			iconbreakenable = e->markerDefine (QPixmap(":/icons/break_enable.png"));
			iconbreakdisable = e->markerDefine (QPixmap(":/icons/break_disable.png"));
			iconbacktrace = e->markerDefine (QPixmap(":/icons/play.png"));
			iconconditionenable = e->markerDefine (QPixmap(":/icons/break_conditional_enable.png"));
			iconconditiondisable = e->markerDefine (QPixmap(":/icons/break_conditional_disable.png"));

			// debug
			// get name of file
//			QString name = MonkeyCore::fileManager()->currentChildFile();
//			((QTextEdit * )(bridge->widget() ))->append("connection" + name);
		}
	}
}

// user clicked margin
void DockGNUDebugger::onMarginClicked(int marginIndex, int line , Qt::KeyboardModifiers state)
{
//	((QTextEdit * )(bridge->widget() ))->append("clicked " + QString::number(line));

	// get the name of file
	QString fileName = MonkeyCore::fileManager()->currentChildFile();

	QGdbMessageCore m;
	m.msg = "^done,interpreter=\"editor\",currentCmd=\"none\",event=\"CTRL+B\",fullname=\"" + fileName.toLocal8Bit() + "\",line=\"" + QByteArray::number(line+1) + "\"";
	kernelDispatcher->process(m);

//	kernelDispatcher->process(-1, "^done,interpreter=\"editor\",currentCmd=\"none\",event=\"CTRL+B\",fullname=\"" + name.toLocal8Bit() + "\",line=\"" + QString::number(line + 1).toLocal8Bit() + "\"");
}

// show icon under file
void DockGNUDebugger::onBreakpoint(QByteArray filename, int line, bool b)
{
	if(MonkeyCore::fileManager()->currentChild() )
	{
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			if(b)
			{
				e->markerAdd (line-1, iconbreakenable);
			}
			else
			{
				e->markerDelete(line-1, iconbreakenable);
			}
		}
	}
}
//
// move backtrace under editor
void DockGNUDebugger::onBacktrace(QByteArray filename, int line)
{
	if(MonkeyCore::fileManager()->currentChild() )
	{
		// get the current file
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
	
		if(e)
		{
			// first delete back trace
			e->markerDeleteAll(iconbacktrace);

			// open file (if is noit same)
			MonkeyCore::workspace()->goToLine(filename, QPoint(1,line), true);

			// now the current file is
			e = MonkeyCore::fileManager()->currentChild()->currentEditor();

			// first delete back trace
			e->markerDeleteAll(iconbacktrace);
			e->markerAdd (line-1, iconbacktrace);

			// debug
//			((QTextEdit * )(bridge->widget() ))->append("open " + filename);
		}
	}
}
//
// some time gdb move breakpoint under next line 
void DockGNUDebugger::onBreakpointMoved(QByteArray filename, int beforLine, int afterLine)
{
	if(MonkeyCore::fileManager()->currentChild() )
	{
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			e->markerDelete (beforLine-1, iconbreakenable);
	
			e->markerAdd(afterLine-1, iconbreakenable);
		}
	}
}
//
// show breakpoint enable or not
void DockGNUDebugger::onBreakpointEnabled(QByteArray filename, int line, bool b)
{
	if(MonkeyCore::fileManager()->currentChild() )
	{
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			if(b)
			{
				e->markerDelete (line-1, iconbreakdisable);
				e->markerAdd(line-1, iconbreakenable);
			}
			else
			{
				e->markerDelete (line-1, iconbreakenable);
				e->markerAdd(line-1, iconbreakdisable);
			}
		}
	}
}
//
void DockGNUDebugger::onBreakpointConditionnaled(QByteArray filename, int line, bool b)
{
	// remove breakpoint
//	editor->addBreakpoint("",/*filename,*/ line, false);
//	if(b) editor->addBreakpoint("./src/resources/break_conditionnal_enable.png",/*filename,*/ line, true);
//		else editor->addBreakpoint("./src/resources/break_enable.png",/*filename,*/ line, true);
}



// Console frome pConsoleManager
void DockGNUDebugger::gdbStarted( const pCommand& c)
{
	kernelDispatcher->gdbStarted();
//	((QTextEdit * )(bridge->widget() ))->append("GDB started");

//	onSendRawData(NULL,"file c:/dev/debugger/debugger.exe");
}


void DockGNUDebugger::gdbFinished( const pCommand& c, int a , QProcess::ExitStatus )
{
	kernelDispatcher->gdbFinished();
//	((QTextEdit * )(bridge->widget() ))->append("GDB finished");
}

void DockGNUDebugger::commandReadyRead( const pCommand& c , const QByteArray& d)
{
	Parser->processParsing(d);
}


void DockGNUDebugger::onSendRawData(GdbCore *plug, QByteArray data)
{
	Parser->setLastCommand(data);
	pConsole->sendRawData(data + crlf);
}


// From parser
void DockGNUDebugger::onDone(int id, QString st)
{
	QGdbMessageCore m;
	m.msg = st.toLocal8Bit();
	m.id = id;
	kernelDispatcher->process(m);
//	kernelDispatcher->process(id,st.toLocal8Bit());
// debug
//	((QTextEdit * )(bridge->widget() ))->setTextColor(Qt::black);
//	((QTextEdit * )(bridge->widget() ))->append(st.toLocal8Bit() + "  id : " + QString::number(id).toLocal8Bit());
}

void DockGNUDebugger::onError(int id, QString st)
{
	QGdbMessageCore m;
	m.msg = st.toLocal8Bit();
	m.id = id;
	kernelDispatcher->process(m);
//	kernelDispatcher->process(id, st.toLocal8Bit());
// debug
//	((QTextEdit * )(bridge->widget() ))->setTextColor(Qt::red);
//	((QTextEdit * )(bridge->widget() ))->append( st.toLocal8Bit() + "  id : " + QString::number(id).toLocal8Bit());
}

void DockGNUDebugger::onInfo(int id, QString st)
{
	QGdbMessageCore m;
	m.msg = st.toLocal8Bit();
	m.id = id;
	kernelDispatcher->process(m);
//	kernelDispatcher->process( id,st.toLocal8Bit() );
// debug
//	((QTextEdit * )(bridge->widget() ))->setTextColor(Qt::blue);
//	((QTextEdit * )(bridge->widget() ))->append( st.toLocal8Bit() + "  id : " + QString::number(id).toLocal8Bit());
}

// target
void DockGNUDebugger::onTargetLoaded(int id, QString st)
{
	kernelDispatcher->targetLoaded();
// debug
//	((QTextEdit * )(bridge->widget() ))->setTextColor(Qt::blue);
//	((QTextEdit * )(bridge->widget() ))->append( st.toLocal8Bit() + "  id : " + QString::number(id).toLocal8Bit());
}

void DockGNUDebugger::onTargetExited(int id, QString st)
{
	kernelDispatcher->targetExited();
// debug
//	((QTextEdit * )(bridge->widget() ))->setTextColor(Qt::blue);
//	((QTextEdit * )(bridge->widget() ))->append( st.toLocal8Bit() + "  id : " + QString::number(id).toLocal8Bit());
}

void DockGNUDebugger::onTargetRunning(int id, QString st)
{
	kernelDispatcher->targetRunning();
// debug
//	((QTextEdit * )(bridge->widget() ))->setTextColor(Qt::blue);
//	((QTextEdit * )(bridge->widget() ))->append( st.toLocal8Bit() + "  id : " + QString::number(id).toLocal8Bit());
}

void DockGNUDebugger::onTargetStopped(int id, QString st)
{
	kernelDispatcher->targetStopped();
// debug
	//	((QTextEdit * )(bridge->widget() ))->setTextColor(Qt::blue);
	//((QTextEdit * )(bridge->widget() ))->append(st.toLocal8Bit() + "  id : " + QString::number(id).toLocal8Bit());
}


DockGNUDebugger:: ~DockGNUDebugger()
{
	QMessageBox::warning(NULL, "fermer Gdb", "test", QMessageBox::Ok ); 

	pConsole->stopCurrentCommand(true);
}




