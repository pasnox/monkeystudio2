#ifndef GDBCORE_H
#define GDBCORE_H

#include "./kernel/gdbParser.h"
#include "./kernel/gdbInterpreter.h"

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QDockWidget>
#include <QMessageBox>

/*
	class GdbCore
*/

// for debuggin
#include <QFile>

enum {PROCESS_SEND_DATA,  PROCESS_TERMINED, PROCESS_WAITING};
enum{MESSAGE_EMPTY = -2, INVALID_INTERPRETER};
enum{TARGET_RUNNING, TARGET_STOPPED, TARGET_EXITED, GDB_STARTED, GDB_FINISHED};


struct gdbMessage
{
		QByteArray msg;
		int id;
};
typedef gdbMessage QGdbMessageCore;


class GdbCore :  public QThread
{
	Q_OBJECT


private :

	QFile file ;

	int mCurrentInterpreterIndex;
	QRegExp mLastCmd;

	int statusProcess;
	QMutex mutexMessage;
	QMutex mutexEndProcess;
	QWaitCondition condition;
	QList<QGdbMessageCore> mMsgQueue;

	bool bTargetLoaded;
	bool bTargetRunning;
	bool bTargetStopped;
	bool bTargetExited;

	bool bGdbStarted;
	bool bGdbFinished;

	class GdbParser	 * mParser;
	class QDockWidget *mContainer;

public :

	
	GdbCore( GdbParser *p);
	virtual   ~GdbCore() ;

	virtual int process(QGdbMessageCore) {return PROCESS_TERMINED;}//= 0;
	virtual int processError(QGdbMessageCore) {return PROCESS_TERMINED;}//= 0;
	virtual void processExit() {}//= 0;

	virtual QString name(){ return QString();};// = 0;

	virtual void gdbStarted(void)
		{bGdbStarted = true; bGdbFinished = false;}

	virtual void gdbFinished(void)
			{bGdbFinished = true; bGdbStarted = false; 
			bTargetStopped = true; bTargetLoaded = false; 
			bTargetRunning = false; bTargetLoaded = false;}

	virtual void targetLoaded(void) 
		{ bTargetLoaded = true, bTargetRunning = false; bTargetStopped = true;}

	virtual void targetStopped(void)
		{bTargetStopped = true; bTargetLoaded = true; bTargetRunning = false;}

	virtual void targetExited(void)
		{ bTargetExited = true, bTargetRunning = false, bTargetStopped = true;}

	virtual void targetRunning(void)
		{ bTargetRunning = true, bTargetStopped = false, bTargetLoaded = true;}


	void run();

	int isTargetLoaded() {return bTargetLoaded;}
	int isTargetStopped() {return bTargetStopped;}
	int isTargExited() {return bTargetExited;}
	int isTargetRunning() {return bTargetRunning;}

	int isGdbStarted() { return bGdbStarted;}
	int isGdbFinished() { return bGdbFinished;}
	//
	GdbParser * getParser() {return mParser;}
	QDockWidget * getContainer() {return mContainer;}
	//
	void addInterpreter(QGdbInterpreter i);
	void removeInterpreter();
	//
	void processPostMessage(int, QByteArray);
	void processSendRawData(GdbCore *p, QByteArray d);
	//
	QGdbMessageCore getGdbMessage();
	void postGdbMessage(QGdbMessageCore);
	int getGdbMessageCount(){ return mMsgQueue.count();}
	void clearMessagesQueue() { mMsgQueue.clear();}
	//
	QByteArray getParametre(QByteArray param, QByteArray line);
	QList<QByteArray> extractBlock(QByteArray line);

	QRegExp getLastCmd(){ return mLastCmd;};

public slots : 
	void onReadyProcess(QGdbMessageCore);

signals:
	
	void sendRawData(GdbCore * , QByteArray);
	void sendPostMessage(int, QByteArray);
	void readyProcess(QGdbMessageCore);
};
#endif
