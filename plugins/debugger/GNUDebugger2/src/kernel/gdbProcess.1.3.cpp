/*
	the origine file is writed by Pasnox, pConsole
*/

#include "gdbProcess.1.3.h"



GdbProcess::GdbProcess( QObject * parent ) : QProcess( parent )
{
	// mixe channels
	setReadChannelMode( QProcess::MergedChannels );
	// connections

	connect( this, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );

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
	stopProcess();
}

// read data from Gdb
void GdbProcess::readyRead()
{
	QByteArray d = readAll ();
	emit commandReadyRead( d );
}

//
void GdbProcess::sendRawData( const QByteArray & a )
{
	write( a + crlf );
}

//
void GdbProcess::stopProcess()
{
	if ( state() == QProcess::Running )
	{
		// stop target into gdb
		sendRawData( "kill" + crlf);
		// some time gdb want confirmation
		sendRawData( "y" + crlf);
		// quit gdb
		sendRawData( "q" + crlf);
		// if gdb not want finish
		if(!waitForFinished(500))
		{
			terminate();
			if(!waitForFinished(500))
				kill();
		}
	}
}

//
void GdbProcess::setCommand( const QString & c )
{
	mCommand = c;
}

void GdbProcess::setWorkingDirectorie(const QString & dir)
{
	mDir = dir;
}

//
void GdbProcess::startProcess()
{
		// execute command
		start( QString( "%1" ).arg( mCommand ) );
		// wait for gdb started
		waitForStarted (500);
}
