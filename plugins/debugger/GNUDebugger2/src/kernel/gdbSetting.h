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
	\file gdbSetting.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Load setting from Monkey .ini and store current configuration
*/

#ifndef GDBSETTING_H
#define GDBSETTING_H

#include <QObject>
#include <coremanager.h>
#include <settingsmanager.h>
#include "monkey.h"
#include "fresh.h"

#include <QMessageBox>

/*! 
	\brief Struct for setting AddOn
*/
typedef struct GdbAddOn
{
	/*! 
		\details Name of AddOn
	*/
	QString name;
	/*! 
	\details Enable or not
	*/
	bool enable;
};

/*!
	\brief Struct for store some informations
*/
typedef struct GdbSettings
{
	/*! 
		\details Current path of Gdb
	*/
	QString pathGdb;
	/*! 
		\details Current path of parse file
	*/
	QString pathParseFile;
	/*! 
		\details Current path of script file
	*/
	QString pathScript;
	/*! 
		\details List of all AddOn
	*/
	QList<GdbAddOn> addOnList;
};


/*!
	\brief Load setting from Monkey .ini and store current configuration
	\details It load setting from Monkey .ini. It configure AddOn (enable or not) 
	and load current path.
*/
class GdbSetting : public QObject, public QSingleton<GdbSetting>
{
	Q_OBJECT
	friend class QSingleton<GdbSetting>;

private :

	/*!
		\details Struct contains current path and AddOn list  
	*/
	GdbSettings gdbSettings;

public :

	GdbSetting(QObject * parent = 0);
	~GdbSetting(){};

	/*! 
		\details Load setting from .ini
	*/
	void load()
	{
		Settings* s = MonkeyCore::settings();
		s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
			gdbSettings.pathGdb = s->value("PathGdb", "gdb").toString();
			gdbSettings.pathParseFile =  s->value("PathParseFile", "" ).toString();
			gdbSettings.pathScript =  s->value("PathScript", "" ).toString();

		QStringList l = s->allKeys();

		foreach(QString k ,l)
		{
			if(k.startsWith("AddOn/"))
			{	
				bool e = s->value(k, true ).toBool();
				GdbAddOn a = { k.remove("AddOn/"),  e};
				gdbSettings.addOnList << a;
			}
		}
		s->endGroup();
	}

	/*! 
		\details Save setting in .ini
	*/
	void save()
	{
			Settings * s = MonkeyCore::settings();
			s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
			s->setValue( "PathGdb", gdbSettings.pathGdb);
			s->setValue( "PathParseFile", gdbSettings.pathParseFile );
			s->setValue( "PathScript", gdbSettings.pathScript );
		
			foreach(GdbAddOn a , gdbSettings.addOnList)
				s->setValue( "AddOn/" + a.name, a.enable ? true : false);
			
			s->endGroup();
	}
	
	/*! 
		\details Set enable or disable AddOn
		\param n is the name of AddOn
		\param b is true if AddOn is enables from .ini
	*/
	void setEnable(QString n, bool b)
	{
		GdbAddOn p={ n , b};
		for(int i = 0; i< gdbSettings.addOnList.count() ;i++)
		{
			if(gdbSettings.addOnList.at(i).name == n)
				gdbSettings.addOnList.replace(i,p) ;
		}
	}

	/*!
		\details Get if AddOn is Enable or Disable
		\retval true if AddOn is found in .ini, else return default GdbAddOn struct
		*/
	bool getStartUp(QString name)
	{
		foreach(GdbAddOn a, gdbSettings.addOnList)
		{
			if(a.name == name) 
				return a.enable;
		}
		
		GdbAddOn p = { name, true };
		gdbSettings.addOnList << p;	
		return true;
	}

	/*!
		\details Get path of Gdb
	*/
	QString getPathGdb() { return gdbSettings.pathGdb;}
	/*!
		\details Set path of Gdb
	*/
	void setPathGdb(QString s){ gdbSettings.pathGdb = s;}

	/*!
		\details Get path of parse file
	*/
	QString getPathParseFile(){ return gdbSettings.pathParseFile;}
	
	/*!
		\details Set path of parse file
	*/
	void setPathParseFile(QString s) { gdbSettings.pathParseFile = s;}
	
	/*!
		\details Get path of path script file
	*/
	QString getPathScript(){ return gdbSettings.pathScript;}
	/*!
		\details Set path of path script file
	*/
	void setPathScript(QString s) { gdbSettings.pathScript = s;}

	/*!
		\details Get list of AddOn
	*/
	QList<GdbAddOn> * getAddOnList() { return &gdbSettings.addOnList;}
};
#endif
