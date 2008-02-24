/*
	GdbPluginManager
*/


#ifndef GDBKERNELDISPATCHER_H
#define GDBKERNELDISPATCHER_H

#include "./kernel/gdbCore.h"

#include <QTextEdit>

class GdbKernelDispatcher :  public GdbCore
{
	Q_OBJECT

public :

	GdbKernelDispatcher( GdbParser *p);
	~GdbKernelDispatcher();

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
//	QWidget *widget();

//	void setupDockWidget(QMainWindow *);

	void add(GdbCore *);
	void stopAll();
public slots:

	void onPostMessage( int, QByteArray data);
	void onSendRawData(GdbCore * , QByteArray);

private:

	typedef QList<GdbCore *> QListPlugin;	
	QListPlugin listPlugin;

	QTextEdit *mWidget;

//	GdbBase *plugSender;

};
#endif
