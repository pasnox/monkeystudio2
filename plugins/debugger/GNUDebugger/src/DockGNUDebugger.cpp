/*
<PasNox> il te faut creer des action
<PasNox> genre
<PasNox> menubar->action( "mDebugger/aLoadTarget", caption, icon, shortcut );
<PasNox> ca ca creer uen actino dans un menu
<PasNox> ensuite
<PasNox> docktoolbar( type )->addAction( menubar->action( "mDebugger/aLoadTarget" ) );
<PasNox> et hop ca ajoute ton action dans la toolbar
<xiantia> ok nickel
*/

#include "DockGNUDebugger.h"

#include <coremanager.h>
#include <maininterface.h>
#include <qscintillamanager.h>

/* debugger */
//

#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>

QSize pixmapSize(16,16);

// for see all AddOn, else comment this line
#define DEBUG_WIDGET

DockGNUDebugger::DockGNUDebugger( QWidget* w )
	: pDockWidget( w )
{

	// closing Monkey
	connect (MonkeyCore::mainWindow(), SIGNAL( aboutToClose()), this , SLOT(onAboutToClose()));

	// create MainWindow
//	mw = new QMainWindow(w);
//	setWidget(mw);
//	QSize s = mw->size();
	
//	setMinimumHeight( 120 );

	// create parser
	Parser = new GdbParser(this);

	// create dispathcer 
	kernelDispatcher = new GdbKernelDispatcher(Parser);

	// create backtrace
	backtraceGdb = new GdbBackTrace(Parser);

	// create registers	
	registersGdb = new GdbRegisters(Parser);
//	registersGdb->getContainer()->setFixedWidth (s.width()*2-100);
//	registersGdb->getContainer()->setMinimumWidth (s.width()*2-100);

	// create watch
	watchGdb = new GdbWatch(Parser);

	// create bridge
	bridgeEditor = new GdbBridgeEditor(Parser);

	// create breakpoint
	breakpointGdb = new  GdbBreakPoint(Parser);

	// create answer
	answerGdb = new  GdbAnswer(Parser);

	// create control
	controlGdb = new  GdbControl(Parser);

	mainTabWidget = new QTabWidget();
	setWidget(mainTabWidget);

	mainTabWidget->addTab(kernelDispatcher->widget(),kernelDispatcher->name());
	mainTabWidget->addTab(registersGdb->widget(),registersGdb->name());
	mainTabWidget->addTab(backtraceGdb->widget(),backtraceGdb->name());
	mainTabWidget->addTab(watchGdb->widget(),watchGdb->name());
	mainTabWidget->addTab(bridgeEditor->widget(),bridgeEditor->name());
	mainTabWidget->addTab(breakpointGdb->widget(),breakpointGdb->name());
	mainTabWidget->addTab(answerGdb->widget(),answerGdb->name());


/*	#ifdef DEBUG_WIDGET
		mw->addDockWidget(Qt::RightDockWidgetArea, kernelDispatcher->getContainer());
		mw->tabifyDockWidget(kernelDispatcher->getContainer(),registersGdb->getContainer());
		mw->tabifyDockWidget(registersGdb->getContainer(),backtraceGdb->getContainer());
		mw->tabifyDockWidget(backtraceGdb->getContainer(),watchGdb->getContainer());
		mw->tabifyDockWidget(watchGdb->getContainer(), bridgeEditor->getContainer());
		mw->tabifyDockWidget(bridgeEditor->getContainer(),breakpointGdb->getContainer());
		mw->tabifyDockWidget(breakpointGdb->getContainer(),answerGdb->getContainer());
	#endif
	
	#ifndef DEBUG_WIDGET
		mw->addDockWidget(Qt::RightDockWidgetArea, registersGdb->getContainer());
		mw->tabifyDockWidget(registersGdb->getContainer(),watchGdb->getContainer());
		mw->tabifyDockWidget(watchGdb->getContainer(), breakpointGdb->getContainer());
	#endif

	mw->setCentralWidget(controlGdb->getContainer());
*/	
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
	connect(Parser, SIGNAL(targetNoLoaded(int, QString)), this , SLOT(onTargetNoLoaded(int, QString)));
	connect(Parser, SIGNAL(targetExited(int, QString)), this , SLOT(onTargetExited(int, QString)));
	connect(Parser, SIGNAL(targetRunning(int, QString)), this , SLOT(onTargetRunning(int, QString)));
	connect(Parser, SIGNAL(targetStopped(int, QString)), this , SLOT(onTargetStopped(int, QString)));

	// connect brigde to Qsci
	connect(bridgeEditor, SIGNAL(breakpoint(QByteArray , int, QByteArray, QByteArray,bool)), this , SLOT(onBreakpoint(QByteArray, int, QByteArray, QByteArray, bool)));
	connect(bridgeEditor, SIGNAL(backtrace(QByteArray , int)), this , SLOT(onBacktrace(QByteArray, int)));
	connect(bridgeEditor, SIGNAL(breakpointMoved(QByteArray , int, int)), this , SLOT(onBreakpointMoved(QByteArray, int, int)));
	connect(bridgeEditor, SIGNAL(breakpointConditionnaled(QByteArray , int,bool)), this , SLOT(onBreakpointConditionnaled(QByteArray, int, bool)));
	connect(bridgeEditor, SIGNAL(breakpointEnabled(QByteArray , int,bool)), this , SLOT(onBreakpointEnabled(QByteArray, int, bool)));
	connect(bridgeEditor, SIGNAL(gotoBreakpoint(QByteArray , int)), this , SLOT(onGotoBreakpoint(QByteArray, int)));

	// create QProcess for GDb
	pConsole =  new GdbProcess(this);
	
	connect(pConsole, SIGNAL( commandStarted( const pCommand& )), this, SLOT(gdbStarted( const pCommand& )));
	connect(pConsole, SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus )), this, SLOT( gdbFinished( const pCommand&, int, QProcess::ExitStatus )));
	connect(pConsole, SIGNAL( commandReadyRead( const pCommand&, const QByteArray& )), this, SLOT( commandReadyRead( const pCommand&, const QByteArray& )));
	connect(pConsole, SIGNAL(commandError( const pCommand&,QProcess::ProcessError)), this, SLOT(gdbError(const pCommand&,QProcess::ProcessError)));

	// plugin send a command to Gdb
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
	connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString& ) ), this, SLOT( onFileOpened( const QString& ) ) );
	// when file is closed
//	connect( MonkeyCore::workspace(), SIGNAL( fileClosed( const QString& ) ), this, SLOT( onFileClosed( const QString& ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( int ) ), this, SLOT( onFileClosed( int ) ) );

	// when user want start or exit gdb
	connect( controlGdb, SIGNAL(wantExit()), this , SLOT(onWantExit()));
	connect( controlGdb, SIGNAL(wantStart(QString)), this , SLOT(onWantStart(QString)));
}
//

GdbControl * DockGNUDebugger::gdbControl()
{
	return controlGdb;
}

void DockGNUDebugger::onWantStart(QString file)
{
//	QMessageBox::warning(NULL,tr("Sorry ..."), "on want start");
	targetName = file;
	// start gdb
	pConsole->executeProcess();
}
//
void DockGNUDebugger::onWantExit()
{
//		QMessageBox::warning(NULL,tr("Sorry ..."), "on want exit");

	// stop gdb
	pConsole->stopCurrentCommand(true);
	
	// first delete back trace under all editor
	for(int i=0; i<editor.pointeur.count(); i++)
	{
		editor.pointeur.at(i)->markerDeleteAll(pEditor::mdPlay);
		editor.pointeur.at(i)->markerDeleteAll(pEditor::mdEnabledBreak);
	}
//	hide();
}

// EDITOR
// a new file is open under editor
void DockGNUDebugger::onFileOpened( const QString& file )
{
//		QMessageBox::warning(NULL,tr("Sorry ..."), "file opened");

	if(MonkeyCore::fileManager()->currentChild())
	{
		// get new file
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			// get name of file
			QString name = MonkeyCore::fileManager()->currentChildFile();
			// save opened editor
			editor.fileName << name;
			editor.pointeur << e;
			// set margin Qsci sensitive
			e->setMarginSensitivity(0,true);
			// connect margin clicked
			connect (e, SIGNAL(marginClicked (int, int , Qt::KeyboardModifiers )), this, SLOT(onMarginClicked(int, int,  Qt::KeyboardModifiers)));
			// get if this editor haved breakpoint befor close / re-opening
			QGdbMessageCore m={"^notify,interpreter=\"editor\",currentCmd=\"none\",event=\"requested-breakpoint\",fileName=\"" + name.toLocal8Bit() + "\"",-1};
			kernelDispatcher->process(m);
			// restor backtrace
			if(name.toLocal8Bit() == currentBacktraceFile)
				onBacktrace(name.toLocal8Bit() , currentBacktraceLine);
		}
	}
}
//
void DockGNUDebugger::onFileClosed( int r )
{
//	QMessageBox::warning(NULL,tr("Sorry ..."), "file closed");

	// delete editor
	editor.pointeur.removeAt(r);
	editor.fileName.removeAt(r);
}
//
pEditor *DockGNUDebugger::findFile(QByteArray file)
{
	for(int i=0; i< editor.pointeur.count(); i++)
	{
		QString fullName = editor.fileName.at(i);
		if(QFileInfo(fullName).fileName().toLocal8Bit() == file) return editor.pointeur.at(i);
	}
	return NULL;
}
// user clicked on the margin
void DockGNUDebugger::onMarginClicked(int marginIndex, int line , Qt::KeyboardModifiers state)
{
	// get the name of file
	QString fileName = MonkeyCore::fileManager()->currentChildFile();

	QGdbMessageCore m;
	m.msg = "^done,interpreter=\"editor\",currentCmd=\"none\",event=\"CTRL+B\",fullname=\"" + fileName.toLocal8Bit() + "\",line=\"" + QByteArray::number(line+1) + "\"";
	kernelDispatcher->process(m);
}
// show icon under file
void DockGNUDebugger::onBreakpoint(QByteArray filename, int line, QByteArray type,QByteArray enable, bool b)
{
//	QMessageBox::warning(NULL,tr("Sorry ..."), "onbreapoint");

	if(MonkeyCore::fileManager()->currentChild() )
	{
		pEditor *e = MonkeyCore::fileManager()->currentChild()->currentEditor();
		if(e)
		{
			if(b)
			{
				if(type == "unconditional-breakpoint")
				{
					if(enable == "true")
						e->markerAdd (line-1, pEditor::mdEnabledBreak);	
					else
						e->markerAdd (line-1, pEditor::mdDisabledBreak);
				}
				else ;
			}
			else
			{
				if(type == "unconditional-breakpoint")
				{
					if(enable == "true")
						e->markerDelete (line-1, pEditor::mdEnabledBreak);	
					else
						e->markerDelete (line-1, pEditor::mdDisabledBreak);
				}
				else ;
			}
		}
	}
}
//
// move backtrace under editor
void DockGNUDebugger::onBacktrace(QByteArray filename, int line)
{
//		QMessageBox::warning(NULL,tr("Sorry ..."), filename);

	// bug fix if no file is open
	if(!filename.isEmpty())
	{
		// save current file for restor after closed
		currentBacktraceFile = filename;
		currentBacktraceLine = line;

		// first delete back trace under all editor
		for(int i=0; i<editor.pointeur.count(); i++)
			editor.pointeur.at(i)->markerDeleteAll(pEditor::mdPlay);

		// open file (if is not same)
		if(MonkeyCore::workspace())
			MonkeyCore::workspace()->goToLine(filename, QPoint(1,line), true);

		// now the current file is
		pEditor * e = MonkeyCore::fileManager()->currentChild()->currentEditor();

		if(e) e->markerAdd (line-1, pEditor::mdPlay);
	}

}
//
// some time gdb move breakpoint under next line 
// some BUG if this line have already one breakpoint
void DockGNUDebugger::onBreakpointMoved(QByteArray filename, int beforLine, int afterLine)
{
//		QMessageBox::warning(NULL,tr("Sorry ..."), "breapoint moved");

	pEditor *e = findFile(filename);
	if(e)
	{
		e->markerDelete (beforLine-1, pEditor::mdEnabledBreak);
		e->markerAdd(afterLine-1, pEditor::mdEnabledBreak);
	}
}
//
// show breakpoint enable or not
void DockGNUDebugger::onBreakpointEnabled(QByteArray filename, int line, bool b)
{
//	QMessageBox::warning(NULL,tr("Sorry ..."), "breapoint enable");
	pEditor *e = findFile(filename);
	if(e)
	{
		if(b) // toggle
		{
			e->markerDelete (line-1, pEditor::mdDisabledBreak);
			e->markerAdd(line-1, pEditor::mdEnabledBreak);
		}
		else
		{
			e->markerDelete (line-1, pEditor::mdEnabledBreak);
			e->markerAdd(line-1, pEditor::mdDisabledBreak);
		}
	}
}
//
// goto line at file and at line contain this breakpoint
// NOT WORK because i dion't have a fullName
void DockGNUDebugger::onGotoBreakpoint(QByteArray filename, int line)
{
//	QMessageBox::warning(NULL,tr("Sorry ..."), "breapoint");
	if(MonkeyCore::workspace() )
	{
		MonkeyCore::workspace()->goToLine(filename, QPoint(1,line), true);
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


// Console from GdbProcess
void DockGNUDebugger::gdbStarted( const pCommand& c)
{
	kernelDispatcher->gdbStarted();
	// open target
	onSendRawData(NULL,"file " + targetName.toLocal8Bit());
}
//
void DockGNUDebugger::gdbFinished( const pCommand& c, int a , QProcess::ExitStatus )
{
	kernelDispatcher->gdbFinished();
}
//
void DockGNUDebugger::gdbError(const pCommand& c, QProcess::ProcessError e)
{
	switch(e)
	{
		case QProcess::FailedToStart: QMessageBox::warning(NULL, tr("Error"), tr("I can't start Gdb, be sure you have gdb in your path."));
		break;
		case QProcess::Crashed : QMessageBox::warning(NULL, tr("Error"), tr("Gdb crashed"));
		break;
		case QProcess::UnknownError : QMessageBox::warning(NULL, tr("Error"), tr("Gdb haved generate an unknow error"));
		break; 
		default : ;
	}
	onWantExit();
}
//
void DockGNUDebugger::commandReadyRead( const pCommand& c , const QByteArray& d)
{
	Parser->processParsing(d);
}
//
void DockGNUDebugger::onSendRawData(GdbCore *plug, QByteArray data)
{
	Parser->setLastCommand(data);
	pConsole->sendRawData(data + crlf);
}

// From parser
void DockGNUDebugger::onDone(int id, QString st)
{
	// get the Gdb Version , NOT WORK
	
/*	QMessageBox::warning(NULL,"version", st);
	QRegExp r("GNU gdb (\\d+).(\\d+).(\\d+).(\\d+)" + crlf);
	if(r.exactMatch(st))
	{
		QStringList list = r.capturedTexts();

		QString version = list.at(2);
		QMessageBox::warning(NULL,"version", list.at(1));
		
		setWindowTitle(version);
	}
*/
	QGdbMessageCore m={st.toLocal8Bit(), id};
	kernelDispatcher->process(m);
}
//
void DockGNUDebugger::onError(int id, QString st)
{
	QGdbMessageCore m={st.toLocal8Bit(), id};
	kernelDispatcher->process(m);
}
//
void DockGNUDebugger::onInfo(int id, QString st)
{
	QGdbMessageCore m={st.toLocal8Bit(), id};
	kernelDispatcher->process(m);
}
//
// target
void DockGNUDebugger::onTargetLoaded(int id, QString st)
{
	kernelDispatcher->targetLoaded();
}

void DockGNUDebugger::onTargetNoLoaded(int id, QString st)
{
	id=id;
	onWantExit();
	QMessageBox::warning(NULL,tr("Sorry ..."), st);
}
//
void DockGNUDebugger::onTargetExited(int id, QString st)
{
//	QMessageBox::warning(NULL,tr("exited"), st);
	kernelDispatcher->targetExited();
}
//
void DockGNUDebugger::onTargetRunning(int id, QString st)
{
	kernelDispatcher->targetRunning();
}
//
void DockGNUDebugger::onTargetStopped(int id, QString st)
{
	kernelDispatcher->targetStopped();
}
//
void DockGNUDebugger::onAboutToClose()
{
	// stop all thread
	kernelDispatcher->stopAll();
	// stop dispatcher
	kernelDispatcher->setStopProcess();
	// stop gdb
	pConsole->stopCurrentCommand(true);

	delete Parser;
	delete pConsole;
	delete Cmd;
	delete kernelDispatcher;
}
//
DockGNUDebugger:: ~DockGNUDebugger()
{
}




