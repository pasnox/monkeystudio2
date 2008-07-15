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

	When GdbParser found an interpreter (className, command and answer are correct) it emit interpreter signal.
	interpreter function in AddOn is call and this template find if the current interpreter is in this class. In this case the
	correct function is call.
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
	\details Add new function to the interpreter
	\code 
	GdbConnectTemplate::add(this, interpreterPointer, &ClassName::functionName)
	\endcode
	\param p is the pointer to the class (generaly is "this")
	\param i is a pointer to BaseInterpreter
	\param f is the pointer to your function that you want call
*/
template <class _T>
void GdbConnectTemplate<_T>::add( _T *p, QPointer<BaseInterpreter> i , void(_T:: *f)(int , QString))
{
	Invoker in={p, i, f};
	invokerList << in;
}

/*!
	\details Remove connexion in list
	\param i is the pointer to the interpreter
*/
template <class _T>
void GdbConnectTemplate<_T>::remove(const QPointer<BaseInterpreter> & i)
{
	int j = findInvoker(i);
	if(j != -1) invokerList.removeAt(i);
}

/*!
	\details If an interpreter is found in this list, the function associate is called.
	\param i is the pointer to the interpreter
	\param id is an Id from GdbParser class
	\param st is the string from GdbParser class
*/
template <class _T>
void GdbConnectTemplate<_T>::call(const QPointer<BaseInterpreter> & i, const int & id, const QString & st)
{ 
	int j = findInvoker(i);
	if( j != -1 )
	 ((invokerList.at(j).mParent)->*(invokerList.at(j).mPf))( id, st );
}
 

/*!
	\details Find interpreter 
	\param i is the pointer to the current interpreter that you want find
	\retval index of interpreter list or -1 if not found
*/
template <class _T>
int GdbConnectTemplate<_T>::findInvoker(const QPointer<BaseInterpreter> & i)
{
	for(int j=0; j<invokerList.count(); j++)
		if(invokerList.at(j).mInterpreter == i) return j;
	return -1;
}
#endif
