/*
	GdbControl
*/

#ifndef GdbControl_H
#define GdbControl_H


#include <QObject>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>

#include "./kernel/gdbBase.h"

class GdbControl : public GdbBase
{
	Q_OBJECT

private: // variable

	bool bJustAdd;
	bool bTargetLoaded;
	bool bTargetRunning;
	bool bGdbStarted;
	
	QPushButton *bStepOver;
	QPushButton *bRun ;
	QPushButton *bStepInto ;
	QPushButton *bContinue;
	QPushButton *bStop;
	QPushButton *bLoadTarget;

public : // function
	GdbControl(QWidget *p=0);
	~GdbControl();
	
	int process(int id, QByteArray);
	int processError(int , QByteArray) ;
	void processExit();

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();
	QWidget * widget();

	void setupDockWidget(QMainWindow *);

public slots:

	void onRun();
	void onStepOver();
	void onStepInto();
	void onContinue();
	void onStop();
	void onLoadTarget();

private: // function
	QWidget *mWidget;
	GdbTemplateProcess<GdbControl> cmd;

	QGdbInterpreter *interpreterEndSteppingRange;

};

#endif
