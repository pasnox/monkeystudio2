

#include "gdbScript.h"
#include <QFile>
#include <QMessageBox>


/*

QList<QString>

  "p varname.d.end" , "p varname.d.array[0]", "p varname.d.array[0]" , ...., 


*/

QScriptValue sendToGdb(QScriptContext *context, QScriptEngine *engine);
QScriptValue repeatSequenceAt(QScriptContext *context, QScriptEngine *engine);


GdbScript::GdbScript(QObject * parent) : QObject(parent)
{
	// register sendToGdb for can use in .js
//	engine.globalObject().setProperty("sendToGdb", engine.newFunction(sendToGdb));
//	engine.globalObject().setProperty("repeatSequenceAt", engine.newFunction(repeatSequenceAt));
	currentScriptIndex = 0;
	gdbScriptList.clear();
}

//

GdbScript::~GdbScript()
{
}

//

bool GdbScript::createScript(const QString & container)
{
//	if(gdbScriptList.count()) return true;

	currentScriptIndex = 0;
	

//	QMessageBox::warning(NULL,"CreateEditor", "I have " + QString::number(gdbScriptList.count()) + " scripts");

	QRegExp r("(\\w+)\\s*<(.+)>\\s*$");
	GdbList list;

	if(r.exactMatch(container))
	{
		QStringList l = r.capturedTexts();
//		if(l.count() == 4 && l.at(3).contains("*"))
//			QMessageBox::warning(NULL,"Script found container pointer", container);
	
			

		
		if(QFile::exists("c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + l.at(1) + ".js")) 
		{
			list.type = l.at(1);
			list.isPointer = false;
//		QMessageBox::warning(NULL,"Script found is container", "["+list.type+"]");
			list.scriptFile = "c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + l.at(1) + ".js";
			list.script = new QScriptEngine(this);
			loadScript(list.scriptFile, list.script);
			list.function = "process";
			gdbScriptList << list;
//			currentScriptIndex = 0;
			return createScript(l.at(2));
		}
		return false;
	}

	container.contains("*") ? list.isPointer = true: list.isPointer = false;
	list.type = container;
	list.type.remove("*");
	
	if(QFile::exists("c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + list.type + ".js")) 
	{
		QMessageBox::warning(NULL,"Script found is not container", "["+list.type+"]" + QString::number(list.isPointer));

		list.scriptFile = "c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + list.type + ".js";
		list.script = new QScriptEngine(this);
		if( ! loadScript(list.scriptFile, list.script)) 	QMessageBox::warning(NULL,"Script Load error", "["+list.type+"]");
	
		list.function = "process";
//		currentScriptIndex = 0;
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

void GdbScript::extractValue(QString d)
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

void GdbScript::exec(QString varName)
{
	mData = varName;
	exec();
}

//

void GdbScript::exec()
{
//	bool mIsPointer = false;
	
 	QScriptValue function = gdbScriptList.at(currentScriptIndex).script->evaluate( gdbScriptList.at(currentScriptIndex).function );

	QMessageBox::warning(NULL,"call ", "Script : "  + gdbScriptList.at(currentScriptIndex).type + "\nFunction : " + gdbScriptList.at(currentScriptIndex).function + "\nData : [" + mData + "]\nisPointer : " + QString::number(gdbScriptList.at(currentScriptIndex).isPointer));
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
				QMessageBox::warning(NULL,"Script TOTAL FINISH", gdbScriptList[currentScriptIndex].command);
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

void GdbScript::data(QString s)
{
	mData += s + "\n" ;
}
















bool GdbScript::findNatif(const QString & t)
{
	QString container;

	// QList<QString>
	QRegExp r("(\\w+)<(.+)>\\s*$");
	if(r.exactMatch(t))
	{
		QStringList l = r.capturedTexts();
		
//		QMessageBox::warning(NULL,"container is not Natif, find .... .js", t + " ..  " + l.at(1) );

		if(QFile::exists("c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + l.at(1) + ".js")) 
		{
//			QMessageBox::warning(NULL,".js not natif found container", l.at(1) + "  ... get child :" + l.at(2));
//			loadScript("c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + l.at(1) + ".js");
//			processSend("r");
//			findNatif(l.at(2));
		}
//		else 			QMessageBox::warning(NULL,".js NO found container !! ", l.at(1) );

	}
	else
	{
//		QMessageBox::warning(NULL,"container is natif, find ... .js", t);
		if(QFile::exists("c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + t + ".js")) 
		{
//			QMessageBox::warning(NULL,".js Natif container found", t);
			return true;
		}
//		else 
//			QMessageBox::warning(NULL,".js Natif container NO found", t);
	}
	return false;
}


void GdbScript::exec(QString var , QString s)
{
	mVarName = var;
	QRegExp r("(\\w+)<(.+)>\\s*$");
	if(r.exactMatch(s))
	{
	}
	else
	{
		mSequence = 0;
		QStringList l = r.capturedTexts();
		loadScript("c:/Dev/monkey/branches/dev/bin/plugins/GNUdbg2/scripts/" + s + ".js");
		// exectute the fisrt command from script
		processSend(mVarName);
	}
}



bool GdbScript::loadScript(QString fileName)
{
	QFile file(fileName);
//	QMessageBox::warning(NULL,"try open script", fileName);
	if( file.open(QIODevice::ReadOnly))
	{
		engine.evaluate(file.readAll());
		file.close();
		mSequence = 0;
		return true;
	}
	return false;
}


QString GdbScript::processSend( QString varName)
{
bool mIsPointer = false;
	
 	QScriptValue cScript = engine.evaluate("send");

		
	QScriptValue res;
	res = cScript.call(cScript, QScriptValueList() << engine.newQObject(this) <<QScriptValue(&engine, varName) << QScriptValue(&engine, mIsPointer)<< QScriptValue(&engine, mSequence));

	return res.toString();
}
//
QString GdbScript::processReceive()//QString data)
{
	     	QScriptValue cScript = engine.evaluate("receive");
bool mIsPointer = false;
//	QMessageBox::warning(NULL,"try exe script", mData);
		
		QScriptValue res;
		res = cScript.call(cScript, QScriptValueList() << engine.newQObject(this) <<QScriptValue(&engine, mData) << QScriptValue(&engine,mIsPointer)<< QScriptValue(&engine, mSequence));
		if(res.toString() == "WAIT_NEXT_COMMAND")
		{
			mSequence++;
			processSend(mVarName);
		}	
		else 	QMessageBox::warning(NULL,"Script return ", res.toString());
			mData.clear();


		return res.toString();
}

void GdbScript::call(QString s)
{
	GdbParser::instance()->setNextCommand("GdbScript", s);
	GdbProcess::instance()->sendRawData(s);
}

QScriptValue sendToGdb(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue a = context->argument(1);
	QScriptValue b =  context->argument(0);

	GdbScript * o =(GdbScript*) b.toQObject();
//	QMessageBox::warning(NULL,"Script want send", a.toString());
	o->call(a.toString());

	
	return a;
}

QScriptValue repeatSequenceAt(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue a = context->argument(1);
	QScriptValue b =  context->argument(0);

	GdbScript * o =(GdbScript*) b.toQObject();
	o->mSequence = a.toInt32() -1 ;

	return a;
}
