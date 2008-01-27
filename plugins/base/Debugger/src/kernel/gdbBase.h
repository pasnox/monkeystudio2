#ifndef GDBBASEPLUGIN_H
#define GDBBASEPLUGIN_H

#include <QWidget>
#include <QDockWidget>
#include <QMainWindow>

#include "./kernel/gdbParser.h"
#include "./kernel/gdbInterpreter.h"
#include "./kernel/gdbMessage.h"
#include "./kernel/gdbTemplateProcess.h"


struct base_interpreter
{
 		QString cmdName;
		QRegExp cmdRegExp;
		QRegExp answerRegExp;
		QString answerExtention;
};

typedef base_interpreter QBaseInterpreter;




class GdbBase :  public QDockWidget , public QGdbMessageQueu, public QGdbInterpreter
{
	Q_OBJECT
	
public :

	enum {PROCESS_SEND_DATA,  PROCESS_TERMINED, PROCESS_WAITING};
	

	GdbBase(QWidget *o = 0);
	virtual   ~GdbBase() = 0;

	virtual int process(int , QByteArray data) = 0;
	virtual int processError(int , QByteArray) = 0;
	virtual void processExit() = 0;

	virtual void gdbStarted(void) = 0;
	virtual void gdbFinished(void)=0;
	virtual void targetLoaded(void) = 0;
	virtual void targetStopped(void) = 0;
	virtual void targetExited(void) = 0;
	virtual void targetRunning(void) = 0;


	virtual QString name() =0;
	virtual QWidget*  widget() =0;

	virtual void setupDockWidget(QMainWindow *)=0;

	void setParser(GdbParser * ) ;


	QByteArray getParametre(QByteArray param, QByteArray line);
	QList<QByteArray> extractBlock(QByteArray line);

	void addInterpreter(QBaseInterpreter);
	void addInterpreter(QGdbInterpreter i);
	void removeInterpreter();

	void processPostMessage(int, QByteArray);
	
	GdbParser *Parser;

	void send(GdbBase *p, QByteArray d)
	{ emit sendRawData(p, d);}

	QRegExp getLastCmd(){ return mLastCmd;};
 
signals:
	
	void sendRawData(GdbBase * , QByteArray);
	void sendMessage(int, QByteArray);

private :

	int currentInterpreter;
	QRegExp mLastCmd;

//	bool bWatchDog;
//	bool bSending;
 //	bool bBreakpointHit;
};
#endif
