

#include "DockGNUDebugger.h"
#include <settingsmanager.h>
#include <coremanager.h>
#include <maininterface.h>
#include <qscintillamanager.h>
#include "monkey.h"

/* debugger */
//

#include <QFileDialog>

#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>


//

void DockGNUDebugger::loadSettings()
{
}

//

void DockGNUDebugger::saveSettings()
{
}


//

DockGNUDebugger::DockGNUDebugger( QWidget * w )
	: pDockWidget( w ), isGdbStarted(false), isTargetRunning(false)

{

	// create main container
	mainTabWidget = new QTabWidget();
	setWidget(mainTabWidget);
	disableStep = false;

	// create rawLog
	rawLog = new QTextEdit(mainTabWidget);
	mainTabWidget->addTab(rawLog,"Raw Log");

	// create kernel (parser ,process, ...)
	Parser =  GdbParser::instance(this);
	Process = GdbProcess::instance(this);
	Bridge =  GdbBridgeEditor::instance(this);
	Dispatcher = GdbKernelDispatcher::instance(this); // requiere Parser

	// create addon
	Breakpoint = new GdbBreakpoint(this);
	Backtrace = new GdbBacktrace(this);
	Register = new GdbRegister(this);
	Watch = new GdbWatch(this);
	Cli = new GdbCli(this);
	ToolTip = new GdbToolTip(this);
	Script = new GdbScript(this);

	// addOn to dispatcher
	Dispatcher->add(Breakpoint);
	Dispatcher->add(Backtrace);
	Dispatcher->add(Register);
	Dispatcher->add(Watch);
	Dispatcher->add(Cli);
	Dispatcher->add(ToolTip);
	Dispatcher->add(Script);


	// connections
	connect(Process, SIGNAL( commandReadyRead( const QString& )), this , SLOT( commandReadyRead( const QString& )));
	connect(Process, SIGNAL( started( )), this, SLOT(gdbStarted()));
	connect(Process, SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(gdbFinished( int , QProcess::ExitStatus)));
	connect(Process, SIGNAL( error ( QProcess::ProcessError )), this, SLOT(gdbError(QProcess::ProcessError)));


	// connection from parser
	connect(Parser, SIGNAL(done(int, QString)), this , SLOT(onDone(int, QString)));
	connect(Parser, SIGNAL(error(int, QString)), this , SLOT(onError(int, QString)));
	connect(Parser, SIGNAL(info(int, QString)), this , SLOT(onInfo(int, QString)));
	connect(Parser, SIGNAL(prompt(int, QString)), this , SLOT(onPrompt(int, QString)));

	connect(Parser, SIGNAL(targetLoaded(int, QString)), this , SLOT(onTargetLoaded(int, QString)));
	connect(Parser, SIGNAL(targetNoLoaded(int, QString)), this , SLOT(onTargetNoLoaded(int, QString)));
	connect(Parser, SIGNAL(targetExited(int, QString)), this , SLOT(onTargetExited(int, QString)));
	connect(Parser, SIGNAL(targetRunning(int, QString)), this , SLOT(onTargetRunning(int, QString)));
	connect(Parser, SIGNAL(targetStopped(int, QString)), this , SLOT(onTargetStopped(int, QString)));

	connect(Parser, SIGNAL(onInterpreter(const QPointer<BaseInterpreter> & ,const int & , const QString &)), this , 
		SLOT(onInterpreter(const QPointer<BaseInterpreter> & , const int & , const QString &)));

	// breakpoint
	
	connect(Breakpoint, SIGNAL(onToggleBreakpoint(const Breakpoint &, const BaseBreakpoint &, const bool &)), Bridge,
		SLOT(onToggleBreakpoint(const Breakpoint &, const BaseBreakpoint &, const bool& )));

	// backtrace
	
	connect(Backtrace, SIGNAL(onToggleBacktrace(const QString & , const int &)), Bridge,
		SLOT(onToggleBacktrace(const QString & , const int &)));

	// Monkey
	connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString & ) ), Bridge, SLOT( addEditor( const QString & ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( int ) ), Bridge, SLOT( removeEditor( int ) ) );

	// connection BridgeEditor
	connect(Bridge, SIGNAL(userToggleBreakpoint(const QString &, const int &)), this, SLOT(onUserToggleBreakpoint(const QString &, const int &)));
	connect(Bridge, SIGNAL(requestBreakpoint(const QString &)), Breakpoint , SLOT(onRequestBreakpoint(const QString &)));
	connect(Bridge, SIGNAL(requestBacktrace(const QString &)), Backtrace , SLOT(onRequestBacktrace(const QString &)));
	connect(Bridge, SIGNAL(requestShowVar(const QString &)), ToolTip , SLOT(onRequestShowVar(const QString &)));

	connect(Watch, SIGNAL(requestScriptTranslate(const QString &, const QString &)), Script , SLOT(onRequestScriptTranslate(const QString &, const QString &)));
	connect(Script, SIGNAL(scriptFinishedTranslate(const QString &)), Watch , SLOT(onScriptFinishedTranslate(const QString &)));

	
	// add permanent Interpreter	
	interpreterStepOver = Parser->addInterpreter(
		"Dock",
		QRegExp("^n$"), 
		QRegExp("\\d+\\s+.*"),
		"^info,interpreter=\"Dock\",event=\"Step-Over\",answerGdb=\"");

	// connect interpreter to function
	Connect.add(this, interpreterStepOver, &DockGNUDebugger::onTargetStopped );

	
	interpreterStepInto = Parser->addInterpreter(
		"Dock",
		QRegExp("^s$"), 
		QRegExp("\\d+\\s+.*"),
		"^info,interpreter=\"Dock\",event=\"Step-into\",answerGdb=\"");

	Connect.add(this, interpreterStepInto, &DockGNUDebugger::onTargetStopped );


	interpreterStepFinish = Parser->addInterpreter(
		"Dock",
		QRegExp("^finish$"), 
		QRegExp("\\d+\\s+.*"),
		"^info,interpreter=\"Dock\",event=\"Step-Finish\",answerGdb=\"");

	Connect.add(this, interpreterStepFinish, &DockGNUDebugger::onTargetStopped );

	// find if addOn is enable ?
	foreach(QPointer< class GdbCore> r, Dispatcher->list())
	{
		r->setEnabled( GdbSetting::instance()->getStartUp( r->name() ));
		if(r->isEnabled() && r->widget()) 
			mainTabWidget->addTab( r->widget(),r->icon(), r->name() );
	}

	crlf = QTextCodec::codecForLocale()->fromUnicode( pMonkeyStudio::getEol() );
}

//

DockGNUDebugger:: ~DockGNUDebugger()
{
	mainTabWidget->deleteLater();
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

void DockGNUDebugger::commandReadyRead(  const QString& d)
{
	rawLog->setTextColor(QColor(0,0,255));
	rawLog->append( d );
	rawLog->setTextColor(QColor(0,0,0));
	Parser->processParsing( d );
}

// Actions

void DockGNUDebugger::onActionLoadTarget()
{
	isTargetRunning = isGdbStarted = false;

	Process->clearAllCommand();
	Parser->clearAllCommand();
	
	// set parser not ready because i start Gdb, i can only send 
	// a command if gdb is started by waitting prompt (gdb)
	// fix V1.3.2
	Parser->setReady(false);
		
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

		Parser->setNextCommand("Dock", "Starting GDB");
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

	Parser->setNextCommand("Dock", "Stop GDB");
	
	if(!isTargetRunning)	
		Process->stopTarget();
	else
		Process->stopProcess();
}

//

void DockGNUDebugger::onActionRestart()
{
	Process->clearAllCommand();
	Parser->clearAllCommand();
	
	Parser->setReady(true);
		
	// fix v1.3.2 when i load target i consider target runing
	isTargetRunning = true;

	if(Parser->isReady())
	{
		setEnabledActions(false);
		rawLog->append("*** User restart ***");
		Parser->setNextCommand("Dock", "r");
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
		Parser->setNextCommand("Dock", "c");
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
		Parser->setNextCommand("Dock", "n");
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
		Parser->setNextCommand("Dock", "s");
		Process->sendRawData("s");
	}
}


void DockGNUDebugger::onActionStepFinish()
{
	if(Parser->isReady())
	{
		setEnabledActions(false);

		rawLog->append("*** User step finish ***");
		Parser->setNextCommand("Dock", "finish");
		Process->sendRawData("finish");
	}
}

// Process

// signal from GdbProcess (QProcess started)
void DockGNUDebugger::gdbStarted()
{

	if(!isGdbStarted) // requiere for Linux, it send two signals started() ??
	{
		rawLog->append("*** Gdb started ***");

		// set options
		Parser->setNextCommand("Dock", "set breakpoint pending on");
		Process->sendRawData("set breakpoint pending on");

		// set directorie
		Parser->setNextCommand("Dock", "cd " + QFileInfo(mSelectedTarget).path() );
		Process->sendRawData("cd " + QFileInfo(mSelectedTarget).path() );

		// gdb is started, now load target under Gdb
		Parser->setNextCommand("Dock", "file \"" + mSelectedTarget + "\"");
		Process->sendRawData("file \"" + mSelectedTarget + "\"");
		
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
		default :;
	}


	Dispatcher->gdbFinished();

	setEnabledActions(false);
	mActionList->value("aLoadTarget")->setEnabled(true);

	isGdbStarted = false;
	isTargetRunning = false;
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
		break;
		default : ;
	}

	Dispatcher->gdbError();

	setEnabledActions(false);
	mActionList->value("aLoadTarget")->setEnabled(true);

	isGdbStarted = false;
	isTargetRunning = false;

	// gdb crashed rmove all marker in editor
	Bridge->removeAllBreakpoints();
	Bridge->removeBacktrace();
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
	// under linux i can stop by ctrl + c
	mActionList->value("aExitGdb")->setEnabled( true );

	Dispatcher->targetRunning(id, st);
	isTargetRunning = true;
}

//

void DockGNUDebugger::onTargetStopped(int id, QString st)

{
/*
when breakpoint hit
or
breakpoint hit and step over command (bug , i have two target stopped signal)
  
*/

/*
	10009 : ^info,interpreter="GdbParser",event="target-stopped",answerGdb="Breakpoint 2, qMain (argc=1, argv=0x3d4c48) at src/main.cpp:37",currentCmd="n"
	*** Target stopped ***
or
	10009 : ^info,interpreter="GdbParser",event="target-stopped",answerGdb="Breakpoint 2, qMain (argc=1, argv=0x3d4c48) at src/main.cpp:37",currentCmd="n"
	*** Target stopped ***
	-73471976 : ^info,interpreter="Dock",event="Step-Over",answerGdb="37	QString s = coucou;",currentCmd="n"
	*** Target stopped ***

or
	-73471976 : ^info,interpreter="Dock",event="Step-Over",answerGdb="37	QString s = coucou;",currentCmd="n"
	*** Target stopped ***
*/

	QString event = GdbCore::findValue(st, "event");

	if(event == "target-stopped")
		disableStep = true;

	if(disableStep && (event == "Step-Over" || event == "Step-Into"))
		return;
	
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

//

void DockGNUDebugger::onPrompt(int id, QString st)
{
	disableStep = false;
	rawLog->setTextColor(QColor(255,0,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));

	Dispatcher->prompt(id, st);

}

// Interpreter for step over / into

void DockGNUDebugger::onInterpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	// connect step into / over
	// call onTargetStopped
	Connect.call( i, id, s);
}

// from editor margin clicked

void DockGNUDebugger::onUserToggleBreakpoint(const QString & fileName, const int & line)
{
	// fix 1.3.2 not send data to gdb if it not started or if target not running
	if(isGdbStarted)
	{
		if(!isTargetRunning)
		{
			rawLog->append("** user toggle breakpoint *** " + fileName + " " + QString::number(line + 1));
			Breakpoint->toggleBreakpoint(fileName, line + 1);
		}
		else
			GdbCore::showMessage(tr("I can't toggle breakpoint when target running."), 5000, _WARNING_ );
	}
}

// close monkey

void DockGNUDebugger::onAboutToClose()
{
}
