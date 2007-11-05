/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pConsoleManager.cpp
** Date      : 2007-11-04T22:45:05
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pConsoleManager.h"
#include "pCommandParser.h"
#include "pMonkeyStudio.h"
#include "pFileManager.h"
#include "pAction.h"

#include <QTimer>

using namespace pMonkeyStudio;

pConsoleManager::pConsoleManager( QObject* o )
	: QProcess( o ), mStopAction( new pAction( "aStopAction", QIcon( ":/console/icons/console/stop.png" ), tr( "Stop current command" ), tr( "Ctrl+End" ), tr( "Console Manager" ) ) )
{
	// set status tip for
	mStopAction->setStatusTip( tr( "Stop the currently running command" ) );
	mStopAction->setEnabled( false );
	// mixe channels
	setReadChannelMode( QProcess::MergedChannels );
	// connections
	connect( this, SIGNAL( error( QProcess::ProcessError ) ), this, SLOT( error( QProcess::ProcessError ) ) );
	connect( this, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( finished( int, QProcess::ExitStatus ) ) );
	connect( this, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
	connect( this, SIGNAL( started() ), this, SLOT( started() ) );
	connect( this, SIGNAL( stateChanged( QProcess::ProcessState ) ), this, SLOT( stateChanged( QProcess::ProcessState ) ) );
	connect( mStopAction, SIGNAL( triggered() ), this, SLOT( stopCurrentCommand() ) );
	// start timerEvent
	mTimerId = startTimer( 100 );
}

pConsoleManager::~pConsoleManager()
{
	terminate();
	waitForFinished();
	kill();
}

void pConsoleManager::addParser( pCommandParser* p )
{
	if ( p && !mParsers.contains( p->name() ) )
	{
		mParsers[p->name()] = p;
		connect( p, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ) );
	}
}

void pConsoleManager::removeParser( pCommandParser* p )
{
	if ( p && mParsers.contains( p->name() ) )
	{
		disconnect( p, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ) );
		mParsers.remove( p->name() );
	}
}

void pConsoleManager::removeParser( const QString& s )
{ removeParser( mParsers.value( s ) ); }

QString pConsoleManager::processInternalVariables( const QString& s )
{
	QString v = s;
	v.replace( "$cpp$", QDir::toNativeSeparators( pFileManager::instance()->currentProjectPath() ) );
	v.replace( "$cp$", QDir::toNativeSeparators( pFileManager::instance()->currentProjectFile() ) );
	v.replace( "$cfp$", QDir::toNativeSeparators( pFileManager::instance()->currentChildPath() ) );
	v.replace( "$cf$", QDir::toNativeSeparators( pFileManager::instance()->currentChildFile() ) );
	v.replace( "$cip$", QDir::toNativeSeparators( pFileManager::instance()->currentItemPath() ) );
	v.replace( "$ci$", QDir::toNativeSeparators( pFileManager::instance()->currentItemFile() ) );
	return v;
}

pCommand pConsoleManager::getCommand( const pCommandList& l, const QString& s )
{
	foreach ( pCommand c, l )
		if ( c.text() == s )
			return c;
	return pCommand();
}

pCommandList pConsoleManager::recursiveCommandList( const pCommandList& l, pCommand c )
{
	pCommandList cl;
	// check if chan command
	QStringList lc = c.command().split( ";" );
	if ( lc.count() > 1 )
	{
		foreach ( QString s, lc )
			cl << recursiveCommandList( l, getCommand( l, s ) );
	}
	else
	{
		// process internal variables
		c.setCommand( processInternalVariables( c.command() ) );
		c.setArguments( processInternalVariables( c.arguments() ) );
		c.setWorkingDirectory( processInternalVariables( c.workingDirectory() ) );
		cl << c;
	}
	// return list
	return cl;
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
	// need emulate state 0 for linux
#ifndef Q_WS_WIN
	if ( e == QProcess::FailedToStart )
		stateChanged( QProcess::NotRunning );
#endif
}

void pConsoleManager::finished( int i, QProcess::ExitStatus e )
{
	// emit signal finished
	emit commandFinished( currentCommand(), i, e );
	// remove command from list
	removeCommand( currentCommand() );
	// disable stop action
	mStopAction->setEnabled( false );
}

void pConsoleManager::readyRead()
{
	// get data
	QByteArray d = readAll();
	// append data to buffer to parse
	mBuffer.buffer().append( d );
	// get current command
	pCommand c = currentCommand();
	// try parse output
	if ( c.isValid() )
	{
		// read complete lines
		while ( mBuffer.canReadLine() )
		{
			QByteArray a = mBuffer.readLine();
			foreach ( QString s, mCurrentParsers )
				if ( pCommandParser* p = mParsers.value( s ) )
					if ( p->processParsing( a ) )
						break;
		}
	}
	// emit signal
	emit commandReadyRead( c, d );
}

void pConsoleManager::started()
{
	// disable stop action
	mStopAction->setEnabled( true );
	// emit signal
	emit commandStarted( currentCommand() );
}

void pConsoleManager::stateChanged( QProcess::ProcessState e )
{
	// emit signal state changed
	emit commandStateChanged( currentCommand(), e );
	// remove command if crashed and state 0
	if ( QProcess::error() == QProcess::FailedToStart && e == QProcess::NotRunning )
		removeCommand( currentCommand() );
}

void pConsoleManager::sendRawCommand( const QString& s )
{ addCommand( pCommand( tr( "User Raw Command" ), s, QString::null, false ) ); }

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
		warning( tr( "sendRawData..." ), tr( "Can't send raw data to console" ) );
}

void pConsoleManager::stopCurrentCommand( bool b )
{
	if ( state() != QProcess::NotRunning )
	{
		// terminate properly
		terminate();
		// if force, wait 30 seconds, then kill
		if ( b )
			QTimer::singleShot( 30, this, SLOT( kill() ) );
	}
}

void pConsoleManager::addCommand( const pCommand& c )
{
	if ( c.isValid() )
		mCommands << c;
}

void pConsoleManager::addCommands( const pCommandList& l )
{
	foreach ( pCommand c, l )
		addCommand( c );
}

void pConsoleManager::removeCommand( const pCommand& c )
{
	if ( mCommands.contains( c ) )
		mCommands.removeAt( mCommands.indexOf ( c ) );
}

void pConsoleManager::removeCommands( const pCommandList& l )
{
	foreach ( pCommand c, l )
		removeCommand( c );
}

void pConsoleManager::executeProcess()
{
	foreach ( pCommand c, mCommands )
	{
		// if last was error, cancel this one if it want to
		if ( c.skipOnError() && QProcess::error() != QProcess::UnknownError )
		{
			// emit command skipped
			emit commandSkipped( c );
			// remove command from command to execute
			removeCommand( c );
			// execute next
			continue;
		}
		// set current parsers list
		// parsers comamnd want to test/check
		mCurrentParsers = c.parsers();
		// check if need tryall, and had all other parsers if needed at end
		if ( c.tryAllParsers() )
			foreach ( QString s, parsersName() )
				if ( !mCurrentParsers.contains( s ) )
					mCurrentParsers << s;
		// clear buffer
		mBuffer.buffer().clear();
		mBuffer.open( QBuffer::ReadOnly );
		// execute command
		setWorkingDirectory( c.workingDirectory() );
		start( QString( "%1 %2" ).arg( c.command() ).arg( c.arguments() ) );
		// exit
		return;
	}
}
