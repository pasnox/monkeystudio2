/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
****************************************************************************/
/*!
    \file pConsoleManager.cpp
    \date 2008-01-14T00:36:50
    \author Filipe AZEVEDO aka Nox PasNox <pasnox@gmail.com>
    \brief Implementation of pConsoleManager class
*/

#include <QTimer>
#include <QDir>
#include <QLibrary>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

#include "pConsoleManager.h"
#include "CommandParser.h"
#include "AbstractCommandParser.h"

#include "coremanager/MonkeyCore.h"
#include <pActionsManager.h>
#include "variablesmanager/VariablesManager.h"

/*!
    Defines maximum count of lines, which are storing in the buffer for parsing
*/
static const int MAX_LINES = 4; //Maximum lines count, that can be parsed by Monkey. Than less - than better perfomance
#define QUOTE_STRING "\""

/*!
    Constructor of class
    \param o Parent object
*/
pConsoleManager::pConsoleManager( QObject* o )
    : QProcess( o )
{
    pActionsManager* am = MonkeyCore::actionsManager();
    am->setPathPartTranslation( "Console Manager", tr( "Console Manager" ) );
    
#ifndef Q_OS_MAC
    mStopAction = MonkeyCore::actionsManager()->newAction( "Console Manager", tr( "Pause" ), "aStopAction" );
#else
    mStopAction = MonkeyCore::actionsManager()->newAction( "Console Manager", tr( "Alt+End" ), "aStopAction" );
#endif
    mStopAction->setIcon( QIcon( ":/console/icons/console/stop.png" ) );
    mStopAction->setText( tr( "Stop the currenttly running command" ) );
    mStopAction->setToolTip( mStopAction->text() );
    mStopAction->setStatusTip( mStopAction->text() );
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
    mStopAttempt = 0;
    
    CommandParser::installParserCommand();
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
void pConsoleManager::addParser( AbstractCommandParser* p )
{
    Q_ASSERT( p );
    
    if (mParsers.contains(p->name()))
    {
        qDebug() << QString("Parser '%1' already had been added").arg(p->name());
        return;
    }
    
    mParsers[p->name()] = p;
    connect( p, SIGNAL( newStepAvailable( const pConsoleManagerStep& ) ), this, SIGNAL( newStepAvailable( const pConsoleManagerStep& ) ) );
    connect( p, SIGNAL( newStepsAvailable( const pConsoleManagerStepList& ) ), this, SIGNAL( newStepsAvailable( const pConsoleManagerStepList& ) ) );
}

/*!
    Remove parser to list of available parsers (which could be used for some commands)
    \param p Pointer to parser
*/
void pConsoleManager::removeParser( AbstractCommandParser* p )
{
    if ( p && mParsers.contains( p->name() ) )
    {
        disconnect( p, SIGNAL( newStepAvailable( const pConsoleManagerStep& ) ), this, SIGNAL( newStepAvailable( const pConsoleManagerStep& ) ) );
        disconnect( p, SIGNAL( newStepsAvailable( const pConsoleManagerStepList& ) ), this, SIGNAL( newStepsAvailable( const pConsoleManagerStepList& ) ) );
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
    Returns pointer to parser, or NULL if not found
 */
AbstractCommandParser* pConsoleManager::getParser(const QString& name) const
{
    return mParsers.value(name);
}

/*!
    Replace internal varibles in the string with it's values

    \param s Source string
    \return Result string
*/
QString pConsoleManager::processInternalVariables( const QString& string, bool quoteValues ) const
{
    return VariablesManager::instance()->replaceAllVariables( string, quoteValues );
}

/*!
    Prepare command for starting (set internal variables)

    \param c Command for execution
    \return Command for execution
    \retval Command, gived as parameter
*/
pCommand pConsoleManager::processCommand( const pCommand& command ) const
{
    pCommand cmd = command;
    cmd.setCommand( processInternalVariables( cmd.command(), true ) );
    cmd.setWorkingDirectory( processInternalVariables( cmd.workingDirectory(), false ) );
    
    if ( cmd.project() ) {
        const QString filePath = cmd.project()->targetFilePath( XUPProjectItem::TargetType( cmd.executableCheckingType() ) );
        const QString qFilePath = quotedFilePath( filePath );
        cmd.setCommand( cmd.command().replace( "$target$", qFilePath, Qt::CaseInsensitive ) );
        cmd.setWorkingDirectory( cmd.workingDirectory().replace( "$target$", filePath, Qt::CaseInsensitive ) );
    }
    
    return cmd;
}

/*!
    Check, if string contains spaces, and, if it do - add quotes <"> to start and end of it
    \param s Source string
    \return Result string
*/
QString pConsoleManager::quotedFilePath( const QString& filePath )
{
    if ( filePath.contains( " " ) && !filePath.startsWith( QUOTE_STRING ) && !filePath.endsWith( QUOTE_STRING ) ) {
        return QString( filePath ).prepend( QUOTE_STRING ).append( QUOTE_STRING );
    }
    
    return filePath;
}

QString pConsoleManager::unquotedFilePath( const QString& filePath )
{
    if ( filePath.contains( " " ) && filePath.startsWith( QUOTE_STRING ) && filePath.endsWith( QUOTE_STRING ) ) {
        QString s = filePath;
        s.remove( 0, 1 );
        s.chop( 1 );
        return s;
    }
    
    return filePath;
}

/*!
    Return human readable string of a QProcess::ProcessError

    \param error The error to get string from
*/
QString pConsoleManager::errorToString( QProcess::ProcessError error )
{
    switch ( error )
    {
        case QProcess::FailedToStart:
            return tr( "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program." );
        case QProcess::Crashed:
            return tr( "The process crashed some time after starting successfully." );
        case QProcess::Timedout:
            return tr( "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again." );
        case QProcess::WriteError:
            return tr( "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel." );
        case QProcess::ReadError:
            return tr( "An error occurred when attempting to read from the process. For example, the process may not be running." );
        case QProcess::UnknownError:
        default:
            return tr( "An unknown error occurred. This is the default return value of error()." );
    }
}

/*!
    Return the help for variables.
*/
QString pConsoleManager::variablesHelp()
{
    return pConsoleManager::tr(
        "<b>Console Manager Variables</b><br><br>"
        "<b>$cpp$</b> : Current project path<br>"
        "<b>$cp$</b> : Current project filepath<br>"
        "<b>$target$</b> : Current project target<br>"
        "<b>$cfp$</b> : Current tab path<br>"
        "<b>$cf$</b> : Current tab filepath<br>"
        "<b>$cip$</b> : Current item path<br>"
        "<b>$ci$</b> : Current item filepath"
    );
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
    const pCommand command = currentCommand();
    // parse output
    parseOutput( true );
    // emit signal finished
    emit commandFinished( command, i, e );
    // remove command from list
    removeCommand( command );
    // disable stop action
    mStopAction->setEnabled( false );
    // clear buffer
    mBuffer.buffer().clear();
}

/*!
    Handler or 'ready read' event from child process

    Reads output from process and tryes to parse it
*/
void pConsoleManager::readyRead()
{
    // append data to buffer to parse
    const QByteArray data = readAll();
    mBuffer.buffer().append( data );
    
    // get current command
    const pCommand command = currentCommand();
    
    // try parse output
    if ( !command.isValid() )
        return;
    
    /*Alrorithm is not ideal, need fix, if will be problems with it
        Some text, that next parser possible to parse, can be removed
        And, possible, it's not idealy quick.   hlamer
        */

    parseOutput( false );

    // emit signal
    emit commandReadyRead( command, data );
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
{ addCommand( pCommand( tr( "User Raw Command" ), s, false ) ); }

void pConsoleManager::sendRawData( const QByteArray& a )
{
    if ( state() != QProcess::NotRunning )
    {
        // if program is starting wait
        while ( state() == QProcess::Starting )
            QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
        // send raw command to process
        write( a );
    }
    else
        emit warning( tr( "Can't send raw data to console" ) );
}

/*!
    Try to stop current command. if stop attempt for same commend = 3 the command is killed
*/
void pConsoleManager::stopCurrentCommand()
{
    if ( state() != QProcess::NotRunning )
    {
        // terminate properly
        terminate();

        // increment attempt
        mStopAttempt++;

        // auto kill if attempt = 3
        if ( mStopAttempt == 3 )
        {
            mStopAttempt = 0;
            kill();
        }
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
void pConsoleManager::addCommands( const pCommand::List& l )
{
    foreach ( const pCommand& c, l )
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
void pConsoleManager::removeCommands( const pCommand::List& l )
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
            emit commandSkipped( c );
            removeCommand( c );
            continue;
        }
        
        switch ( c.executableCheckingType() ) {
            case XUPProjectItem::NoTarget: {
                break;
            }
            case XUPProjectItem::ServicesTarget: {
                if ( QDesktopServices::openUrl( QUrl( c.command() ) ) ) {
                    finished( 0, QProcess::NormalExit );
                    return;
                }
                
                break;
            }
            case XUPProjectItem::DesktopTarget: {
#if defined( Q_OS_WIN )
                c.setCommand( QString( "start %1" ).arg( c.command() ) );
#elif defined( Q_OS_MAC )
                c.setCommand( QString( "open %1" ).arg( c.command() ) );
#elif defined( Q_OS_UNIX )
                c.setCommand( QString( "xdg-open %1" ).arg( c.command() ) );
#endif
                break;
            }
            case XUPProjectItem::DefaultTarget:
            case XUPProjectItem::DebugTarget:
            case XUPProjectItem::ReleaseTarget: {
                if ( !c.project() /*&& !c.command().contains( "$target$" )*/ ) {
                    emit commandSkipped( c );
                    removeCommand( c );
                    continue;
                }
                
                /*if ( c.command().contains( "$target$" ) )*/ {
                    const QString filePath = c.project()->targetFilePath( XUPProjectItem::TargetType( c.executableCheckingType() ) );
                    
                    if ( filePath.isEmpty() ) {
                        emit commandSkipped( c );
                        removeCommand( c );
                        continue;
                    }
                    
                    QString commandLine = c.command().replace( "$target$", quotedFilePath( filePath ) );
                    const QFileInfo fi( c.project()->filePath( filePath ) );
                    
                    if ( fi.exists() && !fi.isExecutable() && !QLibrary::isLibrary( fi.absoluteFilePath() ) ) {
#if defined( Q_OS_WIN )
                        commandLine = QString( "start %1" ).arg( quotedFilePath( c.project()->filePath( filePath ) ) );
#elif defined( Q_OS_MAC )
                        commandLine = QString( "open %1" ).arg( quotedFilePath( c.project()->filePath( filePath ) ) );
#elif defined( Q_OS_UNIX )
                        commandLine = QString( "xdg-open %1" ).arg( quotedFilePath( c.project()->filePath( filePath ) ) );
#endif
                    }
                    
                    c.setCommand( commandLine );
                    
                    if ( c.workingDirectory().isEmpty() ) {
                        c.setWorkingDirectory( QFileInfo( filePath ).absolutePath() );
                    }
                }
                break;
            }
            default: {
                Q_ASSERT( 0 );
                break;
            }
        }

        if ( c.tryAllParsers() ) {
            mCurrentParsers = parsersName();
        }
        else {
            mCurrentParsers = c.parsers();
        }
        
        // execute command
        mStopAttempt = 0;
        setWorkingDirectory( c.workingDirectory() );
        
        QStringList variables = mEnvironmentVariablesManager.variables( false );
        
        // unset some variables environments when parsers is defined
        if ( !mCurrentParsers.isEmpty() )
        {
            const QStringList names = QStringList() << "LANG" << "LANGUAGE" << "LC_MESSAGES" << "LC_ALL";
            
            foreach ( const QString& name, names ) {
                const int index = variables.indexOf( QRegExp( QString( "^%1=.*$" ).arg( name ) ) );
                
                if ( index != -1 )
                {
                    variables.removeAt( index );
                }
            }
        }
        
        setEnvironment( variables );
        
        mCommands.first() = c;

        start( c.command().trimmed() );

        mBuffer.open( QBuffer::ReadOnly );
        
        return;
    }
}

/*!
    Parse output of command, which are storing in the buffer, using parsers.

    \param commandFinished If command already are finished, make processing while
    buffer will not be empty. If not finished - wait for further output.
*/
void pConsoleManager::parseOutput( bool commandFinished )
{
    static QStringList strings;
    
    forever {
        // fill parsing buffer
        while ( mBuffer.canReadLine() && strings.count() < MAX_LINES ) {
            strings << QString::fromLocal8Bit( mBuffer.readLine() );
        }
        
#if PARSERS_DEBUG
        qWarning() << "CommandFinished/Strings" << commandFinished << strings;
#endif

        // read last line not ending by a \n on command finished
        if ( commandFinished && strings.count() < MAX_LINES ) {
            const QString line = QString::fromLocal8Bit( mBuffer.readAll() );
            
            if ( !line.isEmpty() ) {
                strings << line;
                
#if PARSERS_DEBUG
                qWarning() << "Add finished command last line" << line;
#endif
            }
        }
        
        if ( strings.isEmpty() ) {
#if PARSERS_DEBUG
            qWarning() << "Empty strings, exiting / CommandFinished" << commandFinished;
#endif
            return;
        }
        
        // try current command parsers
        foreach ( const QString& parserName, mCurrentParsers ) {
            AbstractCommandParser* parser = mParsers.value( parserName );
            
            if ( !parser ) {
#if PARSERS_DEBUG
                qWarning() << "Invalid parser" << parserName;
                qWarning() << mParsers;
#endif
                continue; // foreach
            }
            
            // parse content
#if PARSERS_DEBUG
            qWarning() << "Parsing..." << strings;
#endif
            parser->processParsing( strings );
#if PARSERS_DEBUG
            qWarning() << "Parsed fails..." << strings;
#endif
            
            if ( strings.isEmpty() ) {
                break; // foreach
            }
        }
        
        // discarding unconsuming data
        strings.clear();
    }
}
