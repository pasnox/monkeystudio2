/********************************************************************************************************
	* PROGRAM      : Debugger
	* DATE - TIME  : lundi 31 dcembre 2007 - 18h04
	* AUTHOR       :  (  )
	* FILENAME     : gdbParser
	* LICENSE      : 
	* COMMENTARY   : 
	********************************************************************************************************/

#ifndef GDBPARSER_H
#define GDBPARSER_H

#include "fresh.h"

#include <QObject>
#include <QString>

#include <QStringList>
#include <QRegExp>
#include <QFile>

#include "gdbInterpreter.1.3.h"
#include "gdbPatternFile.1.3.h"
#include "gdbRestoreLine.1.3.h"

//! Parse all data from Gdb
/**
\author xiantia
\version 1.3.2

When new data is avaible from Gdb, the processParsing function is call.

1 : processParsing function try restore string block if some lines is splited in more lines. For example

\code
Breakpoint 1, 0x3de4ac at 
	main.cpp, line 23
(gdb)
\endcode   

is splited in two lines. processParsing function try restore the original line by use GdbRestoreLine class

\code
Breakpoint 1, 0x3de4ac at main.cpp, line 23
(gdb)
\endcode

2 : processParsing search if this line is know in gdbparsing.txt by use GdbPatternFile class. This file have all strings that Gdb can print in console.

3 : processParsing search if an interpreter can be found by use GdbInterpreter, in this case interpreter signal is emit.

*/

class GdbParser : public QObject, public QSingleton<GdbParser>
{

		Q_OBJECT
		friend class QSingleton<GdbParser>;

		//! Struct for store command list 
		typedef struct Command
		{
			QString className;
			QString cmd;
		};

public:

	GdbParser (QObject * parent = 0);
	~GdbParser();

public slots:

	//! New data from Gdb is avaible, parse this
	bool processParsing(const QString&);
	//! Add new command
	void setNextCommand(QString , QString);
	//! clear all command
	void clearAllCommand();

	//! gateAway restoreLine
	void addRestoreLine(const QString &, const QString &, const QString &);

	//! gateAway interpreter
	QPointer<BaseInterpreter> addInterpreter(const QString & cName, /*const QString & cGdb,*/ const QRegExp & cRegExp,
		const QRegExp & aRegExp, const QString & aExtention);

	//! Remove an interpreter
	bool removeInterpreter( const QPointer<BaseInterpreter> & );
	//! Change answerExtention for an interpreter
	void changeAnswerInterpreter(const QPointer<BaseInterpreter> &, const QString &);

	//! Parser class is ready ?
	bool isReady() { return mIsReady;}
	//! Set Parser class ready or not.
	void setReady(bool a) {mIsReady = a;}

private :

	QPointer<GdbInterpreter> gdbInterpreter;
	QPointer<GdbPatternFile> gdbPatternFile;
	QPointer<GdbRestoreLine> gdbRestoreLine;

	QList<QRegExp> mEndParsing;

	QList<Command> mCmdList;

	QString mCurrentCmd;
	QString mCurrentClassName;

	QString gdbBuffer;
	
	//! Find end of string block
	/**
	All data from Gdb are append while prompt are other string is found

	For example it return true if string block ends with "(gdb) ", "Continue.", ....
	*/
	bool checkEndParsing(const QString data);

	//! var crlf
	QString crlf;

	// base parser
	void onDone(int , QString);
	void onError(int , QString);
	void onInfo(int , QString);

	//! Get the next command
	void getCommand();

	bool mIsReady;

signals:

	//! An interpreter is found
	void onInterpreter(const QPointer<BaseInterpreter> & , const int & , const QString &);

	//! Target is correctly loaded
	void targetLoaded(int , QString);
	//! Target is not correctly loaded
	void targetNoLoaded(int, QString);

	//! Target correctly exit
	void targetExited(int , QString);
	//! Target is stopped
	void targetStopped(int , QString);
	//! Target is running
	void targetRunning(int , QString);
	
	//! Done event, data from Gdb can't be parsed
	void done(int , QString);
	//! Error event, data from Gdb is an error
	void error(int , QString);
	//! Info event, data from Gdb is an information
	void info(int, QString);
	//! Prompt event, data from Gdb is a prompt
	void prompt(int, QString);
};


#endif 
