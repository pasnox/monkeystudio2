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
	\file gdbSetting.cpp
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Load setting from Monkey .ini and store current configuration
*/
/*

	Xiantia@gmail.com

	for Debugger v1.3.0
*/



#include "gdbSetting.h"
#include <QtDebug>

/*!
	\details Create new object 
	\param parent of this object
*/
GdbSetting::GdbSetting(QObject * parent ) : QObject(parent)
{
	load();
}


void GdbSetting::load()
{
}

/*! 
	\details Save setting in .ini
*/
void GdbSetting::save()
{
}


/*! 
	\details Set enable or disable AddOn
	\param addOnName is the name of AddOn
	\param b is true if AddOn is enables from .ini
	\sa getStartUp()
*/
void GdbSetting::setEnable(QString addOnName, bool b)
{
	mAddonList[addOnName] = b;
}

/*!
	\details Get if AddOn is Enable or Disable
	\param addOnName is the name of addOn
	\retval Current state saved in .ini, if AddOn is not found in .ini, return true by default
	\sa setEnable()
*/
bool GdbSetting::getStartUp(QString addOnName)
{

	QHashIterator<QString, bool> i(mAddonList);
	while (i.hasNext()) 
	{
		i.next();
		if(i.key() == addOnName) return i.value(); 
		
	}	
	// default addon is true by default
	mAddonList[ addOnName] = true ;
	return true;
}

/*!
	\details Get path of Gdb
*/
QString GdbSetting::getPathGdb() { return pathGdb;}

/*!
	\details Set path of Gdb
*/
void GdbSetting::setPathGdb(QString s){ pathGdb = s;}

/*!
	\details Get path of path script file
*/
QString GdbSetting::getPathScript(){ return pathScript;}

/*!
	\details Set path of path script file
*/
void GdbSetting::setPathScript(QString s) { pathScript = s;}

void GdbSetting::setAddons(QString s, bool b)
{
	mAddonList[s] = b;
}

QHash<QString, bool>  GdbSetting::getAddons()
{
	return mAddonList;
}


