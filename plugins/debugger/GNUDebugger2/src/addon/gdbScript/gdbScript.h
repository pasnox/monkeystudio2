/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
/*!
	\file gdbScript.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class implements all function for use script .js. This class is an AddOn for GNU debugger
*/

#ifndef GDBSCRIPT_H
#define GDBSCRIPT_H

#include <QtScript>
#include <QObject>
#include "../../kernel/gdbCore.h"
#include "../../kernel/gdbSequencer.h"

/*!
	\brief This class implements all function for use script .js. This class is an AddOn for GNU debugger
	\details This class can translate Qt class (QString, QList, QImage,...) in other format.
This new format can be show in GdbWatch class. This new format can be use by WatchStruct class if is correct.

For example QString can't be show directly. When Gdb print QString variable, it return a struture but the true value
of QString is not present. A script can send other commands for create the true value of QString variable.

This class use .js script.

*/
class GdbScript : public GdbCore
{
	Q_OBJECT

	/*!
		\brief This structure store all informations for use a script.
	*/	
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

	void onRequestScriptTranslate(const QString &, const QString &);

public :

	GdbScript(QObject * parent =0);
	~GdbScript();
	
	static bool tryTranslate(const QString &);
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


	/*!
		\sa GdbInterpreter
	*/
	void interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & );

	int mSequence;

private:

	bool loadScript(const QString & , QPointer<QScriptEngine>);

	QString mData;
	QString mVarName;
	QString mPathScript;

	/*!
		\details List of all script for translate Qt container
	*/
	QList<GdbList> gdbScriptList;
	QPointer<BaseInterpreter> interpreterScript;

	int currentScriptIndex;
	GdbConnectTemplate<GdbScript> Connect;

signals :

	/*!
		\details when the translation is termined , this signal is emit, it contains the new forma of var
	*/
	void scriptFinishedTranslate(const QString &); 

};

#endif
