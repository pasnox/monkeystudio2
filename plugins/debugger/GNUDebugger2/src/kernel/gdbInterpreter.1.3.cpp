/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbInterpreter
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbInterpreter class

	Store all interpreter 


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#include <QMessageBox>
#include "gdbInterpreter.1.3.h"

// base class of interpreter 

BaseInterpreter::BaseInterpreter(QObject *parent): QObject(parent)
{
}

//

void BaseInterpreter::set(/*QString cName, QString cGdb,*/ QRegExp cRegExp, QRegExp aRegExp, QString aExtention)
{
//	mCmdName = cName;
//	mCmdGdb = cGdb;
	mCmdRegExp = cRegExp;
	mAnswerRegExp = aRegExp;
	mAnswerExtention = aExtention;
}

//

BaseInterpreter::~BaseInterpreter()
{
}

// class GdbInterpreter

GdbInterpreter::GdbInterpreter(QObject *parent) : QObject(parent)
{ 
	mParent = parent;
	GdbInterpreterList.clear(); 
}

//

QPointer<BaseInterpreter>  GdbInterpreter::add(/*const QString & cName, const QString & cGdb,*/ const QRegExp & cRegExp,
											   const QRegExp & aRegExp, const QString & aExtention)
{
	QPointer<BaseInterpreter> i = new BaseInterpreter(this);
	if(i)
	{
		i->set(/*cName, cGdb,*/cRegExp, aRegExp, aExtention);
		GdbInterpreterList << i;
	}
	return i;
}	

//

bool GdbInterpreter::remove(const QPointer<BaseInterpreter> & i)
{
	int ind = GdbInterpreterList.indexOf(i); 
	if( ind != -1)
	{
		delete GdbInterpreterList.at(ind);
		GdbInterpreterList.removeAt(ind);
		return true;
	}
	return false;
}

//

void GdbInterpreter::changeAnswer(const QPointer<BaseInterpreter> & i, const QString & s)
{
	foreach(QPointer<BaseInterpreter> b , GdbInterpreterList)
		if(b == i ) b->setAnswerExtention(s);
}

// find if an interpreter have a command (private function)

int GdbInterpreter::findCmd(const QString & cmd)
{
	for(int i=0; i< GdbInterpreterList.count(); i++)
		if(GdbInterpreterList.at(i)->getCmdRegExp().exactMatch(cmd)) return i;
	return -1; 
}

// find interpreter from current command

QPointer<BaseInterpreter> GdbInterpreter::find(const QString & currentCmd, const QString & lineValue)
{
	int t = findCmd(currentCmd);
	if(t != -1 && GdbInterpreterList.at(t)->getAnswerRegExp().exactMatch( lineValue ) )
	{
		return GdbInterpreterList.at(t);
	}
	return NULL;
}
		
//

GdbInterpreter::~GdbInterpreter()
{
	int c = GdbInterpreterList.count();
	for(int i=0; i<c; i++)
		if( GdbInterpreterList.at(i) ) delete GdbInterpreterList.at(i);
	GdbInterpreterList.clear();
}
