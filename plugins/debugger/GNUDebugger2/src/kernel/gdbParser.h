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
	\file gdbParser.1.3.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Parse all datas from Gdb
*/

#ifndef GDBPARSER_H
#define GDBPARSER_H

#include "fresh.h"

#include <QObject>
#include <QString>

#include <QStringList>
#include <QRegExp>
#include <QFile>

#include "gdbInterpreter.h"
#include "gdbPatternFile.h"
#include "gdbRestoreLine.h"

/*!
	\brief Parse all datas from Gdb
	\details When new data is avaible from Gdb, the processParsing function is call.

	- processParsing function try restore string block if some lines is splited in more lines. For example

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

	- processParsing search if this line is know in gdbparsing.txt by use GdbPatternFile class. 
	This file have all strings that Gdb can print in console.

	- processParsing search if an interpreter can be found by use GdbInterpreter, 
	in this case interpreter signal is emit.
*/
class GdbParser : public QObject, public QSingleton<GdbParser>
{

		Q_OBJECT
		friend class QSingleton<GdbParser>;

		/*! 
			\brief Struct for store the current command
		*/	
		typedef struct Command
		{
			QString className;
			QString cmd;
		};

public:

	GdbParser (QObject * parent = 0);
	~GdbParser();

public slots:

	bool processParsing(const QString&);
	void setNextCommand(QString , QString);
	void clearAllCommand();

	void addRestoreLine(const QString &, const QString &, const QString &);
	QPointer<BaseInterpreter> addInterpreter(const QString & cName,  const QRegExp & cRegExp,
		const QRegExp & aRegExp, const QString & aExtention);

	bool removeInterpreter( const QPointer<BaseInterpreter> & );
	void changeAnswerInterpreter(const QPointer<BaseInterpreter> &, const QString &);

	/*! 
		\details GdbParser class is ready ?
	*/
	bool isReady() { return mIsReady;}
	
	/*! 
		\details Set GdbParser class ready or not.
	*/
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
	
	bool checkEndParsing(const QString data);

	/*!
		\details var crlf
	*/
	QString crlf;

	// base parser
	void onDone(int , QString);
	void onError(int , QString);
	void onInfo(int , QString);

	void getCommand();

	bool mIsReady;

signals:

	/*! 
		\details Emit when an interpreter is found
	*/
	void onInterpreter(const QPointer<BaseInterpreter> & , const int & , const QString &);

	/*! 
		\details Emit when target is correctly loaded
	*/
	void targetLoaded(int , QString);

	/*! 
		\details Emit when target is not correctly loaded
	*/
	void targetNoLoaded(int, QString);

	/*! 
		\details Emit when target correctly exit
	*/
	void targetExited(int , QString);

	/*! 
		\details Emit when target is stopped
	*/
	void targetStopped(int , QString);

	/*! 
		\details Emit when target is running
	*/
	void targetRunning(int , QString);
	
	/*! 
		\details Done event, data from Gdb can't be parsed
	*/
	void done(int , QString);

	/*! 
		\details Error event, data from Gdb is an error
	*/
	void error(int , QString);

	/*! 
		\details Info event, data from Gdb is an information
	*/
	void info(int, QString);

	/*! 
		\details Prompt event, data from Gdb is a prompt
	*/
	void prompt(int, QString);
};
#endif 
