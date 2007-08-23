/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox <pasnox@gmail.com>
** Project   : ConsoleManager
** FileName  : ConsoleManager.cpp
** Date      : mar. ao�t 21 20:52:27 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "ConsoleManager.h"

#include <QMetaType>

ConsoleManager::ConsoleManager( QObject* o )
	: QThread( o ), mRunning( false ), mProcess( 0L )
{
	// create process
	mProcess = new QProcess;
	
	// mixe channels
	mProcess->setReadChannelMode( QProcess::MergedChannels );
	
	// register process type for queued connection
	qRegisterMetaType<QProcess::ProcessError>( "QProcess::ProcessError" ); 
	qRegisterMetaType<QProcess::ExitStatus>( "QProcess::ExitStatus" ); 
	qRegisterMetaType<QProcess::ProcessState>( "QProcess::ProcessState" ); 
	
	// connections
	connect( mProcess, SIGNAL( error( QProcess::ProcessError ) ), this, SLOT( error( QProcess::ProcessError ) ) );
	connect( mProcess, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( finished( int, QProcess::ExitStatus ) ) );
	connect( mProcess, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
	connect( mProcess, SIGNAL( started() ), this, SLOT( started() ) );
	connect( mProcess, SIGNAL( stateChanged( QProcess::ProcessState ) ), this, SLOT( stateChanged( QProcess::ProcessState ) ) );
	connect( this, SIGNAL( executeProcessRequire() ), this, SLOT( internalExecuteProcess() ) );
}

ConsoleManager::~ConsoleManager()
{ delete mProcess; }

void ConsoleManager::run()
{
	// set thread running
	mRunning = true;
	
	// tracking commands
	forever
	{
		// exit thread needed
		if ( !mRunning )
			break;
		
		// if running continue
		if ( !mProcess || mProcess->state() != QProcess::NotRunning )
			continue;
		
		// execute next task is available
		if ( !mCommands.isEmpty() )
		{
			emit executeProcessRequire();
			exec();
		}
		// wait 100 milliseconds before checking again
		else
			msleep( 100 );
	}
	
	// set thread not running
	mRunning = false;
}

void ConsoleManager::terminate()
{
	// force loop to return
	mRunning = false;
	
	// if process actif kill the current process
	if ( mProcess )
		mProcess->kill();
	
	// quit event loop
	quit();
	
	// wait for thread finish
	wait();
}

void ConsoleManager::error( QProcess::ProcessError e )
{
	switch ( e )
	{
		case QProcess::FailedToStart:
			emit error( e, tr( "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program." ) );
			break;
		case QProcess::Crashed:
			emit error( e, tr( "The process crashed some time after starting successfully." ) );
			break;
		case QProcess::Timedout:
			emit error( e, tr( "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again." ) );
			break;
		case QProcess::WriteError:
			emit error( e, tr( "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel." ) );
			break;
		case QProcess::ReadError:
			emit error( e, tr( "An error occurred when attempting to read from the process. For example, the process may not be running." ) );
			break;
		case QProcess::UnknownError:
		default:
			emit error( e, tr( "An unknown error occurred. This is the default return value of error()." ) );
			break;
	}
	
	// remove command from list
	removeCommand( currentCommand() );
	
	// quit event loop
	quit();
	
	qWarning( "error" );
}

void ConsoleManager::finished( int i, QProcess::ExitStatus e )
{
	// finished message
	switch ( e )
	{
		case QProcess::NormalExit:
			emit finished( i, e, tr( "%1, the process exited normally." ).arg( currentCommand()->text() ) );
			break;
		case QProcess::CrashExit:
			emit finished( i, e, tr( "%1, the process crashed." ).arg( currentCommand()->text() ) );
			break;
		default:
			emit finished( i, e, tr( "%1, an unknown error occurred." ).arg( currentCommand()->text() ) );
	}
	
	// remove command from list
	removeCommand( currentCommand() );
	
	qWarning( "finished" );
}

void ConsoleManager::readyRead()
{
	// get datas
	const QByteArray a = mProcess->readAllStandardOutput();
	
	// get current command
	pCommand* c = currentCommand();
	
	// append data to parser if available
	if ( c && c->parser() )
		c->parser()->addContents( a );
	
	// emit data
	emit readyRead( a );
	
	qWarning( "readyread" );
}

void ConsoleManager::started()
{
	emit started( currentCommand()->text() );
	
	qWarning( "started" );
}

void ConsoleManager::stateChanged( QProcess::ProcessState e )
{
	switch ( e )
	{
		case QProcess::NotRunning:
			emit stateChanged( e, tr( "The process is not running." ) );
			// quit event loop
			quit();
			break;
		case QProcess::Starting:
			emit stateChanged( e, tr( "The process is starting, but the program has not yet been invoked." ) );
			break;
		case QProcess::Running:
			emit stateChanged( e, tr( "The process is running and is ready for reading and writing." ) );
			break;
	}
	
	qWarning( "state changed" );
}

void ConsoleManager::sendRawData( const QByteArray& a )
{
	if ( mProcess && mProcess->state() != QProcess::NotRunning )
	{
		// send raw command to process
		qWarning( "raw bytes writen: %d", mProcess->write( a ) );
	}
	else
		qWarning( "Can't send raw data to console" );
}

void ConsoleManager::addCommand( pCommand* c )
{
	if ( !mCommands.contains( c ) )
		mCommands << c;
}

void ConsoleManager::addCommands( const QCommandList& l )
{
	foreach ( pCommand* c, l )
		addCommand( c );
}

void ConsoleManager::removeCommand( pCommand* c )
{
	if ( mCommands.contains( c ) )
		delete mCommands.takeAt( mCommands.indexOf( c ) );
}

void ConsoleManager::removeCommands( const QCommandList& l )
{
	foreach ( pCommand* c, l )
		removeCommand( c );
}

void ConsoleManager::internalExecuteProcess()
{
	foreach ( pCommand* c, mCommands )
	{
		// if last was error, cancel this one if it want to
		if ( c->skipOnError() && mProcess->error() != QProcess::UnknownError )
		{
			removeCommand( c );
			continue;
		}
		
		// execute command
		mProcess->setWorkingDirectory( c->workingDirectory() );
		mProcess->start( c->command(), c->arguments() );
		
		// exit
		return;
	}
}

