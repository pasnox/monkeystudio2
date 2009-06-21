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
	\file gdbParser.h
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
	\details When new data is avaible from Gdb, the processParsing() function is call.

	- processParsing() function try restore string block if some lines is splited in more lines. For example

	\code
	Breakpoint 1, 0x3de4ac at 
		main.cpp, line 23
	(gdb)
	\endcode   

	is splited in two lines. processParsing() function try restore the original line by use GdbRestoreLine class

	\code
	Breakpoint 1, 0x3de4ac at main.cpp, line 23
	(gdb)
	\endcode

	- processParsing() search if this line is know in gdbparsing.txt by use GdbPatternFile class. 
	This file have all strings that Gdb can print in console.

	- processParsing() search if an interpreter can be found by use GdbInterpreter, 
	in this case onInterpreter() signal is emit.
	
	\sa GdbProcess, GdbInterpreter, GdbRestoreLine, GdbPatternFile
*/
class GdbParser : public QObject, public QSingleton<GdbParser>
{

		Q_OBJECT
		friend class QSingleton<GdbParser>;

		/*! 
			\brief Struct for store the current command.
			\sa GdbParser::setNextCommand()
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
		When it is ready, GdbProcess can send the next command.
		\retval Current state
		\sa GdbProcess::sendRawData()
	*/
	bool isReady() { return mIsReady;}
	
	/*! 
		\details Set GdbParser class ready or not.
		\param b is true if you want set ready GdbParser.
	*/
	void setReady(bool b) {mIsReady = b;}

private :

	/*!
		\details Current instance of GdbInterpreter class.
	*/
	QPointer<GdbInterpreter> gdbInterpreter;

	/*!
		\details Current instance of GdbPatternFile class. 
	*/
	QPointer<GdbPatternFile> gdbPatternFile;

	/*!
		\details Current instance of GdbRestoreLine
	*/
	QPointer<GdbRestoreLine> gdbRestoreLine;

	/*!
		\details List of all end string block
		\sa checkEndParsing()
	*/
	QList<QRegExp> mEndParsing;

	/*!
		\details List of all commands.
		\sa setNextCommand()
	*/
	QList<Command> mCmdList;

	/*!
		\details Current command used
	*/
	QString mCurrentCmd;

	/*!
		\details Current class name used
	*/
	QString mCurrentClassName;

	/*!
		\details Current buffer for cat string from Gdb
	*/
	QString gdbBuffer;
	
	bool checkEndParsing(const QString data);

	/*!
		\details var crlf
	*/
	QString crlf;

	// base parser
	/*! 
		\details Done event, data from Gdb can't be parsed. In this case the signal done() is emitted. 
		\param stId is an unic id.
		\param string is the string formated
		\sa onInfo(), onError()
	*/
	void onDone(int stId, QString string);

	/*! 
		\details Error event, data from Gdb is an error. In this case the signal error() is emitted.
		\param stId is an unic id.
		\sa onDone(), onInfo()
		\param string is the string formated
	*/
	void onError(int stId, QString string);

	/*! 
		\details Info event, data from Gdb is an information. In this case the signal info() is emitted.
		\param stId is an unic id.
		\param string is the string formated
		\sa onDone(), onError()
	*/
	void onInfo(int stId, QString string);

	void getCommand();

	bool mIsReady;

signals:

	/*! 
		\details Emit when an interpreter is found
		\param pInter is a pointer to the current BaseInterpreter found.
		\param stId is the pInter convert to integer.
		\param string is the string formated from Gdb.
		\sa info() signal
	*/
	void onInterpreter(const QPointer<BaseInterpreter> &pInter , const int &stId , const QString &string);

	/*! 
		\details Emit when target is correctly loaded
		\param stId is an unic id.
		\param string is the string formated
	*/
	void targetLoaded(int stId, QString string);

	/*! 
		\details Emit when target is not correctly loaded
		\param stId is an unic id.
		\param string is the string formated
	*/
	void targetNoLoaded(int stId, QString string);

	/*! 
		\details Emit when target correctly exit
		\param stId is an unic id.
		\param string is the string formated
	*/
	void targetExited(int stId, QString string);

	/*! 
		\details Emit when target is stopped
		\param stId is an unic id.
		\param string is the string formated
	*/
	void targetStopped(int stId, QString string);

	/*! 
		\details Emit when target is running
		\param stId is an unic id.
		\param string is the string formated
	*/
	void targetRunning(int stId, QString string);
	
	/*!
		\details Emit when GdbParser not found an interpreter and not found this string in gdbparsing.txt file. 
		\param stId is an unic id.
		\param string is the string formated
		\sa info(), error(), prompt()
	*/
	void done(int stId, QString string);

	/*!
		\details Emit when GdbParser found an error in gdbparsing.txt file 
		\param stId is an unic id.
		\param string is the string formated
		\sa info(), done(), prompt()
	*/
	void error(int stId, QString string);

	/*!
		\details Emit when GdbParser not found an information in gdbparsing.txt file or if it found an interpreter.
		\note If an interpreter is found, the signal onInterpreter() is emit before info(). A information can be targetStopped();
		targetLoaded(), targetRunning(), targetExited() and targetNoLoaded()
		\param stId is an unic id.
		\param string is the string formated
		\sa done(), error(), prompt()
	*/
	void info(int stId, QString string);

	/*! 
		\details Emit when GdbParser found Gdb prompt
		\param stId egal 0, because is the prompt event.
		\param string is the string formated.
		\sa done(), error(), info()
	*/
	void prompt(int stId, QString string);
};
#endif 
