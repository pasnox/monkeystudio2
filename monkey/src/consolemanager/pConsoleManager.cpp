/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox <pasnox@gmail.com>
** Project   : pConsoleManager
** FileName  : pConsoleManager.cpp
** Date      : mar. ao�t 21 20:52:27 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pConsoleManager.h"

#include <QTimer>

pConsoleManager::pConsoleManager( QObject* o )
	: QProcess( o )
{
	// mixe channels
	setReadChannelMode( QProcess::MergedChannels );
	
	// connections
	connect( this, SIGNAL( error( QProcess::ProcessError ) ), this, SLOT( error( QProcess::ProcessError ) ) );
	connect( this, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( finished( int, QProcess::ExitStatus ) ) );
	connect( this, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
	connect( this, SIGNAL( started() ), this, SLOT( started() ) );
	connect( this, SIGNAL( stateChanged( QProcess::ProcessState ) ), this, SLOT( stateChanged( QProcess::ProcessState ) ) );
	
	// start timerEvent
	mTimerId = startTimer( 100 );
	
	// testing console :)
	pCommandList l;
	pCommand* c;
	
	c = new pCommand;
	c->setText( "Executing calc..." );
	c->setCommand( "calcg" );
	c->setSkipOnError( false );
	l << c;
	
	c = new pCommand;
	c->setText( "Executing assistant..." );
	c->setCommand( "assistant" );
	c->setSkipOnError( true );
	l << c;
	
	c = new pCommand;
	c->setText( "Executing konqueror..." );
	c->setCommand( "explorer" );
	c->setSkipOnError( false );
	l << c;
	
	addCommands( l );
}

pConsoleManager::~pConsoleManager()
{
	terminate();
	waitForFinished();
	kill();
}

void pConsoleManager::timerEvent( QTimerEvent* e )
{
	if ( e->timerId() == mTimerId )
	{
		
	// if running continue
		if ( state() != QProcess::NotRunning )
			return;
		
		// execute next task is available
		if ( !mCommands.isEmpty() )
			executeProcess();
	}
}

void pConsoleManager::error( QProcess::ProcessError e )
{
	QString s;
	switch ( e )
	{
		case QProcess::FailedToStart:
			s = tr( "Error#: %1, %2\nThe process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program." );
			break;
		case QProcess::Crashed:
			s = tr( "Error#: %1, %2\nThe process crashed some time after starting successfully." );
			break;
		case QProcess::Timedout:
			s = tr( "Error#: %1, %2\nThe last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again." );
			break;
		case QProcess::WriteError:
			s = tr( "Error#: %1, %2\nAn error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel." );
			break;
		case QProcess::ReadError:
			s = tr( "Error#: %1, %2\nAn error occurred when attempting to read from the process. For example, the process may not be running." );
			break;
		case QProcess::UnknownError:
		default:
			s = tr( "Error#: %1, %2\nAn unknown error occurred. This is the default return value of error()." );
			break;
	}
	
	// show warning
	qWarning( qPrintable( s.arg( e ).arg( currentCommand()->text() ) ) );
	
	// emit signal error
	emit commandError( currentCommand(), e );
	
	// remove command from list
	removeCommand( currentCommand() );
}

void pConsoleManager::finished( int i, QProcess::ExitStatus e )
{
	QString s;
	switch ( e )
	{
		case QProcess::NormalExit:
			s = tr( "Finished#: %1, Status Code: %2, %3\nThe process exited normally." );
			break;
		case QProcess::CrashExit:
			s = tr( "Finished#: %1, Status Code: %2, %3\nThe process crashed." );
			break;
		default:
			s = tr( "Finished#: %1, Status Code: %2, %3\nAn unknown error occurred." );
	}
	
	// show warning
	qWarning( qPrintable( s.arg( i ).arg( e ).arg( currentCommand()->text() ) ) );
	
	// emit signal finished
	emit commandFinished( currentCommand(), i, e );
	
	// remove command from list
	removeCommand( currentCommand() );
}

void pConsoleManager::readyRead()
{
	// get datas
	const QByteArray a = readAllStandardOutput();
	
	// get current command
	pCommand* c = currentCommand();
	
	// append data to parser if available
	if ( c && c->parser() )
		c->parser()->addContents( a );
	
	// warning
	qWarning( "Datas available: %s", a );
	
	// emit signal
	emit commandReadyRead( c, a );
}

void pConsoleManager::started()
{
	// warning
	qWarning( "Process Started: %s", qPrintable( currentCommand()->text() ) );
	
	// emit signal
	emit commandStarted( currentCommand() );
}

void pConsoleManager::stateChanged( QProcess::ProcessState e )
{
	QString s;
	switch ( e )
	{
		case QProcess::NotRunning:
			s = tr( "State Changed#: %1, %2\nThe process is not running." );
			break;
		case QProcess::Starting:
			s = tr( "State Changed#: %1, %2\nThe process is starting, but the program has not yet been invoked." );
			break;
		case QProcess::Running:
			s = tr( "State Changed#: %1, %2\nThe process is running and is ready for reading and writing." );
			break;
	}
	
	// show warning
	qWarning( qPrintable( s.arg( e ).arg( currentCommand()->text() ) ) );
	
	// emit signal state changed
	emit commandStateChanged( currentCommand(), e );
}

void pConsoleManager::sendRawData( const QByteArray& a )
{
	if ( state() != QProcess::NotRunning )
	{
		// if program is starting wait
		while ( state() == QProcess::Starting )
			QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
		
		// send raw command to process
		qWarning( "sendRawData bytes written: %d", write( a ) );
	}
	else
		qWarning( "Can't send raw data to console" );
}

void pConsoleManager::addCommand( pCommand* c )
{
	if ( !mCommands.contains( c ) )
		mCommands << c;
}

void pConsoleManager::addCommands( const pCommandList& l )
{
	foreach ( pCommand* c, l )
		addCommand( c );
}

void pConsoleManager::removeCommand( pCommand* c )
{
	if ( mCommands.contains( c ) )
		delete mCommands.takeAt( mCommands.indexOf( c ) );
}

void pConsoleManager::removeCommands( const pCommandList& l )
{
	foreach ( pCommand* c, l )
		removeCommand( c );
}

void pConsoleManager::executeProcess()
{
	foreach ( pCommand* c, mCommands )
	{
		// if last was error, cancel this one if it want to
		if ( c->skipOnError() && QProcess::error() != QProcess::UnknownError )
		{
			removeCommand( c );
			continue;
		}
		
		// execute command
		setWorkingDirectory( c->workingDirectory() );
		start( c->command(), c->arguments() );
		
		// exit
		return;
	}
}
