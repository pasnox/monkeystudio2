#ifndef GDBBASEINTERPRETER_H
#define GDBBASEINTERPRETER_H

#include <QString>
#include <QRegExp>

// initialise un interpreter
class  gdb_base_interpreter
{
public :
		gdb_base_interpreter(){};

		gdb_base_interpreter(QString cName, QString cGdb, QRegExp cRegExp,  QRegExp aRegExp, QString aExtention)
		{
			cmdName = cName;
			cmdGdb = cGdb;
			cmdRegExp = cRegExp;
			answerRegExp = aRegExp;
			answerExtention = aExtention;
		};
		~gdb_base_interpreter(){};

		QString cmdName;
		QString cmdGdb;
		QRegExp cmdRegExp;
		QRegExp answerRegExp;
		QString answerExtention;
};

typedef gdb_base_interpreter QGdbInterpreter;

#endif
