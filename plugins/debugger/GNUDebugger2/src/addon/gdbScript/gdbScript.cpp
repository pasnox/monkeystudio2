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
	\file gdbScript.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class implements all function for use script .js. This class is an AddOn for GNU debugger
*/

#include "gdbScript.h"
#include <QFile>
#include <QMessageBox>

#include "../../kernel/gdbSetting.1.3.h"

/*!
	\details Create new object
	\param parent of thi object
*/
GdbScript::GdbScript(QObject * parent) : GdbCore(parent)
{
	currentScriptIndex = 0;
	gdbScriptList.clear();


	interpreterScript = Parser()->addInterpreter(
		name(),
		QRegExp(".*"),
		QRegExp(".*"),
		"^info,interpreter=\"" + name() + "\",event=\"Script sending\",answerGdb=\"");

	// connect interpreter to function
	Connect.add(this, interpreterScript, &GdbScript::data);

	mPathScript = GdbSetting::instance()->getPathScript() + "/";

}

//
/*!
	\details none
*/
GdbScript::~GdbScript()
{
}

//

void GdbScript::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call(i, id, s);
}

// static function

/**
	\details Just test if Qt class type can be translated by .js.
	\param container is type of Qt var (QList<QString> for exemple). 
	\retval true if Qt container can be translated. 
*/
bool GdbScript::tryTranslate(const QString & container)
{
	QRegExp r("(\\w+)\\s*<(.+)>\\s*$");
	GdbList list;
	QString pathScript = GdbSetting::instance()->getPathScript() + "/";
		
	// QList<_T>
	if(r.exactMatch(container))
	{
		QStringList l = r.capturedTexts();
		
		if(QFile::exists(pathScript  + l.at(1) + ".js")) 
			return tryTranslate(l.at(2));				// recusive
		return false;
	}

	// find is pointer ?
	container.contains("*") ? list.isPointer = true: list.isPointer = false;
	list.type = container;
	list.type.remove("*");
	list.type.remove("class ");
	
	if(QFile::exists(pathScript  + list.type + ".js")) 
		return true;
	return false;
}

// from gdbWatch

/*!
	\details Calling when other Addon want translate Qt class in other format.
	\param c is type of Qt var (QList<QString> for exemple).
	\param n is the name of var.
*/
void GdbScript::onRequestScriptTranslate(const QString & c , const QString & n)
{
	createScript(c); // type -> QList<QString> for exemple 
	exec(n); // n -> your varName
}

// 

/*!
	\details Create all script list for translate the current variable type.
	\param container is type of Qt var (QList<QString> for exemple). 
	\retval true if Qt container can be translated. 
*/
bool GdbScript::createScript(const QString & container)
{
	currentScriptIndex=0;	

	QRegExp r("(\\w+)\\s*<(.+)>\\s*$");
	GdbList list;

	// QList<_T>
	if(r.exactMatch(container))
	{
		QStringList l = r.capturedTexts();
		
		if(QFile::exists(mPathScript + l.at(1) + ".js")) 
		{
			list.type = l.at(1);		// QList
			list.cast = l.at(2);
			list.isPointer = false;		// default 
			list.scriptFile = mPathScript + l.at(1) + ".js"; // file name
			list.script = new QScriptEngine(this);		// script pointeur
			if(!loadScript(list.scriptFile, list.script))	// loading
				QMessageBox::warning(NULL,"Script Load error", "["+list.type+"]");
			list.function = "process";					// first function
			gdbScriptList << list;						// add to list
			return createScript(l.at(2));				// recusive
		}
		return false;
	}

	// find is pointer ?
	container.contains("*") ? list.isPointer = true: list.isPointer = false;
	list.type = container;
	list.type.remove("*");
	list.type.remove("class "); //some time gdb print "class QImage"
	
	if(QFile::exists(mPathScript + list.type + ".js")) 
	{
		list.scriptFile = mPathScript + list.type + ".js";
		list.cast = container;
		list.script = new QScriptEngine(this);
		if( ! loadScript(list.scriptFile, list.script))
			QMessageBox::warning(NULL,"Script Load error", "["+list.type+"]");
		list.function = "process";
		gdbScriptList << list;
		return true;
	}
	return false;
}

//

/*!
	\details Load script from .js and store this in GdbList struct
	\param fileName is the name of file
	\param e is the pointer to the QScriptEngine
*/
bool GdbScript::loadScript(const QString & fileName, QPointer<QScriptEngine> e)
{
	QFile file(fileName);
	if( file.open(QIODevice::ReadOnly))
	{
		e->evaluate(file.readAll());
		file.close();
		return true;
	}
	return false;
}

//

/*!
	\details  Extract value from script answer.
	\param d is the string contains data from script function
	\return status, next command and next function to call.
*/
void GdbScript::extractValue(const QString & d)
{
	QStringList l = d.split(":");
	if(l.count() == 3)
	{
		gdbScriptList[currentScriptIndex].status = l.at(0);
		gdbScriptList[currentScriptIndex].command = l.at(1);
		gdbScriptList[currentScriptIndex].function = l.at(2);
	}
}

//

/*!
	\details Execute script with other data, for example <b>varName</b>.
	\param data is the data has to send to script 
*/
void GdbScript::exec(const QString & data)
{
	mData = data;
	exec();
}

//

/*!
	\details Execute script with mData.
*/
void GdbScript::exec()
{
//	bool mIsPointer = false;
	if(	currentScriptIndex >= gdbScriptList.count()){ QMessageBox::warning(NULL,"critical erreur, Mks has crash after close this", "currentScript trop haut"); return ;}

 	QScriptValue function = gdbScriptList.at(currentScriptIndex).script->evaluate( gdbScriptList.at(currentScriptIndex).function );

//	QMessageBox::warning(NULL,"call ", "Script : "  + gdbScriptList.at(currentScriptIndex).type + "\nFunction : " + gdbScriptList.at(currentScriptIndex).function + "\nData : [" + mData + "]\nCast : " + gdbScriptList.at(currentScriptIndex).cast+ "\nisPointer : " + QString::number(gdbScriptList.at(currentScriptIndex).isPointer));
	QScriptValue res = function.call( function, QScriptValueList() << gdbScriptList.at(currentScriptIndex).script->newQObject(this) 
		<< QScriptValue(gdbScriptList.at(currentScriptIndex).script, mData)
		<< QScriptValue(gdbScriptList.at(currentScriptIndex).script, gdbScriptList.at(currentScriptIndex).cast)
		<< QScriptValue(gdbScriptList.at(currentScriptIndex).script, gdbScriptList.at(currentScriptIndex).isPointer));

	mData.clear();
//	QMessageBox::warning(NULL,"Raw result", res.toString());

	//	"WAIT:x /6s s.d.alloc:stringfunction";
	// extract value
	extractValue(res.toString());

	if(gdbScriptList.at(currentScriptIndex).status == "WAIT")
	{
//		QMessageBox::warning(NULL,"Script result WAIT", res.toString());
		GdbParser::instance()->setNextCommand("GdbScript", gdbScriptList.at(currentScriptIndex).command);
		GdbProcess::instance()->sendRawData(gdbScriptList.at(currentScriptIndex).command);
		setWaitEndProcess(true);
		return ;

	}

	if(gdbScriptList.at(currentScriptIndex).status == "FINISH")
	{
//		QMessageBox::warning(NULL,"Script result FINISH", gdbScriptList.at(currentScriptIndex).command);
		if(currentScriptIndex < gdbScriptList.count())
		{
			mData = "none";
			gdbScriptList[currentScriptIndex].function = "process";
			if(currentScriptIndex == 0)
			{
//				QMessageBox::warning(NULL,"Script TOTAL FINISH", gdbScriptList[currentScriptIndex].command);

				emit scriptFinishedTranslate(gdbScriptList[currentScriptIndex].command);

				while( gdbScriptList.count())
				{
					delete gdbScriptList.at(0).script;
					gdbScriptList.removeAt(0);
				}
				return;
			}
			else
			{
				gdbScriptList[currentScriptIndex-1].command = gdbScriptList.at(currentScriptIndex).command;
				currentScriptIndex--;
				exec(gdbScriptList.at(currentScriptIndex).command);
				return;
			}
		}
	}

	if(gdbScriptList.at(currentScriptIndex).status == "LEAVE")
	{
//		QMessageBox::warning(NULL,"Script result LEAVE", gdbScriptList.at(currentScriptIndex).command);
		mData = gdbScriptList.at(currentScriptIndex).command;
		currentScriptIndex++;
		exec();
		return;
	}

}

//

/*!
	\details New datas from Gdb is avaible.
	\param s is the data from GdbParser
*/
void GdbScript::data(int ,QString  s)
{
	setWaitEndProcess(false);
	mData += findValue(s, "answerGdb") + "\n";
}

