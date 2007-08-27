/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox <pasnox@gmail.com>
** Project   : pConsoleManager
** FileName  : pConsoleManager.cpp
** Date      : mar. aout 21 20:52:27 2007
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
	
#ifndef QT_NO_DEBUG
	// testing console :)
	pCommandList l;
	pCommand* c;
	
	c = new pCommand;
	c->setText( "Listing current directory (working)" );
#ifdef Q_WS_WIN
	c->setCommand( "cmd" );
	c->setArguments( QStringList() << "/k" << "dir" << "&&" << "exit" );
#else
	c->setCommand( "ls" );
#endif
	c->setSkipOnError( false );
	l << c;
	
	c = new pCommand;
	c->setText( "Listing current directory (not working)" );
#ifdef Q_WS_WIN
	c->setCommand( "cmddfdf" );
	c->setArguments( QStringList() << "/k" << "dir" << "&&" << "exit" );
#else
	c->setCommand( "lsdfdf" );
#endif
	c->setSkipOnError( false );
	l << c;
	
	addCommands( l );
#endif
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
	// emit signal error
	emit commandError( currentCommand(), e );
	
#ifndef Q_WS_WIN
	// if process failed/crash remove command
	if ( e == QProcess::FailedToStart || e == QProcess::Crashed )
		removeCommand( currentCommand() );
#endif
}

void pConsoleManager::finished( int i, QProcess::ExitStatus e )
{
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
	
	// emit signal
	emit commandReadyRead( c, a );
}

void pConsoleManager::started()
{
	// emit signal
	emit commandStarted( currentCommand() );
}

void pConsoleManager::stateChanged( QProcess::ProcessState e )
{
	// emit signal state changed
	emit commandStateChanged( currentCommand(), e );
	
#ifdef Q_WS_WIN
	// if process crash remove command
	if ( e == QProcess::NotRunning && ( QProcess::error() == QProcess::FailedToStart || QProcess::error() == QProcess::Crashed ) )
		removeCommand( currentCommand() );
#endif
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
		mCommands.takeAt( mCommands.indexOf( c ) )->deleteLater();
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
