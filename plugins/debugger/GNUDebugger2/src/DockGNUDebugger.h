#ifndef PDOCKGNUDEBUGGER_H
#define PDOCKGNUDEBUGGER_H

// Pasnox include
#include <fresh.h>
#include <workspacemanager.h>

// Xiantia include
#include <QTextEdit>
#include <QFileInfo>
#include <QTabWidget>
#include <QPointer>
#include <QHash>


#include "./kernel/gdbParser.1.3.h"
#include "./kernel/gdbProcess.1.3.h"
#include "./kernel/gdbInterpreter.1.3.h"
#include "./kernel/gdbSetting.1.3.h"
#include "./kernel/gdbConnectTemplate.1.3.h"
#include "./kernel/gdbBridgeEditor.1.3.h"
#include "./kernel/gdbKernelDispatcher.1.3.h"

#include "./addon/gdbBreakpoint/gdbBreakpoint.h"
#include "./addon/gdbBacktrace/gdbBacktrace.h"
#include "./addon/gdbRegister/gdbRegister.h"
#include "./addon/gdbWatch/gdbWatch.h"
#include "./addon/gdbCli/gdbCli.h"
#include "./addon/gdbToolTip/gdbToolTip.h"
#include "./addon/gdbScript/gdbScript.h"

//! This class is a main class container.
/**
\author xiantia
\version 1.3.2

DockGNUDebugger is created by GNUDebugger.
Those is the main container class. It creates all AddOn and the connect together.
It has under its GdbParser order which parse warp the data coming from Gdb, 
GdbProcess which controls Gdb, GdbBridgeEditor which control the editor Qsci and GdbKernelDispatcher which control AddOn.

The creation of AddOn is very simple.
AddOn are class which adds news function to the debugger such as GdbBreakpoint and GdbWatch.
For that it has pointers on AddOn and add them to GdbKernelDispatcher so that AddOn receives the messages coming from GdbParser.

in .h

\code
	// declare pointer to AddOn
	class GdbBreakpoint *Breakpoint;
\endcode

in .cpp

\code
	// create new class
	Breakpoint = new GdbBreakpoint(this);
	// add them to Dispatcher
	Dispatcher->add(Breakpoint);
\endcode

Connect signals

\code
	connect(Bridge, SIGNAL(requestBreakpoint(const QString &)), Breakpoint , SLOT(onRequestBreakpoint(const QString &)));
\endcode

Once creates AddOn are visible in QTabWidget of DockGNUDebugger if the function widget() return a pointer not NULL.


\note All AddOn must derive from GdbCore.

*/

 
class DockGNUDebugger : public pDockWidget, public QSingleton<DockGNUDebugger>
{
	Q_OBJECT
	friend class QSingleton<DockGNUDebugger>;
	friend class GNUDebugger;
	
public:

	/**
	 * \brief Pointer to GdbParser Class
	*/
	QPointer<GdbParser> Parser;

	/**
	 * \brief Pointer to GdbProcess Class
	*/
	QPointer<GdbProcess> Process;
	
	/**
	 * \brief Pointer to GdbBridgeEditor Class
	*/
	QPointer<GdbBridgeEditor> Bridge;

	/**
	 * \brief Pointer to GdbKernelDispatcher Class
	*/
	QPointer<GdbKernelDispatcher> Dispatcher;

	/**
	 * \brief Create GdbBreakpoint Pointer Class
	*/
	class GdbBreakpoint *Breakpoint;

	/**
	 * \brief Create GdbBacktrace Pointer Class
	*/
	class GdbBacktrace *Backtrace;

	/**
	 * \brief Create GdbBreakpoint Pointer Class
	*/
	class GdbRegister *Register;

	/**
	 * \brief Create GdbWatch Pointer Class
	*/
	class GdbWatch *Watch;

	/**
	 * \brief Create GdbCli Pointer Class
	*/
	class GdbCli *Cli;

	/**
	 * \brief Create GdbToolTip Pointer Class
	*/
	class GdbToolTip *ToolTip;

	/**
	 * \brief Create GdbBreakpoint Pointer Class
	*/
	class GdbScript *Script;

	/**
	 * \brief Contains all action from GNUDebugger class plugin, this action is viewed in Monkey Studio ToolBar
	*/
	void setAction(QHash<QString, QAction*> *);

private:

	DockGNUDebugger( QWidget* = 0 );
	~DockGNUDebugger();

	/**
	 * \brief Load current setting from Monkey .ini
	*/
	void loadSettings();

	/**
	 * \brief Save current setting from Monkey .ini
	*/
	void saveSettings();

	// interpreter 
	//! Connect is a GdbConnectTemplate var, it's use for connect interperter to other function
	GdbConnectTemplate<DockGNUDebugger> Connect;
	//! Contains interpreterStepOver pointer. This pointer is connect to onTargetStopped function
	QPointer<BaseInterpreter> interpreterStepOver;
	//! Contains interpreterStepInto pointer. This pointer is connect to onTargetStopped function
	QPointer<BaseInterpreter> interpreterStepInto;
	//! Contains interpreterStepFinish pointer. This pointer is connect to onTargetStopped function
	QPointer<BaseInterpreter> interpreterStepFinish;
	//! Main container for GNU debugger is QTabWidget
	QTabWidget *mainTabWidget;
	//! Add raw log in QTabWidget
	QTextEdit *rawLog;
	//! Path and name of target selected
	QString mSelectedTarget;
	//! Action list for remote MonkeyStudio::toolBar
	QHash<QString, QAction*> *mActionList;
	//! End of line
	QString crlf;

	//! Indicate if Gdb is started
	bool isGdbStarted;
	//! Indicate if target is running
	bool isTargetRunning;
	//! This var is use for not have two stopped signal consecutive
	bool disableStep;	

public slots:

	//! Enable or disable all actions in Monkey toolBar
	void setEnabledActions(bool b);

	// Actions
	//! When user click <b>Load target</b> action in Monkey toolBar
	/**
		This function start Gdb and load the target
	*/
	void onActionLoadTarget();

	//! When user click <b>Exit debug mode</b> action in Monkey toolBar
	/**
		Stop and quit Gdb
	*/
	void onActionExit();
	
	//! When user click <b>Restart target</b> action in Monkey toolBar
	/**
		This function restart the target at the begining
	*/
	void onActionRestart();
	
	//! When user click <b>Continue</b> action in Monkey toolBar
	/**
		This function start and stop target to the next breakpoint if has
	*/
	void onActionContinue();
	
	//! When user click <b>Step over</b> action in Monkey toolBar
	/**
		This function execute step over
	*/
	void onActionStepOver();
	
	//! When user click <b>Step into</b> action in Monkey toolBar
	/**
		This function execute step into
	*/
	void onActionStepInto();
	
	//! When user click <b>Step finish</b> action in Monkey toolBar
	/**
		This function carry out the function until the end.
	*/
	void onActionStepFinish();

	// Gdb
	//! Calling when Gdb is started correctly
	/**
		Calling when Gdb is stated correctly and dispatch this event to all AddOn
	*/
	void gdbStarted();
	
	//! Calling when Gdb is finished (exit normaly)
	/**
		Calling when Gdb is finiched correctly and dispatch this event to all AddOn
	*/
	void gdbFinished(  int a , QProcess::ExitStatus );
	
	//! Calling when Gdb generate a error (crash)
	/**
		Calling when Gdb generate a error and dispatch this event to all AddOn
	*/
	void gdbError( QProcess::ProcessError);
	
	//! Calling when Gdb has an answer, Gdb has new data
	/**
		Call GdbParser for parse this string
	*/
	void commandReadyRead(  const QString& d);

	// Target
	//! Target is loaded correctly
	/**
		Dispatch this event to all AddOn
	*/
	void onTargetLoaded(int , QString);
	
	//! Target is no loaded correctly (format is not correct)
	/**
		Dispatch this event to all AddOn
	*/
	void onTargetNoLoaded(int , QString);

	//! Target is running
	/**
		Dispatch this event to all AddOn
	*/
	void onTargetRunning(int , QString);
	
	//! Target is exited correctly
	/**
		Dispatch this event to all AddOn
	*/
	void onTargetExited(int , QString);
	
	//! Target is stopped (breakpoint for example)
	/**
		Dispatch this event to all AddOn
	*/
	void onTargetStopped(int , QString);

	//! Calling when GdbParser class emit interpreter signal
	/**
		Now interpreter switch event to the correct function
	*/
	void onInterpreter(const QPointer<BaseInterpreter> & , const int &, const QString &);
	
	//! Calling when user click in the margin
	/**
		Call GdbBreakpoint::toggleBreakpoint() function
	*/
	void onUserToggleBreakpoint(const QString & , const int & );
	
	// from parser
	//! Done event
	/**
		Calling when GdbParser emit done signal
	*/

	void onDone(int , QString);
	//! Info event
	/**
		Calling when GdbParser emit info signal
	*/
	void onInfo(int , QString);

	//! Error event
	/**
		Calling when GdbParser emit error signal
	*/
	void onError(int , QString);

	//! Prompt event
	/**
		Calling when GdbParser emit prompt signal
	*/
	void onPrompt(int , QString);
};

#endif 
