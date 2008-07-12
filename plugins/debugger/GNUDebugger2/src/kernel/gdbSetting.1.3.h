/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbSetting
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/


#ifndef GDBSETTING_H
#define GDBSETTING_H

#include <QObject>
#include <coremanager.h>
#include <settingsmanager.h>
#include "monkey.h"
#include "fresh.h"

#include <QMessageBox>

//! Small struct for setting AddOn
/**
\author xiantia
\version 1.3.2
*/
typedef struct GdbAddOn
{
	//! Name of AddOn
	QString name;
	//! Enable or not
	bool enable;
};

//! Small struct for store some informations
/**
\author xiantia
\version 1.3.2
*/
typedef struct GdbSettings
{
	//! Current path of Gdb
	QString pathGdb;
	//! Current path of parse file
	QString pathParseFile;
	//! Current path of script file
	QString pathScript;

	//! List of all AddOn
	QList<GdbAddOn> addOnList;
};

//! Load setting from Monkey .ini and store current configuration

/**
\author xiantia.

It load setting from Monkey .ini. It configure AddOn (enable or not) and load current path
*/

class GdbSetting : public QObject, public QSingleton<GdbSetting>
{

	Q_OBJECT
	friend class QSingleton<GdbSetting>;

private :

	//! Struct contains current path and AddOn list  
	GdbSettings gdbSettings;

public :

	GdbSetting(QObject * parent = 0);
	~GdbSetting(){};

	//! Load setting from .ini
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

	//! Save setting in .ini
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
	
	//! Set enable or disable AddOn
	void setEnable(QString n, bool b)
	{
		GdbAddOn p={ n , b};
		for(int i = 0; i< gdbSettings.addOnList.count() ;i++)
		{
			if(gdbSettings.addOnList.at(i).name == n)
				gdbSettings.addOnList.replace(i,p) ;
		}
	}

	//! get if AddOn is Enable or Disable
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

	//! Get path of Gdb
	QString getPathGdb() { return gdbSettings.pathGdb;}
	//! Set path of Gdb
	void setPathGdb(QString s){ gdbSettings.pathGdb = s;}

	//! Get path of parse file
	QString getPathParseFile(){ return gdbSettings.pathParseFile;}
	//! Set path of parse file
	void setPathParseFile(QString s) { gdbSettings.pathParseFile = s;}
	
	//! Get path of path script file
	QString getPathScript(){ return gdbSettings.pathScript;}
	//! Set path of path script file
	void setPathScript(QString s) { gdbSettings.pathScript = s;}

	//! Get list of AddOn
	QList<GdbAddOn> * getAddOnList() { return &gdbSettings.addOnList;}
};

#endif
