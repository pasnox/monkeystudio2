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
 

template <class _T>
class GdbConnectTemplate
{

	// struct
	struct invok
	{
		_T *mParent;		// class _T pointer
		QPointer<BaseInterpreter> mInterpreter; // just pointer
		void (_T:: *mPf)(int , QString ); // for call _T::Function(int, QString)
	};

	typedef invok Invoker;

public :

	// Add connexion
	void add( _T *p, QPointer<BaseInterpreter> i , void(_T:: *f)(int , QString))
	{
		Invoker in={p, i, f};
		invokerList << in;
	}

	// remove connexion
	void remove(const QPointer<BaseInterpreter> & i)
	{
		int j = findInvoker(i);
		if(j != -1) invokerList.removeAt(i);
	}

	// invok _T::Function
	void call(const QPointer<BaseInterpreter> & i, const int & id, const QString & st)
	{ 
		int j = findInvoker(i);
		if( j != -1 )
		 ((invokerList.at(j).mParent)->*(invokerList.at(j).mPf))( id, st );
	}
	 
private :
	
	// find Interpreter id (Pointer)
	int findInvoker(const QPointer<BaseInterpreter> & i)
	{
		for(int j=0; j<invokerList.count(); j++)
			if(invokerList.at(j).mInterpreter == i) return j;
		return -1;
	}

	// list of all connexion for template class
	QList<Invoker> invokerList;

};

#endif
