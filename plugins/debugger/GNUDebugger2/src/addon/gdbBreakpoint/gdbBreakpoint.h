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
	\file gdbBreakpoint.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Implements all functions for remote breakpoints.This class is an AddOn for GNU debugger
*/
/*

	GdbBreakpoint class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBBREAKPOINT_H
#define GDBBREAKPOINT_H

#include <QObject>
#include <QTextEdit>
#include "../../kernel/gdbCore.h"
#include "./ui/UIGdbBreakpoint.h"

#include "gdbBreakpointStruct.h"

/*!
	\brief Implements all functions for remote breakpoints.This class is an AddOn for GNU debugger

	\details This class can toogle breakpoint when user click in the margin, enable or disable breakpoint from UIGdbBreakpoint and set
condition or not.

It will start by adding seven interpreters has GdbParser for parse only specific line:
- add of a breakpoint
\code
	interpreterAddBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^b\\s.*:\\d+$"),
		QRegExp("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))"),
		"");
\endcode

- to remove a breakpoint
\code
	interpreterDelBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^delete\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-delete\",answerGdb=\"");
\endcode

- enable a breakpoint
\code
	interpreterEnabledBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^enable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");
\endcode

- disable a breakpoint
\code
	interpreterDisabledBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^disable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");
\endcode

- to condition a breapoint
\code
	interpreterConditionnedBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^condition\\s\\d+.+$"),
		QRegExp("^\\(gdb\\)\\s"),
		"");
\endcode

- uncondition a breakpoint
\code
	interpreterUnConditionnedBreakpoint = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^condition\\s\\d+$"),
		QRegExp("^Breakpoint\\s\\d+\\snow\\sunconditional.$"),
		"");
\endcode

- set breakpoint before load lib
 \code
	interpreterBreakpointPending = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^b\\s.*:\\d+$"),
		QRegExp("^Breakpoint\\s(\\d+)\\s\\((.*):(\\d+)\\)\\spending\\.$"),
		"^info,interpreter=\"" + name() + "\",event=\"Breakpoint-Add-Pending\",answerGdb=\"");
\endcode

\note These interpreters are not active which if their respective command are in court.

Then to connect them to the corresponding functions.

According to the events, the GdbBreakpoint class  modify the struct Breakpoint and emits the signal onToggleBreakpoint
*/

class GdbBreakpoint : public GdbCore
{
	Q_OBJECT


public:

	GdbBreakpoint(QObject * parent = 0 );
	~GdbBreakpoint();

public slots:

	void onBreakpointEnabled(int, QString);
	void onBreakpointDisabled(int, QString);
	void toggleEnabledBreakpoint(const QString & ,const int & , const bool &);

	void onBreakpointAdd( int , QString );
	void onBreakpointDelete( int , QString );
	void toggleBreakpoint(const QString &, const int &);

	void onBreakpointConditionned(int, QString);
	void onBreakpointUnConditionned(int, QString);
	void toggleConditionnedBreakpoint(const QString &, const int &, const QString &);

	void onRequestBreakpoint(const QString & );
	void onBreakpointPending(int, QString );
	
	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

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
		\details Connect interpreter to function
	*/
	GdbConnectTemplate<GdbBreakpoint> Connect;
	/*! 
		\details  List of all breakpoints, struct Breakpoint
	*/
	QList<Breakpoint *>  breakpointList;

	/*! 
		\details Connect "b fileName:Line" command to onAddBreakpoint() function 
	*/
	QPointer<BaseInterpreter> interpreterAddBreakpoint;
	
	/*! 
		\details Connect "delete numBreak" command to onDeleteBreakpoint() function 
	*/
	QPointer<BaseInterpreter> interpreterDelBreakpoint;

	/*! 
		\details Connect "enable numBreak" command to onEnableBreakpoint() function 
	*/
	QPointer<BaseInterpreter> interpreterEnabledBreakpoint;
	/*! 
		\details Connect "disable numBreake" command to onDisableBreakpoint() function 
	*/
	QPointer<BaseInterpreter> interpreterDisabledBreakpoint;

	/*! 
		\details Same as "b fileName:line" command but lib is not loaded by Gdb 
	*/
	QPointer<BaseInterpreter> interpreterBreakpointPending;

	/*! 
		\details Connect "condition numBreak myCondition" command to onConditionnedBreakpoint() function 
	*/
	QPointer<BaseInterpreter> interpreterConditionnedBreakpoint;

	/*! 
		\details Connect "condition numBreak" command to onUnConditionnedBreakpoint() function 
	*/
	QPointer<BaseInterpreter> interpreterUnConditionnedBreakpoint;

	Breakpoint * findByName(const QString &);
	int asBreakpointAtLine(Breakpoint *, int);
	int asBreakpointIndex(Breakpoint *b, int index);

	void removeBreakpoint(Breakpoint * bp);
	void removeAllBreakpoint();

	void breakpointMoved(const QString & , const int & , const int & );
	void desableBreakpointHit();

	QPointer<UIGdbBreakpoint> mWidget;

signals:

	/*!
	\details Emit for indicats that this breakpoint is add, deleted or modified.
	*/
		void onToggleBreakpoint(const Breakpoint & , const BaseBreakpoint & , const bool &);
};

#endif
