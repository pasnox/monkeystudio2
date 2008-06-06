/*
	the origine file is writed by Pasnox, pConsole
*/

#include "gdbProcess.1.3.h"
#include "monkey.h"


GdbProcess::GdbProcess( QObject * parent ) : QProcess( parent )
{
	// mixe channels
	setReadChannelMode( QProcess::MergedChannels );
	// connections

	connect( this, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
	connect (&t, SIGNAL(timeout()), this , SLOT(onTimer()));
	t.start(100);

	crlf = pMonkeyStudio::getEol().toLocal8Bit();
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

void GdbProcess::onTimer()
{
	if(GdbParser::instance())
	{
		if(mCmdList.count() && GdbParser::instance()->isReady())
		{
			write( mCmdList.at(0) + crlf );
		mCmdList.removeAt(0);
		}
	}
}

//

/*void GdbProcess::onParserReady()
{
	mIsReady = true;

}*/

void GdbProcess::sendRawData( const QByteArray & a )
{
	mCmdList << a;
}

//

void GdbProcess::stopTarget()
{
	// quit gdb
	sendRawData( "q" + crlf);
}

void GdbProcess::stopProcess()
{
	if ( state() == QProcess::Running )
	{
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
	if(!mCommand.isEmpty())
	{
		// execute command
		start(  mCommand  );
		// wait for gdb started
		waitForStarted (500);
	}
}
