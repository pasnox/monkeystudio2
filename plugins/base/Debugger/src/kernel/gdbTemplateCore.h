#ifndef GDBTEMPLATECORE_H
#define GDBTEMPLATECORE_H

#include "./kernel/gdbInterpreter.h"
#include "./kernel/gdbCore.h"


#include <QByteArray>
#include <QList>
#include <QString>

#include <QMessageBox>

template <class mT> class GdbTemplateCore
{
private :

	int currentIndex ;
	mT *pClass;
	QFile file;

	/* this structure contains list of event associat to function */ 
	struct
	{
		QList<int (mT:: *)(QGdbMessageCore)> gdbFunctionStart  ;
		QList<QString> gdbEventStart;
	}start;

	/* structure interpreter */
	struct
	{
		QList<int (mT:: *)(QGdbMessageCore) > gdbFunction;
		QList<QString> gdbEvent;
		QList<QGdbInterpreter*> gdbInterpreter;
	}generic;

	/* structure notify */
	struct
	{
		QList<int (mT:: *)(QGdbMessageCore) > gdbFunction;
		QList<QByteArray> gdbEvent;
	}notify;

	/* check if a "interpreter=" is egal this->name() or currentCmd == my command */	
	bool isForMe(QByteArray data)
	{
		if(pClass->getParametre("interpreter=", data) == pClass->name() ||
			    pClass->getLastCmd().exactMatch(pClass->getParametre("currentCmd=", data) ))
			return true;
		return false; 
	}
	/* check if event is a error */
	bool isError(QByteArray data)
	{
		if(data.startsWith("^error")) return true;
			return false;
	}
	/* check if event is notify */
	bool isNotify(QByteArray data)
	{
		if(data.startsWith("^notify")) return true;
			return false;
	}

public :	

	/* start process if ... */
	int startProcess(QByteArray data)
	{
		QByteArray event = pClass->getParametre("event=", data);
	
		if(start.gdbEventStart.contains(event)  && event == "breakpoint-hit") 
		{
			bBreakpointHit=true;
			return start.gdbEventStart.indexOf(event);
		}

		// bBreakpointHit = true -> inibition  event end-stepping-range because breapointHi occur befor
		if(start.gdbEventStart.contains(event) && event == "end-stepping-range"   && !bBreakpointHit)
			return  start.gdbEventStart.indexOf(event);
		else
		{
			 bBreakpointHit = false;
			 if(start.gdbEventStart.contains(event) &&  event != "end-stepping-range")
				return  start.gdbEventStart.indexOf(event);
		}
		return -1;
	}
	//
	int lockProcess()
	{
		bSending =true;
		return PROCESS_SEND_DATA;
	}
	//
	int unlockProcess()
	{
		bSending = false;
		return PROCESS_TERMINED;
	}
	//
	int getStatusProcess()
	{
		if(bSending)
			return PROCESS_WAITING; 
		else
			return PROCESS_TERMINED;
	}
	//
	void setStatusProcess(int p)
	{
		switch(p)
		{
			case PROCESS_WAITING : 
			case PROCESS_SEND_DATA : bSending = true; break;
			case PROCESS_TERMINED : bSending = false; break;
		}
	}
	void forceProcess()
	{
		bForce = true;
		QGdbMessageCore m ;
		dispatchProcess(m);
	}
	//
	void setClass(mT *p)
	{ 
		bSending = false;
		bBreakpointHit = false;
		currentIndex = 0;
		bResumeError = false;
		bClearMessages = false;
		pClass = p;
		bForce = false;

		file.setFileName("./Thread_Template_" + p->name() + ".txt");
		file.open(QIODevice::WriteOnly);
	}
	//

	void qDebug(QByteArray s)
	{
		file.write(s + "\r\n");
		file.flush();
	}	
	//
	void connectEventStart(QString event, int (mT:: *p)(QGdbMessageCore))
	{
		start.gdbFunctionStart << p;
		start.gdbEventStart << event;
	}
	//
	void connectEventInterpreter(QGdbInterpreter *interpreter,  int (mT:: *p)(QGdbMessageCore))
	{
		//qDebug("try connection : " + pClass->name().toLocal8Bit());
		generic.gdbFunction << p;
		generic.gdbEvent << interpreter->cmdName();
		generic.gdbInterpreter << interpreter;
	}
	//
	void connectEventNotify(QByteArray event, int (mT:: *p)(QGdbMessageCore))
	{
		notify.gdbFunction << p;
		notify.gdbEvent << event;
	}
	//
	void disconnectEventInterpreter(QGdbInterpreter *p)
	{
	//	qDebug("try disconnection : " );//+ pClass->name().toLocal8Bit());
		if(p)
		{
			int index = generic.gdbInterpreter.indexOf(p);
			if( index != -1)
			{
				generic.gdbInterpreter.removeAt(index);
				generic.gdbFunction.removeAt(index);
				generic.gdbEvent.removeAt(index);
				//	qDebug("ok ");
			}
			else 	//qDebug("no found : " + pClass->name().toLocal8Bit());
				QMessageBox::warning(NULL, "disconnectEventToProcess", "Apptente to disconnect a unkow interpreter", QMessageBox::Ok ); 
		}
	}
	//
	QGdbInterpreter * modifyEventInterpreter(QGdbInterpreter *pb, QGdbInterpreter *pa)
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
				QMessageBox::warning(NULL, "modifyEventToProcess", "Apptente to modify a unknow interpreter", QMessageBox::Ok ); 
		}
		else QMessageBox::warning(NULL, "modifyEventToProcess", "Apptente to modify a NULL interpreter", QMessageBox::Ok ); 
		return pa;
	}
	//
	void leaveEventInterpreter(QGdbInterpreter **p)
	{
		//qDebug("leaving");

		if((*p != NULL))
		{
			delete *p;
			*p = NULL;
		}
	}
	//
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
			//qDebug("executing commandProcess ");
			if(generic.gdbInterpreter.at(currentIndex) != NULL)
			{
				qDebug("add interpreter loop " + pClass->name().toLocal8Bit() + "  " + generic.gdbInterpreter.at(currentIndex)->cmdName().toLocal8Bit());
				pClass->addInterpreter(*generic.gdbInterpreter.at(currentIndex));
				pClass->processSendRawData(pClass, generic.gdbInterpreter.at(currentIndex)->cmdGdb().toLocal8Bit());
				currentIndex++;
				//qDebug("Ok");
				return lockProcess();
			}
			else QMessageBox::warning(NULL, "commandProcess", "Apptente to execute a interpreter but it is a NULL pointer", QMessageBox::Ok ); 
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
	void clearLastMessages()
	{
		bClearMessages = true;
	 }
	//
	int dispatchProcess(QGdbMessageCore m)
	{
		// forcing processs
		if( bForce )
		{
			qDebug("force");
			qDebug(m.msg);
			bForce = false;
			currentIndex = 0;

			return commandProcess();
			// verifit si il y a des commandes
//			if(generic.gdbInterpreter.count())
				// la command contient un Interpreter ? donc envoit des données
//				return commandProcess();
		}


		// verifit si la class est abonnée a cette evenement (start)
		int index = startProcess(m.msg);
		if(index != -1 )
		{
				qDebug("function abonnement (start)");
				qDebug(m.msg);
				// nous somme abonnées et on veut passer par une fonction
				if( start.gdbFunctionStart.at(index) !=NULL) 
				{
						setStatusProcess( (pClass->*(start.gdbFunctionStart.at(index)))  (m));
						return getStatusProcess();
				}
				else 
				{
					//qDebug("pas de fonction associ");
					// je suis abonner et je n'est pas de fonction
					// donc commence par les interpreter
					currentIndex = 0;
//					return commandProcess();
//					bForce = false;
					// verifit si il y a des commandes
//					if(generic.gdbInterpreter.count())
					// la command contient un Interpreter ? donc envoit des données
					return commandProcess();
			}
		}

		// the msg is a notify from other plug
		if(isNotify(m.msg))
		{
			qDebug("notify");
			qDebug(m.msg);
			int index = notify.gdbEvent.indexOf(pClass->getParametre("event=", m.msg));
			if(index != -1)
				(pClass->*(notify.gdbFunction.at(index)))(m);
//			unlockProcess();
			return getStatusProcess();
		}

		// the msg start with interpreter="your class name" or currentCmd="your last command"

		// l'evenement est pouir moi
		if( isForMe(m.msg))
		{
			// c'est pas une erreur
			if( isError(m.msg))
			{
				qDebug("erreur");
				qDebug(m.msg);
				// the current command a generate error
				unlockProcess();
				// call processError()
//				int status = pClass->processError(m);
				setStatusProcess( pClass->processError(m) );
				// remove interpreter
				qDebug("remove interpreter (Error) " + pClass->name().toLocal8Bit());
				pClass->removeInterpreter();
				// pass to next command if bResume is true
				if(bResumeError)
					return commandProcess();
				else return getStatusProcess();//return status;
			}
			else
			{
				// ce n'est pas un erreur
				// recherche l'index de  l'evenement
				qDebug("generic process");
				qDebug(m.msg);
				int index = generic.gdbEvent.indexOf(pClass->getParametre("event=", m.msg));
				if(index != -1)
				{
					// recherche la fonction associé a l'evement
					if(pClass && generic.gdbFunction.at(index) != NULL)
					{
//						int status = (pClass->*(generic.gdbFunction.at(index)))(m);
//						if(status != PROCESS_TERMINED) return status;
						setStatusProcess( (pClass->*(generic.gdbFunction.at(index)))(m) ) ;
						if(getStatusProcess() != PROCESS_TERMINED) return getStatusProcess();
					}
					else 	QMessageBox::warning(NULL,"isForMe", "The current interpreter have event true but not have associate function");

					// la fonction est terminée
//					unlockProcess();

					// i have other command
					if(currentIndex < generic.gdbFunction.count())
					{
//						qDebug("next command from same class");
						// yes , remove interpreter
						qDebug("remove interpreter " + pClass->name().toLocal8Bit());
						pClass->removeInterpreter();
						// execute next command
						return commandProcess();
					}
					 lockProcess(); // waiting promt
				}
 
				// event prompt
				else if( pClass->getParametre("event=" , m.msg) == "prompt")
				{
					qDebug("remove interpreter (prompt) " + pClass->name().toLocal8Bit());
					pClass->removeInterpreter();
					pClass->processExit();

					// i have other command after prompt event ?
					if(currentIndex < generic.gdbFunction.count())
					{
						// execute next command
						unlockProcess();
						return commandProcess();
					}
					unlockProcess();
				}
			}
		}
		return getStatusProcess();
	}

private :
	bool bSending;
 	bool bBreakpointHit;
	bool bResumeError;
	bool bForce;
	bool bClearMessages;
};
#endif
