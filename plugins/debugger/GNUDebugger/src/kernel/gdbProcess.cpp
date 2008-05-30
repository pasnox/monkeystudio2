/*
	the origine file is writed by Pasnox, pConsole
*/

#include "gdbProcess.h"
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
//
GdbProcess::~GdbProcess()
{
//		QMessageBox::warning(NULL,"delete", "gdb process");
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
//
void GdbProcess::started()
{
	// emit signal
	emit commandStarted( currentCommand() );
}
//
void GdbProcess::stateChanged( QProcess::ProcessState e )
{
}
//
void GdbProcess::sendRawData( const QByteArray& a )
{
	write( a  );
}
//
void GdbProcess::stopCurrentCommand( bool b )
{
	if ( state() != QProcess::NotRunning )
	{
		sendRawData( "kill" + crlf);
		sendRawData( "y" + crlf);
		sendRawData( "q" + crlf);

		if(!waitForFinished(500))
		{
//			QMessageBox::warning(NULL,"information", "I can't stop GDB normaly.");
			terminate();
			if(!waitForFinished(500))
			{
//				QMessageBox::warning(NULL,"information", "I can't  stop GDB, because your program probabily running\nPlease stop your target befor");
				kill();
			}
		}
	}
}
//
void GdbProcess::addCommand( const pCommand& c )
{
	if ( c.isValid() )
		mCommands << c;
}
//
void GdbProcess::executeProcess()
{
	foreach ( pCommand c, mCommands )
	{
		// execute command
		setWorkingDirectory( c.workingDirectory() );
		start( QString( "%1 %2" ).arg( c.command() ).arg( c.arguments() ) );

		mBuffer.open( QBuffer::ReadOnly );
		// exit
		return;
	}
}
