/*
	GdbBridgeEditor
*/

#ifndef GDBBRIDGEEDITOR_H
#define GDBBRIDGEEDITOR_H


#include <QObject>
#include <QTextEdit>

#include "./kernel/gdbBase.h"

class GdbBridgeEditor : public GdbBase
{
	Q_OBJECT

private: // variable

	bool bJustAdd;
	bool bTargetLoaded;
	bool bTargetRunning;
	bool bGdbStarted;


public : // function
	GdbBridgeEditor(QWidget *p=0);
	~GdbBridgeEditor();
	
	int process(int id, QByteArray);
	int processError(int , QByteArray) ;
	void processExit();
	int processGeneric(int , QByteArray) ;

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();
	QWidget * widget();

	void setupDockWidget(QMainWindow *);

	int processBackTrace(int id , QByteArray data);
	int processBreakpointMoved(int id , QByteArray data);
	int processBreakpointAdd(int id , QByteArray data);
	int processBreakpointDeleted(int id , QByteArray data);
	int processBreakpointEnabled(int id , QByteArray data);
	int processBreakpointDisabled(int id , QByteArray data);
	int processBreakpointConditionned(int id , QByteArray data);
	int processBreakpointUnConditionned(int id , QByteArray data);

private: 
	QTextEdit *mWidget;
	GdbTemplateProcess<GdbBridgeEditor> cmd;

signals:

	void breakpoint(QByteArray , int , bool);
	void breakpointConditionnaled(QByteArray , int , bool);
	void breakpointEnabled(QByteArray , int , bool);
	void backtrace(QByteArray, int);
	void breakpointMoved(QByteArray , int, int);
};

#endif
