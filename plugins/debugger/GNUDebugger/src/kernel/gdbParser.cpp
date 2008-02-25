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


#include "./kernel/gdbParser.h"

#include <QMessageBox>

#define INFO_ID			10000
#define ERROR_ID 		20000
#define PROMPT_ID		0


GdbParser::GdbParser (QObject* p) : QObject (p)
{
	if(loadingPattern("./plugins/debugger/file/know_list_and_id.txt"))
	{
	//	QMessageBox::warning(NULL, "Error","File know_list_and_id.txt not found.\nCopy this file from MONKEY_SRC/plugins/base/debugger/file/ to ./bin/plugins/debugger/");
	//	emit error(0, "(Class GdbPaser) Erreur d'ouverture du fichier de RegExp");
	}
	#ifdef Q_OS_WIN 
		crlf = "\r\n";
	#endif
	#ifdef Q_OS_MAC 
		crlf = "\n";
	#endif
	#ifdef Q_OS_UNIX
		crlf = "\n";
	#endif

	restoringList 
		/*
		^done,reason="generic",value="Breakpoint 2, main (argc=Cannot access memory at address 0x0"
		^done,reason="generic",value=") at src/main.cpp:20"
		*/
		<< QRegExp("^Breakpoint\\s\\d+,\\s.*\\s\\(.*Cannot access memory at address\\s0x[0-9a-FA-F]{1,}$")
		<< QRegExp("^\\)\\sat\\s.*:\\d+$")

		/*
		^done,reason="generic information (not parsing)",value="#0  main (argc=Cannot access memory at address 0x0"  id : -1
		^done,reason="generic information (not parsing)",value=") at src/main.cpp:20"  id : -1
		*/
		<< QRegExp("^#\\d+\\s.*Cannot access memory at address\\s0x[0-9a-FA-F]{1,}$")
		<< QRegExp("^\\)\\sat\\s.*:\\d+$");

		
}
//
bool GdbParser::gotoSection(QString section)
{
	// goto section
	file.seek(0);
	QByteArray line = file.readLine().simplified();
	while(!file.atEnd ())
	{
		if(line == section) return true ;
		line = file.readLine().simplified();
	}
	return false;
}
//
int GdbParser::getId(QString line)
{
// [10000]^Using host libthread_db library .*\.

	int index = line.indexOf("]");
	line.remove(0,1);
	return line.left(index-1).toInt();
}
//
QString GdbParser::getRexp(QString line)
{
// [10000]^Using host libthread_db library .*\.

	int index = line.indexOf("]");
	return line.right(line.length() - index -1);
}
//
bool GdbParser::loadingPattern(QString fileName)
{
int i=0;
	file.setFileName(fileName);
	if(file.open(QIODevice::ReadOnly))
	{
		QByteArray line;
		QRegExp exp;
		if(gotoSection("[know_errors_list_gdb]"))
		{
			line = file.readLine();
			while(line.simplified() != "[/know_errors_list_gdb]" && !file.atEnd ())
			{
				if(!line.isEmpty() && !line.startsWith("//")) 
				{
					int id = getId(line);
					QString rexp = getRexp(line);


					exp.setPattern(rexp);

	if(list.pattern.contains(exp)) i++;// qDebug("skipping regExp : " + rexp.toLocal8Bit());
	else
	{
//qDebug("id " + QByteArray::number(id));
//qDebug("string " + rexp.toLocal8Bit());
					list.pattern << exp;
					list.id << id;
	}
			}
				line = file.readLine();
			}
		}
qDebug("total skipping : " + QByteArray::number(i));
		file.close();
		return 0;
	}
	else return 1;
}

// generique regExp
int GdbParser::check(QString error)
{
	for(int i =0; i< list.pattern.count(); i++)
	{
		if(list.pattern.at(i).exactMatch(error+"\n")) 
		{
			return list.id.at(i);
		}
	}
	return -1;
}
//
/*
		Interpreter
*/
int GdbParser::checkCmd(QString cmd)
{
	for(int i=0; i< interpreter.cmd.count(); i++)
		if(interpreter.cmd.at(i).exactMatch(cmd)) return i;
	return -1; 
}
//
int GdbParser::checkInterpreter(QString oneLine)
{
	int t = checkCmd(currentCmd);
	//qDebug(oneLine.toLocal8Bit());
	if(t !=-1 && interpreter.regexp.at(t).exactMatch(oneLine) )
	{
		//qDebug("match ok");
		oneLine = interpreter.answer.at(t) + oneLine + "\",currentCmd=\"" + currentCmd +"\"";
		onInfo(interpreter.id.at(t), oneLine);

// if interpreter wait prompt event, i don't have promt line after, i emulate prompt sequence
//^info,interpreter="GdbBreakPoint",event="breakpoint-deleted",answerExtention={fullname="main.cpp",nobkpt="1"},answerGdb="(gdb) ",currentCmd="delete 1"  id : 2
//^done,interpreter="GdbParser",currentCmd="file ./Debugger",event="prompt",answerGdb="(gdb) "  id : 0
		if(oneLine.contains( "(gdb) "))
			onDone(0,"emulate prompt");

		return 1;
	}
	//qDebug("no match");
	return 0;
}
//
int GdbParser::nextId()
{
	for(int i=PROMPT_ID + 1; i<INFO_ID -1 ; i++) //id 
		if(!interpreter.id.contains(i)) return i;
	return -1;
}
//
int GdbParser::addInterpreter(QString name,QString cGdb, QRegExp cmd, QRegExp reg, QString answer)
{
// Interpreter ID start
int index;
//qDebug("add interpreter " );

	if( !interpreter.cmd.contains(cmd))
	{
//		index++;
		interpreter.name << name;	

//		interpreter.cmdGdb << cGdb;

		interpreter.cmd << cmd;
		interpreter.regexp << reg;
		interpreter.answer << answer;
		index = nextId();
		interpreter.id << index;
		//qDebug("return id : " + QByteArray::number(index) + " name : " + name.toLocal8Bit());
		return index;
	}
	else qDebug( "(Class GdbParser function addInterpreter) WARNNING : Interpreter " + name.toLocal8Bit() + " is already exist !");
	return -1;
}
//
int GdbParser::removeInterpreter(int id)
{
//	if(id != -1)
	{
		for(int i =0; i< interpreter.cmd.count(); i++)
		{ 
			if( id == interpreter.id.at(i))
			{
				//qDebug("remove interpreter id : " + QByteArray::number( interpreter.id.at(i)) + " name : " + interpreter.name.at(i).toLocal8Bit());
				interpreter.name.removeAt(i);	
				interpreter.cmd.removeAt(i);	
				interpreter.regexp.removeAt(i);	
				interpreter.answer.removeAt(i);	
				interpreter.id.removeAt(i);
				//	qDebug("ok");	
				return -1;
			}
		}
		qDebug("(Class GdbParser function removeInterpreter) WARNNING : Interpreter  not exist ! id : " + QByteArray::number(id));
	}
	return -1;
}
//
/*
	restoring a line
*/
bool GdbParser::checkRestoring(QString a, QString b)
{

	for(int i=0; i<restoringList.count() ;i+=2)
		if(restoringList.at(i).exactMatch(a)  && restoringList.at(i+1).exactMatch(b)) return true;

	return false;
}
//
void GdbParser::addRestorLine(QRegExp l1, QRegExp l2)
{
	restoringList << l1 << l2;
}
//
void GdbParser::restorLine(QStringList *list)
{
	for(int i=0; i<list->count()-1; i++)
	{
		QString a = list->at(i);
		QString b = list->at(i+1); 

		if(checkRestoring( a , b))
		{
			// line splited			
			list->replace(i, a+b);
			list->removeAt(i+1); // remove the next line NOT use (i+1) !

			qDebug( "(Class GdbParser function restorLine) WARNNING : restoring line \"" + QString(a+b).toLocal8Bit() + "\"");
			return ;
		}
	}

}
//
/*
	main parser
*/
bool GdbParser::checkEndParsing(const QString data)
{
	QList<QRegExp> endParsing;
	endParsing << QRegExp(".*\\(gdb\\).$") << QRegExp(".*Continuing\\."+crlf + "$") << QRegExp(".*\\(y or n\\).$");

	for(int i=0; i < endParsing.count(); i++)
		if(endParsing.at(i).exactMatch(data)) 	return true;

	return false;
}
//
bool GdbParser::processParsing(const QByteArray& storg)
{
	QByteArray st = storg;


	// windows
	// swap prompt (gdb)
	// because under windows readAll() read all data from stdout and after stderr
	// exemple :
	//	(gdb) undefined ......
	// after this
	//	undefined ....
	//	(gdb)
	#ifdef Q_OS_WIN
	QRegExp p(".*\\(gdb\\)\\s.+");
	if(p.exactMatch(st))
	{
//		QMessageBox::warning(NULL, "found", st, QMessageBox::Ok ); 
		st.remove(st.indexOf("(gdb) "),6);
			st += "(gdb) ";
//		QMessageBox::warning(NULL, "apres", st, QMessageBox::Ok ); 
	}
	// end windows
	#endif

	// append buffer
	gdbBuffer.append(st);
	
	// st string end with prompt gdb or other?
	// buffer completed
	if(checkEndParsing(gdbBuffer))
	{
		// switch to next command
		currentCmd = waitingCmd;

		QStringList lines = gdbBuffer.split(crlf);

		// if answer is splitted in two string
		restorLine(&lines);

		// read line by line
		for(int i=0; i<lines.count(); i++)
		{
			QString oneLine = lines.at(i);

				// find if this anwser is in the file RegExp
				int id = check(oneLine);

			// find if i have interpreter for this answer ?
			//if(!checkInterpreter(oneLine) || id !=-1)
			{
				// find if this anwser is in the file RegExp
//				int id = check(oneLine);

				// remove all " in the string
				// because getParametre() function error
				// exemple :
				//	^info, .... answerGdb="undefined var "i" in current context",.....
				// getParametre() return -> "undefined var "
				while(oneLine.contains("\""))
					oneLine.remove("\"");

				// more than ERROR_ID (all errors)
				if(id != -1 && id >= ERROR_ID  )
					onError(id, oneLine);
				else if(!checkInterpreter(oneLine))
				{
				// Id is in ^info to ^error   
				if(id != -1 && id > INFO_ID && id < ERROR_ID)
					onInfo(id,oneLine);
			
				// unknow answer or prompt found
				if(id == -1 || id == PROMPT_ID)
					onDone(id, oneLine);
				
				}
			}
		}
		gdbBuffer.clear();
		return true;
	}
	return false;
}
//
void GdbParser::setLastCommand(QString cmd)
{
	waitingCmd = cmd;
}
//
/*
	sub main parser
*/
void GdbParser::onDone(int id, QString st)
{
	switch(id)
	{
		case -1 : emit done(id, "^done,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"generic information (not parsing)\",answerGdb=\"" + st + "\"");break;
		case PROMPT_ID: emit done(id, "^done,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"prompt\",answerGdb=\"" + st + "\""); break;
	}
}
//
void GdbParser::onInfo(int id, QString st)
{
	switch(id)
	{
		case 10004 : // no symbol found
			emit targetNoLoaded(id,st);//tr( "Your target is not builded in debug option.")); 
		break;
		//[10005]^Reading symbols from .*done\.
		case 10005 : 	emit targetLoaded(id, "^info,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"target-loaded\",answerGdb=\"" + st + "\""); break;
		//[10016]^Kill the program being debugged\? \(y or n\) \[answered Y; input not from terminal\]
		case 10016 :
		//[10007]^Program exited normally\.
		case 10007 : 	emit targetExited(id, "^info,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"target-exited\",answerGdb=\"" + st + "\""); break;
		case 10009 :	emit targetStopped(id, "^info,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"target-stopped\",answerGdb=\"" + st + "\""); 
		emit info(id, "^info,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"breakpoint-hit\",answerGdb=\"" + st + "\"");
		break;

		case 10010 :
		case 10011 :
		case 10012 :
		case 10013 :
		case 10014 :	emit targetRunning(id, "^info,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"target-running\",answerGdb=\"" + st + "\""); break;

		default : if(id>=INFO_ID) emit info(id, "^info,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"info found (parsing)\",answerGdb=\"" + st + "\"");
			// Interpreter command
			else emit info(id,st);
	}
}
//
void GdbParser::onError(int id, QString st)
{
	switch(id)
	{
		case 21550 :
		case 23290 :
		case 22833 : 
			emit targetNoLoaded(id, st);

		default : 
			emit error(id, "^error,interpreter=\"GdbParser\",currentCmd=\"" + currentCmd +"\",event=\"error found\",answerGdb=\"" + st + "\"");
	}
}
