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
#include "XUP.h"

#include <coremanager/MonkeyCore.h>
#include <consolemanager/pCommand.h>

GenericXUPProjectItem::GenericXUPProjectItem()
    : XUPProjectItem()
{
}

QString GenericXUPProjectItem::projectType() const
{
    return PLUGIN_NAME;
}

void GenericXUPProjectItem::installCommands()
{
    pCommand cmd;
    
    cmd.setProject( this );
    cmd.setSkipOnError( false );
    cmd.setName( "execute_project_main_file" );
    cmd.setText( tr( "Execute project main file" ) );
    cmd.setCommand( "$target$" );
    cmd.setWorkingDirectory( path() );
    cmd.setParsers( QStringList() );
    cmd.setTryAllParsers( true );
    cmd.setExecutableCheckingType( XUPProjectItem::DefaultTarget );
    
    addCommand( "mBuilder", cmd );
    
    XUPProjectItem::installCommands();
}

void XUP::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "XUP Project" );
    mPluginInfos.Description = tr( "Generic XUP Project support for XUPManager" );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iXUP;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.5.0";
    mPluginInfos.FirstStartEnabled = true;
    mPluginInfos.HaveSettingsWidget = false;
    mPluginInfos.Pixmap = pIconManager::pixmap( "XUP.png", ":/icons" );
}

bool XUP::install()
{
    mFilters = DocumentFilterMap( ":/xupitems" );
    int weight = 0;
    
    mFilters[ "PROJECT" ].weight = weight++;
    mFilters[ "PROJECT" ].label = tr( "XUP Projects" );
    mFilters[ "PROJECT" ].icon = "project.png";
    mFilters[ "PROJECT" ].type = DocumentFilter::Project;
    mFilters[ "PROJECT" ].filters << "*.xup";
    
    mFilters[ "FILES" ].weight = weight++;
    mFilters[ "FILES" ].label = GenericXUPProjectItem::tr( "Files" );
    mFilters[ "FILES" ].icon = "file.png";
    mFilters[ "FILES" ].filters << "*";
    mFilters[ "FILES" ].type = DocumentFilter::File;
    mFilters[ "FILES" ].filtered = true;
    
    MonkeyCore::projectTypesIndex()->registerType( PLUGIN_NAME, &GenericXUPProjectItem::staticMetaObject, mFilters );
    return true;
}

bool XUP::uninstall()
{
    MonkeyCore::projectTypesIndex()->unRegisterType( PLUGIN_NAME );
    mFilters.clear();
    return true;
}

Q_EXPORT_PLUGIN2( XUPXUP, XUP )
