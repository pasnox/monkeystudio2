/*
	GdbBackTrace
*/

#ifndef GDBBACKTRACE_H
#define GDBBACKTRACE_H


#include <QObject>
#include <QTextEdit>

#include "./kernel/gdbBase.h"


class GdbBackTrace : public GdbBase
{

		Q_OBJECT

private: // variable

	bool bJustAdd;
	bool bTargetLoaded;
	bool bTargetRunning;
	bool bGdbStarted;


public : // function


	GdbBackTrace(QWidget *p=0);
	~GdbBackTrace();
	
	int process(int id, QByteArray);
	int processError(int , QByteArray) ;
	void processExit();

	int processBacktrace(int id, QByteArray data);
	int processInfosource(int id, QByteArray data);

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();
	QWidget * widget();

	void setupDockWidget(QMainWindow *);

private: // function

	QTextEdit *mWidget;
	QString fileName;
	QString line;
	QByteArray buffer;

	GdbTemplateProcess<GdbBackTrace> cmd;
	QGdbInterpreter *interpreterBackTrace;
	QGdbInterpreter *interpreterInfoSource;


};

#endif
