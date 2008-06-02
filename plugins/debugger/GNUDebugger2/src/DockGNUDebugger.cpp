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
/*

ajouter au parser :

	// probleme de start gdb
	  QRegExp Reading symbols from C:/DEV/debugger/debug/debugger.exe...
	  QRegExp done.

*/

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


bool loaded = false;

DockGNUDebugger::DockGNUDebugger( QWidget * w )
	: pDockWidget( w )
{

	// set charset for gdb
//	QTextCodec::setCodecForLocale(QTextCodec::codecForName("ISO-8859-1"));
	QTextCodec *n = QTextCodec::codecForLocale ();
	QMessageBox::warning(NULL,n->name(),"name of local");
	// closing Monkey
	connect (MonkeyCore::mainWindow(), SIGNAL( aboutToClose()), this , SLOT(onAboutToClose()));

	// create kernel (parser ,process, ...)
	Parser = new GdbParser(this);
	Process = new GdbProcess(this);
	Bridge = new GdbBridgeEditor(this);
	Dispatcher = new GdbKernelDispatcher(this, Parser);

	// create addon
	Breakpoint = new GdbBreakpoint(this, Parser, Process);
	Backtrace = new GdbBacktrace(this, Parser, Process);


	if( !Parser)
		QMessageBox::warning(NULL,"Critical erreur", "Parser is not ready (NULL pointer");
	
	if( !Process)
		QMessageBox::warning(NULL,"Critical erreur", "Process is not ready (NULL pointer");
	
	if( !Bridge)
		QMessageBox::warning(NULL,"Critical erreur", "Bridge is not ready (NULL pointer");

	if( !Dispatcher)
		QMessageBox::warning(NULL,"Critical erreur", "Dispatcher is not ready (NULL pointer");

	if( !Breakpoint)
		QMessageBox::warning(NULL,"Critical erreur", "Breakpoint is not ready (NULL pointer");

	if( !Backtrace)
		QMessageBox::warning(NULL,"Critical erreur", "Backtrace is not ready (NULL pointer");

	if( Parser && Process && Bridge && Dispatcher && Breakpoint && Backtrace)
	{
		// add to dispatcher
		Dispatcher->add(Breakpoint);
		Dispatcher->add(Backtrace);

		// load setting
		Settings* s = MonkeyCore::settings();

		s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
			mPathGdb = s->value("PathGdb", "gdb").toString();
			QStringList mPluginList = s->allKeys();
			foreach(QString p , mPluginList)
			{
				bool e = s->value( p, true).toBool();
				
				foreach(QPointer< class GdbCore> r, Dispatcher->list())
				{
					if(p.endsWith(r->name()))
					{
						r->setEnabled(e);
					}
				}
			}

		s->endGroup();

		Process->setCommand(mPathGdb);
		
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

		connect(Breakpoint, SIGNAL(onToggleBreakpoint(const QString &, const int &, const bool &)), Bridge,
			SLOT(onToggleBreakpoint(const QString &, const int &, const bool &)));

		connect(Backtrace, SIGNAL(onToggleBacktrace(const QString &, const int &)), Bridge,
			SLOT(onToggleBacktrace(const QString &, const int &)));

		// connection BridgeEditor
		connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString & ) ), Bridge, SLOT( add( const QString & ) ) );
		connect( MonkeyCore::workspace(), SIGNAL( documentAboutToClose( int ) ), Bridge, SLOT( remove( int ) ) );

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

//find parser for 0xb704e450 in __libc_start_main () from /lib/tls/i686/cmov/libc.             so.6

		//connect interpreter to function
		Connect = new GdbConnectTemplate<DockGNUDebugger>;
		Connect->add(this, interpreterStepOver, &DockGNUDebugger::onTargetStopped );
		Connect->add(this, interpreterStepInto, &DockGNUDebugger::onTargetStopped );


		connect(Bridge, SIGNAL(userToggleBreakpoint(const QString &, const int &)), this, SLOT(onUserToggleBreakpoint(const QString &, const int &)));

		connect(Bridge, SIGNAL(requestBreakpoint(const QString &)), Breakpoint , SLOT(onRequestBreakpoint(const QString &)));
		connect(Bridge, SIGNAL(requestBacktrace(const QString &)), Backtrace , SLOT(onRequestBacktrace(const QString &)));
	
		// add plugin under tabWidget
		if(Breakpoint->isEnabled()) mainTabWidget->addTab(Breakpoint->widget(),Breakpoint->name());
		if(Backtrace->isEnabled()) mainTabWidget->addTab(Backtrace->widget(),Backtrace->name());
	}
		

	

	// plugin send a command to Gdb
//	connect(kernelDispatcher, SIGNAL(sendRawData(GdbCore*, QByteArray)) ,this , SLOT(onSendRawData(GdbCore *, QByteArray) ));

//	crlf = pMonkeyStudio::getEol();
	
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

}


DockGNUDebugger:: ~DockGNUDebugger()
{
	// all class herite QObject, Qt delete for me.
}


//
void DockGNUDebugger::setAction(QHash<QString, QAction *> *a)
{
	mActionList = a;
}


void DockGNUDebugger::setEnabledActions(bool b)
{
	foreach(QAction * a, *mActionList)
		a->setEnabled(b);
}


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
	rawLog->append("*** selected target ***");
	// open dialog for select target
	mSelectedTarget = QFileDialog::getOpenFileName(this, tr("Select your target"));
	if( !mSelectedTarget.isEmpty())
	{
		rawLog->append("*** " + mSelectedTarget + " ***");

		Parser->setLastCommand("Starting GDB");
		Process->startProcess();
	}
}

void DockGNUDebugger::onActionExit()
{
	rawLog->append("*** User exit debug mode ***");

	// remove all marquer in magin
	Bridge->removeAllBreakpoints();
	Bridge->removeBacktrace();

	Parser->setLastCommand("Stop GDB");
	Process->stopProcess();
}

void DockGNUDebugger::onActionRestart()
{
	setEnabledActions(false);
	rawLog->append("*** User restart ***");
	Parser->setLastCommand("r");
	Process->sendRawData("r");
}

void DockGNUDebugger::onActionContinue()
{
	setEnabledActions(false);
	rawLog->append("*** User continue ***");
	Parser->setLastCommand("c");
	Process->sendRawData("c");
}

void DockGNUDebugger::onActionStepOver()
{
	setEnabledActions(false);
	
	rawLog->append("*** User step over ***");
	Parser->setLastCommand("n");
	Process->sendRawData("n");

}

void DockGNUDebugger::onActionStepInto()
{
	setEnabledActions(false);

	rawLog->append("*** User step into ***");
	Parser->setLastCommand("s");
	Process->sendRawData("s");
}

void DockGNUDebugger::onActionStop()
{
	rawLog->append("*** User stop debugging ***");

}

// test breakpoint
void DockGNUDebugger::onActionToggleBreak()
{
	rawLog->append("*** User toggle breakpoint ***");
	Process->sendRawData("b main.cpp:69");

}


// Process

// signal from GdbProcess (QProcess started)
void DockGNUDebugger::gdbStarted()
{

qDebug("Gdb state : " + QByteArray::number(Process->state()));
if(!loaded)
{
	rawLog->append("*** Gdb started ***");

	// gdb is started, now load target under Gdb
	Parser->setLastCommand("file " + mSelectedTarget.toLocal8Bit());
	Process->sendRawData("file " + mSelectedTarget.toLocal8Bit());
	
	Dispatcher->gdbStarted();
	loaded = true;
}
}

void DockGNUDebugger::gdbFinished( int a , QProcess::ExitStatus e)
{
qDebug("Gdb Finished");
	rawLog->append("*** Gdb finished successfull code : " + QString::number(a) + " ***");

	setEnabledActions(false);
	mActionList->value("aLoadTarget")->setEnabled(true);

	Dispatcher->gdbFinished();
loaded = false;
}


void DockGNUDebugger::gdbError( QProcess::ProcessError e)
{
	rawLog->append("*** Critical GdbErreur code : " + QString::number(e) + " ***");

	setEnabledActions(false);
	mActionList->value("aLoadTarget")->setEnabled(true);

	Dispatcher->gdbError();
}

// Target

void DockGNUDebugger::onTargetLoaded(int id, QString st)
{
	rawLog->append("*** Target loaded success full ***");

	setEnabledActions(false);
	mActionList->value("aExitGdb")->setEnabled( true );
	mActionList->value("aRestart")->setEnabled( true );

	Dispatcher->targetLoaded(id, st);
}


void DockGNUDebugger::onTargetNoLoaded(int id, QString st)
{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target no Loaded Stop Gdb ***");


	Process->stopProcess();

	Dispatcher->targetNoLoaded(id, st);
}

void DockGNUDebugger::onTargetRunning(int id, QString st)
{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target running success full ***");

	setEnabledActions(false);
	// i can't stop target because gdb have no handle
	// but i can stop gdb directly
	mActionList->value("aExitGdb")->setEnabled( true );

	Dispatcher->targetRunning(id, st);
}


void DockGNUDebugger::onTargetStopped(int id, QString st)

{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target stopped ***");

	setEnabledActions( true );
	mActionList->value("aLoadTarget")->setEnabled( false );

	Dispatcher->targetStopped(id, st);
}


void DockGNUDebugger::onTargetExited(int id, QString st)
{
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->append("*** Target exit success full ***");

	setEnabledActions( false );
	mActionList->value("aExitGdb")->setEnabled( true );
	mActionList->value("aRestart")->setEnabled( true );

	Dispatcher->targetExited(id, st);
}


// Parser

void DockGNUDebugger::onError(int id, QString st)
{
	rawLog->setTextColor(QColor(255,0,0));
	rawLog->append(QString::number(id) + " : " + st);
	rawLog->setTextColor(QColor(0,0,0));

	rawLog->append("*** Error from parser ***");

	// why stp gdb on all error from parser ???	
//	Process->stopProcess();

	Dispatcher->error(id, st);
}

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


void DockGNUDebugger::onAboutToClose()
{
}
