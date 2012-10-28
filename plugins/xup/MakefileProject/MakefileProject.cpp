/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#include "MakefileProject.h"
#include "MakefileProjectItem.h"

#include <coremanager/MonkeyCore.h>

#include <QDir>

void MakefileProject::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "Makefile Project" );
    mPluginInfos.Description = tr( "Allows quickly build Makefile based projects" );
    mPluginInfos.Author = "Andrei Kopats aka hlamer <hlamer@tut.by>, Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iXUP;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "1.0.0";
    mPluginInfos.FirstStartEnabled = true;
    mPluginInfos.HaveSettingsWidget = false;
    mPluginInfos.dependencies << "GNUMake";
}

bool MakefileProject::install()
{
    mFilters = DocumentFilterMap( ":/makefileitems" );
    int weight = 0;
    
    mFilters[ "PROJECT" ].weight = weight++;
    mFilters[ "PROJECT" ].label = tr( "Makefile Projects" );
    mFilters[ "PROJECT" ].icon = "project.png";
    mFilters[ "PROJECT" ].type = DocumentFilter::Project;
    mFilters[ "PROJECT" ].filters << "*Makefile*";
    
    MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &MakefileProjectItem::staticMetaObject, mFilters );
    return true;
}

bool MakefileProject::uninstall()
{
    MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
    mFilters.clear();
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( ProjectMakefileProject, MakefileProject )
#endif
