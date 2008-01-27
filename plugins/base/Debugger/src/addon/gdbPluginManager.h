/*
	GdbPluginManager
*/


#ifndef GDBPLUGINMANAGER_H
#define GDBPLUGINMANAGER_H

#include "./kernel/gdbBase.h"

#include <QTextEdit>

class GdbPluginManager :  public GdbBase
{
	Q_OBJECT

public:
	GdbPluginManager(QWidget *p= 0);
	~GdbPluginManager();

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
	QWidget *widget();

	void setupDockWidget(QMainWindow *);

	void addPlugin(QObject *);

public slots:

	void onPosteGdbMessage( int, QByteArray data);
	void onSendRawData(GdbBase * , QByteArray);

private:

	typedef QList<GdbBase *> QListPlugin;	
	QListPlugin listPlugin;

	QTextEdit *mWidget;
};
#endif
