/*
	GdbBreakpoint
*/

#ifndef GdbBreakPoint_H
#define GdbBreakPoint_H


#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileInfo>

#include "../kernel/gdbCore.h"
#include "../kernel/gdbTemplateCore.h"


struct BreakPoint
{
	QString fileName;
	QList<int> lineInFile;
	QList<int> indexInGdb;
	QList<QByteArray> type;
	QList<QByteArray> enable;
};
typedef BreakPoint QBreakPoint;


class GdbBreakPoint : public GdbCore
{
	Q_OBJECT

private: // variable

	typedef QList<QBreakPoint*> QBreakPointList;
	QBreakPointList BreakPointList;
	QString icon_hit;
	bool bJustAdd;

public : // function
	GdbBreakPoint(GdbParser *p);
	~GdbBreakPoint();
	
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

public slots:
	void onEnableChanged(QTableWidgetItem *item);
	void onConditionChanged(QTableWidgetItem *item);
	void onBreakpointDoubleClicked(QTableWidgetItem *it);

	void onTopLevelChanged ( bool b);

public :
	int processFromEditor(QGdbMessageCore);

private :

	GdbTemplateCore<GdbBreakPoint> cmd;
	QGdbInterpreter *interpreterAdd;
	QGdbInterpreter *interpreterDel;
	QGdbInterpreter *interpreterEnable;
	QGdbInterpreter *interpreterDisable;

	QByteArray widgetSize;

	bool waitEndProcess;

private : // function

	int addBreakpoint(QGdbMessageCore);
	int deleteBreakpoint(QGdbMessageCore);
	int hitBreakpoint(QGdbMessageCore);

	int enableBreakpoint(QGdbMessageCore);
	int disableBreakpoint(QGdbMessageCore);

	int requestedBreakpoint(QGdbMessageCore);

	void toggleBreakPoint(QString fullName, int numLine);
	QBreakPoint * getBreakPointByName(QByteArray fileName);
//	QBreakPoint * getBreakPointByItem(QTableWidgetItem * item);
	int getBreakPoint(QString);
	int asBreakPointAt(QBreakPoint *GdbBreakPoint, int);

	QTableWidget *tabWidget;
	void addRowInTab(/*QBreakPoint *bp ,*/QByteArray number, QByteArray line, QByteArray file);
	void hit(QBreakPoint * ,int);
};

#endif
