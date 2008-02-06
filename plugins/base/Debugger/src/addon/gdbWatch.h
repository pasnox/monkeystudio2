/*
	GdbControl
*/

#ifndef GDBWATCH_H
#define GDBWATCH_H


#include <QObject>
#include <QTableWidget>
#include <QMouseEvent>

#include "./kernel/gdbCore.h"
#include "./kernel/gdbTemplateCore.h"

/*

*/

#include <QTreeWidget>
#include <QTreeWidgetItem>


class QTableWidgetEx : public QTableWidget
{
	Q_OBJECT
public :
	
	QTableWidgetEx(QWidget *p=0);

	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);

	void mousePressEvent ( QMouseEvent * event ) ;

private :

signals :
	void newVariable(QString);
	void removeVariable(int);
};


class GdbWatch : public GdbCore
{
	Q_OBJECT

public : // function
	GdbWatch( GdbParser *p);
	~GdbWatch();
	
	int process(QGdbMessageCore);
	int processError(QGdbMessageCore) ;
	void processExit();

	int processWatchPrint(QGdbMessageCore);
	int processWatchAdresse(QGdbMessageCore);
	int processWatchType(QGdbMessageCore);

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();

public slots :

	void addNewVar(QString );
	void removeVar(int );

private : // function

	struct 
	{	
		QList<QGdbInterpreter *> interpreterPrint;
		QList<QGdbInterpreter *> interpreterAdresse;
		QList<QGdbInterpreter *> interpreterType;
	}varList;

	GdbTemplateCore<GdbWatch> cmd;
	void showColor(QTableWidgetItem *p, QString a);

	QTableWidgetEx *mWidget;
	QTreeWidget *rootTree;

	int currentRow;
	int currentColumn;

	// for multi line sequence
	QString buffer;
	bool isPrintSequence;

private :

	QStringList extractMember(QString val);
	int memberType(QString val);
	QStringList formatMember(QString val);
	void decompilStrut(QTreeWidgetItem *parentItem, QString val);
	void decompilStrutUpdate(QTreeWidgetItem *parentItem, QString val);






};

#endif
