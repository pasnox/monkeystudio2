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
#include "Irc.h"

#include <coremanager/MonkeyCore.h>
#include <maininterface/UIMain.h>
#include <consolemanager/pConsoleManager.h>
#include <pDockToolBar.h>

#include <QIcon>
#include <QTabWidget>

void Irc::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "Chat Irc" );
    mPluginInfos.Description = tr( "Plugin Irc for Monkey studio live" );
    mPluginInfos.Author = "Pinon yannick aka Xiantia <private mail>";
    mPluginInfos.Type = BasePlugin::iBase;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "1.0.0";
    mPluginInfos.FirstStartEnabled = false;
    mPluginInfos.Pixmap = QPixmap( ":/icons/irc.png" );
}

bool Irc::install()
{
    // create docks
    mIrcDock = IrcDock::instance();
    // add docks to main window
    MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mIrcDock, infos().Caption, QIcon( infos().Pixmap ) );
    return true;
}

bool Irc::uninstall()
{
    // delete docks
    delete mIrcDock;
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( BaseIrc, Irc )
#endif
