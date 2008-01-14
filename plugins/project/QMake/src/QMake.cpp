/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : QMake.cpp
** Date      : 2008-01-14T00:54:10
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
#include "QMake.h"
#include "QMakeItem.h"
#include "UIProjectsManager.h"
#include "ProjectsProxy.h"
#include "UISettingsQMake.h"
#include "MonkeyCore.h"

#include <QDir>

QMake::QMake()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "QMake" );
	mPluginInfos.Description = tr( "Plugin for managing QMake project" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Roper Alexander aka minirop <minirop@peyj.com>";
	mPluginInfos.Type = BasePlugin::iProject;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

bool QMake::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// set usable suffixes
		mSuffixes[tr( "Qt Projects" )] = QStringList() << "*.pro";
		// set filtered items
		MonkeyCore::projectsManager()->proxy()->addFilterValues( UISettingsQMake::readFilters() );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// clear suffixes
		mSuffixes.clear();
		// unset filtered items
		MonkeyCore::projectsManager()->proxy()->removeFilterValues( UISettingsQMake::readFilters() );
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QWidget* QMake::settingsWidget()
{ return new UISettingsQMake(); }

ProjectItem* QMake::getProjectItem( const QString& s )
{
	// don t open project if plugin is not enabled
	if ( !isEnabled() || !QDir::match( mSuffixes[tr( "Qt Projects" )], s ) )
		return 0;
	// create project item
	ProjectItem* it = new QMakeItem( ProjectItem::ProjectType );
	// set project filename
	it->setValue( s );
	// set item plugin
	it->setPlugin( this );
	// return item
	return it;
}

Q_EXPORT_PLUGIN2( ProjectQMake, QMake )
