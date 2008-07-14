/****************************************************************************
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
****************************************************************************/
/*!
	\file gdbProcess.1.3.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Remote Gdb
*/

#include "gdbProcess.1.3.h"
#include "monkey.h"

#include <QTextCodec>

/*!
	\details Create new object
	\param parent of this object
*/
GdbProcess::GdbProcess( QObject * parent ) : QProcess( parent )
{
	// mixe channels
	setReadChannelMode( QProcess::MergedChannels );
	// connections

	connect( this, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
	connect (&t, SIGNAL(timeout()), this , SLOT(onTimer()));
	t.start(100);

	crlf = pMonkeyStudio::getEol();
	mCmdList.clear();
}
//

/*!
	\details Stop target and Gdb
*/
GdbProcess::~GdbProcess()
{
	stopProcess();
}

// read data from Gdb
/*!
	\details  New data from Gdb is avaible
*/
void GdbProcess::readyRead()
{
	QString buf = QTextCodec::codecForLocale()->toUnicode( readAll() );
	emit commandReadyRead( buf );
}

//
/*!
	\details Execute new command only if GdbParser class is ready and have a command
*/
void GdbProcess::onTimer()
{
	if(GdbParser::instance())
	{
		if(mCmdList.count() && GdbParser::instance()->isReady())
		{
			GdbParser::instance()->setReady(false);
			write( QTextCodec::codecForLocale()->fromUnicode( mCmdList.at(0) + crlf ) );
			mCmdList.removeAt(0);
		}
	}
}

//
/*!
	\details Send command to Gdb, this command can not have crlf at end.
	\param a is the string command has to send to Gdb 
*/
void GdbProcess::sendRawData( const QString& a )
{
	mCmdList << a;
}

//
/*!
	\details Clear command in standbye
*/
void GdbProcess::clearAllCommand()
{
	mCmdList.clear();
}

/*!
	\details Stop and quit the target.
*/
void GdbProcess::stopTarget()
{
	// quit gdb
	sendRawData( "q" + crlf);
}

/*!
	\details Stop Gdb
*/
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
/*!
	\details Add new command, if Gdb execute other command, this new command is standbyed
	\param c is the string command.
*/
void GdbProcess::setCommand( const QString & c )
{
	mCommand = c;
}

/*!
	\details Set working directorie
	\param dir is the directorie
*/
void GdbProcess::setWorkingDirectorie(const QString & dir)
{
	mDir = dir;
}

//
/*!
	\details 	//! Start Gdb
*/
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
