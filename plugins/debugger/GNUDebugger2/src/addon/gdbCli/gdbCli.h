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
	\file gdbCli.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Implements all functions for send user command to Gdb.This class is an AddOn for GNU debugger
*/
/*

	GdbCli class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBCLI_H
#define GDBCLI_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbCli.h"


/*!
	\brief Implements all functions for send user command to Gdb.This class is an AddOn for GNU debugger.
	
	This is a same as a console.
*/
class GdbCli : public GdbCore
{
	Q_OBJECT


public:

	GdbCli(QObject * parent = 0);
	~GdbCli();

public slots:

	void onSend();

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	// gdb
	void gdbFinished();
	void gdbStarted();
	void gdbError();

	// target
	void targetLoaded(const int &, const QString &);
	void targetNoLoaded(const int &, const QString &);
	void targetRunning(const int &, const QString &);
	void targetStopped(const int &, const QString &);
	void targetExited(const int &, const QString &);

	// Parser
	void error(const int &, const QString &);
	void done(const int &, const QString &);
	void info(const int &, const QString &);
	void prompt(const int &, const QString &);

private:

	QPointer<UIGdbCli> mWidget;
};

#endif
