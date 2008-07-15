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
	\file gdbBacktrace.h 
	\date 14/08/08
	\author Xiantia
	\brief Implements all functions for parse backtrace commands. 
	This class is an AddOn for GNU debugger
	\version 1.3.2
*/

/*

	GdbBacktrace class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBBACKTRACE_H
#define GDBBACKTRACE_H

#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"
#include "./ui/UIGdbBacktrace.h"

/*!
	\brief Implements all functions for parse backtrace commands.
	This class is an AddOn for GNU debugger
	\details  When target is stopped, this class search where is Gdb has stopped.
	For that it send "bt" and "info source" commands for extract the line number of file and the absolue path of file.

	GdbBacktrace thus will start by adding two interpreters has GdbParser:
	- interpreterBacktrace which goes parser the line " ^#\\d+\\s.*\\sat\\s.*:\\d+", i.e. all the lines which starts with #.
	\code
	#0 main (arc, arg) at src/main.cpp:23
	\endcode

	- interpreterInfosource which goes parser all the lines which starts with "^Located\\sin\\s.*"
	\code
	Located in \usr\local\bin\dev\test.cpp
	\endcode

	\note These two interpreters are not active which if their respective command are in court.

	Once created, this interpreters are connected has two functions:
	- onBacktrace which will extract from the string the number of the line where Gdb it is stopped.
	- onInfosource which will extract from the string the file name where Gdb is stopped.

	\note The two commands has to send has Gdb are directly stored in Sequencer

	When the sequence is finished the signal onToggleBacktrace (mCurrentFile, mCurrentLine) is emitted and 
	GdbBridgeEditor is informed by it, those which will place the icon on the file and the line corresponding.

	Another function is to add has GdbBacktrace class. When a new editor is opened, GdbBridgeEditor emitted the sinal 
	requestBacktrace, those the purpose of which is to ask whether the editor had the backtrace just before closing/opening.
	If the name of the open file is the same one that mCurrentFile, then the signal onToggleBacktrace is emitted.
*/

class GdbBacktrace : public GdbCore
{
	Q_OBJECT

public:

	GdbBacktrace(QObject * parent = 0);
	~GdbBacktrace();

public slots:

	void onBacktrace( int , QString );
	void onInfoSource( int , QString );
	void onRequestBacktrace(const QString &);

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
	 * \details This variable is a template for connect GdbBacktrace::onBacktrace() with interpreterBacktrace
	 * and GdbBacktrace::onInfoSource() with interpreterInfosource.
	*/
	GdbConnectTemplate<GdbBacktrace> Connect;

	/*!
	* \details Store interpreterBacktrace pointer returned by GdbParser class.
	*/
	QPointer<BaseInterpreter> interpreterBacktrace;
	
	/*!
	* \details Store interpreterInfoSource pointer returned by GdbParser class.
	*/
	QPointer<BaseInterpreter> interpreterInfoSource;

	/*!
	* \details Store Sequencer. It contains all commands has send to Gdb
	*/
	QPointer<GdbSequencer> Sequencer;

	QPointer<UIGdbBacktrace> mWidget;

	/*!
	* \details Current line where Gdb is stopped.
	*/
	int mCurrentLine;
	/*!
	* \details Current file name where Gdb is stopped.
	*/
	QString mCurrentFile;
	int numBacktrace;

signals:

	/*!
	 * \details This signal is emit for post backtrace icon on editor margin.
	 * \param fileName of file
	 * \param line number of file
	*/
	void onToggleBacktrace(const QString &fileName, const int &line);
};

#endif
