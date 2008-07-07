

#ifndef GDBSCRIPT_H
#define GDBSCRIPT_H

#include <QtScript>
#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"


class GdbScript : public GdbCore
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

public slots:

	void onRequestScriptTranslate(const QString &, const QString &);

public :

	GdbScript(QObject * parent =0);
	~GdbScript();
	
	static bool canTranslate(const QString &);

	bool createScript(const QString &);
	void exec();
	void exec(const QString & );
	void extractValue(const QString &);

	void data(int , QString);
	
	void gdbStarted(){}
	void gdbFinished() {}
	void gdbError() {}

	void targetLoaded(const int &, const QString &){}
	void targetNoLoaded(const int &, const QString &) {}
	void targetRunning(const int &, const QString &) {}
	void targetStopped(const int &, const QString &) {}
	void targetExited(const int &, const QString &) {}

	void error(const int &, const QString &) 
	{
		setWaitEndProcess(false);
	}

	void done(const int &, const QString &) {}
	void info(const int &, const QString &) {}
	void prompt(const int &, const QString &)
	{ 
		setWaitEndProcess(false);
		exec();
	}

	QString name(){ return "GdbScript";} 
	QPointer<QWidget> widget(){return (QPointer<QWidget>)(NULL);}
	QIcon icon() { return QIcon();}

	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	int mSequence;

private:

	bool loadScript(const QString & , QPointer<QScriptEngine>);

	QString mData;
	QString mVarName;
	QString mPathScript;

	QList<GdbList> gdbScriptList;
	QPointer<BaseInterpreter> interpreterScript;

	int currentScriptIndex;

	GdbConnectTemplate<GdbScript> Connect;

signals :

		void scriptFinishedTranslate(const QString &); 

};

#endif
