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
	\file DockGNUDebugger.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class is a main class container. 
*/

#ifndef PDOCKGNUDEBUGGER_H
#define PDOCKGNUDEBUGGER_H

// Pasnox include
#include <fresh.h>

// Xiantia include
#include <QTextEdit>
#include <QFileInfo>
#include <QTabWidget>
#include <QPointer>
#include <QHash>


#include "./kernel/gdbParser.h"
#include "./kernel/gdbProcess.h"
#include "./kernel/gdbInterpreter.h"
#include "./kernel/gdbSetting.h"
#include "./kernel/gdbConnectTemplate.h"
#include "./kernel/gdbBridgeEditor.h"
#include "./kernel/gdbKernelDispatcher.h"

#include "./addon/gdbBreakpoint/gdbBreakpoint.h"
#include "./addon/gdbBacktrace/gdbBacktrace.h"
#include "./addon/gdbRegister/gdbRegister.h"
#include "./addon/gdbWatch/gdbWatch.h"
#include "./addon/gdbCli/gdbCli.h"
#include "./addon/gdbToolTip/gdbToolTip.h"
#include "./addon/gdbScript/gdbScript.h"

/*!
	\brief This class is a main class container. 
	\details 
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
*/
	
class DockGNUDebugger : public pDockWidget, public QSingleton<DockGNUDebugger>
{
		Q_OBJECT
		friend class QSingleton<DockGNUDebugger>;
		friend class GNUDebugger;
		
public:

	/*!
		* \details Pointer to GdbParser Class
		*/
		QPointer<GdbParser> Parser;

	/*!
		* \details Pointer to GdbProcess Class
		*/
		QPointer<GdbProcess> Process;
		
		/*!
		* \details Pointer to GdbBridgeEditor Class
		*/
		QPointer<GdbBridgeEditor> Bridge;

	/*!
		* \details Pointer to GdbKernelDispatcher Class
		*/
		QPointer<GdbKernelDispatcher> Dispatcher;

	/*!
		* \details Create GdbBreakpoint Pointer Class
		*/
		class GdbBreakpoint *Breakpoint;

	/*!
		* \details Create GdbBacktrace Pointer Class
		*/
		class GdbBacktrace *Backtrace;

	/*!
		* \details Create GdbBreakpoint Pointer Class
		*/
		class GdbRegister *Register;

	/*!
		* \details Create GdbWatch Pointer Class
		*/
		class GdbWatch *Watch;

	/*!
		* \details Create GdbCli Pointer Class
		*/
		class GdbCli *Cli;

	/*!
		* \details Create GdbToolTip Pointer Class
		*/
		class GdbToolTip *ToolTip;

	/*!
		* \details Create GdbScript Pointer Class
		*/
		class GdbScript *Script;

	/*!
		* \details Contains all action from GNUDebugger class plugin, this action is viewed in Monkey Studio ToolBar
		*/
		void setAction(QHash<QString, QAction*> *);

private:

	DockGNUDebugger( QWidget* = 0 );
	~DockGNUDebugger();

	void loadSettings();
	void saveSettings();

	// interpreter 
	/*!
		* \details Connect is a GdbConnectTemplate var, it's use for connect interperter to other function
		*/
		GdbConnectTemplate<DockGNUDebugger> Connect;

	/*!
		* \details Contains interpreterStepOver pointer. This pointer is connect to onTargetStopped function
		*/
		QPointer<BaseInterpreter> interpreterStepOver;

	/*!
		* \details Contains interpreterStepInto pointer. This pointer is connect to onTargetStopped function
		*/
		QPointer<BaseInterpreter> interpreterStepInto;

	/*!
		* \details Contains interpreterStepFinish pointer. This pointer is connect to onTargetStopped function
		*/
		QPointer<BaseInterpreter> interpreterStepFinish;
		
		/*!
		* \details Main container for GNU debugger is QTabWidget
		*/
		QTabWidget *mainTabWidget;

	/*!
		* \details Add raw log in QTabWidget
		*/
		QTextEdit *rawLog;

	/*!
		* \details Path and name of target selected
		*/
		QString mSelectedTarget;

	/*!
		* \details Action list for remote MonkeyStudio::toolBar
		*/
		QHash<QString, QAction*> *mActionList;

	/*!
		* \details End of line
		*/
		QString crlf;

	/*!
		* \details Indicate if Gdb is started
		*/
		bool isGdbStarted;
		
		/*!
		* \details Indicate if target is running
		*/
		bool isTargetRunning;

	/*!
		* \details This var is use for not have two stopped signal consecutive
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
