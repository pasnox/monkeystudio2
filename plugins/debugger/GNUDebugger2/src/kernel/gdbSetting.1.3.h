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

typedef struct GdbAddOn
{
	QString name;
	bool enable;
};

typedef struct GdbSettings
{
	QString pathGdb;
	QString pathParseFile;

	QList<GdbAddOn> addOnList;
};


class GdbSetting : public QObject, public QSingleton<GdbSetting>
{

	Q_OBJECT
	friend class QSingleton<GdbSetting>;

private :

	GdbSettings gdbSettings;

public :

	GdbSetting(QObject * parent = 0);
	~GdbSetting(){};

	void load()
	{
		Settings* s = MonkeyCore::settings();
		s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
			gdbSettings.pathGdb = s->value("PathGdb", "gdb").toString();
			gdbSettings.pathParseFile =  s->value("PathParseFile", "gdbparsing.txt" ).toString();
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

	void save()
	{
			Settings * s = MonkeyCore::settings();
			s->beginGroup( QString( "Plugins/%1" ).arg( PLUGIN_NAME ) );
			s->setValue( "PathGdb", gdbSettings.pathGdb);
			s->setValue( "PathParseFile", gdbSettings.pathParseFile );
		
			foreach(GdbAddOn a , gdbSettings.addOnList)
				s->setValue( "AddOn/" + a.name, a.enable ? true : false);
			
			s->endGroup();
	}
	
	void setEnable(QString n, bool b)
	{
		for(int i = 0; i< gdbSettings.addOnList.count() ;i++)
		{
			GdbAddOn p={ n , b};
			if(gdbSettings.addOnList.at(i).name == n)
				gdbSettings.addOnList.replace(i,p) ;
		}
	}

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


	QString getPathGdb() { return gdbSettings.pathGdb;}
	void setPathGdb(QString s){ gdbSettings.pathGdb = s;}

	QString getPathParseFile(){ return gdbSettings.pathParseFile;}
	void setPathParseFile(QString s) { gdbSettings.pathParseFile = s;}
	
	QList<GdbAddOn> * getAddOnList() { return &gdbSettings.addOnList;}

};

#endif
