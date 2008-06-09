/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbKernelDispatcher
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/


#ifndef GDBCORE_H
#define GDBCORE_H

#include <QTimer>

#include "gdbConnectTemplate.1.3.h"
#include "gdbInterpreter.1.3.h"
#include "gdbParser.1.3.h"
#include "gdbProcess.1.3.h"

#include <coremanager.h>
#include <settingsmanager.h>
#include <monkey.h>
#include <queuedstatusbar.h>

enum SHOW{ _WARNING_,  _CRITICAL_, _INFO_};
			

class GdbCore : public QObject
{
	Q_OBJECT

public:

	GdbCore(QObject * parent = 0);
	~GdbCore();

	QPointer<GdbParser> Parser() {  return mParser;}
	QPointer<GdbProcess> Process() {  return mProcess;}

public slots:

	virtual void gdbStarted();
	virtual void gdbFinished();
	virtual void gdbError();

	virtual void targetLoaded(const int &, const QString &);
	virtual void targetNoLoaded(const int &, const QString &);
	virtual void targetRunning(const int &, const QString &);
	virtual void targetStopped(const int &, const QString &);
	virtual void targetExited(const int &, const QString &);

	virtual void error(const int &, const QString &);
	virtual void done(const int &, const QString &);
	virtual void info(const int &, const QString &);

	virtual void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	virtual QString name(); 
	virtual QPointer<QWidget> widget();

	QString findValue(const QString & , const QString &);

	void setEnabled(const bool &  );
	bool isEnabled() ;

	void setWaitEndProcess(const bool & );
	bool isWaitEndProcess();

	void showMessage(QString, int, SHOW);
private slots:

	void onTimer();
	
private : 

	QPointer<GdbParser> mParser;
	QPointer<GdbProcess> mProcess;
	bool mEnabled;
	bool mWaitEndProcess;
	QTimer watchDog;
};

#endif
