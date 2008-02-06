/*
	GdbControl
*/

#ifndef GDBCONTROL_H
#define GDBCONTROL_H


#include <QObject>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>

#include "./kernel/gdbCore.h"
#include "./kernel/gdbTemplateCore.h"

class GdbControl : public GdbCore
{
	Q_OBJECT

	QPushButton *bStepOver;
	QPushButton *bRun ;
	QPushButton *bStepInto ;
	QPushButton *bContinue;
	QPushButton *bStop;
	QPushButton *bLoadTarget;

public : // function
	GdbControl( GdbParser *p);
	~GdbControl();
	
	int process(QGdbMessageCore);
	int processError(QGdbMessageCore) ;
	void processExit();

	int processSteps(QGdbMessageCore);
	int processQuestion(QGdbMessageCore);


	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();
/*	QWidget * widget();

	void setupDockWidget(QMainWindow *);
*/
public slots:

	void onRun();
	void onStepOver();
	void onStepInto();
	void onContinue();
	void onStop();
	void onLoadTarget();

private: // function
	QWidget *mWidget;
	GdbTemplateCore<GdbControl> cmd;

	QGdbInterpreter *interpreterEndSteppingRange;
	QGdbInterpreter *interpreterKill;
	QGdbInterpreter *interpreterStop;

};

#endif
