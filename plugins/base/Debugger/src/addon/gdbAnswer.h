/*
	GdbBackTrace
*/

#ifndef GDBANSWER_H
#define GDBANSWER_H


#include <QObject>
#include <QTextEdit>

#include "./kernel/gdbBase.h"


class GdbAnswer : public GdbBase
{

		Q_OBJECT

private: // variable

	bool bTargetLoaded;
	bool bTargetRunning;
	bool bGdbStarted;


public : // function


	GdbAnswer(QWidget *p=0);
	~GdbAnswer();
	
	int process(int id, QByteArray);
	int processError(int , QByteArray) ;
	void processExit();

	int processNo(int id, QByteArray);
	int processYes(int id, QByteArray);
	
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
	GdbTemplateProcess<GdbAnswer> cmd;
	QGdbInterpreter * interpreterNo;
	QGdbInterpreter * interpreterYes;
};

#endif
