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
	\file gdbRegister.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Implements all functions for show values of cpu registers.This class is an AddOn for GNU debugger
*/
/*

	GdbBacktrace class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/

#include "gdbRegister.h"
#include <QMessageBox>
#include <QFileInfo>

/*!
	\details Create a new object
	\param parent of this object
*/
GdbRegister::GdbRegister(QObject * parent) : GdbCore(parent)
{
	setEnabled(true);
	setWaitEndProcess(false);

	/*
	eax            0x1	1
	
	*/

	interpreterRegister = Parser()->addInterpreter(
		name(),
		QRegExp("^info registers"),
		QRegExp("^\\w+\\s+0x\\w+\\s+.*"),
		"^info,interpreter=\"" + name() + "\",event=\"Register\",answerGdb=\"");

	// connect interpreter to function
	Connect.add(this, interpreterRegister, &GdbRegister::onRegister);



	Sequencer = new GdbSequencer(this);
	QList<SequencerCmd> s = QList<SequencerCmd>() << SequencerCmd("inforegister", "info registers")  ; 
	Sequencer->add(name() , s);

	mWidget = UIGdbRegister::self(0);
    mWidget->treeWidget->setAlternatingRowColors(true);

	numRegister = 0;
}

//
/*!
	\details None.
*/
GdbRegister::~GdbRegister()
{
	delete mWidget;
}

//

QString GdbRegister::name()
{
	return "GdbRegister";
}

//

QPointer<QWidget> GdbRegister::widget()
{
	return (QPointer<QWidget>)( mWidget );
}

//

QIcon GdbRegister::icon()
{
	return QIcon(":/icons/cpu.png");
}


void GdbRegister::interpreter(const QPointer<BaseInterpreter> & i, const int & id, const QString & s)
{
	Connect.call( i, id, s);
}

// Gdb status

void GdbRegister::gdbStarted()
{
	setWaitEndProcess(false);
	mWidget->treeWidget->clear();
}

//

void GdbRegister::gdbFinished()
{
	setWaitEndProcess(false);
	mWidget->treeWidget->clear();
}

//

void GdbRegister::gdbError(){}

// Taget status

void GdbRegister::targetLoaded(const int &, const QString &){}
void GdbRegister::targetNoLoaded(const int &, const QString &){}

//

void GdbRegister::targetRunning(const int & id, const QString & s)
{
}

//

void GdbRegister::targetStopped(const int & id, const QString & s)
{
	setWaitEndProcess(true);
	Sequencer->start();
}

//

void GdbRegister::targetExited(const int & id, const QString & s)
{
	mWidget->treeWidget->clear();
}

// Parser status

void GdbRegister::error(const int &, const QString & s)
{
	showMessage(name() + " have generate error : " + s, 5000, _CRITICAL_);
	setWaitEndProcess(false);
}

void GdbRegister::done(const int &, const QString &){}
void GdbRegister::info(const int &, const QString &){}

void GdbRegister::prompt(const int &, const QString &)
{
	numRegister = 0;
	setWaitEndProcess(false);
}

// Interpreters

/*!
	\details Calling when GdbParser class found correct interpreter (in this case interpreterRegister)
	\param s is the string from GdbParser class.
*/
void GdbRegister::onRegister(int , QString s)
{

	/*
		#0  qMain (argc=1, argv=0x3d4c20) at src/main.cpp:65
	*/

	QRegExp r("^(\\w+)\\s+(0x\\w+)\\s+(.*)$");

	if(r.exactMatch(findValue(s,"answerGdb")))
	{
		QStringList l = r.capturedTexts();
		
		QTreeWidgetItem * i;

		if(numRegister < mWidget->treeWidget->topLevelItemCount()) 
		{
			i = mWidget->treeWidget->topLevelItem ( numRegister);
			showColor(i, l.at(2));

		}
		else i = new QTreeWidgetItem(mWidget->treeWidget);

		i->setText(0,l.at(1));
		i->setText(1,l.at(2));
		i->setText(2,l.at(3));
		numRegister++;
	}
}

//

/*!
	\details Toogle color if new value of register has changed.
	\param p is the QTreeWidgetItem pointer
	\param a is the new string value.
*/
void GdbRegister::showColor(QTreeWidgetItem *p, QString a)
{
	// toggle color (black / red if value in treeWidget is not egal than new value
	if(p->text(1) != a)
	{
		p->setForeground( 1, QBrush(Qt::red));
		p->setForeground( 2, QBrush(Qt::red));
	}
	else
	{
		p->setForeground( 1 , QBrush(Qt::black));
		p->setForeground( 2 , QBrush(Qt::black));
	}
}

