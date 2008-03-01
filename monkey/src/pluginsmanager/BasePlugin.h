/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : BasePlugin.h
** Date      : 2008-01-14T00:37:00
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
#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include "MonkeyExport.h"
#include "MonkeyCore.h"
#include "pSettings.h"

#include <QtPlugin>
#include <QLabel>

#ifdef __COVERAGESCANNER__
#include <QCoreApplication>
#include <QDir>
#endif

class Q_MONKEY_EXPORT BasePlugin : public QObject
{
	Q_OBJECT
	Q_ENUMS( Type )
	
public:
	// plugin type enums
	enum Type
	{	iAll = -1,
		iBase,
		iChild,
		iCLITool,
		iBuilder,
		iCompiler,
		iDebugger,
		iInterpreter,
		iProject,
		iXUP,
		iLast };
		
	// plugin info structure
	struct Q_MONKEY_EXPORT PluginInfos
	{
		QString Caption; // the string to show as caption
		QString Description; // the plugin description
		QString Author; // the plugin author
		BasePlugin::Type Type; // the plugin type
		QString Name; // the plugin name for version control
		QString Version; // the plugin version for version control
		bool Enabled; // to know if this plugin is enabled
	};
	
	BasePlugin()
	{ mPluginInfos.Enabled = false; }
	virtual ~BasePlugin()
	{ if ( isEnabled() ) setEnabled( false ); }
	
	virtual PluginInfos infos() const
	{ return mPluginInfos; }
	
	virtual QWidget* settingsWidget()
	{ return new QLabel( QObject::tr( "This plugin can't be configured" ) ); }
	
	virtual bool isEnabled() const
	{ return mPluginInfos.Enabled; }
	
	virtual bool setEnabled( bool )
	{ return false; }
	
	virtual QString settingsKey( const QString& k ) const
	{ return QString( "Plugins/%1/%2" ).arg( mPluginInfos.Name ).arg(  k ); }
	
	virtual QVariant settingsValue( const QString& k, const QVariant& v = QVariant() ) const
	{ return MonkeyCore::settings()->value( settingsKey( k ), v ); }
	
	virtual void setSettingsValue( const QString& k, const QVariant& v ) const
	{ MonkeyCore::settings()->setValue( settingsKey( k ), v ); }
	
	// coverage support members
#ifdef __COVERAGESCANNER__
	virtual void saveCodeCoverage( const QString& n, const QString& s )
	{
		// set path
		QString s = QCoreApplication::applicationDirPath();
#ifndef Q_OS_WIN
		s = QDir::homePath();
#endif
		s.append( "/monkeystudio_tests" );
		
		// create path if it not exists
		QDir d( s );
		if ( !d.exists() )
			d.mkdir( s );
		
		// set os specific filename
		s = QDir::toNativeSeparators( s.append( "/monkey_cov" ) ); 
		
		// deal with coverage meter
		__coveragescanner_filename( qPrintable( s ) );
		__coveragescanner_teststate( qPrintable( s ) );
		__coveragescanner_testname( qPrintable( QString( "%1/%2" ).arg( n, infos().Name ) ) );
		__coveragescanner_save();
	}
#endif
	
protected:
	PluginInfos mPluginInfos;
	
};

Q_DECLARE_INTERFACE( BasePlugin, "org.monkeystudio.MonkeyStudio.BasePlugin/1.0" )

#endif // BASEPLUGIN_H
