/****************************************************************************
**
**      Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Builder Plugins
** FileName  : GNUMake.cpp
** Date      : 2008-01-14T00:52:24
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
#include "GNUMake.h"

#include <QTabWidget>

#include <consolemanager/pConsoleManager.h>

void GNUMake::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "GNUMake" );
    mPluginInfos.Description = tr( "Plugin for execute GNU Make in console and parse it's output" );
    mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>, Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iCLITool;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.5.0";
    mPluginInfos.FirstStartEnabled = false;
    mPluginInfos.HaveSettingsWidget = true;
}

bool GNUMake::install()
{
    return true;
}

bool GNUMake::uninstall()
{
    return true;
}

QStringList GNUMake::availableParsers() const
{
    return QStringList()
        << "GNU Make"
        << "GCC"
        ;
}

pCommand GNUMake::defaultCommand() const
{
#ifdef Q_OS_WIN
    const QString make = "mingw32-make -w";
#else
    const QString make = "make -w";
#endif
    pCommand cmd( "Build", make, false, availableParsers(), "$cpp$", true );
    cmd.setName( PLUGIN_NAME );
    return cmd;
}

Q_EXPORT_PLUGIN2( BuilderGNUMake, GNUMake )
