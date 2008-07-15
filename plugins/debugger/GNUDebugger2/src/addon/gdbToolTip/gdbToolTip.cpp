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
	\file gdbToolTip.cpp
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

#include "gdbToolTip.h"
#include <QToolTip>

/*!
	\details Create new object
	\param parent of this object
*/
GdbToolTip::GdbToolTip(QObject * parent) : GdbCore(parent)
{
	// new connexion 

	setEnabled(true);
	setWaitEndProcess(false);
	enteredShowVar = false;
	isGdbStarted = false;
	isTargetStopped = false;

	GdbCore::Parser()->addRestoreLine(name(),
		"^\\$\\d+\\s+=\\s+.*" , 
		".*\\(gdb\\)\\s*$");

	interpreterValue = GdbCore::Parser()->addInterpreter(
		name(),
		QRegExp("^print .*"), 
		QRegExp("^\\$\\d+\\s+=\\s+.*"),
		"^info,interpreter=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");

	// connect interpreters to functions
	Connect.add(this, interpreterValue, &GdbToolTip::onValue);

	// create sequencer 
	Sequencer = new GdbSequencer(this);
}

//

/*!
	\details None.
*/
GdbToolTip::~GdbToolTip()
{
}

//

QString GdbToolTip::name()
{
	return "GdbToolTip";
}

//

QPointer<QWidget> GdbToolTip::widget()
{
	return (QPointer<QWidget>)( NULL );
}

//

QIcon GdbToolTip::icon()
{
	return QIcon();
}

//

/**
 * \details Calling when Parser class found an interpreter.
 *
 * An interpreter is a small parser for one command and one answer from Gdb.
 * For example when you to send a command has Gdb, Gdb answers by a specific string.
 * If you want to be on this answer and to call a function of your class, then creates an interpreter and connect your function has it.
 *
 * In your constructor class :
 * \code
 *	interpreterValue = GdbCore::Parser()->addInterpreter(
 *	name(),
 *	QRegExp("^print .*"), 
 *	QRegExp("^\\$\\d+\\s+=\\s+.*"),
 *	"^info,interpreter=\"" + name() + "\",event=\"WatchValue\",answerGdb=\"");
 *
 * Connect.add(this, interpreterValue, &GdbToolTip::onValue);
 * \endcode
 * 
 * In interpreter function :
 * \code
 * Connect.call(interpreter, id, string);
 * \endcode
 *
 * onValue function is only and automatically calls, when you send your command and that the answer of Gdb is correctly parsed.
 *
 * \note Connect is GdbConnectTemplate class.
 *
 * An interpreter can also modify the answer of Gdb.
 * For example when you remove a breakpoint, Gdb answer just by prompt event and you have any information on what it passed. 
 * You can modify the answer of Gdb by specifying this one in answerExtention string.
*/

void GdbToolTip::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call( i, id, s);
}

// Gdb status

void GdbToolTip::gdbStarted()
{
	setWaitEndProcess(false);
	isGdbStarted = true;
}

//

void GdbToolTip::gdbFinished()
{
	setWaitEndProcess(false);
	isGdbStarted = false;
}

//

void GdbToolTip::gdbError()
{
	isGdbStarted= false;
}

// Taget status

void GdbToolTip::targetLoaded(const int &, const QString &){}
void GdbToolTip::targetNoLoaded(const int &, const QString &){}
void GdbToolTip::targetRunning(const int & id, const QString & s)
{
	isTargetStopped = false;
}

void GdbToolTip::targetStopped(const int & id, const QString & s)
{
	isTargetStopped = true;
}

void GdbToolTip::targetExited(const int & id, const QString & s)
{
	isTargetStopped = true;
}

// Parser status

void GdbToolTip::error(const int &, const QString & s)
{
	enteredShowVar=false;
}

//

void GdbToolTip::done(const int &, const QString & s){}
void GdbToolTip::info(const int &, const QString &){}

void GdbToolTip::prompt(const int &, const QString & )
{
	setWaitEndProcess(false);
}

// Interpreters

/*!
 * \details This function is call when Gdb answered. This function show ToopTip at cursor post.
 * \param s is the value from GdbParser. 
*/
void GdbToolTip::onValue(int, QString s)
{
	setWaitEndProcess(false);
	enteredShowVar=false;
	Sequencer->remove();
	QString v = findValue(s,"answerGdb");
	v = v.right(v.length() - v.indexOf(" = ") - 3);
	QToolTip::showText(QCursor::pos (), v, NULL);
}

// slot from Qsci

/**
 * \details This function is call when user select text in editor. This function send "print varName" to Gdb.
 * \param n is the name of var from editor.
*/
void GdbToolTip::onRequestShowVar(const QString & n)
{
	if(!enteredShowVar && isGdbStarted && isTargetStopped)
	{
		enteredShowVar = true;
		QToolTip::hideText();

		QList<SequencerCmd> s;
		s <<  SequencerCmd("printValue", "print " + n) ; 
		Sequencer->add(name() , s);
		Sequencer->start();
		setWaitEndProcess(true);
	}
}
