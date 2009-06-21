/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : MonkeyCore.cpp
** Date      : 2008-01-14T00:36:51
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/

#include "gdbCore.h"
#include "gdbParser.h"
#include "gdbInterpreter.h"
#include "gdbProcess.h"
#include "gdbKernelDispatcher.h"
#include "gdbControler.h"
#include "gdbSetting.h"


#include <QtDebug>



QHash<const QMetaObject*, QObject*> GdbCore::mInstances;


GdbSetting * GdbCore::Setting()
{
	
	if ( !mInstances.contains( &GdbSetting::staticMetaObject ) )
	{qDebug() << "creating Setting";mInstances[&GdbSetting::staticMetaObject] = new GdbSetting( qApp );}
	return qobject_cast<GdbSetting*>( mInstances[&GdbSetting::staticMetaObject] );
}

GdbParser * GdbCore::Parser()
{
	
	if ( !mInstances.contains( &GdbParser::staticMetaObject ) )
	{qDebug() << "creating Parser";mInstances[&GdbParser::staticMetaObject] = new GdbParser( qApp );}
	return qobject_cast<GdbParser*>( mInstances[&GdbParser::staticMetaObject] );
}

GdbInterpreter * GdbCore::Interpreter()
{
	if ( !mInstances.contains( &GdbInterpreter::staticMetaObject ) )
	{	qDebug() << "creating Interpreter";
	mInstances[&GdbInterpreter::staticMetaObject] = new GdbInterpreter(qApp);}
	return qobject_cast<GdbInterpreter*>( mInstances[&GdbInterpreter::staticMetaObject] );
}


GdbProcess * GdbCore::Process()
{ 	
	if ( !mInstances.contains( &GdbProcess::staticMetaObject ) )
	{	qDebug() << "creating Process";
	mInstances[&GdbProcess::staticMetaObject] = new GdbProcess(qApp);}
	return qobject_cast<GdbProcess*>( mInstances[&GdbProcess::staticMetaObject] );
}

GdbKernelDispatcher * GdbCore::KernelDispatcher()
{ 	
	if ( !mInstances.contains( &GdbKernelDispatcher::staticMetaObject ) )
	{	qDebug() << "creating KernelDispatcher";
	mInstances[&GdbKernelDispatcher::staticMetaObject] = new GdbKernelDispatcher(qApp);}
	return qobject_cast<GdbKernelDispatcher*>( mInstances[&GdbKernelDispatcher::staticMetaObject] );
}

GdbControler * GdbCore::Controler()
{ 	
	if ( !mInstances.contains( &GdbControler::staticMetaObject ) )
	{	qDebug() << "creating Controler";
	mInstances[&GdbControler::staticMetaObject] = new GdbControler(qApp);}
	return qobject_cast<GdbControler*>( mInstances[&GdbControler::staticMetaObject] );
}

GdbRestoreLine * GdbCore::RestoreLine()
{ 	
	if ( !mInstances.contains( &GdbRestoreLine::staticMetaObject ) )
	{	qDebug() << "creating RestoreLine";
	mInstances[&GdbRestoreLine::staticMetaObject] = new GdbRestoreLine(qApp);}
	return qobject_cast<GdbRestoreLine*>( mInstances[&GdbRestoreLine::staticMetaObject] );
}

void GdbCore::clear()
{
	foreach( QObject *p,mInstances)
	{
		qDebug() << "deleted " + QString(p->metaObject()->className());
		delete p;
	}
	mInstances.clear();
}

void GdbCore::init(QObject *o)
{
	Q_UNUSED(o);
	Setting();
      	Parser();
	Process();
	KernelDispatcher();
	Controler();
	RestoreLine();
}