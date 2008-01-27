/*
	GdbBreakpoint
*/

#ifndef GdbBreakPoint_H
#define GdbBreakPoint_H


#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileInfo>

#include "./kernel/gdbBase.h"

struct BreakPoint
{
	QString fileName;
	QList<int> lineInFile;
	QList<int> indexInGdb;

	QList<QTableWidgetItem *> enable;
	QList<QTableWidgetItem *> condition;
	QList<QTableWidgetItem *> bkptno;
	QList<QTableWidgetItem *> line;
	QList<QTableWidgetItem *> file;
	
};
typedef BreakPoint QBreakPoint;


class GdbBreakPoint : public GdbBase
{
	Q_OBJECT

private: // variable

	typedef QList<QBreakPoint*> QBreakPointList;
	QBreakPointList BreakPointList;
	QString icon_hit;
	bool bJustAdd;
	bool bTargetLoaded;
	bool bTargetRunning;
	bool bGdbStarted;

public : // function
	GdbBreakPoint(QWidget *p=0);
	~GdbBreakPoint();
	
	int process(int , QByteArray);
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
	void onEnableChanged(QTableWidgetItem *item);
	void onConditionChanged(QTableWidgetItem *item);

public :
	int processFromEditor(int id, QByteArray data);

private :

	GdbTemplateProcess<GdbBreakPoint> cmd;
	QGdbInterpreter *interpreterAdd;
	QGdbInterpreter *interpreterDel;
	QGdbInterpreter *interpreterEnable;
	QGdbInterpreter *interpreterDisable;

private : // function

	

	int addBreakpoint(int, QByteArray);
	int deleteBreakpoint(int, QByteArray);
	int hitBreakpoint(int, QByteArray);

	int enableBreakpoint(int, QByteArray);
	int disableBreakpoint(int, QByteArray);

	void toggleBreakPoint(QString fullName, int numLine);
	QBreakPoint * getBreakPointByName(QByteArray fileName);
	QBreakPoint * getBreakPointByItem(QTableWidgetItem * item);
	int getBreakPoint(QString);
	int asBreakPointAt(QBreakPoint *GdbBreakPoint, int);

	QTableWidget *tabWidget;
	void addRowInTab(QBreakPoint *bp ,QByteArray number, QByteArray line, QByteArray file);
	void hit(QBreakPoint * ,int);
};

#endif
