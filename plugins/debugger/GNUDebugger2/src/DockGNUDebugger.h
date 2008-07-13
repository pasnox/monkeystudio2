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

/*!
* \brief This class is a main class container. 
* \file DockGNUDebugger.h 
* \author xiantia
* \version 1.3.2
* DockGNUDebugger is created by GNUDebugger.
* Those is the main container class. It creates all AddOn and the connect together.
* It has under its GdbParser order which parse warp the data coming from Gdb, 
* GdbProcess which controls Gdb, GdbBridgeEditor which control the editor Qsci and GdbKernelDispatcher which control AddOn.
* The creation of AddOn is very simple.
* AddOn are class which adds news function to the debugger such as GdbBreakpoint and GdbWatch.
* For that it has pointers on AddOn and add them to GdbKernelDispatcher so that AddOn receives the messages coming from GdbParser.
*
* in .h
*
* \code
*	// declare pointer to AddOn
*	class GdbBreakpoint *Breakpoint;
* \endcode
*
* in .cpp
*
* \code
*	// create new class
*	Breakpoint = new GdbBreakpoint(this);
*	// add them to Dispatcher
*	Dispatcher->add(Breakpoint);
* \endcode
*
* Connect signals
*
* \code
*	connect(Bridge, SIGNAL(requestBreakpoint(const QString &)), Breakpoint , SLOT(onRequestBreakpoint(const QString &)));
* \endcode
*
* Once creates AddOn are visible in QTabWidget of DockGNUDebugger if the function widget() return a pointer not NULL.
*
*
*\note All AddOn must derive from GdbCore.
*
*/

 
class DockGNUDebugger : public pDockWidget, public QSingleton<DockGNUDebugger>
{
	Q_OBJECT
	friend class QSingleton<DockGNUDebugger>;
	friend class GNUDebugger;
	
public:

	/*!
	 * \detail Pointer to GdbParser Class
	*/
	QPointer<GdbParser> Parser;

	/*!
	 * \detail Pointer to GdbProcess Class
	*/
	QPointer<GdbProcess> Process;
	
	/*!
	 * \detail Pointer to GdbBridgeEditor Class
	*/
	QPointer<GdbBridgeEditor> Bridge;

	/*!
	 * \detail Pointer to GdbKernelDispatcher Class
	*/
	QPointer<GdbKernelDispatcher> Dispatcher;

	/*!
	 * \detail Create GdbBreakpoint Pointer Class
	*/
	class GdbBreakpoint *Breakpoint;

	/*!
	 * \detail Create GdbBacktrace Pointer Class
	*/
	class GdbBacktrace *Backtrace;

	/*!
	 * \detail Create GdbBreakpoint Pointer Class
	*/
	class GdbRegister *Register;

	/*!
	 * \detail Create GdbWatch Pointer Class
	*/
	class GdbWatch *Watch;

	/*!
	 * \detail Create GdbCli Pointer Class
	*/
	class GdbCli *Cli;

	/*!
	 * \detail Create GdbToolTip Pointer Class
	*/
	class GdbToolTip *ToolTip;

	/*!
	 * \detail Create GdbScript Pointer Class
	*/
	class GdbScript *Script;

	/*!
	 * \detail Contains all action from GNUDebugger class plugin, this action is viewed in Monkey Studio ToolBar
	*/
	void setAction(QHash<QString, QAction*> *);

private:

	DockGNUDebugger( QWidget* = 0 );
	~DockGNUDebugger();

	void loadSettings();
	void saveSettings();

	// interpreter 
	/*!
	 * \detail Connect is a GdbConnectTemplate var, it's use for connect interperter to other function
	*/
	GdbConnectTemplate<DockGNUDebugger> Connect;

	/*!
	 * \detail Contains interpreterStepOver pointer. This pointer is connect to onTargetStopped function
	*/
	QPointer<BaseInterpreter> interpreterStepOver;

	/*!
	 * \detail Contains interpreterStepInto pointer. This pointer is connect to onTargetStopped function
	*/
	QPointer<BaseInterpreter> interpreterStepInto;

	/*!
	 * \detail Contains interpreterStepFinish pointer. This pointer is connect to onTargetStopped function
	*/
	QPointer<BaseInterpreter> interpreterStepFinish;
	
	/*!
	 * \detail Main container for GNU debugger is QTabWidget
	*/
	QTabWidget *mainTabWidget;

	/*!
	 * \detail Add raw log in QTabWidget
	*/
	QTextEdit *rawLog;

	/*!
	 * \detail Path and name of target selected
	*/
	QString mSelectedTarget;

	/*!
	 * \detail Action list for remote MonkeyStudio::toolBar
	*/
	QHash<QString, QAction*> *mActionList;

	/*!
	 * \detail End of line
	*/
	QString crlf;

	/*!
	 * \detail Indicate if Gdb is started
	*/
	bool isGdbStarted;
	
	/*!
	 * \detail Indicate if target is running
	*/
	bool isTargetRunning;

	/*!
	 * \detail This var is use for not have two stopped signal consecutive
	*/
	bool disableStep;	

public slots:

	void setEnabledActions(bool b);
	void onActionLoadTarget();
	void onActionExit();
	void onActionRestart();
	void onActionContinue();
	void onActionStepOver();
	void onActionStepInto();
	void onActionStepFinish();

	void gdbStarted();
	
	void gdbFinished(  int a , QProcess::ExitStatus );
	void gdbError( QProcess::ProcessError);
	void commandReadyRead(  const QString& d);

	// Target
	void onTargetLoaded(int , QString);
	void onTargetNoLoaded(int , QString);
	void onTargetRunning(int , QString);
	void onTargetExited(int , QString);
	void onTargetStopped(int , QString);

	void onInterpreter(const QPointer<BaseInterpreter> & , const int &, const QString &);
	
	void onUserToggleBreakpoint(const QString & , const int & );
	
	// from parser
	void onDone(int , QString);
	void onInfo(int , QString);
	void onError(int , QString);
	void onPrompt(int , QString);
};

#endif 
