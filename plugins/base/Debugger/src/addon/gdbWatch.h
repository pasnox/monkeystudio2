/*
	GdbControl
*/

#ifndef GDBWATCH_H
#define GDBWATCH_H


#include <QObject>
#include <QTableWidget>
#include <QMouseEvent>

#include "./kernel/gdbBase.h"


class QTableWidgetEx : public QTableWidget
{
	Q_OBJECT
public :
	
	QTableWidgetEx(QWidget *p);

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


class GdbWatch : public GdbBase
{
	Q_OBJECT

private: // variable

	bool bJustAdd;
	bool bTargetLoaded;
	bool bTargetRunning;
	bool bGdbStarted;


public : // function
	GdbWatch(QWidget *p=0);
	~GdbWatch();
	
	int process(int id, QByteArray);
	int processError(int , QByteArray) ;
	void processExit();

	int processWatchPrint(int , QByteArray);
	int processWatchAdresse(int , QByteArray);
	int processWatchType(int , QByteArray);

	void gdbStarted();
	void gdbFinished();

	void targetLoaded();
	void targetRunning();
	void targetStopped();
	void targetExited();

	QString name();
	QWidget * widget();

	void setupDockWidget(QMainWindow *);

public slots :

	void addNewVar(QString );
	void removeVar(int );

private : // function

	struct svar
	{	
		QList<QGdbInterpreter *> interpreterPrint;
		QList<QGdbInterpreter *> interpreterAdresse;
		QList<QGdbInterpreter *> interpreterType;
	};

	svar varList;

	struct svariable
	{
		QString varName;
		QTableWidgetItem *varNameItem;
		QString varType;
		QTableWidgetItem *varTypeItem;
		QString varAdresse;
		QTableWidgetItem *varAdresseItem;
		QString varValue;
		QTableWidgetItem *varValueItem;
	};

	QList<svariable> variableList;

	GdbTemplateProcess<GdbWatch> cmd;

	QTableWidgetEx *mWidget;

	int currentRow;
	int currentColumn;
};

#endif
