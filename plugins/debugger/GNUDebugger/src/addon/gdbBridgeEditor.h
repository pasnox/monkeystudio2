/*
	GdbBridgeEditor
*/

#ifndef GDBBRIDGEEDITOR_H
#define GDBBRIDGEEDITOR_H


#include <QObject>
#include <QTextEdit>

#include "../kernel/gdbCore.h"
#include "../kernel/gdbTemplateCore.h"

class GdbBridgeEditor : public GdbCore
{
	Q_OBJECT


public : // function
	GdbBridgeEditor(GdbParser *p);
	~GdbBridgeEditor();
	
	int process(QGdbMessageCore);
	int processError(QGdbMessageCore) ;
	void processPrompt();

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();

	int processBackTrace(QGdbMessageCore);
	int processBreakpointMoved(QGdbMessageCore);
	int processBreakpointAdd(QGdbMessageCore);
	int processBreakpointDeleted(QGdbMessageCore);
	int processBreakpointEnabled(QGdbMessageCore);
	int processBreakpointDisabled(QGdbMessageCore);
	int processBreakpointConditionned(QGdbMessageCore);
	int processBreakpointUnConditionned(QGdbMessageCore);
	int processGotoBreakpoint(QGdbMessageCore);

private: 
	QTextEdit *mWidget;
	GdbTemplateCore<GdbBridgeEditor> cmd;

signals:

	void breakpoint(QByteArray , int , QByteArray, QByteArray ,bool);
	void breakpointConditionnaled(QByteArray , int , bool);
	void breakpointEnabled(QByteArray , int , bool);
	void backtrace(QByteArray, int);
	void breakpointMoved(QByteArray , int, int);
	void gotoBreakpoint(QByteArray, int);
};

#endif
