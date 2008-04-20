#ifndef GDBBASEINTERPRETER_H
#define GDBBASEINTERPRETER_H

#include <QString>
#include <QRegExp>

class  QGdbInterpreter
{
public :
		QGdbInterpreter(){};

		QGdbInterpreter(QString cName, QString cGdb, QRegExp cRegExp,  QRegExp aRegExp, QString aExtention)
		{
			mCmdName = cName;
			mCmdGdb = cGdb;
			mCmdRegExp = cRegExp;
			mAnswerRegExp = aRegExp;
			mAnswerExtention = aExtention;
		};
		~QGdbInterpreter(){};

		QString cmdName(){ return mCmdName;}
		QString cmdGdb(){ return mCmdGdb;}
		QString answerExtention(){ return mAnswerExtention;}
		QRegExp answerRegExp(){ return mAnswerRegExp;}
		QRegExp cmdRegExp(){ return mCmdRegExp;}

private :
		QString mCmdName; 			// name of your command
		QString mCmdGdb;			// command  send to gdb
		QRegExp mCmdRegExp;		// command gdb in RegExp
		QRegExp mAnswerRegExp;	// answer from gdb in QregExp
		QString mAnswerExtention;	// if you want passe other infomations
};
#endif
