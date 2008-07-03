

#ifndef GDBSCRIPT_H
#define GDBSCRIPT_H

#include <QObject>
#include <QtScript>
#include "gdbCore.1.3.h"
#include "gdbParser.1.3.h"
#include "gdbProcess.1.3.h"

class GdbScript : public QObject
{
	Q_OBJECT

	typedef struct GdbList
	{
		QString type;
		QString scriptFile;
		QString status;
		QString function;
		QString command;
		QPointer<QScriptEngine> script;
		bool isPointer;
	};

/*	typedef struct GdbScriptReturn
	{
		QString status;
		QString command;
		QString dec;
	};
*/
public :

	GdbScript(QObject * parent =0);
	~GdbScript();
	
	bool createScript(const QString &);
	void exec(/*QString data*/);
	void exec(QString);
	void extractValue(QString d);


	bool findNatif(const QString &);
	void exec(QString, QString);
	void data(QString);
	/*
	void gdbStarted(){};
	 void gdbFinished() {};
	 void gdbError() {};

	 void targetLoaded(const int &, const QString &){};
	 void targetNoLoaded(const int &, const QString &) {};
	 void targetRunning(const int &, const QString &) {};
	 void targetStopped(const int &, const QString &) {};
	 void targetExited(const int &, const QString &) {};

	 void error(const int &, const QString &) {};
	 void done(const int &, const QString &) {};
	 void info(const int &, const QString &) {};
	 void prompt(const int &, const QString &) {};

	 QString name(){ return "GdbScript";} 
	 QPointer<QWidget> widget(){return NULL;}
	 QIcon icon() { return QIcon();}

	 void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & ){};
*/
	void call(QString);
	QString processSend(QString);
	QString processReceive();//QString data);

	int mSequence;
private:

	QScriptEngine engine;
	bool loadScript(QString fileName);
	bool loadScript(const QString & fileName, QPointer<QScriptEngine>);

	QString mData;
	QString mVarName;

	QList<GdbList> gdbScriptList;

	int currentScriptIndex;

};

#endif
