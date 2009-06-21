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
	\file gdbBreakpoint.cpp
	\date 15/05/09
	\author Xiantia
	\version 1.4.0
	\brief Implements all functions for remote breakpoints.This class is an AddOn for GNU debugger
*/
/*

		GdbBreakpoint class


	Xiantia@gmail.com

	for Debugger v1.4.0
*/

#include "gdbBreakpoint.h"
#include "./ui/UIGdbBreakpoint.h"

#include <QFileInfo>
#include <QTextCodec>
#include <QtDebug>

/*!
	\details Set the breakpoint list.
	Form : pathOfFile:lineNumber
*/
void GdbBreakpoint::restoreBreakpoint(QStringList l)
{
	bThread->restoreBreakpoint(l);
}


/*!
	\details Create a new object, some interpreters and connect it to the functions
*/
GdbBreakpoint::GdbBreakpoint(QObject * parent) : GdbAddonBase(parent), mForce(false)
{

	GdbCore::KernelDispatcher()->add(this);
	GdbCore::Interpreter()->remove(name());

	setEnabled(true);
	setWaitEndProcess(false);


	/*
		create new parser : For add breakpoint

		cRegExpCmd = "b main.cpp:12"
		aRegExp = "Breakpoint 1 at 0x437bdf: file src/addon/gdbBackTrace.cpp, line 15."
		or
		aRegExp = "Breakpoint 2 at 0x437d2b: file src/addon/gdbBackTrace.cpp, line 22. (2 locations)"
	*/

	interpreterAddBreakpoint = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^b\\s.*:\\d+$"),
		QRegExp("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))"),
		"");

	// connect interpreter to function
	Connect.add(this, interpreterAddBreakpoint, &GdbBreakpoint::onBreakpointAdd);


	/*
		create new parser : For delete breakpoint

		cRegExpCmd = "delete 1"
		aRegExp = "(gdb) "
	*/
	interpreterDelBreakpoint = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^delete\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect.add(this, interpreterDelBreakpoint, &GdbBreakpoint::onBreakpointDelete);

	/*
		create new parser : For enable breakpoint

		cRegExpCmd = "enable 1"
		aRegExp = "(gdb) "
	*/
	interpreterEnabledBreakpoint = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^enable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect.add(this, interpreterEnabledBreakpoint, &GdbBreakpoint::onBreakpointEnabled);

	/*
		create new parser : For disable breakpoint

		cRegExpCmd = "disable 1"
		aRegExp = "(gdb) "
	*/
	interpreterDisabledBreakpoint = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^disable\\s\\d+"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect.add(this, interpreterDisabledBreakpoint, &GdbBreakpoint::onBreakpointDisabled);

	/*
		create new parser : For breakpoint pending

		cRegExpCmd = "disable 1"
		aRegExp = "(gdb) "
	*/
	interpreterBreakpointPending = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^b\\s.*:\\d+$"),
		QRegExp("^Breakpoint\\s(\\d+)\\s\\(\"(.*)\":(\\d+)\\)\\spending\\.$"),
		"^done,className=\"" + name() + "\",event=\"Breakpoint-Add-Pending\",answerGdb=\"");

	Connect.add(this, interpreterBreakpointPending, &GdbBreakpoint::onBreakpointPending);

	/*
		create new parser : For breakpoint Condition

		cRegExpCmd = "condition 1 i==2"
		aRegExp = "(gdb) "
	*/
	interpreterConditionnedBreakpoint = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^condition\\s\\d+.+$"),
		QRegExp("^\\(gdb\\)\\s"),
		"");

	Connect.add(this, interpreterConditionnedBreakpoint, &GdbBreakpoint::onBreakpointConditionned);

	/*
		create new parser : For breakpoint unCondition

		cRegExpCmd = "condition 1"
		aRegExp = "Breakpoint 1 now unconditional."
	*/
	interpreterUnConditionnedBreakpoint = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^condition\\s\\d+$"),
		QRegExp("^Breakpoint\\s\\d+\\snow\\sunconditional.$"),
		"");

	Connect.add(this, interpreterUnConditionnedBreakpoint, &GdbBreakpoint::onBreakpointUnConditionned);

	//Ui
	mWidget = new UIGdbBreakpoint(qobject_cast<QWidget*>(this));
	connect( mWidget, SIGNAL(enabledBreakpoint(const QString &, const int &, const bool &)), this, SLOT(onToggleEnabledBreakpoint(const QString &, const int &, const bool &)));
	connect( mWidget, SIGNAL(conditionnedBreakpoint(const QString &,const  int &, const QString &)), this, SLOT(onToggleConditionnedBreakpoint(const QString &, const  int &, const QString &)));
	connect( mWidget, SIGNAL(gotoBreakpoint(QString, int)), SIGNAL(gotoBreakpoint(QString, int)));

	// for restore breakpoint when GnuGdb is re-started
	bThread = new GdbBreakpointThread(this);
}

//

/*!
	\details Remove all breakpoint in lists and delete widget container
*/
GdbBreakpoint::~GdbBreakpoint()
{
	emit removeAllBreakpoints();
	clearAllBreakpoints();
}

//

QString GdbBreakpoint::name()
{
	return "GdbBreakpoint";
}

//

QPointer<QWidget> GdbBreakpoint::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

//

QIcon GdbBreakpoint::icon()
{
	return QIcon(":/icons/breakpoint.png");
}

//

void GdbBreakpoint::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call( i, id, s);
}

//

/*!
	\details Find if file has already breakpoint at this line
	\param fileName is the name of file
	\param line is line that you find.
	\return A pointer to this breakpoint or NULL
*/
QHash<QString, QString> * GdbBreakpoint::findBreakpointByLine(QString fileName, int line)
{
	foreach(BREAKPOINT * hash, B)
		if((*hash)["fileName"] == fileName && (*hash)["line"].toInt() == line) return hash;
	return NULL;
}

//

/*!
	\details Find if file has already breakpoint with this index (index is returned by Gdb) 
	\param fileName is the name of file
	\param index is the index that you find.
	\return A pointer to this breakpoint or NULL
*/
QHash<QString, QString> * GdbBreakpoint::findBreakpointByIndex(QString fileName, int index)
{
Q_UNUSED(fileName);
	foreach(BREAKPOINT * hash, B)
		if(/*(*hash)["fileName"] == fileName && */(*hash)["index"].toInt() == index) return hash;
	return NULL;
}

//

/*!
	\details Remove all breakpoints for this file
*/
void GdbBreakpoint::clearBreakpoints(QString fileName)
{
	for(int i=0; i<B.count(); i++)
	{
		BREAKPOINT * hash = B.at(i); 
		if( (*hash)["fileName"] == fileName) { B.removeAt(i); delete hash;}
		i--;
	}
}

//

/*!
	\details Remove all breakpoints (clear list)
*/
void GdbBreakpoint::clearAllBreakpoints()
{
	for(int i=0; i<B.count(); i++)
	{
		BREAKPOINT * hash = B.at(i); 
		delete hash;
	}
	B.clear();
}

// Gdb status

void GdbBreakpoint::gdbStarted()
{
	clearAllBreakpoints();
	desableBreakpointHit();
	mWidget->upDateData(B);
	setWaitEndProcess(false);
}

//

void GdbBreakpoint::gdbFinished()
{
	mBreakpointAtOpened.clear();
	
	// save breakpoint, create list
	foreach(BREAKPOINT * hash, B)
	      mBreakpointAtOpened << (*hash)["fileName"] + ":" + (*hash)["line"] ;
	
	// save current breakpoint
	bThread->restoreBreakpoint(mBreakpointAtOpened);
	// save 
	emit saveBreakpoint(mBreakpointAtOpened);

	clearAllBreakpoints();
	desableBreakpointHit();
	mWidget->upDateData(B);

	emit removeAllBreakpoints();
	setWaitEndProcess(false);
}

//

void GdbBreakpoint::gdbError()
{
	emit removeAllBreakpoints();
}

// Target status

void GdbBreakpoint::targetLoaded(const int &, const QString &)
{
	// start restoring breakpoint on re-open project
	bThread->start();
}

//

void GdbBreakpoint::targetNoLoaded(const int &, const QString &)
{
//	none
}

//

void GdbBreakpoint::targetRunning(const int & , const QString & )
{
	desableBreakpointHit();
	mWidget->upDateData(B);
}

//

void GdbBreakpoint::targetStopped(const int & , const QString & s)
{
	/*
		reason : breakpoint-hit
		reason : end-stepping-range (step over or step into)
		Breakpoint 1, main (argc=1, argv=0xbfe6fc74) at src/main.cpp:21
		Breakpoint 1, main (argc=604801882, argv=0x8b240489) at src/main.cpp:13
	*/

	QString answer = findValue(s, "answerGdb");
	QRegExp r("^Breakpoint\\s+(\\d+),\\s*.*at\\s+(.*):(\\d+)");

	desableBreakpointHit();

	if(r.exactMatch(answer))
	{
		QStringList l = r.capturedTexts();
		breakpointMoved(l.at(2), l.at(3).toInt(), l.at(1).toInt());
	}
	mWidget->upDateData(B);
}

//

void GdbBreakpoint::targetExited(const int & , const QString & )
{
	desableBreakpointHit();
	mWidget->upDateData(B);
}


// Parser status


void GdbBreakpoint::done(const int & id, const QString & st)
{
	if(id>0) mRawData += findValue(st,"answerGdb");
}

void GdbBreakpoint::prompt(const int &, const QString & )
{
	if(isWaitEndProcess())
	{
		// possible error
		emit errorMessage(tr("Error : ") + mRawData, 5000 );
//		GdbAddonBase::showMessage( tr("Error : ") + mRawData, 5000 );
	}
	mRawData.clear();
	setWaitEndProcess(false);
	// restore next breakpoint if has
	if(bThread->isRunning())
		bThread->mStop = false;
}

// Interpreters

/*!
	\details Hide "hit" icon in UIGdbBreakpoint. 
*/
void GdbBreakpoint::desableBreakpointHit()
{
	foreach(BREAKPOINT *hash , B)
		(*hash)["hit"] = "0";
}

/*!
	\details Calling when a breakpoint has moved
	Some time Gdb move breakpoint to the next line. Modify Breakpoint struct for store this new index.
	\param fileName is the file name that the breakpoint has moved.
	\param line is the line number that the breakpoint has moved.
	\param index is a index from Gdb.
*/
void GdbBreakpoint::breakpointMoved(const QString & fileName, const int & line, const int & index)
{
	BREAKPOINT * hash1 = findBreakpointByIndex(fileName, index);

	if(hash1 && (*hash1)["line"].toInt() != line ) // then current line has changed
	{
		BREAKPOINT * hash2 = findBreakpointByLine((*hash1)["fileName"],line);
		if(hash2) // un breakpoint est deja present
		{
			  mForce = true;(*hash2)["index"] = (*hash1)["index"];
			  onToggleBreakpoint((*hash1)["fileName"], (*hash1)["line"].toInt() -1);
			  mForce = false;hash1 = hash2;
		}
		else
		{// move breakpoint to the new line
			 emit toggleBreakpoint(*hash1, false);
			 (*hash1)["line"] = QString::number(line);
			 emit toggleBreakpoint(*hash1, true);
		}
	}

	if(hash1 && (*hash1)["index"].toInt() == index)
	{
		(*hash1)["hit"] = "1";
		if((*hash1)["pending"] == "1") 
		{
			(*hash1)["pending"] = "0"; 
			emit toggleBreakpoint(*hash1, true);
		}
	}
}

// ================= Add /Del Breakpoint ===========================

/*!
	\details Calling when user click in the margin
	This function send "delete numBreak" or "b fileName:line" to Gdb. 
	If the answer from Gdb is correct, an interpreter switch to the correct function 
	onBreakpointDelete() or onBreakpointAdd().
	\param fileName is the name of file.
	\param line is the line number.
*/
void GdbBreakpoint::onToggleBreakpoint(const QString & fileName, const int & line)
{
	// fix 1.3.2 not send data to gdb if it not started or if target running
	if(GdbCore::Controler()->isGdbStarted() )
	{

		if(!GdbCore::Controler()->isTargetStopped() && !mForce)
		{
			emit defaultMessage(tr("I can't toggle breakpoint when target running."));
//			GdbAddonBase::setMessage(tr("I can't toggle breakpoint when target running."));
			return;
		}
	

		if(isWaitEndProcess()) // breakpoint is already build, wait answer from gdb
			return;


		if( !QFile::exists ( fileName )) 
		{
			emit errorMessage(tr("I can't restore breakpoint because this file not exist ") + fileName, 5000);
			prompt(0,""); // force prompt fixed 1.4.0, else the nest breakpoint is not set (bloqued at this breakpoint)
			return;
		} 

		BREAKPOINT *hash = findBreakpointByLine(fileName, line+1);
		if(hash) // breakpoint found
		{
			GdbCore::Parser()->setNextCommand(name(), "delete " + (*hash)["index"]);
			GdbCore::Interpreter()->changeAnswer(interpreterDelBreakpoint, 
			    "^done,className=\"" + name() + "\",event=\"Breakpoint-Delete\",fileName=\""+ fileName +"\",line=\""+ (*hash)["line"] +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("delete " +  (*hash)["index"]);
			setWaitEndProcess(true);
		}
		else
		{
			GdbCore::Parser()->setNextCommand(name(), "b " + fileName + ":" + QString::number(line+1));
			GdbCore::Interpreter()->changeAnswer(interpreterAddBreakpoint, 
				"^done,className=\"" + name() + "\",event=\"Breakpoint-Add\",fileName=\""+ fileName +"\",line=\""+ QString::number(line+1) +"\",answerGdb=\"");
			GdbCore::Process()->sendRawData("b \"" + fileName + "\":" + QString::number(line+1));
			setWaitEndProcess(true);
		}
	}
}

//

/*!
	\details Add breakpoint, calling when "b main.cpp:23" command is correctly executed. 
	\param s is the string from GdbParser class.
*/
	
void GdbBreakpoint::onBreakpointAdd( int , QString s)
{
	QString n = findValue(s,"fileName");

	if(!n.isEmpty())
	{
		QRegExp r = interpreterAddBreakpoint->getAnswerRegExp();
		if(r.exactMatch(findValue(s, "answerGdb")))
		{
			QString pending = findValue(s,"pending");
			QStringList l = r.capturedTexts();
	
			BREAKPOINT *hash = new BREAKPOINT;
			(*hash)["fileName"] = n;
			(*hash)["hit"] = "0";
			(*hash)["line"] = findValue(s,"line");
			(*hash)["index"] = l.at(1);
			(*hash)["type"] = "normalBreak";
			(*hash)["enable"] = "1";
			(*hash)["condition"] = tr("none");
			(*hash)["pending"] = pending.isEmpty() ? "0" : "1";

			B << hash;

			setWaitEndProcess(false);
			emit  toggleBreakpoint(*hash,true);

		}
		mWidget->upDateData(B);
	}
}

//

/*!
	\details Delete breakpoint, calling when "delete numBreak" command is correctly executed.
	\param s is the string from GdbParser class.
*/
void GdbBreakpoint::onBreakpointDelete( int , QString s)
{
	QString n = findValue(s,"fileName");
	if( !n.isEmpty())
	{
		BREAKPOINT * hash = findBreakpointByLine(n, findValue(s,"line").toInt());
		if(hash)
		{
			emit toggleBreakpoint(*hash,false);
			B.removeOne(hash);
			delete hash;
			setWaitEndProcess(false);
		}
		else
			emit errorMessage(tr("Critical erreur : Repport this bug please : Delete breakpoint but no have at this line !") , 0);

		mWidget->upDateData(B);
	}
}

//

/*!
	\details Calling when a new file is opened.
	Find if this file have some breakpoints. In this case, this function emit onToggleBreakpoint()
	signal.
	\param fileName is the name of file.
	\sa GdbBridgeEditor::requestBreakpoint()
*/
void GdbBreakpoint::onRequestBreakpoint(const QString & fileName)
{
	foreach(BREAKPOINT *hash , B)
		if((*hash)["fileName"] == fileName) emit toggleBreakpoint(*hash, true);
}

//=================== Enable / desable breakpoint =====================

/*!
	\details Calling when user want enable or disable breakpoint from UIGdbBreakpoint.
	 This function send "enable numBreak" or "disable numBreak" to Gdb. If the answer from Gdb is correct, 
	 an interpreter switch to the correct function.	 onBreakpointEnabled() or onBreakpointDisabled().
	\param fileName is the name of file.
	\param index is the index in Gdb
	\param b is true if you want enable breakpoint else b is false.
*/
void GdbBreakpoint::onToggleEnabledBreakpoint(const QString & fileName, const int & index, const bool &b)
{
	if(!GdbCore::Controler()->isTargetStopped())
	{
		emit defaultMessage(tr("I can't Enable / disable breakpoint when target running."));
//		GdbAddonBase::setMessage(tr("I can't Enable / disable breakpoint when target running."));
		return;
	}

	if(isWaitEndProcess())
		return;
	
	if(b)
	{
		GdbCore::Interpreter()->changeAnswer(interpreterEnabledBreakpoint, 
			"^done,className=\"" + name() + "\",event=\"Breakpoint-Enabled\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",enabled=\"true\",answerGdb=\"");
		GdbCore::Parser()->setNextCommand(name(), "enable " + QString::number(index));
		GdbCore::Process()->sendRawData("enable " + QString::number(index));
		setWaitEndProcess(true);
	}
	else
	{
		GdbCore::Interpreter()->changeAnswer(interpreterDisabledBreakpoint, 
			"^done,className=\"" + name() + "\",event=\"Breakpoint-Disable\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",enabled=\"false\",answerGdb=\"");
		GdbCore::Parser()->setNextCommand(name() ,"disable " + QString::number(index));
		GdbCore::Process()->sendRawData("disable " + QString::number(index));
		setWaitEndProcess(true);
	}
}

//
/*!
	\details Calling when Gdb has executed correctly the last command "enable numBreak".
*/
void GdbBreakpoint::onBreakpointEnabled(int, QString s)
{
	QString n = findValue(s,"fileName");
	
	BREAKPOINT *hash = findBreakpointByIndex(n, findValue(s,"index").toInt());
	if(hash)
	{
	      (*hash)["enable"] = "1";
	      emit  toggleBreakpoint(*hash, true );
	      setWaitEndProcess(false);
	}
}

//
/*!
	\details Calling when Gdb has executed correctly the last command "Disable numBreak".
*/
void GdbBreakpoint::onBreakpointDisabled(int, QString s)
{
	QString n = findValue(s,"fileName");
	
	BREAKPOINT * hash = findBreakpointByIndex(n, findValue(s,"index").toInt());
	if(hash)
	{
	      (*hash)["enable"] = "0";
	      emit  toggleBreakpoint(*hash, true );
	      setWaitEndProcess(false);
	}
}

// ================================ Breakpoint pending =================

/*!
	\details Set breakpoint, but the current lib is not loaded by Gdb. this function is same as 
	onBreakpointAdd() function.
	\param s is the string from GdbParser class.
*/
void GdbBreakpoint::onBreakpointPending(int, QString s)
{
	QString n = findValue(s,"answerGdb");

	if( !n.isEmpty() )
	{
		//("^Breakpoint\\s+(\\d+)\\s+at\\s(\\w+):\\s+file\\s+([^,]+),\\s+line\\s+(\\d+)\\.(|\\s+\\(\\d+\\s\\w*\\))");
		QRegExp r = interpreterBreakpointPending->getAnswerRegExp();
		if(r.exactMatch(n))
		{
			QStringList l = r.capturedTexts();
			onBreakpointAdd(0,"fileName=\"" + l.at(2) + "\",line=\"" + l.at(3) + "\",pending=\"1\",answerGdb=\"Breakpoint " + l.at(1) + " at pending: file " + l.at(2) + ", line " + l.at(3) + ".");
		}
	}
}

// =============================== Breakpoint Conditionned ================

/*!
	\details Calling when user want contionned or not a breakpoint from UIGdbBreakpoint
	This function send "condition numBreak condition" or "condition numBreak" to Gdb. 
	If the answer from Gdb is correct, an interpreter switch to the correct function
	 onBreakpointUnConditionned() or onBreakpointConditionned().
	\param fileName is the name of file.
	\param index is the index in Gdb
	\param condition is the condition that you want set.
*/
void GdbBreakpoint::onToggleConditionnedBreakpoint(const QString & fileName, const int & index, const QString & condition)
{
	if(!GdbCore::Controler()->isTargetStopped())
	{
		emit defaultMessage(tr("I can't set condition when target running."));
//		GdbAddonBase::setMessage(tr("I can't set condition when target running."));
		return;
	}
	
	
	if(isWaitEndProcess())
		return;

	GdbCore::Interpreter()->changeAnswer(interpreterConditionnedBreakpoint, 
		"^done,className=\"" + name() + "\",event=\"Breakpoint-Conditionned\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",condition=\"" + condition + "\",answerGdb=\"");
	GdbCore::Interpreter()->changeAnswer(interpreterUnConditionnedBreakpoint, 
		"^done,className=\"" + name() + "\",event=\"Breakpoint-unConditionned\",fileName=\""+ fileName +"\",index=\""+ QString::number(index) +"\",condition=\"\",answerGdb=\"");

	if(condition.isEmpty() || condition == tr("none"))
	{
		GdbCore::Parser()->setNextCommand(name() ,"condition " + QString::number(index));
		GdbCore::Process()->sendRawData("condition " + QString::number(index));
	}
	else
	{
		GdbCore::Parser()->setNextCommand(name() ,"condition " + QString::number(index) + " " + condition);
		GdbCore::Process()->sendRawData("condition " + QString::number(index) + " " + condition);
	}
	setWaitEndProcess(true);
}

//

/*!
	\details Condition or not a breakpoint, calling when "condition numBreak condition" command 
	is correctly executed.
	\param s is the string from GdbParser class.
*/
void GdbBreakpoint::onBreakpointConditionned(int, QString s)
{
	QString n = findValue(s,"fileName");
	
	BREAKPOINT * hash = findBreakpointByIndex(n, findValue(s,"index").toInt());
	if(hash)
	{
	      QString c = findValue(s,"condition");
	      (*hash)["condition"] = c.isEmpty() ? tr("none") : c;
	      (*hash)["type"] = "conditionnedBreak";
	      emit  toggleBreakpoint(*hash, true );
	      setWaitEndProcess(false);
	      mWidget->upDateData(B);
	}
}

//

/*!
	\details Condition or not a breakpoint, calling when "condition numBreak" command is correctly executed.
	\param s is the string from GdbParser class.
*/
void GdbBreakpoint::onBreakpointUnConditionned(int, QString s)
{
	QString n = findValue(s,"fileName");
	
	BREAKPOINT * hash = findBreakpointByIndex(n, findValue(s,"index").toInt());
	if(hash)
	{
	      QString c = findValue(s,"condition");
	      (*hash)["condition"] = c.isEmpty() ? tr("none") : c;
	      (*hash)["type"] = "normalBreak";
	      emit  toggleBreakpoint(*hash, true );
	      setWaitEndProcess(false);
	      mWidget->upDateData(B);
	}
}
