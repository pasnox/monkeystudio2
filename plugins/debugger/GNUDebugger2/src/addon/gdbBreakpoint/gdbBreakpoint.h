/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbBreakpoint
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbBreakpoint class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#ifndef GDBBREAKPOINT_H
#define GDBBREAKPOINT_H

#include <QObject>
#include <QTextEdit>
#include "../../kernel/gdbCore.1.3.h"
#include "./ui/UIGdbBreakpoint.h"

#include "gdbBreakpointStruct.h"

//! Implements all functions for remote breakpoint.This class is an AddOn for GNU debugger

/**
\author xiantia
\version 1.3.2

This class can toogle breakpoint when user click in the margin, enable or disable breakpoint from UIGdbBreakpoint and set
condition or not
*/

class GdbBreakpoint : public GdbCore
{
	Q_OBJECT


public:

	GdbBreakpoint(QObject * parent = 0 );
	~GdbBreakpoint();

public slots:

	//! Calling when Gdb has executed correctly the last command "enable numBreak", Calling by GdbConnectTemplate
	/**
	When this command is executed correctly , this function is call by an interpreter. Now this function can emit onToggleBreakpoint
	*/
	void onBreakpointEnabled(int, QString);
	
	//! Calling when Gdb has executed correctly the last command "Disable numBreak", Calling by GdbConnectTemplate
	/**
	When this command is correctly executed , this function is call by an interpreter. Now this function can emit onToggleBreakpoint
	*/
	void onBreakpointDisabled(int, QString);
	
	//! Calling when user want enable or disable breakpoint from UIGdbBreakpoint
	/**
	 This function send "enable numBreak" or "disable numBreak" to Gdb. If the answer from Gdb is correct, an interpreter switch to the correct function
	 onBreakpointEnabled() or onBreakpointDisabled()
	*/
	void toggleEnabledBreakpoint(const QString & ,const int & , const bool &);


	
	
	//! Add breakpoint, calling when "delete numBreak" command is correctly executed. Calling by GdbConnectTemplate
	/**
	When this command is correctly executed , this function is call by an interpreter. Now this function can emit onToggleBreakpoint
	*/
	void onBreakpointAdd( int , QString );
	//! Delete breakpoint,calling when "b main.cpp:23" command is correctly executed. Calling by GdbConnectTemplate
	/**
	When this command is correctly executed , this function is call by an interpreter. Now this function can emit onToggleBreakpoint
	*/
	void onBreakpointDelete( int , QString );
	//! Calling when user click in the margin
	/**
	This function send "delete numBreak" or "b fileName:line" to Gdb. If the answer from Gdb is correct, an interpreter switch to the correct function
	 onBreakpointDelete() or onBreakpointAdd()
	*/
	void toggleBreakpoint(const QString &, const int &);


	//! Condition or not a breakpoint, calling when "condition numBreak condition" command is correctly executed. Calling by GdbConnectTemplate
	/**
	When this command is correctly executed , this function is call by an interpreter. Now this function can emit onToggleBreakpoint
	*/
	void onBreakpointConditionned(int, QString);
	//! Condition or not a breakpoint, calling when "condition numBreak" command is correctly executed. Calling by GdbConnectTemplate
	/**
	When this command is correctly executed , this function is call by an interpreter. Now this function can emit onToggleBreakpoint
	*/
	void onBreakpointUnConditionned(int, QString);

	//! Calling when user want contionned breakpoint from UIGdbBreakpoint
	/**
	This function send "condition numBreak condition" or "condition numBreak" to Gdb. If the answer from Gdb is correct, an interpreter switch to the correct function
	 onBreakpointUnConditionned() or onBreakpointConditionned()
	*/
	void toggleConditionnedBreakpoint(const QString &, const int &, const QString &);

	//! Calling when a new file is opened
	/**
	Find if this file have some breakpoints. In this case, this function emit onToggleBreakpoint signal
	*/
	void onRequestBreakpoint(const QString & );

	//! Set breakpoint, but the current lib is not loaded by Gdb
	void onBreakpointPending(int, QString );
	
	//! Generique interpreter 
	/**
	Switch all interpreter to the correct function
	*/
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

	GdbConnectTemplate<GdbBreakpoint> Connect;
	QList<Breakpoint *>  breakpointList;

	//! Connect "b fileName:Line" command to onAddBreakpoint function 
	QPointer<BaseInterpreter> interpreterAddBreakpoint;
	//! Connect "delete numBreak" command to onDeleteBreakpoint function 
	QPointer<BaseInterpreter> interpreterDelBreakpoint;

	//! Connect "enable numBreak" command to onEnableBreakpoint function 
	QPointer<BaseInterpreter> interpreterEnabledBreakpoint;
	//! Connect "disable numBreake" command to onDisableBreakpoint function 
	QPointer<BaseInterpreter> interpreterDisabledBreakpoint;

	//! Same as "b fileName:line" command but lib is not loaded by Gdb 
	QPointer<BaseInterpreter> interpreterBreakpointPending;

	//! Connect "condition numBreak myCondition" command to onConditionnedBreakpoint function 
	QPointer<BaseInterpreter> interpreterConditionnedBreakpoint;
	//! Connect "condition numBreak" command to onUnConditionnedBreakpoint function 
	QPointer<BaseInterpreter> interpreterUnConditionnedBreakpoint;

	//! Find in list 
	/**
	* \retval A pointer to the main struct, in this struct you have all breakpoint for this file, if it has. Else return NULL 
	*/
	Breakpoint * findByName(const QString &);

	//! Find if file has already breakpoint at this line
	/**
	* \retval true if have, else false
	*/
	int asBreakpointAtLine(Breakpoint *, int);

	//! Find if file has already breakpoint with this index (index is returned by Gdb)
	/**
	* \retval true if have, else false
	*/
	int asBreakpointIndex(Breakpoint *b, int index);

	//! Remove all breakpoints for this file
	void removeBreakpoint(Breakpoint * bp);
	//! Remove all breakpoints (clear list)
	void removeAllBreakpoint();

	//! Calling when a breakpoint has moved
	/**
	Some time Gdb move breakpoint to the next line
	*/
	void breakpointMoved(const QString & , const int & , const int & );
	//! Hide icon in UIGdbBreakpoint. 
	void desableBreakpointHit();

	QPointer<UIGdbBreakpoint> mWidget;

signals:

	//! Emit for indicats that this breakpoint is add, deleted or modified.
	/**
	Breakpoint is a struct, it use for have file name (Breakpoint.fileName).
	BaseBreakpoint is a struct, only one breakpoint, in this case is the current breakpoint.
	*/
	void onToggleBreakpoint(const Breakpoint & , const BaseBreakpoint & , const bool &);
};

#endif
