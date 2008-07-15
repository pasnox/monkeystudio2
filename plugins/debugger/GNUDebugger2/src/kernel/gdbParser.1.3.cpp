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
	\file gdbParser.1.3.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Parse all datas from Gdb
*/

/* main parser

		this file parse all data from Gdb
		it use a file for searsh data 
		it emit done() , error() and info()

	this parser can add user parser by call addInterpreter()
	if the new data can be parse with new interpreter, this data send 
*/


#include "gdbParser.1.3.h"

#include <monkey.h>


#define INFO_ID			10000
#define ERROR_ID 		20000
#define PROMPT_ID		0

/*!
	\details Create new object
	\param parent of this object
*/
GdbParser::GdbParser (QObject * parent) : QObject (parent), mIsReady(0)
{
	// get the current instance
	gdbPatternFile = GdbPatternFile::instance();
	// new instance child class
	gdbInterpreter = new GdbInterpreter(this);
	gdbRestoreLine = new GdbRestoreLine(this);

	if(gdbInterpreter && gdbPatternFile && gdbRestoreLine)
	{
	
		crlf = pMonkeyStudio::getEol();

		mEndParsing.clear();
		mEndParsing << QRegExp(".*\\(gdb\\).$")
			<< QRegExp("^\\(gdb\\)\\s$")
			<< QRegExp(".*Continuing\\."+ crlf + "$") 
			<< QRegExp("^Starting program:.*") 
			<< QRegExp(".*\\(y or n\\).$");

			/*
				Breakpoint 2, main (argc=Cannot access memory at address 0x0
				) at src/main.cpp:20
			*/
//			gdbRestoreLine->add( "^Breakpoint\\s\\d+,\\s.*\\s\\(.*Cannot access memory at address\\s0x[0-9a-FA-F]{1,}$" , 
//				"^\\)\\sat\\s.*:\\d+$");
		
			/*
				#0  main (argc=Cannot access memory at address 0x0
				) at src/main.cpp:2
			*/
//			gdbRestoreLine->add( "^#\\d+\\s.*Cannot access memory at address\\s0x[0-9a-FA-F]{1,}" ,
//				"\\)\\sat\\s.*:\\d+$");
	
		/*
			Reading symbols from C:/DEV/debugger/debug/debugger.exe...
			done.
		*/
		gdbRestoreLine->add("Dock",
			"^Reading symbols from .*\\.\\.\\..*",
			".*done\\.$");

		/*
			Breakpoint 14, UIForm::UIForm (this=0x4b61ee0, parent=0x0)
				at src/ui/UIForm.cpp:40

			Breakpoint 3, QInternal::activateCallbacks (cb=EventNotifyCallback, 
				parameters=0x22f600) at global/qglobal.cpp:2690

		*/			
		gdbRestoreLine->add("Dock",
			"^Breakpoint\\s\\d+,\\s.*",
			".*at\\s+[^:]+:\\d+$");

		mCmdList.clear();
	}
}

//
/*!
	\details Clear some lists
*/
GdbParser::~GdbParser()
{
	mCmdList.clear();
	mEndParsing.clear();
	mCurrentCmd.clear();
	// Qt delete for me all child. 
}


/*!
	\details GateAway for GdbRestoreLine class
	\param className is the class name
	\param l1 is the fisrt line
	\param l2 is the last line
*/
void GdbParser::addRestoreLine(const QString & className, const QString & l1, const QString & l2)
{
	if(gdbRestoreLine)
		gdbRestoreLine->add(className, l1, l2);
}

/*!
	\details GateAway GdbInterpreter class
	\param cName is the name of class
	\param cRegExp is the command
	\param aRegExp is the answer
	\param aExtention is the answer extention 
*/
QPointer<BaseInterpreter> GdbParser::addInterpreter(const QString & cName,  const QRegExp & cRegExp, 
													const QRegExp & aRegExp,const QString & aExtention)
{
	if(gdbInterpreter)
		return gdbInterpreter->add(cName ,/* cGdb , */cRegExp ,  aRegExp , aExtention );
	else return NULL;
}

/*!
	\details Remove an interpreter
	\param i is the pointer to this interpreter
*/
bool GdbParser::removeInterpreter( const QPointer<BaseInterpreter> & i )
{
	if(gdbInterpreter)
		return  gdbInterpreter->remove( i );
	else return false;
}

/*!
	\details Change answerExtention for an interpreter
	\param i is the pointer to this interpreter
	\param s is the new answer
*/
void GdbParser::changeAnswerInterpreter(const QPointer<BaseInterpreter> & i, const QString & s)
{
	if(gdbInterpreter)
		gdbInterpreter->changeAnswer(i, s);
}

/*!
	\details Find end of string block
	All data from Gdb are append while prompt are other string is found
	For example it return true if string block ends with "(gdb) ", "Continue.", ....
	\param data is data from GdbProcess
	\retval true if the string block ends with mEndParsing list
*/
bool GdbParser::checkEndParsing(const QString data)
{
	for(int i=0; i < mEndParsing.count(); i++)
		if(mEndParsing.at(i).exactMatch(data)) 	return true;

	return false;
}

/*!
	\details Get the next command if it has
	This attach the current string block from Gdb and the current command 
*/
void GdbParser::getCommand()
{
	if(mCmdList.count())
	{
		mCurrentCmd = mCmdList.at(0).cmd;
		mCurrentClassName = mCmdList.at(0).className;

		onInfo(-1,"\"Get current command : ");
		mCmdList.removeAt(0);
	}
}

// 
/*!
	\details New data from Gdb is avaible, parse this.
	\param storg is the raw data from GdbProcess class
	\retval true if the string block from GdbParser is completed, else false
*/
bool GdbParser::processParsing(const QString & storg)
{
	QString st = storg;

	setReady(false);

#ifdef Q_OS_WIN

	/*
		For windows
		swap prompt "(gdb) "
		because under windows readAll() function read ALL data from stdout and AFTER stderr
	
		False sequence :
	
		(gdb)
		(gdb) error undefined command
	
		True sequence :
	
		(gdb)
		error undefined command
		(gdb)
	*/
	QRegExp p(".*\\(gdb\\)\\s.+");
	if(p.exactMatch(st))
	{
		st.remove(st.indexOf("(gdb) "),6);
		st += "(gdb) ";
		onInfo(-1," Swap stdOut / sdtErr -> (Windows)\n\"" +  st);
	}
	// end windows

#endif


	// append buffer
	gdbBuffer.append(st);
	
	// st end with prompt gdb or other?
	// buffer completed
	if(checkEndParsing(gdbBuffer))
	{

		// get the current command
		getCommand();

		// spilt line
		QStringList lines = gdbBuffer.split(crlf);

		// if answer is splitted in more string
		if(gdbRestoreLine && gdbRestoreLine->tryRestore(mCurrentClassName, &lines)) 
		{
			for(int i=0; i<lines.count(); i++)
				onInfo(-1," !! Restoring -> \"" +  lines.at(i));
		}

		// read line by line
		for(int i=0; i<lines.count(); i++)
		{
			// extract one line from all lines
			QString oneLine = lines.at(i);


			/*
				since V1.3.2 this is not using
						|
						V
			*/

			/*
				for Linux : some time the prompt have not crlf at the end of "(gdb) "
				True sequence :
	
				GNU gdb 6.8-debian
				Copyright (C) 2008 Free Software Foundation, Inc.
				License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
				This is free software: you are free to change and redistribute it.
				There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
				and "show warranty" for details.
				This GDB was configured as "i486-linux-gnu".
	
				(gdb) 
				Reading symbols from /home/yannick/dev/debugger/Debugger...done.
				(gdb)
				-------------------------------------------------------------------------------
				False sequence :
	
				GNU gdb 6.8-debian
				Copyright (C) 2008 Free Software Foundation, Inc.
				License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
				This is free software: you are free to change and redistribute it.
				There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
				and "show warranty" for details.
				This GDB was configured as "i486-linux-gnu".
				(gdb) Reading symbols from /home/yannick/dev/debugger/Debugger...
				done.
				(gdb)
			*/

			QRegExp f("^\\(gdb\\)\\s.+");
			if(f.exactMatch(oneLine))
			{
				onInfo(-1,"Prompt not have crlf");
				onInfo(-1,oneLine); 
				//(gdb) Reading symbols from /home/yannick/dev/debugger/Debugger...done
				QString c = oneLine.remove(0, 6);
				// Reading symbols from /home/yannick/dev/debugger/Debugger...done
				oneLine = lines[i] = "(gdb) ";
				// (gdb) 
				lines.insert(i+1, c);
				// (gdb)
				// Reading symbols from /home/yannick/dev/debugger/Debugger...done
			}


			// find if this anwser is present under file ini
			GdbPattern p={"", QRegExp(), false, false};
			if( gdbPatternFile && !oneLine.isEmpty())
				p = gdbPatternFile->find(oneLine);

			
			/*
				since V1.3.2 this is not using
			*/

			// remove all " in the string
			// because getParametre() function bug 
			// example :
			//	^info, .... answerGdb="undefined var "i" in current context",.....
			// getParametre() return -> "undefined var "
			
			while(oneLine.contains("\""))
				oneLine.remove("\"");

			if(p.enable == 2) // just show message
				onDone(p.id, "FOUND ERROR BUT THIS IS DISABLE, PLEASE REPORT THIS LINE TO MONKEY TEAM");

			// more than ERROR_ID (all errors)
			if(p.id != -1 && p.id >= ERROR_ID  && p.enable == 1)
				// error found, if is enbaled in file
				onError(p.id, oneLine);
			else 
			{
				// find interpreter
				QPointer<BaseInterpreter> bi = gdbInterpreter->find(mCurrentClassName , mCurrentCmd, oneLine);
					
				if( !bi )
				{
					// interpreter no found for this command

					// Id is in ^info to ^error   
					if(p.id != -1 && p.id > INFO_ID && p.id < ERROR_ID)
						onInfo(p.id,oneLine);
			
					// unknow answer or prompt found
					// fixed for not have prompt when the line is empty. v1.3.2 09/07/08
					if( (p.id == -1 || p.id == PROMPT_ID) && !oneLine.isEmpty())
						onDone(p.id, oneLine);
				}
				else
				{
					// fixed 1.3.2 07/07/08
					// remove prompt event 
					if(oneLine.contains( "(gdb) "))
					{
						oneLine.remove("(gdb) ");
						emit onInterpreter(bi, - bi->getId(), bi->getAnswerExtention() + oneLine + "\",currentCmd=\"" + mCurrentCmd +"\"");
						onInfo( - bi->getId() , bi->getAnswerExtention() + oneLine);
						onDone(0,"Emulate prompt");
					}
					else
					{
						emit onInterpreter(bi, - bi->getId(), bi->getAnswerExtention() + oneLine + "\",currentCmd=\"" + mCurrentCmd +"\"");
						onInfo( - bi->getId() , bi->getAnswerExtention() + oneLine);
					}
				}
			}
		}

		gdbBuffer.clear();
		setReady(true);
		return true;
	}
	return false;
}
//
/*!
	\details Add new command
	\param className is the class name.
	\param cmd is command to send to GdbProcess.
*/
void GdbParser::setNextCommand(QString className ,QString cmd)
{
	Command c = { className, cmd};
	mCmdList << c;
}

/*!
	\details Clear all commands in list
*/
void GdbParser::clearAllCommand()
{
	mCmdList.clear();
}

//

void GdbParser::onDone(int id, QString st)
{
	switch(id)
	{
		case PROMPT_ID: //emit done(id, "^done,interpreter=\"" + mCurrentClassName + "\",event=\"prompt\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
			emit prompt(id, "^prompt,interpreter=\"" + mCurrentClassName + "\",event=\"prompt\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
			mIsReady = true;
			break;
		default : emit done(id, "^done,interpreter=\"" + mCurrentClassName + "\",event=\"generic information (not parsing)\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");break;

	}
}

//

void GdbParser::onInfo(int id, QString st)
{
	switch(id)
	{
		case 10004 : // no symbol found compiled in release
			emit targetNoLoaded(id,st); 
		break;
		//[10005]^Reading symbols from .*done\.
		case 10005 : 
			emit targetLoaded(id, "^info,interpreter=\"GdbParser\",event=\"target-loaded\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); break;
		//[10016]^Kill the program being debugged\? \(y or n\) \[answered Y; input not from terminal\]
		case 10016 :
		//[10007]^Program exited normally\.
		case 10007 : 
			emit targetExited(id, "^info,interpreter=\"GdbParser\",event=\"target-exited\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); break;
	
		case 10021 : // Program received signal SIGSEGV, Segmentation fault.
		case 10020 : // Step finish but no can execute this
		case 10009 : // breakpoint hit
//				emit targetStopped(id, "^info,interpreter=\"GdbParser\",event=\"target-stopped\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
//				emit info(id, "^info,interpreter=\"GdbParser\",event=\"breakpoint-hit\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
				emit targetStopped(id, "^info,interpreter=\"GdbParser\",event=\"target-stopped\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
		break;

		case 10010 : // run "r" command
		case 10011 :
		case 10012 : // continue. "c" command
		case 10013 :
		case 10014 :
			emit targetRunning(id, "^info,interpreter=\"GdbParser\",event=\"target-running\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); break;

		default : if(id>=INFO_ID) emit info(id, "^info,interpreter=\"GdbParser\",event=\"info found (parsing)\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			// Interpreter command
			else emit info(id, st + "\",currentCmd=\"" + mCurrentCmd +"\"");
	}
}

//

void GdbParser::onError(int id, QString st)
{
	switch(id)
	{
		case 21550 : // format non reconnu
		case 23290 :
		case 22833 : 
			emit targetNoLoaded(id, st);
		break;
		default : 
			emit error(id, "^error,interpreter=\"" + mCurrentClassName + "\",event=\"error found\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
	}
}
