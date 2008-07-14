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
	\file gdbBacktrace.cpp 
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Implements all functions for parse backtrace commands.This class is an AddOn for GNU debugger
*/

/*

	GdbBacktrace class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#include "gdbBacktrace.h"
#include <QMessageBox>
#include <QFileInfo>

/*!
* \details Create a new object
*/
GdbBacktrace::GdbBacktrace(QObject * parent) : GdbCore(parent)
{
	setEnabled(true);
	setWaitEndProcess(false);

	mCurrentLine = -1;
	mCurrentFile.clear();
	
	/*
		Backtrace line splitted in two lines
			"#0  UIForm (this=0x80dc268, parent=0x0) "
			"  at src/ui/UIForm.cpp:43"
	*/
	/*
		#0 main (arc, arg) at src/main.cpp:23
	  
		#0  QObject::connect (sender=<incomplete type>, 
		signal=0x48e278 "2toggleBreakpoint(QString, int, bool)", 
		receiver=<incomplete type>, 
		method=0x48e250 "1onToggleBreakpoint(QString, int, bool)", 
		type=AutoConnection) at kernel/qobject.cpp:2348
	*/

	GdbCore::Parser()->addRestoreLine(name(),
		"^#\\d+\\s.*" , 
		".*\\sat\\s.*:\\d+$");


	/*
		create new parser :

		cRegExpCmd = "bt"
		aRegExp = 
	*/

/*
#0  0x00400085 in ?? ()
*/	
	interpreterBacktrace = Parser()->addInterpreter(
		name(),
		QRegExp("^bt"),
		QRegExp("^#\\d+\\s.*\\sat\\s.*:\\d+"),
		"^info,interpreter=\"" + name() + "\",event=\"Backtrace\",answerGdb=\"");

	// connect interpreter to function
	Connect.add(this, interpreterBacktrace, &GdbBacktrace::onBacktrace);


	/*
		create new parser :

		cRegExpCmd = "info source"
		aRegExp = "(gdb) "
	*/
	interpreterInfoSource = Parser()->addInterpreter(
		name(),
		QRegExp("^info source"),
		QRegExp("^Located\\sin\\s.*"),
		"^info,interpreter=\"" + name() + "\",event=\"Info-source\",answerGdb=\"");

	Connect.add(this, interpreterInfoSource, &GdbBacktrace::onInfoSource);

	Sequencer = new GdbSequencer(this);
	QList<SequencerCmd> s = QList<SequencerCmd>() << SequencerCmd("Backtrace", "bt") << SequencerCmd("Infosource", "info source") ; 
	Sequencer->add( name() , s);

	mWidget = UIGdbBacktrace::self(0);
    mWidget->treeWidget->setAlternatingRowColors(true);

	numBacktrace=0;
}

//
/*!
* \details Delete mWidget
*/
GdbBacktrace::~GdbBacktrace()
{
	delete mWidget;
}

QString GdbBacktrace::name()
{
	return "GdbBacktrace";
}

QPointer<QWidget> GdbBacktrace::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

QIcon GdbBacktrace::icon()
{
	return QIcon(":/icons/backtrace.png");
}

/*!
* \details This function switches interpreterBacktrace attached to onBacktrace function or
* interpreterInfosource attached to onInfoSource function.
* \param i is a pointer to BaseInterpreter struct.
* \param id is an Id od string.
* \param s is a string. 
*/
void GdbBacktrace::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call( i, id, s);
}

// Gdb status

void GdbBacktrace::gdbStarted()
{
	setWaitEndProcess(false);
	mCurrentLine = -1;
	mCurrentFile.clear();
	mWidget->treeWidget->clear();
}

//

void GdbBacktrace::gdbFinished()
{
	setWaitEndProcess(false);
	mCurrentLine = -1;
	mCurrentFile.clear();
	mWidget->treeWidget->clear();
}

//

void GdbBacktrace::gdbError(){}

// Taget status

void GdbBacktrace::targetLoaded(const int &, const QString &){}
void GdbBacktrace::targetNoLoaded(const int &, const QString &){}

//

void GdbBacktrace::targetRunning(const int & id, const QString & s)
{
	// just delete bactrace when target running
	emit onToggleBacktrace("", -1);
}

//

void GdbBacktrace::targetStopped(const int & id, const QString & s)
{
	setWaitEndProcess(true);
	Sequencer->start();
}

//

void GdbBacktrace::targetExited(const int & id, const QString & s)
{
	// just delete bactrace when target exited
	emit onToggleBacktrace("", -1);
	mWidget->treeWidget->clear();
}

// Parser status

void GdbBacktrace::error(const int &, const QString & s)
{
	showMessage(name() + " have generate error : " + s, 5000, _CRITICAL_);
	setWaitEndProcess(false);
}

void GdbBacktrace::done(const int &, const QString &){}
void GdbBacktrace::info(const int &, const QString &){}

void GdbBacktrace::prompt(const int &, const QString & s)
{
	// promt from bactrace command
	if(Sequencer->currentCmd() == "Backtrace")
	{
		// clean up
		int f = mWidget->treeWidget->topLevelItemCount();

		for(int i=numBacktrace; i< f; i++)
			mWidget->treeWidget->takeTopLevelItem(i);

		numBacktrace=0;
	}
	setWaitEndProcess(false);
	
	Sequencer->loop();
}

// Interpreters

/*!
* \details Calling when interpreter() function found onBacktrace event attached with interpreterBacktrace
* \param s is a string from Gdb.
*/
void GdbBacktrace::onBacktrace(int , QString s)
{
	/*
		see all backtrace in Ui
	*/
	
	QRegExp rb("^#\\d+.*\\sat\\s[^:]+:\\d+$");

	if(rb.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = rb.capturedTexts();
		QTreeWidgetItem * i;

		if(numBacktrace < mWidget->treeWidget->topLevelItemCount()) 
			i = mWidget->treeWidget->topLevelItem (numBacktrace);
		else 
			i = new QTreeWidgetItem(mWidget->treeWidget);

		i->setText(0,findValue(s,"answerGdb"));
		numBacktrace++;
	}

	/*
		just extract current backtrace
		#0  qMain (argc=1, argv=0x3d4c20) at src/main.cpp:65
	*/

	QRegExp r("^#0.*\\sat\\s([^:]+):(\\d+)$");

	if(r.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = r.capturedTexts();
		mCurrentLine = l.at(2).toInt();
		mCurrentFile = l.at(1);
	}
}

/*!
* \details Calling when interpreter) function found onInfoSource event attached with interpreterInfosource
* \param s is a string from Gdb.
*/
void GdbBacktrace::onInfoSource(int , QString s)
{
	/*
		Info source receiver
	*/

	QRegExp r("^Located\\sin\\s(.*)" );

	if(r.exactMatch( findValue(s,"answerGdb") ))
	{
		//get the full path of file
		QStringList l = r.capturedTexts();
		mCurrentFile = l.at(1);
		emit onToggleBacktrace(mCurrentFile, mCurrentLine);
	}
}

/*!
* \details Calling when an editor is open. Restore backtrace icon 
* If mCurrentfile is equal in the name of the opened file, onToggleBacktrace signal is 
* emitted indicating that Gdb is stopped on this file. the editor can now show icon backtrace on the margin
* \param s is name of file that request backtrace.
*/
void GdbBacktrace::onRequestBacktrace(const QString & s)
{
	if(s == mCurrentFile)
		emit onToggleBacktrace(mCurrentFile, mCurrentLine);
}
