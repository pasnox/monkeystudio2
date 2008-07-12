/********************************************************************************************************
	* PROGRAM      : Debugger
	* DATE - TIME  : lundi 31 mai 2008 - 18h04
	* AUTHOR       : Xiantia
	* FILENAME     : GdbInterpreter
	* LICENSE      : GPL
	* COMMENTARY   : 
	********************************************************************************************************/


#ifndef GDBINTERPRETER_H
#define GDBINTERPRETER_H

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QPointer>

//! This class store only one interpreter

/**
\author xiantia
\version 1.3.2

This class store all informations for only one interpreter
*/

class BaseInterpreter : public QObject
{

public :

		BaseInterpreter(QObject *parent = 0);

		/**
		 * \brief Set or configure interpreter 
		*/
		void set(QString cName,/* QString cGdb,*/ QRegExp cRegExp,  QRegExp aRegExp, QString aExtention);


		~BaseInterpreter();

		/**
		 * \brief Return the class name 
		*/
		QString getClassName() {return mClassName;} 

		/**
		 * \brief Return the current command in QRegExp format 
		*/
		QRegExp getCmdRegExp() { return mCmdRegExp;}

		/**
		 * \brief Return the correct answer in RegExp format 
		*/
		QRegExp getAnswerRegExp() { return mAnswerRegExp;}

		/**
		 * \brief Return the new answer from Gdb (change answer from Gdb) 
		*/
		QString getAnswerExtention() { return mAnswerExtention;}

		/**
		 * \brief Return an unic Id for
		*/
		quintptr getId(){ return (quintptr) this;}

		void setClassName(QString s) { mClassName = s;}
		void setAnswerExtention(QString s) { mAnswerExtention = s ;}
		void setAnswerRegExp(QRegExp s) { mAnswerRegExp = s ;}
		void setCmdRegExp(QRegExp s) { mCmdRegExp = s ;}
		
private :
		//! Class name
		QString mClassName;		
		//! command gdb in RegExp format
		QRegExp mCmdRegExp;		
		//! answer from gdb in QregExp
		QRegExp mAnswerRegExp;		
		//! if you want pass other informations
		QString mAnswerExtention;	
};


//! This class store all interpreters from all AddOn

/**
	This is list of all interpreters
*/

class  GdbInterpreter : public QObject
{
		Q_OBJECT

public :
	
		GdbInterpreter(QObject *parent = 0);

		/**
		 * \brief Add new interpreter in list
		*/
		QPointer<BaseInterpreter> add(const QString &,/* const QString &,*/ const QRegExp &,  const QRegExp &, const QString & );

		/**
		 * \brief Remove an interpreter in member list
		*/
		bool remove(const QPointer<BaseInterpreter> &);

		/**
		 * \brief Change answer extention
		*/
		void changeAnswer(const QPointer<BaseInterpreter> &, const QString &);

		/**
		 * \brief Fin if an interpreter can be found for the current class ,command, answer from Gdb.
		 * \retval A pointer to this interpreter.
		*/
		QPointer<BaseInterpreter> find(const QString & , const QString & , const QString & );

		~GdbInterpreter();

private :

		/**
		 * \brief List off all interpreters
		*/
		QList< QPointer<BaseInterpreter> > GdbInterpreterList;

};
#endif

