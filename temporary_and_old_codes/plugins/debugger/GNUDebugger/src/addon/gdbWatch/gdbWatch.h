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
	\file gdbWatch.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief  This class implements all function for see user values. This class is an AddOn for GNU debugger 
*/
/*

	GdbWatch class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBWATCH_H
#define GDBWATCH_H

#include <QObject>
#include "../../kernel/gdbCore.h"
#include "../../kernel/gdbSequencer.h"
#include "./ui/UIGdbWatch.h"
#include "../gdbScript/gdbScript.h"

#include "gdbWatchStruct.h"

/*!
	\brief  This class implements all function for see user values. This class is an AddOn for GNU debugger 
*/
class GdbWatch : public GdbCore
{
	Q_OBJECT


public:

	GdbWatch(QObject * parent = 0);
	~GdbWatch();

public slots:

	void onType( int , QString );
	void onAddress( int , QString );
	void onValue( int , QString );
	void onUserAddVar(QString);
	void onScriptFinishedTranslate(const QString &);

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

	GdbConnectTemplate<GdbWatch> Connect;
	QPointer<BaseInterpreter> interpreterAddress;
	QPointer<BaseInterpreter> interpreterWhatis;
	QPointer<BaseInterpreter> interpreterValue;

	QPointer<GdbSequencer> Sequencer;
	QPointer<UIGdbWatch> mWidget;


	void showColor(QTreeWidgetItem *, int , QString );
	int currentVar;
	void createSequencer();
	bool isPointer(const QString & );

	QString mOldValue;

signals:

	void requestScriptTranslate(const QString &, const QString &);

};

#endif
