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
	\file gdbKernelDispatcher.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief This class dispatch all messages to all AddOn
*/

#include "gdbKernelDispatcher.h"


/*!
	\details Create new object
	\param parent of this object
*/
GdbKernelDispatcher::GdbKernelDispatcher(QObject * parent ) : QObject(parent)
{

	addonList.clear();
}

/*!
	\details None.
*/
GdbKernelDispatcher::~GdbKernelDispatcher()
{
}


QPointer<class GdbAddonBase> GdbKernelDispatcher::findAddon(const QString & name)
{
	return mAddonList[name];//NULL;
}


/*!
	\details Add AddOn to the list
	\param p is a pointer to the GdbCore class
*/
void GdbKernelDispatcher::add(const QPointer<class GdbAddonBase> & p)
{
qDebug() << "Add : " + p->name();

	mAddonList[p->name()] = p;

	connect(GdbCore::Parser(), SIGNAL(onInterpreter(const QPointer<BaseInterpreter> & ,const int & , const QString &)), p , 
		SLOT(interpreter(const QPointer<BaseInterpreter> & , const int & , const QString &)),Qt::DirectConnection);

	connect(p,SIGNAL(errorMessage(QString, int)), SIGNAL(errorMessage(QString, int)));
	connect(p,SIGNAL(defaultMessage(QString)), SIGNAL(defaultMessage(QString)));
}


void GdbKernelDispatcher::clear()
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
	{qDebug() << "Delete : " + p->name();	  delete p;}
	mAddonList.clear();
}

//
/*!
	\details Call all AddOn::gdbStarted() function
*/
void GdbKernelDispatcher::gdbStarted()
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled())		p->gdbStarted();
}

/*!
	\details Call all AddOn::gdbFinished() function
*/
void GdbKernelDispatcher::gdbFinished()
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled())		p->gdbFinished();
}

/*!
	\details Call all AddOn::gdbError() function
*/
void GdbKernelDispatcher::gdbError()
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled())		p->gdbError();
}

/*!
	\details Call all AddOn::targetLoaded() function
*/
void GdbKernelDispatcher::targetLoaded(const int & id , const QString & s)
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled()) p->targetLoaded(id, s);
}

/*!
	\details Call all AddOn::targetNoLoaded() function
*/
void GdbKernelDispatcher::targetNoLoaded(const int & id , const QString & s)
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled()) p->targetNoLoaded(id, s);
}

/*!
	\details Call all AddOn::targetRunning() function
*/
void GdbKernelDispatcher::targetRunning(const int & id , const QString & s)
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled()) p->targetRunning(id, s);
}

/*!
	\details Call all AddOn::targetStopped() function
*/
void GdbKernelDispatcher::targetStopped(const int & id , const QString & s)
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled()) p->targetStopped(id, s);
}

/*!
	\details Call all AddOn::targetExited() function
*/
void GdbKernelDispatcher::targetExited(const int & id , const QString & s)
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
		if(p->isEnabled()) p->targetExited(id, s);
}


/*!
	\details Call all AddOn::info() function
*/
void GdbKernelDispatcher::done(const int & id, const QString & s)
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
	{
		if(p->isEnabled() && ( p->wantAllMessages() || (p->name() == GdbAddonBase::findValue(s, "className" )) ))
			p->done(id, s);
	}
}

/*!
	\details Call all AddOn::prompt() function
*/
void GdbKernelDispatcher::prompt(const int & id, const QString & s)
{
	foreach(QPointer<GdbAddonBase> p , mAddonList)
	{
		if(p->isEnabled() && ( p->wantAllMessages() || (p->name() == GdbAddonBase::findValue(s, "className" )) ))
			p->prompt(id, s);
	}
}

