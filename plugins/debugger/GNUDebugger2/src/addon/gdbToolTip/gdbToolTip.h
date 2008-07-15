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
	\file gdbToolTip.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Show the value of a variable in Tooltip. This class is an AddOn for GNU debugger.
*/
/*

	GdbToolTip class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBTOOLTIP_H
#define GDBTOOLTIP_H

#include <QObject>
#include "../../kernel/gdbCore.h"
#include "../../kernel/gdbSequencer.h"

/*!
	\brief Show the value of a variable in Tooltip. This class is an AddOn for GNU debugger.
	\details When the user selects text in the editor, the function onRequestShowVar is called (use GdbConnectTemplate and GdbInterpreter class). 
With this call there sendings "print valueName" to Gdb (use GdbSequencer class). In answer, onValue function is called and
ToolTip is show with a value of variable.

*/

class GdbToolTip : public GdbCore
{
	Q_OBJECT


public:

	GdbToolTip(QObject * parent = 0);
	~GdbToolTip();

public slots:

	void onValue( int , QString );
	void onRequestShowVar(const QString &);

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

	/*!
	 * \details This variable is a template for connect GdbToolTip::onValue() and interpreterValue.
	 * When onRequestShowVar function is call. it send "print variableName" to Gdb.
	 * The answer from Gdb is parse in GdbParser class, this class search an interpreter and found interpreterValue.
	 * Parser Class emit interpreter signal and call GdbToolTip::interpreter(..) function, in this it call Connect.call() function 
	 * and found onValue function attached with this interpreter.
	*/
	GdbConnectTemplate<GdbToolTip> Connect;
	QPointer<BaseInterpreter> interpreterValue;

	/**
	 * \details This variable store all commands to send to Gdb. In this case Sequencer has just one command, "print variableName".
	*/
	QPointer<GdbSequencer> Sequencer;

	bool enteredShowVar;
	bool isGdbStarted;
	bool isTargetStopped;
};

#endif
