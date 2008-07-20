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
	\file gdbInterpreter.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class store interpreters
*/

#ifndef GDBINTERPRETER_H
#define GDBINTERPRETER_H

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QPointer>


/*!
	\brief This class store only one interpreter
	\details This class store all informations for only one interpreter.
	\sa GdbInterpreter
*/
class BaseInterpreter : public QObject
{

public :

	BaseInterpreter(QObject *parent = 0);
	~BaseInterpreter();

	void set(QString cName, QRegExp cRegExp,  QRegExp aRegExp, QString aExtention);

	/*!
	 * \details Return the class name 
	*/
	QString getClassName() {return mClassName;} 

	/*!
	 * \details Return the current command in QRegExp format 
	*/
	QRegExp getCmdRegExp() { return mCmdRegExp;}

	/*!
	 * \details Return the correct answer in RegExp format 
	*/
	QRegExp getAnswerRegExp() { return mAnswerRegExp;}

	/*!
	 * \details Return the new answer from Gdb (change the answer from Gdb) 
	*/
	QString getAnswerExtention() { return mAnswerExtention;}

	/*!
	 * \details Return an unic Id for this interpreter
	*/
	quintptr getId(){ return (quintptr) this;}
	
	/*!
	 * \details Set name of class
	*/
	void setClassName(QString s) { mClassName = s;}

	/*!
	 * \details Set the answer extention
	*/
	void setAnswerExtention(QString s) { mAnswerExtention = s ;}
	
	/*!
	 * \details Set the answer
	*/
	void setAnswerRegExp(QRegExp s) { mAnswerRegExp = s ;}
	
	/*!
	 * \details Set the command
	*/
	void setCmdRegExp(QRegExp s) { mCmdRegExp = s ;}
		
private :

	QString mClassName;		
	QRegExp mCmdRegExp;		
	QRegExp mAnswerRegExp;		
	QString mAnswerExtention;	
};

/*!
	\brief This class store all interpreters from all AddOn.
	\details This is the list of all interpreters.
	An interpreter is small parser for a one class, one command and one an answer.
	When data arrives of gdb, GdbParser seeks if the className in court (set by setNextCommand() function), 
	the command and the reponce correspond has one interpreter.
	In this case, the signal GdbParser::onInterpreter() is emitted.

	By example: if you want print the value of a variable X, in a consol Gdb, you made:

	\code
	p X
	\endcode

	Gdb answers by:

	\code
	$2 = 23
	(gdb)
	\endcode
	
	Has this point you want just to extract the value and to treat it by a specific function.

	For parse this sequence you will create an interpreter, which has like name that the name of the class (name() function) 
	and which parse only command "p ..." and the reponce "$2 = ...".

	\code
	BaseInterpreter *pInter = GdbInterpreter::add( name(),		// Name of AddOn sush as "GdbWatch"
		QRegExp("^p .*"),		// Command 
		QRegExp("^\\$\\d+\\s=\\s.*"),	// Answer
		"");				// answerExtention
	\endcode

	Now when your AddOn sending has Gdb the command "p varName" by GdbCore::Process()->sendRawData("p varName"), an interpreter is found by GdbParser 
	because "p varName" == "^p .*" and "$2 = 23" == "^\$\d+\s=\s.*". In this case the signal GdbParser::onInterpreter() is emitted and call interpreter() function in
	your AddOn.

	\note AddOn can have more interpreters for parser more sequences, but only one function is called.
	You can call other functions if you use GdbConnectTemplate class.

	\sa BaseInterpreter ,  GdbConnectTemplate
*/
class  GdbInterpreter : public QObject
{
	Q_OBJECT

public :
	
	GdbInterpreter(QObject *parent = 0);
	~GdbInterpreter();

	QPointer<BaseInterpreter> add(const QString &, const QRegExp &,  const QRegExp &, const QString & );
	bool remove(const QPointer<BaseInterpreter> &);
	void changeAnswer(const QPointer<BaseInterpreter> &, const QString &);
	QPointer<BaseInterpreter> find(const QString & , const QString & , const QString & );

private :

	/*!
	 * \details List off all interpreters
	*/
	QList< QPointer<BaseInterpreter> > GdbInterpreterList;

};
#endif

