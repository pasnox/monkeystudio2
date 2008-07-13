

#ifndef GDBSCRIPT_H
#define GDBSCRIPT_H

#include <QtScript>
#include <QObject>
#include "../../kernel/gdbCore.1.3.h"
#include "../../kernel/gdbSequencer.1.3.h"

//! This class implements all function for use script .js. This class is an AddOn for GNU debugger
 
/**
\author xiantia
\version 1.3.2

This class can translate Qt class (QString, QList, QImage,...) in other format.
This new format can be show in GdbWatch class. This new format can be use by WatchStruct class if is correct.

For example QString can't be show directly. When Gdb print QString variable, it return a struture but the true value
of QString is not present. A script can send other commands for create the true value of QString variable.

This class use .js script.
*/
class GdbScript : public GdbCore
{
	Q_OBJECT

	//! This structure store all informations for use a script. 
	typedef struct GdbList
	{
		//! Type of script, QString , QList<int>, ...
		QString type;
		//! contains the casting type
		QString cast;
		//! Path of script file.
		QString scriptFile;
		//! Status returned by script , FINISH, ERROR, WAIT.
		QString status;
		//! Function name to call.
		QString function;
		//! Command returned by script.
		QString command;
		//! Pointer to script engine.
		QPointer<QScriptEngine> script;
		//! The type of variable is a pointer ?
		bool isPointer;
	};

public slots:

	/**
	 * \brief Calling when other Addon want translate Qt class in other format.
	*/
	void onRequestScriptTranslate(const QString &, const QString &);

public :

	GdbScript(QObject * parent =0);
	~GdbScript();
	
	/**
	 * \brief Just test if Qt class type can be translated by .js.
	*/
	static bool tryTranslate(const QString &);

	/**
	 * \brief Create all script list for translate the current variable type.
	 * \retval true if Qt container can be translated. 
	*/
	bool createScript(const QString &);
	
	/**
	 * \brief Execute script with mData
	*/
	void exec();

	/**
	 * \brief execute script with other data, for example <b>varName</b>.
	*/
	void exec(const QString & );
	
	/**
	 * \brief Extract value from script answer.
	*/
	void extractValue(const QString &);

	/**
	 * \brief New datas from Gdb is avaible.
	*/
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


	//! Connect interpreter to function
	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	int mSequence;

private:

	//! Load script from .js and store this in GdbList struct
	bool loadScript(const QString & , QPointer<QScriptEngine>);

	QString mData;
	QString mVarName;
	QString mPathScript;

	//! List of all script for translate Qt container
	QList<GdbList> gdbScriptList;
	QPointer<BaseInterpreter> interpreterScript;

	int currentScriptIndex;

	GdbConnectTemplate<GdbScript> Connect;

signals :

	/**
	 * \brief when the translation is termined , this signal is emit, it contains the new forma of var
	*/
	void scriptFinishedTranslate(const QString &); 

};

#endif
