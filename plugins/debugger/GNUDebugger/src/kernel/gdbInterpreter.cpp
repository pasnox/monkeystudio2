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
	\file gdbInterpreter.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class store interpreters
*/

/*

	GdbInterpreter class

	Store all interpreter 


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#include <QMessageBox>
#include "gdbInterpreter.h"

// base class of interpreter 
/*!
	\details Create new object
	\param parent of this object
*/
BaseInterpreter::BaseInterpreter(QObject *parent): QObject(parent)
{
}

//
/*!
	\details Set an interpreter 
	\param cName is the ClassName sush as "GdbWatch"
	\param cRegExp is the command in QRegExp sush as "^p *." 
	\param aRegExp is the answer in QRegExp sush as "^\\$\\d+\\s=\\s.*"
	\param aExtention is the answer extention in QRegExp
	\note answerExtention is used to modify the reponce of Gdb, for example when Gdb removes a breakpoint, 
	it answers just by the prompt one and we do not have any information on what it passed
*/
void BaseInterpreter::set(QString cName,QRegExp cRegExp, QRegExp aRegExp, QString aExtention)
{
	mClassName = cName;
	mCmdRegExp = cRegExp;
	mAnswerRegExp = aRegExp;
	mAnswerExtention = aExtention;
}

//
/*!
	\details None.
*/
BaseInterpreter::~BaseInterpreter()
{
}

// class GdbInterpreter
/*!
	\details Create new object
	\param parent of this object
*/
GdbInterpreter::GdbInterpreter(QObject *parent) : QObject(parent)
{ 
	GdbInterpreterList.clear(); 
}

//
/*!
	\details Add new interpreter in list.
	\param cName is the ClassName sush as "GdbWatch" returned by name() function of GdbCore class
	\param cRegExp is the command in QRegExp sush as "^p *." 
	\param aRegExp is the answer in QRegExp sush as "^\\$\\d+\\s=\\s.*"
	\param aExtention is the answer extention in QRegExp format
	\retval Pointer to new BaseInterpreter class or NULL if an error occure
*/
QPointer<BaseInterpreter>  GdbInterpreter::add(const QString & cName, const QRegExp & cRegExp,
												const QRegExp & aRegExp, const QString & aExtention)
{
	QPointer<BaseInterpreter> i = new BaseInterpreter(this);
	if(i)
	{
		i->set(cName, cRegExp, aRegExp, aExtention);
		GdbInterpreterList << i;
		return i;
	}
	return NULL;
}	

//
/*!
	\details Remove an interpreter in member list
	\param pInter is the pointer to the BaseInterpreter class.
	\retval true if remove successfull, else false.
*/
bool GdbInterpreter::remove(const QPointer<BaseInterpreter> & pInter)
{
	int ind = GdbInterpreterList.indexOf(pInter); 
	if( ind != -1)
	{
		delete GdbInterpreterList.at(ind);
		GdbInterpreterList.removeAt(ind);
		return true;
	}
	return false;
}

//
/*!
	\details Change answer extention
	\param pInter is the pointer to the BaseInterpreter class that you want change.
	\param string is the new answer
*/
void GdbInterpreter::changeAnswer(const QPointer<BaseInterpreter> & pInter, const QString & string)
{
	foreach(QPointer<BaseInterpreter> b , GdbInterpreterList)
		if(b == pInter ) b->setAnswerExtention(string);
}


// 
/*!
	\details Find if an interpreter can be found for the current class ,command and answer from Gdb.
	\param currentClassName is the current class name.
	\param currentCmd is the current command
	\param lineValue is the line for parse
	\retval A pointer to this interpreter or NULL if interpreter not found.
*/
QPointer<BaseInterpreter> GdbInterpreter::find(const QString & currentClassName , const QString & currentCmd, const QString & lineValue)
{
	// V 1.3.2 fix for searsh all interpreter , not just one
	for(int i = 0; i< GdbInterpreterList.count(); i++)
	{
		QPointer<BaseInterpreter> b = GdbInterpreterList.at(i);
		if(b->getClassName() == currentClassName && b->getCmdRegExp().exactMatch(currentCmd) && b->getAnswerRegExp().exactMatch( lineValue )  )
			return b;
	}
	return NULL;
}
//
/*!
	\details None.
*/
GdbInterpreter::~GdbInterpreter()
{
}
