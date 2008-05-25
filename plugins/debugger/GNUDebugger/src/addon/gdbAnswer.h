/*
	GdbBackTrace
*/

#ifndef GDBANSWER_H
#define GDBANSWER_H


#include <QObject>
#include <QTextEdit>

#include "../kernel/gdbCore.h"
#include "../kernel/gdbTemplateCore.h"

class GdbAnswer : public GdbCore
{

		Q_OBJECT

public : // function


	GdbAnswer(GdbParser *p=0);
	~GdbAnswer();
	
	int process(QGdbMessageCore m);
	int processError(QGdbMessageCore m) ;
	void processPrompt();

	int processNo(QGdbMessageCore m);
	int processYes(QGdbMessageCore m);
	
	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();
	QWidget * widget() { return mWidget;}

private: // function

	QTextEdit *mWidget;
	GdbTemplateCore<GdbAnswer> cmd;
	QGdbInterpreter * interpreterNo;
	QGdbInterpreter * interpreterYes;
};

#endif
