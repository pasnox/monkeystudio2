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
#include "MkSShell.h"

#include <pDockWidget.h>
#include <pDockToolBar.h>
#include <shellmanager/MkSShellConsole.h>
#include <coremanager/MonkeyCore.h>
#include <maininterface/UIMain.h>
#include <pActionsManager.h>

class MkSShellDock : public pDockWidget
{
public:
    MkSShellDock( QWidget* parent = 0 )
        : pDockWidget( parent )
    {
        setObjectName( metaObject()->className() );
        setWidget( new MkSShellConsole( this ) );
    }
};

void MkSShell::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "MkS Shell" );
    mPluginInfos.Description = tr( "This plugin allow you to manually use the MkS Shell interpreter" );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "1.0.0";
    mPluginInfos.FirstStartEnabled = false;
    mPluginInfos.Pixmap = pIconManager::pixmap( "konsole.png", ":/icons" );
}

bool MkSShell::install()
{
    mDock = new MkSShellDock( MonkeyCore::mainWindow() );
    // add dock to dock toolbar entry
    MonkeyCore::mainWindow()->dockToolBar( Qt::TopToolBarArea )->addDock( mDock, infos().Caption, QIcon( infos().Pixmap ) );
    // create menu action for the dock
    pActionsManager::setDefaultShortcut( mDock->toggleViewAction(), QKeySequence( "F6" ) );
    return true;
}

bool MkSShell::uninstall()
{
    mDock->deleteLater();
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( BaseMkSShell, MkSShell )
#endif
