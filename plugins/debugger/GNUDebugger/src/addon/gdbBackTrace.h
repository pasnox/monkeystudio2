/*
	GdbBackTrace
*/

#ifndef GDBBACKTRACE_H
#define GDBBACKTRACE_H


#include <QObject>
#include <QTextEdit>

#include "../kernel/gdbCore.h"
#include "../kernel/gdbTemplateCore.h"


class GdbBackTrace : public GdbCore
{

		Q_OBJECT

public : // function


	GdbBackTrace(GdbParser *p);
	~GdbBackTrace();
	
	int process(QGdbMessageCore);
	int processError(QGdbMessageCore) ;
	void processPrompt();

	int processBacktrace(QGdbMessageCore);
	int processInfosource(QGdbMessageCore);

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();

private: // function

	QTextEdit *mWidget;
	QString fileName;
	QString line;

	GdbTemplateCore<GdbBackTrace> cmd;
	QGdbInterpreter *interpreterBackTrace;
	QGdbInterpreter *interpreterInfoSource;


};

#endif
