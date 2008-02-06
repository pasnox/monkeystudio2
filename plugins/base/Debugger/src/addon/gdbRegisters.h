/*
	GdbRegisters
*/

#ifndef GDBREGISTERS_H
#define GDBREGISTERS_H


#include <QObject>
#include <QTableWidget>
#include <QStringListModel>

#include "./kernel/gdbCore.h"
#include "./kernel/gdbTemplateCore.h"

class GdbRegisters : public GdbCore
{
	Q_OBJECT

public : // function
	GdbRegisters( GdbParser *p);
	~GdbRegisters();
	
	int process(QGdbMessageCore);
	int processError(QGdbMessageCore) ;
	void processExit();

	int processRegisters(QGdbMessageCore);

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();

public :

	QGdbInterpreter *interpreterRegisters;

private: // function

	QTableWidget *mWidget;
//	QStringListModel *model;
	QByteArray widgetSize;

	int numberOfRegisters;

	GdbTemplateCore<GdbRegisters> cmd;

	void showColor(QTableWidgetItem *p, QString a);

public slots:

	void onTopLevelChanged ( bool b);

};

#endif
