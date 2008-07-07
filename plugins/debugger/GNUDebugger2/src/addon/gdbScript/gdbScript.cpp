

#include "gdbScript.h"
#include <QFile>
#include <QMessageBox>

#include "../../kernel/gdbSetting.1.3.h"
/*

QList<QString>

  "p varname.d.end" , "p varname.d.array[0]", "p varname.d.array[0]" , ...., 


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

GdbScript::~GdbScript()
{
}

//

void GdbScript::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call(i, id, s);
}

// static function

bool GdbScript::canTranslate(const QString & container)
{
	QRegExp r("(\\w+)\\s*<(.+)>\\s*$");
	GdbList list;
	QString pathScript = GdbSetting::instance()->getPathScript() + "/";
		
	// QList<_T>
	if(r.exactMatch(container))
	{
		QStringList l = r.capturedTexts();
		
		if(QFile::exists(pathScript  + l.at(1) + ".js")) 
			return canTranslate(l.at(2));				// recusive
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

void GdbScript::onRequestScriptTranslate(const QString & c , const QString & n)
{
	createScript(c); // type -> QList<QString> for exemple 
	exec(n); // n -> your varName
}

// 
	
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

void GdbScript::exec(const QString & data)
{
//	QMessageBox::warning(NULL, "exec","execute script " + data);
	mData = data;
	exec();
}

//

void GdbScript::exec()
{
//	bool mIsPointer = false;
	if(	currentScriptIndex >= gdbScriptList.count()) QMessageBox::warning(NULL,"erreur", "currentScript trop haut");

 	QScriptValue function = gdbScriptList.at(currentScriptIndex).script->evaluate( gdbScriptList.at(currentScriptIndex).function );

//	QMessageBox::warning(NULL,"call ", "Script : "  + gdbScriptList.at(currentScriptIndex).type + "\nFunction : " + gdbScriptList.at(currentScriptIndex).function + "\nData : [" + mData + "]\nisPointer : " + QString::number(gdbScriptList.at(currentScriptIndex).isPointer));
	QScriptValue res = function.call( function, QScriptValueList() << gdbScriptList.at(currentScriptIndex).script->newQObject(this) 
		<< QScriptValue(gdbScriptList.at(currentScriptIndex).script, mData)
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

void GdbScript::data(int ,QString  s)
{
	setWaitEndProcess(false);
	mData += findValue(s, "answerGdb") + "\n";
}

