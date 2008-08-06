/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
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
/*!
	\file pConsoleManager.cpp
	\date 2008-01-14T00:36:50
	\author Filipe AZEVEDO aka Nox PasNox <pasnox@gmail.com>
	\brief Implementation of pConsoleManager class
*/

#include <QTimer>

#include "pConsoleManager.h"
#include "pCommandParser.h"
#include "../pMonkeyStudio.h"
#include "../workspace/pFileManager.h"
#include "../coremanager/MonkeyCore.h"

/*!
	Defines maximum count of lines, which are storing in the buffer for parsing
*/
static const int MAX_LINES = 4; //Maximum lines count, that can be parsed by Monkey. Than less - than better perfomance

using namespace pMonkeyStudio;

/*!
	Constructor of class
	\param o Parent object
*/
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
	/*FIXME need to use environment(), that returns systemEnvironment, 
	if process env was not changed, but it's not works on Qt 4.3.4 X11 */
	QStringList l = systemEnvironment();
	if ( ( i = l.indexOf( QRegExp( "^LANG=.*$" ) ) ) != -1 )
		l.removeAt( i );
	setEnvironment( l );
	
	// set status tip for
	mStopAction->setStatusTip( tr( "Stop the currently running command" ) );
	mStopAction->setEnabled( false );
	// mixe channels
	setReadChannelMode( QProcess::MergedChannels );
	// connections
	
	// :-/
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

/*!
	Destructor of class
*/
pConsoleManager::~pConsoleManager()
{
	terminate();
	waitForFinished();
	kill();
}

/*!
	Append parser to list of alailable parsers (which could be used for some commands)
	\param p Pointer to parser
*/
void pConsoleManager::addParser( pCommandParser* p )
{
	if ( p && !mParsers.contains( p->name() ) )
	{
		mParsers[p->name()] = p;
		connect( p, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ) );
	}
}

/*!
	Remove parser to list of available parsers (which could be used for some commands)
	\param p Pointer to parser
*/
void pConsoleManager::removeParser( pCommandParser* p )
{
	if ( p && mParsers.contains( p->name() ) )
	{
		disconnect( p, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ), this, SIGNAL( newStepAvailable( const pConsoleManager::Step& ) ) );
		mParsers.remove( p->name() );
	}
}

/*!
	Remove parser to list of available parsers (which could be used for some commands)
	\param s Name of parser
*/
void pConsoleManager::removeParser( const QString& s )
{ removeParser( mParsers.value( s ) ); }

/*!
	Convert path separators to native for OS
*/
QString pConsoleManager::nativeSeparators( const QString& s )
{ return QDir::toNativeSeparators( s ); }

/*!
	Check, if string contains spaces, and, if it do - add quotes <"> to start and end of it
	\param s Source string
	\return Result string
*/
QString pConsoleManager::quotedString( const QString& s )
{
	QString t = s;
	if ( t.contains( " " ) && !t.startsWith( '"' ) && !t.endsWith( '"' ) )
		t.prepend( '"' ).append( '"' );
	return t;
}

/*!
	Replace internal varibles in the string with it's values
	
	FIXME function should be replaced with using of VariablesManager
	\param s Source string
	\return Result string
*/
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

/*!
	Prepare command for starting (set internal variables)
	
	\param c Command for execution
	\return Command for execution
	\retval Command, gived as parameter
*/
pCommand pConsoleManager::processCommand( pCommand c )
{
	// process variables
	c.setCommand( processInternalVariables( c.command() ) );
	c.setArguments( processInternalVariables( c.arguments() ) );
	c.setWorkingDirectory( processInternalVariables( c.workingDirectory() ) );
	// return command
	return c;
}

/*!
	Search command in the list by it's text, or return empty one
	\param l List of commands, where to search
	\param s Text of command for searhing
	\return Finded command, or empty command if not finded
*/
pCommand pConsoleManager::getCommand( const pCommandList& l, const QString& s )
{
	foreach ( pCommand c, l )
		if ( c.text() == s )
			return c;
	return pCommand();
}

/*!
	FIXME PasNox, comment please
	
*/
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

/*!
	Handler of timer event
	
	Exucutes next command, if there is available in the list, and no currently running commands
	FIXME Check, if it's realy nessesery to use timer
	\param e Timer event
*/
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

/*!
	Emit signal, when process failing with error
	\param e Process error
*/
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

/*!
	Handler of finishing of execution of command
	
	\param i Ask PasNox, what is it
	\param e Exit status of process
*/
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

/*!
	Handler or 'ready read' event from child process
	
	Reads output from process and tryes to parse it
*/
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

/*!
	Handler of 'started' event from child process
*/
void pConsoleManager::started()
{
	// enable stop action
	mStopAction->setEnabled( true );
	// emit signal
	emit commandStarted( currentCommand() );
}

/*!
	Handler of changing status of child process
	\param e New process state
*/
void pConsoleManager::stateChanged( QProcess::ProcessState e )
{
	// emit signal state changed
	emit commandStateChanged( currentCommand(), e );
	// remove command if crashed and state 0
	if ( QProcess::error() == QProcess::FailedToStart && e == QProcess::NotRunning )
		removeCommand( currentCommand() );
}

/*!
	Create command and append it to list for executing
	\param s Command to execute
*/
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
		write( a  );
	}
	else
		warning( tr( "sendRawData..." ), tr( "Can't send raw data to console" ) );
}

/*!
	Try to stop current command. Stop can be forced
	
	\param b Force stop. If false - process will just try to terminate child by executing
		terminate () function, 
		if true - process will be killed in 30 seconds, if will not terminate self
	
	FIXME Check, do it's possible, that process will be terminated correctly, and timer will
	kill next executed command
*/
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

/*!
	Add command to list for executing
	\param c  Command
*/
void pConsoleManager::addCommand( const pCommand& c )
{
	if ( c.isValid() )
		mCommands << c;
}

/*!
	Add list of command for executing
	\param l List of commands
*/
void pConsoleManager::addCommands( const pCommandList& l )
{
	foreach ( pCommand c, l )
		addCommand( c );
}

/*!
	Remove command from list of commands for executing
	
	\param c Command
*/
void pConsoleManager::removeCommand( const pCommand& c )
{
	if ( mCommands.contains( c ) )
		mCommands.removeAt( mCommands.indexOf ( c ) );
}

/*!
	Remove list of commands from list for executing
	\param l List of commands
*/
void pConsoleManager::removeCommands( const pCommandList& l )
{
	foreach ( pCommand c, l )
		removeCommand( c );
}

/*!
	Execute commands, which currently are in the list
*/
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

/*!
	Parse output of command, which are storing in the buffer, using parsers.
	
	\param commandFinished If command already are finished, make processing while
	buffer will not be empty. If not finished - wait for further output.
*/
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

