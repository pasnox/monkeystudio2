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
#include "PHP.h"

void PHP::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "PHP" );
    mPluginInfos.Description = tr( "This plugin provide PHP interpreter and php parser." );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iCLITool;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.1.0";
    mPluginInfos.FirstStartEnabled = true;
    mPluginInfos.HaveSettingsWidget = true;
    mPluginInfos.Pixmap = pIconManager::pixmap( "php.png", ":/icons" );
}

bool PHP::install()
{
    return true;
}

bool PHP::uninstall()
{
    return true;
}

pCommand PHP::defaultCommand() const
{
    const QString php = "php";
    return pCommand( "Interpret", php, false, availableParsers(), "$cpp$" );
}

Q_EXPORT_PLUGIN2( InterpreterPHP, PHP )
