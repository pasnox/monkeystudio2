/*
	GdbControl
*/

#ifndef GDBREGISTERS_H
#define GDBREGISTERS_H


#include <QObject>
#include <QTextEdit>

#include "./kernel/gdbBase.h"

class GdbRegisters : public GdbBase
{
	Q_OBJECT

private: // variable

	bool bJustAdd;
	bool bTargetLoaded;
	bool bTargetRunning;
	bool bGdbStarted;


public : // function
	GdbRegisters(QWidget *p=0);
	~GdbRegisters();
	
	int process(int id, QByteArray);
	int processError(int , QByteArray) ;
	void processExit();

	int processRegisters(int id, QByteArray data);

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();
	QWidget * widget();

	void setupDockWidget(QMainWindow *);

public :

	QGdbInterpreter *interpreterRegisters;

private: // function
	QTextEdit *mWidget;
	bool enteringBlock;
	GdbTemplateProcess<GdbRegisters> cmd;
};

#endif
