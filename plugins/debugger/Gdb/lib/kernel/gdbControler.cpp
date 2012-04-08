
#include "gdbControler.h"

#include <QtDebug>

GdbControler::GdbControler(QObject *parent) : QObject(parent)
{
	GdbCore::Interpreter()->remove(name());
	GdbCore::RestoreLine()->remove(name());
	//Breakpoint 1, main (argc=1, argv=0x3f4098) at src/main.cpp:12
	// because some time argc is null 
	GdbCore::RestoreLine()->add(name(), "Breakpoint\\s\\d+,\\s.*" , ".*\\sat\\s.*:\\d+$");


	mGdbStarted = mGdbRunning = mTargetLoaded = mTargetStopped = false;

	// signals from parser
	connect(GdbCore::Parser(), SIGNAL(targetLoaded(int, QString)), this , SLOT(onTargetLoaded(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(targetNoLoaded(int, QString)), this , SLOT(onTargetNoLoaded(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(targetExited(int, QString)), this , SLOT(onTargetExited(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(targetRunning(int, QString)), this , SLOT(onTargetRunning(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(targetStopped(int, QString)), this , SLOT(onTargetStopped(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(targetCrashed(int, QString)), this , SLOT(onTargetCrashed(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(noExecCommand(int, QString)), this , SLOT(onNoExecCommand(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(onInterpreter(const QPointer<BaseInterpreter> & ,const int & , const QString &)), this , 
		SLOT(onInterpreter(const QPointer<BaseInterpreter> & , const int & , const QString &)));
	connect(GdbCore::Parser(), SIGNAL(done(int, QString)), this , SLOT(onDone(int, QString)));
	connect(GdbCore::Parser(), SIGNAL(prompt(int, QString)), this , SLOT(onPrompt(int, QString)));

	// signals from gdb process
	connect(GdbCore::Process(), SIGNAL( started( )), this, SLOT(onGdbStarted()));
	connect(GdbCore::Process(), SIGNAL( finished(  int , QProcess::ExitStatus  )), this, SLOT(onGdbFinished( int , QProcess::ExitStatus)));
	connect(GdbCore::Process(), SIGNAL( error ( QProcess::ProcessError )), this, SLOT(onGdbError(QProcess::ProcessError)));

	// create action
	setMenuAction();

	// disable step
	mDisableStep = false;

	// create interpreter
	interpreterStepOver = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^n$"), 
		QRegExp("\\d+\\s+.*"),
		"^done,className=\"" + name() + "\",event=\"Step-Over\",answerGdb=\"");

	// connect interpreter to function
	Connect.add(this, interpreterStepOver, &GdbControler::onTargetStopped );

	interpreterStepInto = GdbCore::Interpreter()->add(
	name(),
	QRegExp("^s$"), 
	QRegExp("\\d+\\s+.*"), // 473	    inline void show() { setVisible(true); }
	"^done,className=\"" + name() + "\",event=\"Step-into\",answerGdb=\"");

	Connect.add(this, interpreterStepInto, &GdbControler::onTargetStopped );


	interpreterStepFinish = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^finish$"), 
		QRegExp("\\d+\\s+.*"),
		"^done,className=\"" + name() + "\",event=\"Step-Finish\",answerGdb=\"");

	Connect.add(this, interpreterStepFinish, &GdbControler::onTargetStopped );
}


GdbControler::~GdbControler()
{
	removeMenuAction();
}


QString GdbControler::name()
{
	return "GdbControler";
}

/*!
* \details Enable or disable all actions in Monkey toolBar.
* \param b is true, all actions are enable, else are disable.
*/
void GdbControler::setEnabledActions(bool b)
{
	foreach(QAction * a, mActionList)
		a->setEnabled(b);
}

void GdbControler::setMenuAction()
{
	// add actions to main window
	mActionList["aLoadTarget"] = new QAction(   QIcon( ":/icons/open.png" ) ,tr( "Load target" ),this);
	mActionList["aRestart"] = new QAction(  QIcon( ":/icons/update.png" ) ,tr( "Start or restart target" ),this);
	mActionList["aContinue"] = new QAction(  QIcon( ":/icons/continue.png" ) ,tr( "Continue to next breakpoint" ),this);
	mActionList["aStepOver"] = new QAction(  QIcon( ":/icons/stepover.png" ) ,tr( "Step over" ),this);
	mActionList["aStepInto"] = new QAction(  QIcon( ":/icons/stepinto.png" ) ,tr( "Step into" ),this);
	mActionList["aStepFinish"] = new QAction(  QIcon( ":/icons/stepfinish.png" ) ,tr( "Step Finish" ),this);
	mActionList["aExitGdb"] = new QAction( QIcon( ":/icons/close.png" ) ,tr( "Quit debug mode" ), this);
	// connections
	connect( mActionList["aLoadTarget"], SIGNAL( triggered() ), this, SLOT( onActionLoadTarget() ) );
	connect( mActionList["aRestart"], SIGNAL( triggered() ), this, SLOT( onActionRestart() ) );
	connect( mActionList["aContinue"], SIGNAL( triggered() ), this, SLOT( onActionContinue() ) );
	connect( mActionList["aStepOver"], SIGNAL( triggered() ), this, SLOT( onActionStepOver() ) );
	connect( mActionList["aStepInto"] , SIGNAL( triggered() ), this, SLOT( onActionStepInto() ) );
	connect( mActionList["aStepFinish"] , SIGNAL( triggered() ), this, SLOT( onActionStepFinish() ) );
	connect( mActionList["aExitGdb"], SIGNAL( triggered() ), this, SLOT( onActionExit() ) );

	// init action disable
	mActionList["aRestart"]->setEnabled( false );
	mActionList["aContinue"]->setEnabled( false );
	mActionList["aStepOver"]->setEnabled( false );
	mActionList["aStepInto"]->setEnabled( false );
	mActionList["aStepFinish"]->setEnabled( false );
	mActionList["aExitGdb"]->setEnabled( false );
}


void GdbControler::removeMenuAction()
{
	// delete actions
	qDeleteAll( mActionList );
	delete aSeparator;
	mActionList.clear();
	// clear eventual message
	emit defaultMessage("");
}


// user actions


void GdbControler::onActionLoadTarget()
{
	mGdbStarted = mGdbRunning = mTargetLoaded = mTargetStopped = false;


	GdbCore::Process()->clearAllCommand();
	GdbCore::Parser()->clearAllCommand();
	
	// set parser not ready because i start Gdb, i can only send 
	// a command if gdb is started by waitting prompt (gdb)
	// fix V1.3.2
	GdbCore::Parser()->setReady(false);
		
//	UIXUPManager p* = MonkeyCore::projectsManager()->currentProject();
//	if(MonkeyCore::projectsManager()->currentProject())
//		QString s = MonkeyCore::projectsManager()->currentProject()->projectSettingsValue( "EXECUTE_DEBUG" );
	
	// open dialog for select target
	mSelectedTarget = QFileDialog::getOpenFileName(NULL, tr("Select your target"));
	if( !mSelectedTarget.isEmpty())
	{
		emit defaultMessage(tr( "*** Starting Gdb *** "));
		GdbCore::Process()->setCommand( GdbCore::Setting()->getPathGdb() );
		GdbCore::Parser()->setNextCommand(name(), "Starting GDB");
		GdbCore::Process()->startProcess();
	}
}

void GdbControler::onActionExit()
{
	emit defaultMessage( tr("Wait ..."));
	GdbCore::Parser()->setNextCommand(name(), "Stop GDB");
	
	if(mTargetStopped)	
		GdbCore::Process()->stopTarget();
	else
		GdbCore::Process()->stopProcess();
}

void GdbControler::onActionRestart()
{
	emit defaultMessage( tr("Wait ..."));

	GdbCore::Process()->clearAllCommand();
	GdbCore::Parser()->clearAllCommand();
	
	GdbCore::Parser()->setReady(true);

	if(GdbCore::Parser()->isReady())
	{
		// fix v1.3.2 when i load target i consider target running
		mTargetStopped = false;
		setEnabledActions(false);
		GdbCore::Parser()->setNextCommand(name(), "r");
		GdbCore::Process()->sendRawData("r");
	}
}

void GdbControler::onActionContinue()
{
	emit defaultMessage( tr("Wait ..."));
//	GdbCore::BridgeEditor()->removeBacktrace();


	if(GdbCore::Parser()->isReady())
	{
		mTargetStopped = false;
		setEnabledActions(false);
		GdbCore::Parser()->setNextCommand(name(), "c");
		GdbCore::Process()->sendRawData("c");
	}
}

void GdbControler::onActionStepOver()
{
	emit defaultMessage( tr("Wait ..."));
//	GdbCore::BridgeEditor()->removeBacktrace();

	if(GdbCore::Parser()->isReady())
	{
		mTargetStopped = false;
		setEnabledActions(false);
		GdbCore::Parser()->setNextCommand(name(), "n");
		GdbCore::Process()->sendRawData("n");
	}
}

void GdbControler::onActionStepInto()
{
	emit defaultMessage( tr("Wait ..."));
//	GdbCore::BridgeEditor()->removeBacktrace();

	if(GdbCore::Parser()->isReady())
	{
		mTargetStopped = false;
		setEnabledActions(false);
		GdbCore::Parser()->setNextCommand(name(), "s");
		GdbCore::Process()->sendRawData("s");
	}
}

void GdbControler::onActionStepFinish()
{
	emit defaultMessage( tr("Wait ..."));
//	GdbCore::BridgeEditor()->removeBacktrace();

	if(GdbCore::Parser()->isReady())
	{
		mTargetStopped = false;
		setEnabledActions(false);
		GdbCore::Parser()->setNextCommand(name(), "finish");
		GdbCore::Process()->sendRawData("finish");
	}
}

// Interpreter events

void GdbControler::onInterpreter(const QPointer<BaseInterpreter> &i , const int &id, const QString &d)
{
	Connect.call(i, id, d);
}



// Parser events



void GdbControler::onTargetStopped(int id, QString st)

{
	if(mTargetStopped) return ;

	// fixed bug if breakpoint hi and current command is step over

	QString event = GdbAddonBase::findValue(st, "event");

	if(event == "target-stopped")
		mDisableStep = true;

	if(mDisableStep && (event == "Step-Over" || event == "Step-Into"))
		return;

	emit defaultMessage( tr("*** Target stopped *** Reason : <b>")  + event + "</b>");
	GdbCore::KernelDispatcher()->targetStopped(id, st);

	setEnabledActions( true );
	mActionList.value("aLoadTarget")->setEnabled( false );

	mTargetStopped = true;
}


void GdbControler::onTargetRunning(int id , QString st)
{
	setEnabledActions(false);
	// i can't stop target because gdb have no handle
	// but i can stop gdb directly (icon)
	// under linux i can stop by ctrl + c
	mActionList.value("aExitGdb")->setEnabled( true );


	emit defaultMessage( tr("*** Target running success full ***"));
	GdbCore::KernelDispatcher()->targetRunning(id, st);
	mTargetStopped = false;
}

void GdbControler::onTargetLoaded(int id, QString st)
{	
	emit defaultMessage( tr("*** Target loaded success full ***"));
	GdbCore::KernelDispatcher()->targetLoaded(id, st);

	setEnabledActions(false);
	mActionList.value("aExitGdb")->setEnabled( true );
	mActionList.value("aRestart")->setEnabled( true );

	mTargetLoaded = true;
	mTargetStopped = true;
}

void GdbControler::onTargetNoLoaded(int id, QString st)
{	
	GdbCore::Process()->stopTarget();
	// fixed 1.4.0
//	GdbCore::Process()->stopProcess();
	// view message as error
	emit errorMessage(tr("*** Target no Loaded Stop Gdb *** Reason : <b>") + st + "</b>", 5000);
	GdbCore::KernelDispatcher()->targetNoLoaded(id, st);

	mTargetLoaded = false;
	mTargetStopped = true;
}

void GdbControler::onTargetExited(int id , QString st)
{
	emit defaultMessage( tr("*** Target exit success full ***"));
	GdbCore::KernelDispatcher()->targetExited(id, st);
	
	setEnabledActions( false );
	mActionList.value("aExitGdb")->setEnabled( true );
	mActionList.value("aRestart")->setEnabled( true );

	mTargetStopped = true;
}

void GdbControler::onTargetCrashed(int id , QString st)
{
	// view message as error
	emit errorMessage(tr("*** Target CRACHED ***") , 5000);
	onTargetStopped(id,st);
}

void GdbControler::onNoExecCommand(int id , QString st)
{
	onTargetStopped(id,st);
}

/*!
* \details Info event.	Calling when GdbParser emit info signal.
* \param id is an Id of string
* \param st is the Info string.
*/
void GdbControler::onDone(int id, QString st)
{
	GdbCore::KernelDispatcher()->done(id, st);
}

/*!
* \details Prompt event. Calling when GdbParser emit prompt signal.
* \param id egal 0
* \param st is the Prompt string.
*/
void GdbControler::onPrompt(int id, QString st)
{
	mDisableStep = false;
	GdbCore::KernelDispatcher()->prompt(id, st);
}


// GDB Process events

void GdbControler::onGdbStarted()
{

	if(!mGdbStarted) // requiere for Linux, it send two signals started() ??
	{
		emit defaultMessage( tr("*** Gdb running ***"));

		/* set options at startup */

		// set height
		GdbCore::Parser()->setNextCommand(name(), "set height 0" );
		GdbCore::Process()->sendRawData("set height 0" );

		// set width
		GdbCore::Parser()->setNextCommand(name(), "set width 0" );
		GdbCore::Process()->sendRawData("set width 0" );

		// set options
		GdbCore::Parser()->setNextCommand(name(), "set breakpoint pending on");
		GdbCore::Process()->sendRawData("set breakpoint pending on");

		// set directorie
		GdbCore::Parser()->setNextCommand(name(), "cd " + QFileInfo(mSelectedTarget).path() );
		GdbCore::Process()->sendRawData("cd " + QFileInfo(mSelectedTarget).path() );

		// gdb is started, now load target under Gdb
		GdbCore::Parser()->setNextCommand(name(), "file \"" + mSelectedTarget + "\"");
		GdbCore::Process()->sendRawData("file \"" + mSelectedTarget + "\"");
		
		mGdbStarted = true;
		GdbCore::KernelDispatcher()->gdbStarted();
	}
}

void GdbControler::onGdbError(QProcess::ProcessError e)
{
	switch(e)
	{
		case QProcess::FailedToStart : emit errorMessage("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.",5000);
		break;
		case QProcess::Crashed : ;//emit errorMessage("The process crashed some time after starting successfully.",5000);
		break;
		case  QProcess::Timedout : emit errorMessage("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.",5000);
		break;
		case QProcess::WriteError : emit errorMessage("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.",5000);
		break;
		case QProcess::ReadError : emit errorMessage("An error occurred when attempting to read from the process. For example, the process may not be running.",5000);
		break;
		case QProcess::UnknownError : emit errorMessage("An unknown error occurred. This is the default return value of error().",5000);
		break;
		default : ;
	}

	GdbCore::KernelDispatcher()->gdbError();

	setEnabledActions(false);
	mActionList.value("aLoadTarget")->setEnabled(true);

	mGdbStarted = mGdbRunning = mTargetLoaded = false;
	mTargetStopped = false;
}


void GdbControler::onGdbFinished( int a , QProcess::ExitStatus e)
{
	Q_UNUSED(a);
	switch(e)
	{
		case QProcess::NormalExit : emit defaultMessage( tr("*** Gdb process exited normally ***"));
		break;
		case QProcess::Crashed : emit defaultMessage( tr("*** Gdb process crashed. ***"));
		default :;
	}


	GdbCore::KernelDispatcher()->gdbFinished();

	setEnabledActions(false);
	mActionList.value("aLoadTarget")->setEnabled(true);

	mGdbStarted = mGdbRunning = mTargetLoaded = false;
	mTargetStopped = false;
}
