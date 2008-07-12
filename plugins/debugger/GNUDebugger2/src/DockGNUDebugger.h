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

It create all Addon pointers, connect some signals from Addon to other AddOn.
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
	 * \brief Contains all action from GNUDebugger class plugin
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
	//! Connect is a GdbConnectTemplate, it use for connect interperter to other function
	GdbConnectTemplate<DockGNUDebugger> Connect;
	//! Contains interpreterStepOver pointer. With this pointer you can connect onTargetStopped function
	QPointer<BaseInterpreter> interpreterStepOver;
	//! Contains interpreterStepInto pointer. With this pointer you can connect onTargetStopped function
	QPointer<BaseInterpreter> interpreterStepInto;
	//! Contains interpreterStepFinish pointer. With this pointer you can connect onTargetStopped function
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
	//! This var is use for not have two stopped signal consecutif
	bool disableStep;	

public slots:

	//! Enable or disable all action in Monkey toolBar
	void setEnabledActions(bool b);

	// Actions
	//! When user click <b>Load target</b> action in Monkey toolBar
	void onActionLoadTarget();
	//! When user click <b>Exit debug mode</b> action in Monkey toolBar
	void onActionExit();
	//! When user click <b>Restart target</b> action in Monkey toolBar
	void onActionRestart();
	//! When user click <b>Continue</b> action in Monkey toolBar
	void onActionContinue();
	//! When user click <b>Step over</b> action in Monkey toolBar
	void onActionStepOver();
	//! When user click <b>Step into</b> action in Monkey toolBar
	void onActionStepInto();
	//! When user click <b>Step finish</b> action in Monkey toolBar
	void onActionStepFinish();

	// Gdb
	//! Calling when Gdb is started correctly
	void gdbStarted();
	//! Calling when Gdb is finished (exit normaly)
	void gdbFinished(  int a , QProcess::ExitStatus );
	//! Calling when Gdb generate a error (crash)
	void gdbError( QProcess::ProcessError);
	//! Calling when Gdb answer, Gdb have new data aviable
	void commandReadyRead(  const QString& d);

	// Target
	//! Target is loaded correctly
	void onTargetLoaded(int , QString);
	//! Target is no loaded correctly (format is not correct)
	void onTargetNoLoaded(int , QString);
	//! Target is running
	void onTargetRunning(int , QString);
	//! Target is exited correctly
	void onTargetExited(int , QString);
	//! Target is stopped (breakpoint for example)
	void onTargetStopped(int , QString);

	//! Calling when GdbParser class emit interpreter signal
	void onInterpreter(const QPointer<BaseInterpreter> & , const int &, const QString &);
	//! Calling when user click in the margin
	void onUserToggleBreakpoint(const QString & , const int & );
	
	// from parser
	void onDone(int , QString);
	void onInfo(int , QString);
	void onError(int , QString);
	void onPrompt(int , QString);

	void onAboutToClose();
};

#endif 
