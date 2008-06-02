/********************************************************************************************************
	* PROGRAM      : Debugger (PARSER)
	* DATE - TIME  : mardi 01 janvier 2008 - 18h48
	* AUTHOR       :  (  )
	* FILENAME     : GdbParser.cpp
	* LICENSE      : 
	* COMMENTARY   : 
	********************************************************************************************************/

/* main parser

		this file parse all data from Gdb
		it use a file for searsh data 
		it emit done() , error() and info()

	this parser can add user parser by call addInterpreter()
	if the new data can be parse with new interpreter, this data send 
*/


#include "gdbParser.1.3.h"

#include <coremanager.h>
#include <settingsmanager.h>
#include <monkey.h>
#include <queuedstatusbar.h>

//#include <QApplication>


#define INFO_ID			10000
#define ERROR_ID 		20000
#define PROMPT_ID		0


GdbParser::GdbParser (QObject * parent) : QObject (parent)
{
	gdbInterpreter = new GdbInterpreter(this);
	gdbPatternFile = new GdbPatternFile(this);
	gdbRestoreLine = new GdbRestoreLine(this);

	if(gdbInterpreter && gdbPatternFile && gdbRestoreLine)
	{
		// get plugins paths
		QStringList pluginsPath = MonkeyCore::settings()->value( "Plugins/Path" ).toStringList();
		// get all files in plugins path nammed 'know_list_and_id.txt'
		QFileInfoList files;
		QDir pluginsDir;
		for ( int i = 0; i < pluginsPath.count(); i++ )
		{
			QString path = pluginsPath.at( i );
			if ( QFileInfo( path ).isRelative() )
				path = QDir::cleanPath( QApplication::applicationDirPath() +"/" + path );
			pluginsDir.setPath( path );
			files << pMonkeyStudio::getFiles( pluginsDir, QString( "gdbparsing.txt" ), true );
		}
	
		// load txt file if possible, else warn user in status bar
		if ( files.isEmpty() || ! gdbPatternFile->load( files.first().absoluteFilePath() ) )
			MonkeyCore::statusBar()->appendMessage( tr( "gdbparsing.txt not found. Debugger can not work !" ), 5000 );
		else
		{
	
			#ifdef Q_OS_WIN 
				crlf = "\r\n";
			#endif
			#ifdef Q_OS_MAC 
				crlf = "\n";
			#endif
			#ifdef Q_OS_UNIX
				crlf = "\n";
			#endif


			mEndParsing.clear();
			mEndParsing << QRegExp(".*\\(gdb\\).$")
				<< QRegExp("^\\(gdb\\)\\s$")
				<< QRegExp(".*Continuing\\."+ crlf + "$") 
				<< QRegExp("^Starting program:.*") 
				<< QRegExp(".*\\(y or n\\).$");

			/*
				^done,reason="generic",value="Breakpoint 2, main (argc=Cannot access memory at address 0x0"
				^done,reason="generic",value=") at src/main.cpp:20"
			*/
			gdbRestoreLine->add( "^Breakpoint\\s\\d+,\\s.*\\s\\(.*Cannot access memory at address\\s0x[0-9a-FA-F]{1,}$" , 
				"^\\)\\sat\\s.*:\\d+$");
		

			/*
				^done,reason="generic information (not parsing)",value="#0  main (argc=Cannot access memory at address 0x0"  id : -1
				^done,reason="generic information (not parsing)",value=") at src/main.cpp:20"  id : -1
			*/
			gdbRestoreLine->add( "^#\\d+\\s.*Cannot access memory at address\\s0x[0-9a-FA-F]{1,}$" ,
				"^\\)\\sat\\s.*:\\d+$");
	
			/*
				Reading symbols from C:/DEV/debugger/debug/debugger.exe...
				done.
			*/
// ->  BUG not work
			gdbRestoreLine->add("^Reading symbols from .*\\.\\.\\.$",
				"^done\\.$");
			
			cmdList.clear();

			MonkeyCore::statusBar()->appendMessage( tr( "GdbParser initializing sucess full" ), 5000 );
		}
	}
	else MonkeyCore::statusBar()->appendMessage( tr( "GdbParser initializing FAILED" ), 5000 );
}

//
GdbParser::~GdbParser()
{
	cmdList.clear();
	mEndParsing.clear();
	currentCmd.clear();
	// Qt delete for me all child. 
}


// gateAway RestoreLine

void GdbParser::addRestoreLine(const QString & l1, const QString & l2)
{
	if(gdbRestoreLine)
		gdbRestoreLine->add(l1, l2);
}

// gateAway Interpreter

QPointer<BaseInterpreter> GdbParser::addInterpreter(const QString & cName, const QString & cGdb, const QRegExp & cRegExp, 
													const QRegExp & aRegExp,const QString & aExtention)
{
	if(gdbInterpreter)
		return gdbInterpreter->add(cName , cGdb , cRegExp ,  aRegExp , aExtention );
	else return NULL;
}


bool GdbParser::removeInterpreter( const QPointer<BaseInterpreter> & i )
{
	if(gdbInterpreter)
		return  gdbInterpreter->remove( i );
	else return false;
}

void GdbParser::changeAnswerInterpreter(const QPointer<BaseInterpreter> & i, const QString & s)
{
	if(gdbInterpreter)
		gdbInterpreter->changeAnswer(i, s);
}

//
/*
	main parser
*/
bool GdbParser::checkEndParsing(const QString data)
{

	for(int i=0; i < mEndParsing.count(); i++)
		if(mEndParsing.at(i).exactMatch(data)) 	return true;

	return false;
}
//

void GdbParser::getCommand()
{
	if(cmdList.count())
	{
		currentCmd = cmdList.at(0);
		cmdList.removeAt(0);
	}
}

//
void GdbParser::switchCommand(const QString & s)
{
	if(cmdList.count() && s == "(gdb) ")
	{
		currentCmd = cmdList.at(0);
		cmdList.removeAt(0);
	}
}

// 
bool GdbParser::processParsing(const QByteArray & storg)
{
	QByteArray st = storg;


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

		// if answer is splitted in two string
		if(gdbRestoreLine) 
			gdbRestoreLine->tryRestore(&lines);

		// read line by line
		for(int i=0; i<lines.count(); i++)
		{
			// extract one line from all lines
			QString oneLine = lines.at(i);

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
				Reading symbols from /home/yannick/dev/debugger/Debugger...
				done.
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
				(done)
			*/

			QRegExp f("^\\(gdb\\)\\s.+");
			if(f.exactMatch(oneLine))
			{
				qDebug("PARSER : Prompt formated false");
				qDebug("PARSER : " + oneLine.toLocal8Bit());
				QString c = oneLine.remove(0, 6);
				oneLine = lines[i] = "(gdb) ";
				lines.insert(i+1, c);
			}

			// find if this anwser is present under file ini
			int id = -1;
			if( gdbPatternFile )
				id = gdbPatternFile->find(oneLine).id;

			// remove all " in the string
			// because getParametre() function bug 
			// example :
			//	^info, .... answerGdb="undefined var "i" in current context",.....
			// getParametre() return -> "undefined var "
			
			while(oneLine.contains("\""))
				oneLine.remove("\"");


			// more than ERROR_ID (all errors)
			if(id != -1 && id >= ERROR_ID  )
				// error found
				onError(id, oneLine);
			else 
			{
				// find interpreter
				QPointer<BaseInterpreter> bi = gdbInterpreter->find(currentCmd, oneLine);
					
				if( !bi )
				{
					// interpreter no found for this command

					// Id is in ^info to ^error   
					if(id != -1 && id > INFO_ID && id < ERROR_ID)
						onInfo(id,oneLine);
			
					// unknow answer or prompt found
					if(id == -1 || id == PROMPT_ID)
						onDone(id, oneLine);
				}
				else
				{
					emit onInterpreter(bi, - bi->getId(), bi->getAnswerExtention() + oneLine);

					onInfo( - bi->getId() , bi->getAnswerExtention() + oneLine);

					// if interpreter wait prompt event, i don't have prompt line after, i emulate prompt sequence
					//^info,interpreter="GdbBreakPoint",event="breakpoint-deleted",answerExtention={fullname="main.cpp",nobkpt="1"},answerGdb="(gdb) ",currentCmd="delete 1"  id : 2
					//^done,interpreter="GdbParser",currentCmd="file ./Debugger",event="prompt",answerGdb="(gdb) "  id : 0
					if(oneLine.contains( "(gdb) "))
						onDone(0,"Emulate prompt");
				}
			}

			// oneLine == ((gdb) ?
			// yes i have receive one block from gdb generate by two commands
			// switch to next commant
			switchCommand(oneLine);
		}

		gdbBuffer.clear();
//		currentCmd = "Passible sync command failed : current command is Cleaning";
		return true;
	}
	return false;
}
//
void GdbParser::setLastCommand(QString cmd)
{
	cmdList << cmd;
}
//
/*
	sub main parser
*/
void GdbParser::onDone(int id, QString st)
{
	switch(id)
	{
		case -1 : emit done(id, "^done,interpreter=\"GdbParser\",event=\"generic information (not parsing)\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\"");break;
		case PROMPT_ID: emit done(id, "^done,interpreter=\"GdbParser\",event=\"prompt\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\""); break;
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
			emit targetLoaded(id, "^info,interpreter=\"GdbParser\",event=\"target-loaded\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\""); break;
		//[10016]^Kill the program being debugged\? \(y or n\) \[answered Y; input not from terminal\]
		case 10016 :
		//[10007]^Program exited normally\.
		case 10007 : 
			emit targetExited(id, "^info,interpreter=\"GdbParser\",event=\"target-exited\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\""); break;
	
		case 10009 : // breakpoint hit
				emit targetStopped(id, "^info,interpreter=\"GdbParser\",event=\"target-stopped\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\""); 
				emit info(id, "^info,interpreter=\"GdbParser\",event=\"breakpoint-hit\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\"");
		break;

		case 10010 : // run "r" command
		case 10011 :
		case 10012 : // continue. "c" command
		case 10013 :
		case 10014 :
			emit targetRunning(id, "^info,interpreter=\"GdbParser\",event=\"target-running\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\""); break;

		default : if(id>=INFO_ID) emit info(id, "^info,interpreter=\"GdbParser\",event=\"info found (parsing)\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\"");
			// Interpreter command
			else emit info(id,st + "\",currentCmd=\"" + currentCmd +"\"");
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
			emit error(id, "^error,interpreter=\"GdbParser\",event=\"error found\",answerGdb=\"" + st + "\",currentCmd=\"" + currentCmd +"\"");
	}
}
