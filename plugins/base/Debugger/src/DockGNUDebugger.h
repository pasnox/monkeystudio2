#ifndef PDOCKGNUDEBUGGER_H
#define PDOCKGNUDEBUGGER_H

#include "QSingleton.h"
#include "pConsoleManager.h"
#include "pCommand.h"

#include "pDockWidget.h"
#include <QMainWindow>

#include "./kernel/gdbCore.h"

#include "./kernel/gdbParser.h"
#include "./kernel/gdbKernelDispatcher.h"
#include "./addon/gdbBridgeEditor.h"
#include "./addon/gdbBreakPoint.h"
#include "./addon/gdbControl.h"
#include "./addon/gdbBackTrace.h"
#include "./addon/gdbRegisters.h"
#include "./addon/gdbWatch.h"
#include "./addon/gdbAnswer.h"




class DockGNUDebugger : public pDockWidget, public QSingleton<DockGNUDebugger>
{
	Q_OBJECT
	friend class QSingleton<DockGNUDebugger>;
	friend class GNUDebugger;
	
public:

	QMainWindow *mw;
	class GdbKernelDispatcher *kernelDispatcher;
	class GdbBridgeEditor *bridgeEditor;
	class GdbParser *Parser;
	class GdbBackTrace * backtraceGdb;
	class GdbRegisters *registersGdb;
	class GdbWatch *watchGdb;
	class GdbBreakPoint *breakpointGdb;
	class GdbControl *controlGdb;
	class GdbAnswer *answerGdb;	
	
	class pConsoleManager *pConsole;
	class pCommand *Cmd;


protected:

private:

	DockGNUDebugger( QWidget* = 0 );
	 ~DockGNUDebugger();

	QByteArray crlf;

	int iconbreakenable;
	int iconbreakdisable;
	int iconbacktrace;

	int iconconditionenable;
	int iconconditiondisable;

public slots:

private slots:
	void onFileOpened( const QString& );
	void onMarginClicked(int , int  , Qt::KeyboardModifiers );

	// frome Bridge Editor
	void onBreakpoint(QByteArray filename, int line, bool b);
	void onBacktrace(QByteArray filename, int line);
	void onBreakpointMoved(QByteArray filename, int beforLine, int afterLine);
	void onBreakpointEnabled(QByteArray filename, int line, bool b);
	void onBreakpointConditionnaled(QByteArray filename, int line, bool b);


	// frome console
	void gdbStarted( const pCommand& c);
	void gdbFinished( const pCommand& c, int a , QProcess::ExitStatus );

	void commandReadyRead( const pCommand& c , const QByteArray& d);
	
	// frome plugin
	void onSendRawData(GdbCore *plug, QByteArray data);

	// frome parser
	void onDone(int , QString);
	void onInfo(int , QString);
	void onError(int , QString);

	void onTargetExited(int , QString);
	void onTargetLoaded(int , QString);
	void onTargetRunning(int , QString);
	void onTargetStopped(int , QString);

signals:

};

#endif 
