

#include "DockGNUDebugger.h"
#include <settingsmanager.h>
#include <coremanager.h>
#include <maininterface.h>
#include <qscintillamanager.h>
#include "monkey.h"
//#include "xupmanager.h"

/* debugger */
//

#include <QFileDialog>

#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>


//

void DockGNUDebugger::loadSettings()
{

/*	foreach(GdbAddOn *p , GdbSetting::instance()->getAddOn() )
	{
		foreach(QPointer< class GdbCore> r, Dispatcher->list())
		{
			if(p->name == r->name)
			{
				// enable or disable addOn
				r->setEnabled(p.enable);
			}
		}
	}
*/}

//

void DockGNUDebugger::saveSettings()
{
/*	Settings * s = MonkeyCore::settings();
	s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );

	foreach(QPointer< class GdbCore> r, Dispatcher->list())
	{
		// write "./AddOn/GdbBreakpoint = true"
		if(r->isEnabled())
			s->setValue("AddOn/" + r->name(), true);
		else
			s->setValue("AddOn/" + r->name(), false);
	}
	s->endGroup();
*/}


//

DockGNUDebugger::DockGNUDebugger( QWidget * w )
	: pDockWidget( w ), isGdbStarted(false), isTargetRunning(false)

{

	// set charset for gdb not work
	//	QTextCodec::setCodecForLocale(QTextCodec::codecForName("ISO-8859-1"));

	// closing Monkey
	connect (MonkeyCore::mainWindow(), SIGNAL( aboutToClose()), this , SLOT(onAboutToClose()));

	// create kernel (parser ,process, ...)
	Parser =  GdbParser::instance(this);
	Process = GdbProcess::instance(this);
	Bridge =  GdbBridgeEditor::instance(this);
	Dispatcher = GdbKernelDispatcher::instance(this);

	// create addon
	Breakpoint = new GdbBreakpoint(this);
	Backtrace = new GdbBacktrace(this);


	if( Parser && Process && Bridge && Dispatcher && Breakpoint && Backtrace)
	{
		// addOn to dispatcher
		Dispatcher->add(Breakpoint);
		Dispatcher->add(Backtrace);


		// connections
		connect(Process, SIGNAL( commandReadyRead( const QByteArray & )), this , SLOT( commandReadyRead( const QByteArray & )));
		connect(Process, SIGNAL( started( )), this, SLOT(gdbStarted()));
		connect(Process, SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(gdbFinished( int , QProcess::ExitStatus)));
		connect(Process, SIGNAL( error ( QProcess::ProcessError )), this, SLOT(gdbError(QProcess::ProcessError)));

		// create main container
		mainTabWidget = new QTabWidget(this);
		setWidget(mainTabWidget);

		// create rawLog
		rawLog = new QTextEdit(this);
		mainTabWidget->addTab(rawLog,"Raw Log");

		// connection from parser
		connect(Parser, SIGNAL(done(int, QString)), this , SLOT(onDone(int, QString)));
		connect(Parser, SIGNAL(error(int, QString)), this , SLOT(onError(int, QString)));
		connect(Parser, SIGNAL(info(int, QString)), this , SLOT(onInfo(int, QString)));

		connect(Parser, SIGNAL(targetLoaded(int, QString)), this , SLOT(onTargetLoaded(int, QString)));
		connect(Parser, SIGNAL(targetNoLoaded(int, QString)), this , SLOT(onTargetNoLoaded(int, QString)));
		connect(Parser, SIGNAL(targetExited(int, QString)), this , SLOT(onTargetExited(int, QString)));
		connect(Parser, SIGNAL(targetRunning(int, QString)), this , SLOT(onTargetRunning(int, QString)));
		connect(Parser, SIGNAL(targetStopped(int, QString)), this , SLOT(onTargetStopped(int, QString)));

		connect(Parser, SIGNAL(onInterpreter(const QPointer<BaseInterpreter> & ,const int & , const QString &)), this , 
			SLOT(onInterpreter(const QPointer<BaseInterpreter> & , const int & , const QString &)));

		// breakpoint
		
		connect(Breakpoint, SIGNAL(onToggleBreakpoint(const QString &, const int &, const bool &)), Bridge,
			SLOT(onToggleBreakpoint(const QString &, const int &, const bool &)));

		connect(Backtrace, SIGNAL(onToggleBacktrace(const QString &, const int &)), Bridge,
			SLOT(onToggleBacktrace(const QString &, const int &)));

		// Monkey
		connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString & ) ), Bridge, SLOT( add( const QString & ) ) );
		connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( int ) ), Bridge, SLOT( remove( int ) ) );

		// connection BridgeEditor
		connect(Bridge, SIGNAL(userToggleBreakpoint(const QString &, const int &)), this, SLOT(onUserToggleBreakpoint(const QString &, const int &)));
		connect(Bridge, SIGNAL(requestBreakpoint(const QString &)), Breakpoint , SLOT(onRequestBreakpoint(const QString &)));
		connect(Bridge, SIGNAL(requestBacktrace(const QString &)), Backtrace , SLOT(onRequestBacktrace(const QString &)));

		
		// add permanent Interpreter	
		interpreterStepOver = Parser->addInterpreter("step-over", 
			"n",
			QRegExp("^n$"), 
			QRegExp("\\d+\\s+.*"),
			"^info,interpreter=\"Step-Over\",event=\"End-Stepping-Range\",answerGdb=\"");

		interpreterStepInto = Parser->addInterpreter("step-into", 
			"s", 
			QRegExp("^s$"), 
			QRegExp("\\d+\\s+.*"),
			"^info,interpreter=\"Step-Into\",event=\"End-Stepping-Range\",answerGdb=\"");


		//connect interpreter to function
		Connect = new GdbConnectTemplate<DockGNUDebugger>;
		Connect->add(this, interpreterStepOver, &DockGNUDebugger::onTargetStopped );
		Connect->add(this, interpreterStepInto, &DockGNUDebugger::onTargetStopped );
	
		// find if addOn is enable ?
		foreach(QPointer< class GdbCore> r, Dispatcher->list())
		{
			r->setEnabled( GdbSetting::instance()->getStartUp( r->name() ));
			if(r->isEnabled()) mainTabWidget->addTab( r->widget(),r->name() );
		}

		crlf = pMonkeyStudio::getEol().toLocal8Bit();
	}
}

//

DockGNUDebugger:: ~DockGNUDebugger()
{
	// all class herite QObject, Qt delete for me.
}

//

void DockGNUDebugger::setAction(QHash<QString, QAction *> *a)
{
	mActionList = a;
}

//

void DockGNUDebugger::setEnabledActions(bool b)
{
	foreach(QAction * a, *mActionList)
		a->setEnabled(b);
}

//

void DockGNUDebugger::commandReadyRead(  const QByteArray & d)
{
	rawLog->setTextColor(QColor(0,0,255));
	rawLog->append(d);
	rawLog->setTextColor(QColor(0,0,0));
	Parser->processParsing(d);
}

// Actions

void DockGNUDebugger::onActionLoadTarget()
{
	isTargetRunning = isGdbStarted = false;
	Parser->clearAllCommand();

//	UIXUPManager p* = MonkeyCore::projectsManager()->currentProject();
//	if(MonkeyCore::projectsManager()->currentProject())
//		QString s = MonkeyCore::projectsManager()->currentProject()->projectSettingsValue( "EXECUTE_DEBUG" );
	
	rawLog->append("*** selected target ***");
	// open dialog for select target
	mSelectedTarget = QFileDialog::getOpenFileName(this, tr("Select your target"));
	if( !mSelectedTarget.isEmpty())
	{
		rawLog->append("*** " + mSelectedTarget + " ***");

		Process->setCommand( GdbSetting::instance()->getPathGdb() );

		Parser->setNextCommand("Starting GDB");
		Process->startProcess();
	}
}

//

void DockGNUDebugger::onActionExit()
{
	rawLog->append("*** User exit debug mode ***");

	// remove all marquer in magin
	Bridge->removeAllBreakpoints();
	Bridge->removeBacktrace();

	Parser->setNextCommand("Stop GDB");
	
	if(!isTargetRunning)	
		Process->stopTarget();
	else
		Process->stopProcess();
}

//

void DockGNUDebugger::onActionRestart()
{
	if(Parser->isReady())
	{
		setEnabledActions(false);
		rawLog->append("*** User restart ***");
		Parser->setNextCommand("r");
		Process->sendRawData("r");
	}
}

//

void DockGNUDebugger::onActionContinue()
{
	if(Parser->isReady())
	{
		setEnabledActions(false);
		rawLog->append("*** User continue ***");
		Parser->setNextCommand("c");
		Process->sendRawData("c");
	}
}

//

void DockGNUDebugger::onActionStepOver()
{
	if(Parser->isReady())
	{
		setEnabledActions(false);
	
		rawLog->append("*** User step over ***");
		Parser->setNextCommand("n");
		Process->sendRawData("n");
	}
}

//

void DockGNUDebugger::onActionStepInto()
{
	if(Parser->isReady())
	{
		setEnabledActions(false);

		rawLog->append("*** User step into ***");
		Parser->setNextCommand("s");
		Process->sendRawData("s");
	}
}


// Process

// signal from GdbProcess (QProcess started)
void DockGNUDebugger::gdbStarted()
{

	if(!isGdbStarted) // requiere for Linux, it send two signals started() ??
	{
		rawLog->append("*** Gdb started ***");

		Parser->setReady(true);

//		Parser->setNextCommand("set options for gdb");
//		Process->sendRawData("set breakpoint pending on");

		// gdb is started, now load target under Gdb
		Parser->setNextCommand("file \"" + mSelectedTarget.toLocal8Bit() + "\"");
		Process->sendRawData("file \"" + mSelectedTarget.toLocal8Bit() + "\"");
		
		Dispatcher->gdbStarted();
		isGdbStarted = true;
	}
}

//

void DockGNUDebugger::gdbFinished( int a , QProcess::ExitStatus e)
{
	rawLog->append("*** Gdb finished successfull code : " + QString::number(a) + " ***");

	switch(e)
	{
		case QProcess::NormalExit : rawLog->append("The process exited normally.");
		break;
		case QProcess::Crashed : rawLog->append("The process crashed.");
	}


	Dispatcher->gdbFinished();

	setEnabledActions(false);
	mActionList->value("aLoadTarget")->setEnabled(true);

	isGdbStarted = false;
}


//

void DockGNUDebugger::gdbError( QProcess::ProcessError e)
{
	rawLog->append("*** Critical gdb Erreur code : " + QString::number(e) + " ***");
	switch(e)
	{
		case QProcess::FailedToStart : rawLog->append("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
		break;
		case QProcess::Crashed : rawLog->append("The process crashed some time after starting successfully.");
		break;
		case  QProcess::Timedout : rawLog->append("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
		break;
		case QProcess::WriteError : rawLog->append("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
		break;
		case QProcess::ReadError : rawLog->append("An error occurred when attempting to read from the process. For example, the process may not be running.");
		break;
		case QProcess::UnknownError : rawLog->append("An unknown error occurred. This is the default return value of error().");
	}

	Dispatcher->gdbError();

	setEnabledActions(false);
	mActionList->value("aLoadTarget")->setEnabled(true);

	isGdbStarted = false;
}

// Target

void DockGNUDebugger::onTargetLoaded(int id, QString st)
{
	rawLog->append("*** Target loaded success full ***");

	Dispatcher->targetLoaded(id, st);

	setEnabledActions(false);
	mActionList->value("aExitGdb")->setEnabled( true );
	mActionList->value("aRestart")->setEnabled( true );
}

//

void DockGNUDebugger::onTargetNoLoaded(int id, QString st)
{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target no Loaded Stop Gdb ***");


	Process->stopProcess();

	Dispatcher->targetNoLoaded(id, st);
}

//

void DockGNUDebugger::onTargetRunning(int id, QString st)
{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target running success full ***");

	setEnabledActions(false);
	// i can't stop target because gdb have no handle
	// but i can stop gdb directly (icon)
	mActionList->value("aExitGdb")->setEnabled( true );

	Dispatcher->targetRunning(id, st);
	isTargetRunning = true;
}

//

void DockGNUDebugger::onTargetStopped(int id, QString st)

{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target stopped ***");


	Dispatcher->targetStopped(id, st);

	setEnabledActions( true );
	mActionList->value("aLoadTarget")->setEnabled( false );

	isTargetRunning = false;
}

//

void DockGNUDebugger::onTargetExited(int id, QString st)
{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target exit success full ***");

	Dispatcher->targetExited(id, st);
	
	setEnabledActions( false );
	mActionList->value("aExitGdb")->setEnabled( true );
	mActionList->value("aRestart")->setEnabled( true );

	isTargetRunning = false;
}

// Parser

void DockGNUDebugger::onError(int id, QString st)
{
	rawLog->append("*** Error from parser ***");

	rawLog->setTextColor(QColor(255,0,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));

	Dispatcher->error(id, st);
}

//

void DockGNUDebugger::onDone(int id, QString st)
{
	rawLog->setTextColor(QColor(0,255,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));
	
	Dispatcher->done(id, st);
}

//

void DockGNUDebugger::onInfo(int id, QString st)
{
	rawLog->setTextColor(QColor(255,255,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));

	Dispatcher->info(id, st);
}

// Interpreter for step over / into

void DockGNUDebugger::onInterpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	// connect step into / over
	// call onTargetStopped
	Connect->call( i, id, s);
}

// from editor margin clicked

void DockGNUDebugger::onUserToggleBreakpoint(const QString & fileName, const int & line)
{
	rawLog->append("** user toggle breakpoint *** " + fileName + " " + QString::number(line + 1));
	Breakpoint->toggleBreakpoint(fileName, line + 1);
}

// close monkey

void DockGNUDebugger::onAboutToClose()
{
}
