#ifndef PDOCKGNUDEBUGGER_H
#define PDOCKGNUDEBUGGER_H

// Pasnox include
#include "QSingleton.h"
#include "pEditor.h"

//#include "pConsoleManager.h"
#include "pCommand.h"
 #include "UIMain.h"

#include "pDockWidget.h"

// Xiantia include
#include <QMainWindow>
#include <QFileInfo>

#include "./kernel/gdbCore.h"

#include "./kernel/gdbProcess.h"
#include "./kernel/gdbParser.h"
#include "./kernel/gdbKernelDispatcher.h"
#include "./addon/gdbBridgeEditor.h"
#include "./addon/gdbBreakPoint.h"
#include "./addon/gdbControl.h"
#include "./addon/gdbBackTrace.h"
#include "./addon/gdbRegisters.h"
#include "./addon/gdbWatch.h"
#include "./addon/gdbAnswer.h"
#include "./kernel/gdbProcess.h"



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
	
	class GdbProcess *pConsole;
	class pCommand *Cmd;

private:

	DockGNUDebugger( QWidget* = 0 );
	 ~DockGNUDebugger();

	QByteArray crlf;

	int iconbreakenable;
	int iconbreakdisable;
	int iconbacktrace;

	int iconconditionenable;
	int iconconditiondisable;

	QString targetName;

	struct
	{
		QList<QString> fileName;
		QList<pEditor*> pointeur;
	}editor;
	pEditor *findFile(QByteArray file);

	int currentBacktraceLine;
	QByteArray currentBacktraceFile;

private slots:

void onFileOpened( const QString& );
//	void onFileClosed( const QString&  );
	void onFileClosed( int);

	void onWantStart(QString);
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

	// frome parser
	void onDone(int , QString);
	void onInfo(int , QString);
	void onError(int , QString);

	void onTargetExited(int , QString);
	void onTargetLoaded(int , QString);
	void onTargetNoLoaded(int , QString);
	void onTargetRunning(int , QString);
	void onTargetStopped(int , QString);

	void onAboutToClose();
};

#endif 
