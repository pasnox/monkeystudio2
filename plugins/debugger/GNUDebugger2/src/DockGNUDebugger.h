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
#include "./kernel/gdbConnectTemplate.h"
#include "./kernel/gdbBridgeEditor.h"
#include "./kernel/gdbKernelDispatcher.h"

#include "./addon/gdbBreakpoint.h"
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

//	QPointer<class GdbBreakpoint> Breakpoint;
	class GdbBreakpoint *Breakpoint;
	class GdbBacktrace *Backtrace;

	void setAction(QHash<QString, QAction*> *);

private:

	DockGNUDebugger( QWidget* = 0 );
	~DockGNUDebugger();

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
	QByteArray crlf;

	QString mPathGdb;
	
	/*	struct
	{
		QList<QString> fileName;
		QList<pEditor*> pointeur;
	}editor;
	pEditor *findFile(QByteArray file);

	int currentBacktraceLine;
	QByteArray currentBacktraceFile;
*/
	
public slots:

	void setEnabledActions(bool b);

	// Actions
	void onActionLoadTarget();
	void onActionExit();
	void onActionRestart();
	void onActionContinue();
	void onActionStepOver();
	void onActionStepInto();
	void onActionStop();

// test breakpoint
void onActionToggleBreak();
	
	// Gdb
	void gdbStarted();
	void gdbFinished(  int a , QProcess::ExitStatus );
	void gdbError( QProcess::ProcessError);
	void commandReadyRead(  const QByteArray& d);

	// Target
	void onTargetLoaded(int , QString);
	void onTargetNoLoaded(int , QString);
	void onTargetRunning(int , QString);
	void onTargetExited(int , QString);
	void onTargetStopped(int , QString);
	void onInterpreter(const QPointer<BaseInterpreter> & , const int &, const QString &);
	
	void onUserToggleBreakpoint(const QString & , const int & );
	
	//	void onFileOpened( const QString& );
//	void onFileClosed( const QString&  );
//	void onFileClosed( int);

/*	void onWantStart(QString);
	void onWantExit();

	void onMarginClicked(int , int  , Qt::KeyboardModifiers );

	// frome Bridge Editor
	void onBreakpoint(QByteArray filename, int line, QByteArray, QByteArray, bool);
	void onBacktrace(QByteArray filename, int line);
	void onBreakpointMoved(QByteArray filename, int beforLine, int afterLine);
	void onBreakpointEnabled(QByteArray filename, int line, bool b);
	void onBreakpointConditionnaled(QByteArray filename, int line, bool b);
	void onGotoBreakpoint(QByteArray filename, int line);

	// frome console
	void gdbStarted( const pCommand& c);
	void gdbFinished( const pCommand& c, int a , QProcess::ExitStatus );
	void gdbError(const pCommand&, QProcess::ProcessError);
	
	void commandReadyRead( const pCommand& c , const QByteArray& d);
	
	// from plugin
	void onSendRawData(GdbCore *plug, QByteArray data);

	// from parser
*/	void onDone(int , QString);
	void onInfo(int , QString);
	void onError(int , QString);
/*
	void onTargetExited(int , QString);
	void onTargetLoaded(int , QString);
	void onTargetNoLoaded(int , QString);
	void onTargetRunning(int , QString);
	void onTargetStopped(int , QString);
*/
	void onAboutToClose();
};

#endif 
