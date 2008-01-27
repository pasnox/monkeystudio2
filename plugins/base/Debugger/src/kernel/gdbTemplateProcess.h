#ifndef GDBBASECOMMAND_H
#define GDBBASECOMMAND_H


#include "./kernel/gdbInterpreter.h"


#include <QByteArray>
#include <QList>
#include <QString>

#include <QMessageBox>

template <class mT> class GdbTemplateProcess
{
public :

	int currentIndex ;

	mT *pClass;

	int (mT:: *gdbFunctionStart)(int , QByteArray)  ;
	QString gdbEventStart;

	struct
	{
		QList<int (mT:: *)(int , QByteArray) > gdbFunction;
		QList<QString> gdbEvent;
		QList<QGdbInterpreter*> gdbInterpreter;
	}generic;

	struct
	{
		QList<int (mT:: *)(int , QByteArray) > gdbFunction;
		QList<QByteArray> gdbEvent;
	}notify;



	bool isForMe(QByteArray data)
	{
		if(pClass->getParametre("interpreter=", data) == pClass->name() ||
			    pClass->getLastCmd().exactMatch(pClass->getParametre("currentCmd=", data) ))
			return true;
		return false; 
	}
	//
	bool isError(QByteArray data)
	{
		if(data.startsWith("^error")) return true;
			return false;
	}
	//
	bool isNotify(QByteArray data)
	{
		if(data.startsWith("^notify")) return true;
			return false;
	}
	//
	bool startProcess(QByteArray data)
	{
		QByteArray event = pClass->getParametre("event=", data);

		if(event == "end-stepping-range"   && !bBreakpointHit)
				return true;
		else bBreakpointHit = false;

		if(event == "breakpoint-hit") 
		{
			bBreakpointHit=true;
			return true;
		}

		return false;
	}
	//
	int lockProcess()
	{
		bSending =true;
		return pClass->PROCESS_SEND_DATA;
	}
	//
	int unlockProcess()
	{
		bSending = false;
		return pClass->PROCESS_TERMINED;
	}
	//
	int statusProcess()
	{
		if(bSending)
			return pClass->PROCESS_WAITING; 
		else
			return pClass->PROCESS_TERMINED;
	}
	//
	void forceProcess()
	{
		bForce = true;
		dispatchProcess(-1,"");
	}
	//
	void setClass(mT *p)
	{ 
		bSending = false;
		bBreakpointHit = false;
		currentIndex = 0;
		bResumeError = false;
		pClass = p;
		bForce = false;

		gdbEventStart = "";
		gdbFunctionStart = NULL;
	}
	//
	
	/* startin if */
/*	void setProcessStartOnEvent( QString event, int (mT:: *p)(int , QByteArray))
	{
		gdbFunctionStart = p;
		gdbEventStart = event;
	}
*/	//
	void connectEventStartToProcess( QString event, int (mT:: *p)(int , QByteArray))
	{
		gdbFunctionStart = p;
		gdbEventStart = event;
	}

/*
	void setProcessCommandOnEvent( QString event, int (mT:: *p)(int , QByteArray))
	{
		generic.gdbFunction << p;
		generic.gdbEvent << event ;
		generic.gdbInterpreter << NULL;
	}
*/

/*	void connectEventToProcess( QString event, int (mT:: *p)(int , QByteArray))
	{
		generic.gdbFunction << p;
		generic.gdbEvent << event ;
		generic.gdbInterpreter << NULL;
	}
*/
	
/*	void setProcessCommandOnEvent(QGdbInterpreter *interpreter,  int (mT:: *p)(int , QByteArray))
	{
		generic.gdbFunction << p;
		generic.gdbEvent << interpreter->cmdName;
		generic.gdbInterpreter << interpreter;
	}
*/
	void connectEventToProcess(QGdbInterpreter *interpreter,  int (mT:: *p)(int , QByteArray))
	{
		generic.gdbFunction << p;
		generic.gdbEvent << interpreter->cmdName;
		generic.gdbInterpreter << interpreter;
	}

	
	
/*	void removeProcessCommandOnEvent(QGdbInterpreter *p)
	{
		if(p)
		{
//			QMessageBox::warning(NULL, "remove interpreterr", p->cmdName, QMessageBox::Ok ); 

			int index = generic.gdbInterpreter.indexOf(p);
			if( index != -1)
			{
				generic.gdbInterpreter.removeAt(index);
				generic.gdbFunction.removeAt(index);
				generic.gdbEvent.removeAt(index);
			}
	//		else 	QMessageBox::warning(NULL, "error", "pointer P interpreter no found", QMessageBox::Ok ); 

		}
	//	else
//			QMessageBox::warning(NULL, "error", "remove interpreter NULL", QMessageBox::Ok ); 
	}
*/
	void disconnectEventToProcess(QGdbInterpreter *p)
	{
		if(p)
		{
			int index = generic.gdbInterpreter.indexOf(p);
			if( index != -1)
			{
				generic.gdbInterpreter.removeAt(index);
				generic.gdbFunction.removeAt(index);
				generic.gdbEvent.removeAt(index);
			}
		}
	}
	
/*	QGdbInterpreter * modifyProcessCommandOnEvent(QGdbInterpreter *pb, QGdbInterpreter *pa)
	{
		if(pb)
		{
			int index = generic.gdbInterpreter.indexOf(pb);
			if( index != -1)
			{ 
				generic.gdbInterpreter.replace(index,pa);
				pClass->removeInterpreter();
				pClass->addInterpreter(*generic.gdbInterpreter.at(index));
			}
			else 		
				QMessageBox::warning(NULL, "Critical Error", "modify interpreter no found", QMessageBox::Ok ); 
		}
		else QMessageBox::warning(NULL, "Critical Error", "modify interpreter is NULL", QMessageBox::Ok ); 
		return pa;
	}
*/

	QGdbInterpreter * modifyEventToProcess(QGdbInterpreter *pb, QGdbInterpreter *pa)
	{
		if(pb)
		{
			int index = generic.gdbInterpreter.indexOf(pb);
			if( index != -1)
			{ 
				generic.gdbInterpreter.replace(index,pa);
				pClass->removeInterpreter();
				pClass->addInterpreter(*generic.gdbInterpreter.at(index));
			}
			else 		
				QMessageBox::warning(NULL, "Critical Error", "modify interpreter no found", QMessageBox::Ok ); 
		}
		else QMessageBox::warning(NULL, "Critical Error", "modify interpreter is NULL", QMessageBox::Ok ); 
		return pa;
	}

/*	void leaveProcessCommandOnEvent(QGdbInterpreter **p)
	{
		delete *p;
		*p = NULL;
	}
*/

	void leaveEventToProcess(QGdbInterpreter **p)
	{
		delete *p;
		*p = NULL;
	}

	/* notifier */
	
/*	void setProcessNotifyOnEvent( QByteArray event, int (mT:: *p)(int , QByteArray))
	{
		notify.gdbFunction << p;
		notify.gdbEvent << event;
	}
*/	//
	void connectEventNotifyToProcess(QByteArray event, int (mT:: *p)(int , QByteArray))
	{
		notify.gdbFunction << p;
		notify.gdbEvent << event;
	}
	

	
	QString convertRegExp(QString data)
	{
		data.replace("(","\\("); // function self()
		data.replace(")","\\)");
		data.replace("*","\\*"); // pointer **argv
		data.replace("[", "\\["); // table tab[i]
		data.replace("]", "\\]");
		data.replace(".","\\."); // member of struct struct.member
		return data;
	}
	//
private :

	int commandProcess()
	{
		if(currentIndex < generic.gdbInterpreter.count())
		{
			if(generic.gdbInterpreter.at(currentIndex) != NULL)
			{
				qDebug("add interpreter loop " + pClass->name().toLocal8Bit());
				pClass->addInterpreter(*generic.gdbInterpreter.at(currentIndex));
				pClass->send(pClass, generic.gdbInterpreter.at(currentIndex)->cmdGdb.toLocal8Bit());
				currentIndex++;
				return lockProcess();
			}
		}
		return unlockProcess();
	}
	//
public :

	void setResumeError(bool b)
	{
		bResumeError = b;
	}
	//
	int dispatchProcess(int id, QByteArray data)
	{
		// start process if event is breakpoint-hit or end-stepping-range
		if(startProcess(data) || bForce )
		{
			currentIndex = 0;
			bForce = false;
			// verifit si il y a des commandes
			if(generic.gdbInterpreter.count())
			{
				// la command contient un Interpreter ? donc envoit des données
				return commandProcess();
 			}
		}
		
		if(gdbEventStart == pClass->getParametre("event=", data))
		{
			if(gdbFunctionStart !=NULL)
				return (pClass->*(gdbFunctionStart))(id, data);
			return unlockProcess();
		}

		// the msg is a notify from other plug
		if(isNotify(data))
		{
			int index = notify.gdbEvent.indexOf(pClass->getParametre("event=", data));
			if(index != -1)
				return (pClass->*(notify.gdbFunction.at(index)))(id, data);
			return unlockProcess();
		}

		// the msg start with interpreter="your class name" or currentCmd="your last command"

		if( isForMe(data))
		{
			unlockProcess();

			if( isError(data))
			{
				int status = pClass->processError(id, data);
				pClass->removeInterpreter();
				if(bResumeError)
					return commandProcess();
				return status;
			}
			else
			{
				int index = generic.gdbEvent.indexOf(pClass->getParametre("event=", data));
				if(index != -1)
				{
					if(pClass && generic.gdbFunction.at(index) != NULL)
					{
						int status = (pClass->*(generic.gdbFunction.at(index)))(id, data);
						if(status != pClass->PROCESS_TERMINED) return status; 
					}
					qDebug("remove interpreter " + pClass->name().toLocal8Bit());
					pClass->removeInterpreter();

					// passe a la commande suivante
					if(currentIndex < generic.gdbFunction.count())
						return commandProcess();
				}
				else if( pClass->getParametre("event=" , data) == "prompt")
				{
					qDebug("remove interpreter (prompt)" + pClass->name().toLocal8Bit());
					pClass->removeInterpreter();
					pClass->processExit();
					return unlockProcess();
				}
			}
		}
		return statusProcess();
	}

private :
	bool bSending;
 	bool bBreakpointHit;
	bool bResumeError;
	bool bForce;
};
#endif
