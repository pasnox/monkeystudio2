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
	\date 15/05/09
	\author Xiantia
	\version 1.4.0
	\brief Implements all functions for show values of cpu registers.This class is an AddOn for GNU debugger
*/
/*

	GdbBacktrace class


	Xiantia@gmail.com

	for Debugger v1.4.0
*/

#include "gdbRegister.h"
#include "./ui/UIGdbRegister.h"

#include <QMessageBox>
#include <QFileInfo>

/*!
	\details Create a new object
	\param parent of this object
*/
GdbRegister::GdbRegister(QObject * parent) : GdbAddonBase(parent)
{
	GdbCore::KernelDispatcher()->add(this);
	GdbCore::Interpreter()->remove(name());

	setEnabled(true);
	setWaitEndProcess(false);

	/*
	eax            0x1	1
	
	*/

	interpreterRegister = GdbCore::Interpreter()->add(
		name(),
		QRegExp("^info registers"),
		QRegExp("^\\w+\\s+0x\\w+\\s+.*"),
		"^done,interpreter=\"" + name() + "\",event=\"Register\",answerGdb=\"");

	// connect interpreter to function
	Connect.add(this, interpreterRegister, &GdbRegister::onRegister);



	Sequencer = new GdbSequencer(this);
	QList<SequencerCmd> s = QList<SequencerCmd>() << SequencerCmd("inforegister", "info registers")  ; 
	Sequencer->add(name() , s);

	mWidget = new UIGdbRegister(qobject_cast<QWidget*>(this));
    mWidget->treeWidget->setAlternatingRowColors(true);

	numRegister = 0;
}

//
/*!
	\details None.
*/
GdbRegister::~GdbRegister()
{

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

void GdbRegister::targetRunning(const int & , const QString & )
{}

//

void GdbRegister::targetStopped(const int & , const QString & )
{
	setWaitEndProcess(true);
	Sequencer->start();
}

//

void GdbRegister::targetExited(const int & , const QString & )
{
	mWidget->treeWidget->clear();
}

// Parser status

void GdbRegister::done(const int &, const QString &){}

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

		setSharedData(l.at(1),l.at(3));

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

