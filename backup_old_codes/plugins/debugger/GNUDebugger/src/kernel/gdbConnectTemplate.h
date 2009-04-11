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
	\file gdbConnectTemplate.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class is a template for connect a signal to the function
*/
/*

	GdbInterpreter class

	Store all connexion for class _T from GdbInterpreter


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#ifndef GDBCONNECTEMPLATE_H
#define GDBCONNECTEMPLATE_H

#include "gdbInterpreter.h"
 
/*!
	\brief This class is a template for connect a signal to the function
	\details This connect is a same as QObject::connect but any signal is passed to the function.

	When GdbParser found an interpreter (className, command and answer are correct) it emit onInterpreter signal.
	interpreter() function in AddOn is call and this template find if the current interpreter is in this class. In this case the
	correct function is call.

	\code
	myAddon::myAddon(QObject *parent) : GdbCore(parent)
	{
		...
		// create interpreter by use GdbCore::Parser()->addInterpreter()
		...
		// connect this interpreter to the onValue() function
		Connect.add(this, pInterpreter, &myAddon::onValue);
	}
	\endcode

	When an interpreter is found, only this function is call, but other function can be called after.
	Connect.call() switches directly to the correct function declared in Connect.add()
	
	\code
	void myAddon::interpreter(const QPointer<BaseInterpreter> & pInter, const int & id, const QString & st)
	{
		Connect.call(pInter, id, st); // switches to the correct function
	}

	// call by Connect.call() function
	void myAddon::onValue(int id, QString st)
	{
		...
	}
	\endcode
	\sa GdbInterpreter, GdbParser::addInterpreter(), GdbCore::Parser()
*/
template <class _T>
class GdbConnectTemplate
{

	/*! 
		\brief Struct invoker, store parent class, interpreter pointer and address function
	*/
	typedef struct Invoker
	{
		/*! 
			\details Pointer to the class (generaly this)
		*/
		_T *mParent;		// class _T pointer
		/*! 
			\details Pointer to the interpreter
		*/
		QPointer<BaseInterpreter> mInterpreter; // just pointer
		/*! 
			\details Prototype of function is "void function(int, QString)" 
		*/
		void (_T:: *mPf)(int , QString ); // for call _T::Function(int, QString)
	};

public :

	void add( _T *p, QPointer<BaseInterpreter> i , void(_T:: *f)(int , QString));
	void remove(const QPointer<BaseInterpreter> & i);
	void call(const QPointer<BaseInterpreter> & i, const int & id, const QString & st);

private :
	
	int findInvoker(const QPointer<BaseInterpreter> & i);
	
	/*! 
		\details List of all connexion for this template class
	*/
	QList<Invoker> invokerList;

};

/*!
	\details Add new function
	\code 
	GdbConnectTemplate::add(this, interpreterPointer, &ClassName::functionName)
	\endcode
	\param pClass is the pointer to the class (generaly is "this")
	\param pInter is a pointer to BaseInterpreter
	\param pFunct is the pointer to your function that you want call
*/
template <class _T>
void GdbConnectTemplate<_T>::add( _T *pClass, QPointer<BaseInterpreter> pInter , void(_T:: *pFunct)(int , QString))
{
	Invoker in={pClass, pInter, pFunct};
	invokerList << in;
}

/*!
	\details Remove connexion in list
	\param pInter is the pointer to the interpreter BaseInterpreter
*/
template <class _T>
void GdbConnectTemplate<_T>::remove(const QPointer<BaseInterpreter> & pInter)
{
	int j = findInvoker(pInter);
	if(j != -1) invokerList.removeAt(j);
}

/*!
	\details If an interpreter is found in this list, the function associate is called.
	\param pInter is the pointer to the interpreter BaseInterpreter
	\param stId is an Id from GdbParser class
	\param string is the string from GdbParser class
*/
template <class _T>
void GdbConnectTemplate<_T>::call(const QPointer<BaseInterpreter> & pInter, const int & stId, const QString & string)
{ 
	int j = findInvoker(pInter);
	if( j != -1 )
	 ((invokerList.at(j).mParent)->*(invokerList.at(j).mPf))( stId, string );
}
 

/*!
	\details Find interpreter 
	\param pInter is the pointer to the current interpreter that you want find
	\retval index of interpreter list or -1 if not found
*/
template <class _T>
int GdbConnectTemplate<_T>::findInvoker(const QPointer<BaseInterpreter> & pInter)
{
	for(int j=0; j<invokerList.count(); j++)
		if(invokerList.at(j).mInterpreter == pInter) return j;
	return -1;
}
#endif
