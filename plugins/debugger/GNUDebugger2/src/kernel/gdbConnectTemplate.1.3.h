/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbConnectTemplate
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbInterpreter class

	Store all connexion for class _T from GdbInterpreter


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#ifndef GDBCONNECTEMPLATE_H
#define GDBCONNECTEMPLATE_H

#include "gdbInterpreter.1.3.h"
 
//! This class is a template for connect a signal to the function
/**
\author xiantia
\version 1.3.2

This connect is a same as QObject::connect but any signal is passed to the function.

When GdbParser found an interpreter (className, command and answer are correct) it emit interpreter signal.
interpreter function in AddOn is call and this template find if the current interpreter is in this class. In this case the
correct function is call.
*/

template <class _T>
class GdbConnectTemplate
{

	//! Struct invok, store parent class, interpreter pointer and address function
	struct invok
	{
		//! Pointer to the class (generaly this)
		_T *mParent;		// class _T pointer
		//! Pointer to the interpreter
		QPointer<BaseInterpreter> mInterpreter; // just pointer
		//! Proptype of function (void function(int, QString) 
		void (_T:: *mPf)(int , QString ); // for call _T::Function(int, QString)
	};


	typedef invok Invoker;

public :

	//! Add connexion
	void add( _T *p, QPointer<BaseInterpreter> i , void(_T:: *f)(int , QString));
	//! Remove connexion
	void remove(const QPointer<BaseInterpreter> & i);
	//! call function
	void call(const QPointer<BaseInterpreter> & i, const int & id, const QString & st);

private :
	
	//! Find Interpreter id (Pointer)
	int findInvoker(const QPointer<BaseInterpreter> & i);
	//! List of all connexion for this template class
	QList<Invoker> invokerList;

};


/**
* Add new function to the interpreter
* \code 
* GdbConnectTemplate::add(this, interpreterPointer, &ClassName::functionName)
* \endcode
*
* Where <b>this</b> is the current class, <b>interpreterPointer</b> is a pointer returned by GdbParser::addInterpreter(), <b>className</b> is the name of your class and
* <b>functionName</b> is the name of function that you want call.
*/
template <class _T>
void GdbConnectTemplate<_T>::add( _T *p, QPointer<BaseInterpreter> i , void(_T:: *f)(int , QString))
{
	Invoker in={p, i, f};
	invokerList << in;
}

// remove connexion
template <class _T>
void GdbConnectTemplate<_T>::remove(const QPointer<BaseInterpreter> & i)
{
	int j = findInvoker(i);
	if(j != -1) invokerList.removeAt(i);
}

/**
If an interpreter is found in this list, the function associate is called
*/
template <class _T>
void GdbConnectTemplate<_T>::call(const QPointer<BaseInterpreter> & i, const int & id, const QString & st)
{ 
	int j = findInvoker(i);
	if( j != -1 )
	 ((invokerList.at(j).mParent)->*(invokerList.at(j).mPf))( id, st );
}
 

/**
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
