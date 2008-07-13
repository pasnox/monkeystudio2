

#include "DockGNUDebugger.h"
#include <settingsmanager.h>
#include <coremanager.h>
#include <maininterface.h>
#include <qscintillamanager.h>
#include "monkey.h"


#include <QFileDialog>

#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>


/**
 \detail Load current setting from Monkey .ini
*/
void DockGNUDebugger::loadSettings()
{
}

/**
 \detail Save current setting from Monkey .ini
*/
void DockGNUDebugger::saveSettings()
{
}

/**
 \detail Create a new object.
*/
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
	// TODO 
	// You can add your addOn here

	// addOn to dispatcher
	Dispatcher->add(Breakpoint);
	Dispatcher->add(Backtrace);
	Dispatcher->add(Register);
	Dispatcher->add(Watch);
	Dispatcher->add(Cli);
	Dispatcher->add(ToolTip);
	Dispatcher->add(Script);
	// TODO
	// You can add your addOn to dispatcher


	// Process
	connect(Process, SIGNAL( commandReadyRead( const QString& )), this , SLOT( commandReadyRead( const QString& )));
	connect(Process, SIGNAL( started( )), this, SLOT(gdbStarted()));
	connect(Process, SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(gdbFinished( int , QProcess::ExitStatus)));
	connect(Process, SIGNAL( error ( QProcess::ProcessError )), this, SLOT(gdbError(QProcess::ProcessError)));


	// Parser
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

	// Breakpoint
	
	connect(Breakpoint, SIGNAL(onToggleBreakpoint(const Breakpoint &, const BaseBreakpoint &, const bool &)), Bridge,
		SLOT(onToggleBreakpoint(const Breakpoint &, const BaseBreakpoint &, const bool& )));

	// Backtrace
	
	connect(Backtrace, SIGNAL(onToggleBacktrace(const QString & , const int &)), Bridge,
		SLOT(onToggleBacktrace(const QString & , const int &)));

	// Monkey
	connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString & ) ), Bridge, SLOT( addEditor( const QString & ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( int ) ), Bridge, SLOT( removeEditor( int ) ) );

	// BridgeEditor
	connect(Bridge, SIGNAL(userToggleBreakpoint(const QString &, const int &)), this, SLOT(onUserToggleBreakpoint(const QString &, const int &)));
	connect(Bridge, SIGNAL(requestBreakpoint(const QString &)), Breakpoint , SLOT(onRequestBreakpoint(const QString &)));
	connect(Bridge, SIGNAL(requestBacktrace(const QString &)), Backtrace , SLOT(onRequestBacktrace(const QString &)));
	connect(Bridge, SIGNAL(requestShowVar(const QString &)), ToolTip , SLOT(onRequestShowVar(const QString &)));

	// Watch
	connect(Watch, SIGNAL(requestScriptTranslate(const QString &, const QString &)), Script , SLOT(onRequestScriptTranslate(const QString &, const QString &)));
	
	// Script
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

/**
 \detail Delete main container
*/
DockGNUDebugger:: ~DockGNUDebugger()
{
	mainTabWidget->deleteLater();
}

/**
 \detail Set actions from GNUDebugger to this class.
*/
void DockGNUDebugger::setAction(QHash<QString, QAction *> *a)
{
	mActionList = a;
}

/**
 \details Enable or disable all actions in Monkey toolBar.
 \param If b is true, all actions are enable, else are disable.
*/
void DockGNUDebugger::setEnabledActions(bool b)
{
	foreach(QAction * a, *mActionList)
		a->setEnabled(b);
}

/**
 \detail Calling when Gdb has an answer or Gdb has new datas, after it call GdbParser for parse this string.
 \param d is raw data from Gdb
*/
void DockGNUDebugger::commandReadyRead(  const QString& d)
{
	rawLog->setTextColor(QColor(0,0,255));
	rawLog->append( d );
	rawLog->setTextColor(QColor(0,0,0));
	Parser->processParsing( d );
}

// Actions

/**
 \detail When user click <b>Load target</b> action in Monkey toolBar. This function start Gdb and load the target.
*/
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

/**
 \detail When user click <b>Exit debug mode</b> action in Monkey toolBar. Stop and quit Gdb.
*/
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

	
/**
 \detail When user click <b>Restart target</b> action in Monkey toolBar. This function restart the target at the begining.
*/
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

/**
 \detail When user click <b>Continue</b> action in Monkey toolBar. This function start and stop target to the next breakpoint, if has.
*/
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

/**
 \detail When user click <b>Step over</b> action in Monkey toolBar. This function execute step over.
*/
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

	
/**
 \detail When user click <b>Step into</b> action in Monkey toolBar. This function execute step into.
*/
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

	
/**
 \detail When user click <b>Step finish</b> action in Monkey toolBar. This function carry out the function until the end.
*/
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


// Gdb

/**
 \detail Calling when Gdb is started correctly and dispatch this event to all AddOn.
*/
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


/**
 \detail Calling when Gdb is finiched correctly and dispatch this event to all AddOn.
 \param e is QProcess::ExitStatus var.
*/
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


/**
 \detail Calling when Gdb generate a error and dispatch this event to all AddOn.
 \param e is QProcess::ProcessError var.
*/

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

/**
 \detail Target is loaded correctly. Dispatch now this event to all AddOn
 \param id is an Id of string
 \param st is the string.
*/
void DockGNUDebugger::onTargetLoaded(int id, QString st)
{
	rawLog->append("*** Target loaded success full ***");

	Dispatcher->targetLoaded(id, st);

	setEnabledActions(false);
	mActionList->value("aExitGdb")->setEnabled( true );
	mActionList->value("aRestart")->setEnabled( true );
}

/**
 \detail Target is no loaded correctly (format is not correct). Dispatch this event to all AddOn.
 \param id is an Id of string
 \param st is the string.
*/
void DockGNUDebugger::onTargetNoLoaded(int id, QString st)
{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target no Loaded Stop Gdb ***");

	Process->stopProcess();

	Dispatcher->targetNoLoaded(id, st);
}

//

/**
 \detail Target is running. Dispatch this event to all AddOn.
 \param id is an Id of string
 \param st is the string.
*/

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

/**
 \detail Target is stopped (breakpoint for example). Dispatch this event to all AddOn.
 \param id is an Id of string
 \param st is the string.
*/
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

	
/**
 \detail Target is exited correctly. Dispatch this event to all AddOn.
 \param id is an Id of string
 \param st is the string.
*/
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
/**
 \detail Error event. Calling when GdbParser emit error signal
  \param id is an Id of error
  \param st is the Error string.
*/

void DockGNUDebugger::onError(int id, QString st)
{
	rawLog->append("*** Error from parser ***");

	rawLog->setTextColor(QColor(255,0,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));

	Dispatcher->error(id, st);
}

/**
 \detail Done event. Calling when GdbParser emit done signal.
 \param id is an Id of string
 \param st is the Done string.
*/
void DockGNUDebugger::onDone(int id, QString st)
{
	rawLog->setTextColor(QColor(0,255,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));
	
	Dispatcher->done(id, st);
}

/**
 \detail Info event.	Calling when GdbParser emit info signal.
 \param id is an Id of string
 \param is the Info string.
*/
void DockGNUDebugger::onInfo(int id, QString st)
{
	rawLog->setTextColor(QColor(255,255,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));

	Dispatcher->info(id, st);
}

/**
 \detail Prompt event. Calling when GdbParser emit prompt signal.
 \param id egal 0
 \param st is the Prompt string.
*/
void DockGNUDebugger::onPrompt(int id, QString st)
{
	disableStep = false;
	rawLog->setTextColor(QColor(255,0,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));

	Dispatcher->prompt(id, st);

}

// Interpreter for step over / into

/**
 \detail Calling when GdbParser class emit interpreter signal. Now interpreter switch event to the correct function.
 \param i is a pointer to the BaseInterpreter struct
 \param id is an Id of string
 \param s is the string.
*/
void DockGNUDebugger::onInterpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	// connect step into / over
	// call onTargetStopped
	Connect.call( i, id, s);
}

// from editor margin clicked

/**
 \detail Calling when user click in the margin. Call GdbBreakpoint::toggleBreakpoint() function.
 \param fileName is the name of file
 \param line is the line number.
*/
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
