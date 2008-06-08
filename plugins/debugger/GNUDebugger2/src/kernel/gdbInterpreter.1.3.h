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



class BaseInterpreter : public QObject
{

public :

		BaseInterpreter(QObject *parent = 0);

		void set(/*QString cName, QString cGdb,*/ QRegExp cRegExp,  QRegExp aRegExp, QString aExtention);


		~BaseInterpreter();

		QRegExp getCmdRegExp() { return mCmdRegExp;}
		QRegExp getAnswerRegExp() { return mAnswerRegExp;}
		QString getAnswerExtention() { return mAnswerExtention;}
		int getId(){ return (int) this;}

		void setAnswerExtention(QString s) { mAnswerExtention = s ;}
		void setAnswerRegExp(QRegExp s) { mAnswerRegExp = s ;}
		void setCmdRegExp(QRegExp s) { mCmdRegExp = s ;}
		
private :

		QRegExp mCmdRegExp;			// command gdb in RegExp format
		QRegExp mAnswerRegExp;		// answer from gdb in QregExp
		QString mAnswerExtention;	// if you want passe other informations


};



class  GdbInterpreter : public QObject
{
		Q_OBJECT


public :
	
		GdbInterpreter(QObject *parent = 0);

		QPointer<BaseInterpreter> add(/*const QString &, const QString &,*/ const QRegExp &,  const QRegExp &, const QString & );
		bool remove(const QPointer<BaseInterpreter> &);
		void changeAnswer(const QPointer<BaseInterpreter> &, const QString &);
		QPointer<BaseInterpreter> find(const QString & , const QString & );

		~GdbInterpreter();

private :
		QObject *mParent;

		int findCmd(const QString & );

		QList< QPointer<BaseInterpreter> > GdbInterpreterList;

};
#endif

