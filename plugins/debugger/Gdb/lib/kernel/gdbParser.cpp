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
	\file gdbParser.cpp
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


#include "gdbParser.h"

//#include <pMonkeyStudio.h>

#include <QtDebug>


/*!
	\details Create new object
	\param parent of this object
*/
GdbParser::GdbParser (QObject * parent) : QObject (parent), mIsReady(0)
{
	mEndParsing.clear();


	// Reading symbols from H:/Homega/DEV/monkeytest/src/ui/release/My Project.exe...(no debugging symbols found)...done.
	T_EVENT("^Reading symbols from .*...\\(no debugging symbols found\\)...done.", TARGET_NOLOADED);
	
	//Reading symbols from H:/Homega/DEV/monkeytest/src/ui/release/My Project.exe...done.
	T_EVENT("^Reading symbols from .*\\.\\.\\.done\\.", TARGET_LOADED);
	
	//Starting program: H:/Homega/DEV/monkeytest/src/ui/debug/My.exe 
	T_EVENT("^Starting program:\\s.*$", TARGET_RUNNING, true);
	
	// Breakpoint 10, 0x12345 at main.cpp:12
	T_EVENT("^Breakpoint\\s\\d+,\\s.*at\\s+[^:]+:\\d+$", TARGET_STOPPED);
	T_EVENT("^Continuing.$", TARGET_RUNNING, true);	
	T_EVENT("^Program exited normally." , TARGET_EXITED);
	T_EVENT("^The program is not being run." , TARGET_EXITED);
	T_EVENT("^The program is not running." , TARGET_EXITED);

	T_EVENT("^\"finish\" not meaningful in the outermost frame.", NOT_EXEC_COMMAND);
	// Program exited with code 030000000005.
	T_EVENT("^Program exited with code\\s\\w+.$", TARGET_EXITED);
	T_EVENT(".* not in executable format: File format not recognized$", TARGET_NOLOADED);

	T_EVENT("\\d+\\s+.*", TARGET_STOPPED);
	// when target crash Program terminated with signal SIGSEGV, Segmentation fault.
	T_EVENT("^Program\\s.*\\ssignal\\s.*,\\s.*", TARGET_CRASHED);
	//T_EVENT("^Program received signal SIGSEGV, Segmentation fault.$", TARGET_CRASHED);
	//T_EVENT("^Program received signal SIGTRAP, Trace/breakpoint trap.$", TARGET_CRASHED);

	// warning: Source file is more recent than executable.
	T_EVENT("^warning: Source file is more recent than executable.$", TARGET_NOLOADED);

	/* fix me */
	// stop 0x0040124b in __mingw_CRTStartup ()
	// stop 0x7c91e480 in ntdll!LdrDisableThreadCalloutsForDll () from C:\WINDOWS\system32\ntdll.dll
	// stop buged (linux) 0xb7047775 in __libc_start_main () from /lib/tls/i686/cmov/libc.so.6
	// no stop 0x00401ab1 in MyFormClass::MyFormClass (this=0x22ff10, parent=0x0) at MyFormClass.cpp:6
	// fix me
//	T_EVENT("^0x\\w+\\sin\\s.*[^at\\s.*:\\d+]$", TARGET_EXITED);
	T_EVENT("^0x\\w+\\sin\\s.*\\(\\).*", TARGET_EXITED);

	// Process
	connect(GdbCore::Process(), SIGNAL( commandReadyRead( const QString& )), this , SLOT( processParsing( const QString& )),Qt::DirectConnection);


	// default prompt
	mEndParsing << QRegExp("^\\(gdb\\)\\s$");

	mCmdList.clear();
	mCurrentCmd.clear();
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
	\details Find end of string block.
	All data from Gdb are append while prompt are other string is found
	For example it return true if string block ends with "(gdb) ", "Continue.", ....
	\param data is datas from GdbProcess (Gdb)
	\retval true if the string block ends with mEndParsing list
*/
bool GdbParser::isBufferCompleted(const QString data)
{
	QStringList lines = data.split(crlf);
	for(int j=0; j< lines.count(); j++)
	{
		for(int i=0; i < mEndParsing.count(); i++)
			if(mEndParsing.at(i).exactMatch(lines.at(j))) 	return true;
	}
	return false;
}

/*!
	\details Get the next command if it has.
	This attach the current string block from Gdb and the current command 
*/
void GdbParser::getCommand()
{
	if(mCmdList.count())
	{
		mCurrentCmd = mCmdList.at(0).cmd;
		mCurrentClassName = mCmdList.at(0).className;

		emit done(-1,"\"Get current command : " + mCurrentCmd);
		mCmdList.removeAt(0);
	}
}

// 
/*!
	\details New data from Gdb is avaible, parse this.
	When the string block is completed, GdbParser emit some signals, by example if it found "(gdb)" in string block,it emit prompt() signal
	\param storg is the raw data from GdbProcess class
	\retval true if the string block from GdbParser is completed, else false.
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
	if(isBufferCompleted(gdbBuffer))
	{
		// get the current command
		getCommand();

		// split line
		QStringList lines = gdbBuffer.split(crlf);

		// if answer is splitted in more line
		if(GdbCore::RestoreLine()->tryRestore(mCurrentClassName, &lines)) 
		{
			for(int i=0; i<lines.count(); i++)
				emit done(-1," !! Restoring -> \"" +  lines.at(i));
		}

		// read line by line
		for(int i=0; i<lines.count(); i++)
		{
			// extract one line from all lines
			QString oneLine = lines.at(i);

			if(!oneLine.isEmpty())
			{

// posible not use now v 1.4.0
/*				QRegExp f("^\\(gdb\\)\\s.+");
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

*/
				/*
					since V1.3.2 this is not using
				*/

				// remove all " in the string
				// because getParametre() function bug 
				// example :
				//	^info, .... answerGdb="undefined var "i" in current context",.....
				// getParametre() return -> "undefined var "
				
		//		while(oneLine.contains("\""))
		//			oneLine.remove("\"");

				// find if an interpreter is present for this line and command
				QPointer<BaseInterpreter> bi = GdbCore::Interpreter()->find(mCurrentClassName , mCurrentCmd, oneLine);
					
				if( !bi )
				{
					// interpreter no found for this command

					// generique informations or prompt event

					if(oneLine.contains( "(gdb)"))	
						emit prompt(0, "^prompt,className=\"" + mCurrentClassName + "\",event=\"prompt\",answerGdb=\"" + oneLine + "\",currentCmd=\"" + mCurrentCmd +"\""); 
					else
						// parse generique information
						onInfo(1, oneLine);
				}
				else
				{
					// fixed 1.3.2 07/07/08
					// remove prompt event 
					// re fixed v1.4.0 -> not remove prompt
					if(bi->getAnswerRegExp().exactMatch( "(gdb) "))
					{
						emit onInterpreter(bi,  bi->getId(), bi->getAnswerExtention() + oneLine + "\",currentCmd=\"" + mCurrentCmd +"\"");
						emit done(bi->getId(), bi->getAnswerExtention() + oneLine + "\",currentCmd=\"" + mCurrentCmd +"\""); 
						emit prompt(0, "^prompt,className=\"" + mCurrentClassName + "\",event=\"prompt\",answerGdb=\"Emulate prompt\",currentCmd=\"" + mCurrentCmd +"\""); 
					}
					else
					{
						// true line parsed, no prompt
						emit onInterpreter(bi,  bi->getId(), bi->getAnswerExtention() + oneLine + "\",currentCmd=\"" + mCurrentCmd +"\"");
						emit done(bi->getId(), bi->getAnswerExtention() + oneLine + "\",currentCmd=\"" + mCurrentCmd +"\""); 
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
	\sa GdbCore::Parser()
*/
void GdbParser::setNextCommand(QString className ,QString cmd)
{
	Command c = { className, cmd};
	mCmdList << c;
}

/*!
	\details Clear all commands in list
	\sa setNextCommand()
*/
void GdbParser::clearAllCommand()
{
	mCmdList.clear();
}

//


int GdbParser::find(QString st)
{
	for(int i=0; i< eventList.count(); i++)
	{
		if(eventList.at(i).rx.exactMatch(st)) return eventList.at(i).id;
	}
	return -1;
}

void GdbParser::onInfo(int id, QString st)
{

	switch(find(st))
	{
		case TARGET_NOLOADED : 
			emit done(TARGET_NOLOADED, "^done,className=\"GdbParser\",event=\"target-no-loaded\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			emit targetNoLoaded(TARGET_NOLOADED,st); 
		break;
		
		case TARGET_LOADED : 
			emit done(TARGET_LOADED, "^done,className=\"GdbParser\",event=\"target-loaded\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			emit targetLoaded(TARGET_LOADED, "^done,className=\"GdbParser\",event=\"target-loaded\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
		break;
	
		case TARGET_EXITED :
			emit done(TARGET_EXITED, "^done,className=\"GdbParser\",event=\"target-exited\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			emit targetExited(TARGET_EXITED, "^done,className=\"GdbParser\",event=\"target-exited\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
		break;
	
		case TARGET_STOPPED : 
			emit done(TARGET_STOPPED, "^done,className=\"GdbParser\",event=\"target-stopped\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			emit targetStopped(TARGET_STOPPED, "^done,className=\"GdbParser\",event=\"Breakpoint-hit\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
		break;

		case TARGET_RUNNING: 
			emit done(TARGET_RUNNING, "^done,className=\"GdbParser\",event=\"target-running\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			emit targetRunning(TARGET_RUNNING, "^done,className=\"GdbParser\",event=\"target-running\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
		break;

		case TARGET_CRASHED: 
			emit done(TARGET_CRASHED, "^done,className=\"GdbParser\",event=\"target-crashed\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			emit targetCrashed(TARGET_CRASHED, "^done,className=\"GdbParser\",event=\"target-crashed\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
		break;

		case NOT_EXEC_COMMAND : 
			emit done(NOT_EXEC_COMMAND, "^done,className=\"GdbParser\",event=\"target-stopped\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
			emit noExecCommand(NOT_EXEC_COMMAND, "^done,className=\"GdbParser\",event=\"" + st + "\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\""); 
		break;
		default :  emit done(id, "^done,className=\"" + mCurrentClassName +"\",event=\"info NOT found (no parsing)\",answerGdb=\"" + st + "\",currentCmd=\"" + mCurrentCmd +"\"");
	}
}

