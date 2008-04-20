/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pConsoleManager.cpp
** Date      : 2008-01-14T00:36:50
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#include <QTimer>

#include "pConsoleManager.h"
#include "pCommandParser.h"
#include "../pMonkeyStudio.h"
#include "../workspace/pFileManager.h"
#include "../coremanager/MonkeyCore.h"

static const int MAX_LINES = 4; //Maximum lines count, that can be parsed by Monkey. Than less - than better perfomance

using namespace pMonkeyStudio;
pConsoleManager::pConsoleManager( QObject* o )
	: QProcess( o ), mLinesInStringBuffer (0)
{
#ifndef Q_OS_MAC
	mStopAction = new pAction( "aStopAction", QIcon( ":/console/icons/console/stop.png" ), tr( "Stop current command" ), tr( "Pause" ), tr( "Console Manager" ) ) ;
#else
	mStopAction = new pAction( "aStopAction", QIcon( ":/console/icons/console/stop.png" ), tr( "Stop current command" ), tr( "Alt+End" ), tr( "Console Manager" ) ) ; // Mac has no pause key
#endif
	
	// unset some variables environments
	int i;
	QStringList l = environment();
	if ( ( i = l.indexOf( QRegExp( "^LANG=.*$" ) ) ) != -1 )
		l.removeAt( i );
	setEnvironment( l );
	
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
	mStringBuffer.reserve (MAX_LINES *200);
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

QString pConsoleManager::nativeSeparators( const QString& s )
{ return QDir::toNativeSeparators( s ); }

QString pConsoleManager::quotedString( const QString& s )
{
	QString t = s;
	if ( t.contains( " " ) && !t.startsWith( '"' ) && !t.endsWith( '"' ) )
		t.prepend( '"' ).append( '"' );
	return t;
}

QString pConsoleManager::processInternalVariables( const QString& s )
{
	QString v = s;
	v.replace( "$cpp$", nativeSeparators( MonkeyCore::fileManager()->currentProjectPath() ) );
	v.replace( "$cp$", nativeSeparators( MonkeyCore::fileManager()->currentProjectFile() ) );
	v.replace( "$cfp$", nativeSeparators( MonkeyCore::fileManager()->currentChildPath() ) );
	v.replace( "$cf$", nativeSeparators( MonkeyCore::fileManager()->currentChildFile() ) );
	v.replace( "$cip$", nativeSeparators( MonkeyCore::fileManager()->currentItemPath() ) );
	v.replace( "$ci$", nativeSeparators( MonkeyCore::fileManager()->currentItemFile() ) );
	return v;
}

pCommand pConsoleManager::processCommand( pCommand c )
{
	// process variables
	c.setCommand( processInternalVariables( c.command() ) );
	c.setArguments( processInternalVariables( c.arguments() ) );
	c.setWorkingDirectory( processInternalVariables( c.workingDirectory() ) );
	// return command
	return c;
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
	// process variables
	else
	{
		// process variables
		pCommand pc = processCommand( c );
		// set skit on error
		pc.setSkipOnError( true );
		// add to list
		cl << pc;
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
	parseOutput (true);
	// emit signal finished
	emit commandFinished( currentCommand(), i, e );
	// remove command from list
	removeCommand( currentCommand() );
	// disable stop action
	mStopAction->setEnabled( false );
		// clear buffer
	mBuffer.buffer().clear();
	mStringBuffer.clear(); // For perfomance issues
	mLinesInStringBuffer = 0;
}

void pConsoleManager::readyRead()
{
	// append data to buffer to parse
	QByteArray d = readAll ();
	mBuffer.buffer().append( d );
	// get current command
	pCommand c = currentCommand();
	// try parse output
	if (! c.isValid() )
		return;
	
	/*Alrorithm is not ideal, need fix, if will be problems with it
	  Some text, that next parser possible to parse, can be removed
	  And, possible, it's not idealy quick.   hlamer
	*/
	
	parseOutput (false);
		
	// emit signal
	emit commandReadyRead( c, d );
}

void pConsoleManager::started()
{
	// enable stop action
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
		qWarning( "sendRawData bytes written: %s", qPrintable( QString::number( write( a  ) ) ) );
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
		// execute command
		setWorkingDirectory( c.workingDirectory() );
		start( QString( "%1 %2" ).arg( c.command() ).arg( c.arguments() ) );

		mBuffer.open( QBuffer::ReadOnly );
		// exit
		return;
	}
}

void pConsoleManager::parseOutput (bool commandFinished)
{
	bool finished;
	do 
	{
		// Fill string buffer
		while ( mBuffer.canReadLine() && mLinesInStringBuffer < MAX_LINES)
		{
			
			mStringBuffer.append ( QString::fromLocal8Bit (mBuffer.readLine()));
			mLinesInStringBuffer ++;
		}
		
		if ( ! mLinesInStringBuffer )
			return;
		
		finished = true;
		int linesToRemove = 0;
		//try all parsers
		foreach ( QString s, mCurrentParsers )
		{
			pCommandParser* p = mParsers.value( s );
			if ( ! p )
				continue; //for
			linesToRemove =  p->processParsing(&mStringBuffer);
			if (linesToRemove)
				break; //for
		}
		if (linesToRemove == 0 || commandFinished) //need to remove one
			linesToRemove = 1;
		
		if ( ! linesToRemove )
			continue; // do-while
		
		finished = false; //else one iteration of do-while after it
		
		//removing of lines
		mLinesInStringBuffer -= linesToRemove;
		int posEnd = 0;
		while (linesToRemove --)
			posEnd = mStringBuffer.indexOf ('\n', posEnd)+1;
		mStringBuffer.remove (0, posEnd);

	}
	while (!finished && mLinesInStringBuffer);
}

