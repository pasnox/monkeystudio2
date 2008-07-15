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
	\file gdbProcess.1.3.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Remote Gdb
*/

#ifndef GDBPROCESS_H
#define GDBPROCESS_H
//
#include "fresh.h"

#include <QProcess>
#include <QBuffer>
#include <QPoint>
#include <QHash>
#include <QTimer>

#include "gdbParser.h"

/*!
	\brief Remote Gdb 
	\details This Class is QProcess. It use for remote Gdb, send and receive data.
	For send command to Gdb use sendRawData() 
*/

class  GdbProcess : public QProcess, public QSingleton<GdbProcess>
{

	Q_OBJECT
	friend class QSingleton<GdbProcess>;

public:

	GdbProcess( QObject *parent = 0 );
	~GdbProcess();

private:
	
	/*!
		\details Current crlf
	*/
	QString crlf;
	
	/*!
		\details Current command
	*/
	QString mCommand;
	
	/*!
		\details Current work directorie
	*/
	QString mDir;

	/*!
		\details List of commands
	*/
	QList<QString> mCmdList;

	/*!
		\details Timer for start next command if has.	
	*/
	QTimer t;
	
	
public slots:

	void sendRawData( const QString& );
	void stopProcess();
	void stopTarget();
	void setCommand( const QString & );
	void setWorkingDirectorie(const QString &);
	void startProcess();
	void clearAllCommand();
	
private slots:

	void readyRead();
	void onTimer();
	
signals:

	//! Data can be read from Gdb
	void commandReadyRead(  const QString& );
};


//
#endif // GDBPROCESS_H
