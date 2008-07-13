/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBacktrace
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

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

//! Implements all functions for parse backtrace commands.This class is an AddOn for GNU debugger

/**
\author xiantia
\version 1.3.2

When target is stopped, this class search where is Gdb has stopped.
For that it send "bt" and "info source" commands for extract the line number of file and the absolue path of file.

GdbBacktarce thus will start by adding two to interpreters has GdbParser:
- interpreterBacktrace which goes parser the line " ^#\\d+\\s.*\\sat\\s.*:\\d+", i.e. all the lines which starts with #.
\code
 #0 main (arc, arg) at src/main.cpp:23
\endcode

- interpreterInfosource which goes parser all the lines which starts with "^Located\\sin\\s.*"
\code
 Located in \usr\local\bin\dev\test.cpp
\endcode

\note These two interpreters are not active which if their respective command are in court.

Once created to interpreter them is connected has two functions:
- onBacktrace which will extract from the chain the number of the line where Gdb it is stopped.
- onInfosource which will extract from the chain the file name where Gdb it is stopped.

\note The two commands has to send has Gdb are directly stored in Sequencer

When the sequence is finished the signal onToggleBacktrace (mCurrentFile, mCurrentLine) is emitted and 
GdbBridgeEditor is informed by it, those which will place the icon on the file and the line corresponding.

Another function is to add has GdbBacktrace class. When a new editor is opened, GdbBridgeEditor emitted the sinal 
requestBacktrace, those the purpose of which is to ask whether the editor had the backtrace just before closing/opening
If the name of the open file is the same one that mCurrentFile, then the signal onToggleBacktrace is emitted.


*/

class GdbBacktrace : public GdbCore
{
	Q_OBJECT


public:

	GdbBacktrace(QObject * parent = 0);
	~GdbBacktrace();

public slots:

	/**
	 * \brief Calling when interpreter "Connect.call(interpreter, id , string);" 
	 * function found onBacktrace function attached with interpreterBacktrace
	*/
	void onBacktrace( int , QString );

	/**
	 * \brief Calling when interpreter "Connect.call(interpreter, id , string);" 
	 * function found onBacktrace function attached with interpreterInfosource
	*/
	void onInfoSource( int , QString );

	/**
	 * \brief Calling when an editor is open. Restore backtrace icon 
	 * If mCurrentfile is equal in the name of the opened file, onToggleBacktrace signal is 
	 * emitted indicating that Gdb is stopped on this file. the editor can now show icon backtrace on the margin
	*/
	void onRequestBacktrace(const QString &);

	QString name();
	QPointer<QWidget> widget();
	QIcon icon();

	/**
	 * \brief This function switches interpreterBacktrace attached to onBacktrace function or
	 * interpreterInfosource attached to onInfoSource function.
	*/
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

	/**
	 * \brief This variable is a template for connect GdbBacktrace::onBacktrace() and interpreterBacktrace
	 * or GdbBacktrace::onInfoSource() and interpreterInfosource.
	*/
	GdbConnectTemplate<GdbBacktrace> Connect;

	/**
	* \brief Store interpreterBacktrace pointer returned by GdbParser class.
	*/
	QPointer<BaseInterpreter> interpreterBacktrace;
	
	/**
	* \brief Store interpreterInfoSource pointer returned by GdbParser class.
	*/
	QPointer<BaseInterpreter> interpreterInfoSource;

	QPointer<GdbSequencer> Sequencer;
	QPointer<UIGdbBacktrace> mWidget;

	/**
	* \brief current line where Gdb is stopped.
	*/
	int mCurrentLine;
	/**
	* \brief current file name where Gdb is stopped.
	*/
	QString mCurrentFile;
	int numBacktrace;

signals:

	/**
	 * \brief This signal is emit for post backtrace icon on editor margin.
	 * \param fileName and line number
	*/
	void onToggleBacktrace(const QString &, const int &);
};

#endif
