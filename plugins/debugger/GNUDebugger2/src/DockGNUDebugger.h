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
#include "./addon/gdbBacktrace.h"


class DockGNUDebugger : public pDockWidget, public QSingleton<DockGNUDebugger>
{
	Q_OBJECT
	friend class QSingleton<DockGNUDebugger>;
	friend class GNUDebugger;
	
public:


	QPointer<GdbParser> Parser;
	QPointer<GdbProcess> Process;
	QPointer<GdbBridgeEditor> Bridge;
	QPointer<GdbKernelDispatcher> Dispatcher;

	class GdbBreakpoint *Breakpoint;
	class GdbBacktrace *Backtrace;

	void setAction(QHash<QString, QAction*> *);

private:

	DockGNUDebugger( QWidget* = 0 );
	~DockGNUDebugger();

	void loadSettings();
	void saveSettings();

	// interpreter 
	GdbConnectTemplate<DockGNUDebugger> *Connect;
	QPointer<BaseInterpreter> interpreterStepOver;
	QPointer<BaseInterpreter> interpreterStepInto;
	// main widget
	QTabWidget *mainTabWidget;
	// add log
	QTextEdit *rawLog;
	// path and name of target seleted
	QString mSelectedTarget;
	// Action list for remote MonkeyStudio::toolBar
	QHash<QString, QAction*> *mActionList;

	// end of line
	QString crlf;

//	QString mPathGdb;
	bool isGdbStarted;
	bool isTargetRunning;
	
public slots:

	void setEnabledActions(bool b);

	// Actions
	void onActionLoadTarget();
	void onActionExit();
	void onActionRestart();
	void onActionContinue();
	void onActionStepOver();
	void onActionStepInto();

	// Gdb
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

	void onAboutToClose();
};

#endif 
