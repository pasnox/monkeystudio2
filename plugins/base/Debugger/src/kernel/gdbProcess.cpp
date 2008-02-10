
#include "./kernel/gdbProcess.h"
#include <QTimer>
#include <QTimerEvent>

#include <QMessageBox>

static const int MAX_LINES = 4; //Maximum lines count, that can be parsed by Monkey. Than less - than better perfomance


GdbProcess::GdbProcess( QObject* o )
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

//	mTimerId = startTimer( 100 );
	mStringBuffer.reserve (MAX_LINES *200);
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

GdbProcess::~GdbProcess()
{
}

QString GdbProcess::processInternalVariables( const QString& s )
{
	QString v = s;
/*	v.replace( "$cpp$", nativeSeparators( MonkeyCore::fileManager()->currentProjectPath() ) );
	v.replace( "$cp$", nativeSeparators( MonkeyCore::fileManager()->currentProjectFile() ) );
	v.replace( "$cfp$", nativeSeparators( MonkeyCore::fileManager()->currentChildPath() ) );
	v.replace( "$cf$", nativeSeparators( MonkeyCore::fileManager()->currentChildFile() ) );
	v.replace( "$cip$", nativeSeparators( MonkeyCore::fileManager()->currentItemPath() ) );
	v.replace( "$ci$", nativeSeparators( MonkeyCore::fileManager()->currentItemFile() ) );
*/	return v;
}

pCommand GdbProcess::processCommand( pCommand c )
{
	// process variables
	c.setCommand( processInternalVariables( c.command() ) );
	c.setArguments( processInternalVariables( c.arguments() ) );
	c.setWorkingDirectory( processInternalVariables( c.workingDirectory() ) );
	// return command
	return c;
}

void GdbProcess::timerEvent( QTimerEvent* e )
{
/*	if ( e->timerId() == mTimerId )
	{
		// if running continue
		if ( state() != QProcess::NotRunning )
			return;
		// execute next task is available
		if ( !mCommands.isEmpty() )
			executeProcess();
	}*/
}

pCommand GdbProcess::getCommand( const pCommandList& l, const QString& s )
{
	foreach ( pCommand c, l )
		if ( c.text() == s )
			return c;
	return pCommand();
}

pCommandList GdbProcess::recursiveCommandList( const pCommandList& l, pCommand c )
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
		cl << processCommand( c );
	// return list
	return cl;
}


void GdbProcess::error( QProcess::ProcessError e )
{
	// emit signal error
	emit commandError( currentCommand(), e );
	// need emulate state 0 for linux
#ifndef Q_WS_WIN
	if ( e == QProcess::FailedToStart )
		stateChanged( QProcess::NotRunning );
#endif
}

void GdbProcess::finished( int i, QProcess::ExitStatus e )
{
	emit commandFinished( currentCommand(), i, e );
}

void GdbProcess::readyRead()
{
	// append data to buffer to parse
	QByteArray d = readAll ();
	mBuffer.buffer().append( d );
	emit commandReadyRead(mCommands.at(0), d );
}

void GdbProcess::started()
{
	// emit signal
	emit commandStarted( currentCommand() );
}

void GdbProcess::stateChanged( QProcess::ProcessState e )
{
}

void GdbProcess::sendRawCommand( const QString& s )
{ addCommand( pCommand( tr( "User Raw Command" ), s, QString::null, false ) ); }

void GdbProcess::sendRawData( const QByteArray& a )
{
//	if ( state() != QProcess::NotRunning )
	{
		// if program is starting wait
//		while ( state() == QProcess::Starting )
//			QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
		// send raw command to process
//		qWarning( "sendRawData bytes written: %s", qPrintable( QString::number( 
write( a  );// ) ) );
	}
//	else
//		warning( tr( "sendRawData..." ), tr( "Can't send raw data to console" ) );
}

void GdbProcess::stopCurrentCommand( bool b )
{
	if ( state() != QProcess::NotRunning )
	{
		sendRawData( "kill" + crlf);
		sendRawData( "y" + crlf);
		sendRawData( "q" + crlf);

		if(!waitForFinished(1000))
		{
			QMessageBox::warning(NULL,"information", "I can't  stop GDB nomaly.");
			// terminate properly
			terminate();
			if(!waitForFinished(1000))
			{
				QMessageBox::warning(NULL,"information", "I can't  stop GDB, because your program probabily running\nPlease stop your target befor");
				kill();
			}
		}
//	waitForFinished ();
	//	else QMessageBox::warning(NULL,"information", "I stopped GDB nomaly.");
	}
}

void GdbProcess::addCommand( const pCommand& c )
{
	if ( c.isValid() )
		mCommands << c;
}

void GdbProcess::addCommands( const pCommandList& l )
{
	foreach ( pCommand c, l )
		addCommand( c );
}

void GdbProcess::removeCommand( const pCommand& c )
{
	if ( mCommands.contains( c ) )
		mCommands.removeAt( mCommands.indexOf ( c ) );
}

void GdbProcess::removeCommands( const pCommandList& l )
{
	foreach ( pCommand c, l )
		removeCommand( c );
}

void GdbProcess::executeProcess()
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
//		mCurrentParsers = c.parsers();
		// check if need tryall, and had all other parsers if needed at end
//		if ( c.tryAllParsers() )
//			foreach ( QString s, parsersName() )
//				if ( !mCurrentParsers.contains( s ) )
//					mCurrentParsers << s;
		// execute command
		setWorkingDirectory( c.workingDirectory() );
		start( QString( "%1 %2" ).arg( c.command() ).arg( c.arguments() ) );

		mBuffer.open( QBuffer::ReadOnly );
		// exit
		return;
	}
}
