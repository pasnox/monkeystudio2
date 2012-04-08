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
/*!
    \file GNUDebugger.cpp
    \date 14/08/08
    \author Xiantia
    \version 1.3.2
    \brief Main class, interface of pluginFactory
*/

#include "GNUDebugger.h"

#include <UIMain.h>
#include <MonkeyCore.h>
#include <Settings.h>
#include <pMonkeyStudio.h>
#include <QueuedStatusBar.h>

#include <QIcon>
    
/*!
        \brief Main class, interface of pluginFactory
        \details Create main DockGNUDebugger class and configure this
*/
GNUDebugger::GNUDebugger()
{
    // set plugin infos
    mPluginInfos.Caption = tr( "Debugger (Gdb)" );
    mPluginInfos.Description = tr( "Plugin for Gdb intergration" );
    mPluginInfos.Author = "Pinon Yannick aka Xiantia  <private mail>";
    mPluginInfos.Type = BasePlugin::iDebugger;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "1.4.0 (beta)";
    mPluginInfos.FirstStartEnabled = true;
}

/*!
    \details Save current settings 
*/
GNUDebugger::~GNUDebugger()
{
    if ( isEnabled() )
        setEnabled( false );
}

/*!
    \details Set enabled or disable plugin
    \param b is trus for enabled plugin
*/
bool GNUDebugger::setEnabled( bool b )
{
    if ( b && !isEnabled() )
    {
        // get instance
        mDockGNUDebugger = new DockGNUDebugger;
        // add dock to dock toolbar entry
        MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDockGNUDebugger, infos().Caption, QIcon( pixmap() ) );
        // set plugin enabled
        stateAction()->setChecked( true );
    }
    else if ( !b && isEnabled() )
    {
        delete mDockGNUDebugger;
        // set plugin disabled
        stateAction()->setChecked( false );
    }
    // return default value
    return true;
}

QPixmap GNUDebugger::pixmap() const
{
    return QPixmap( ":/icons/debugger.png" ); 
}

void GNUDebugger::saveSettings()
{}

void GNUDebugger::restoreSettings()
{}

QWidget* GNUDebugger::settingsWidget()
{
    return new UIGNUDebuggerSetting;
}
// DebuggerPlugin
Q_EXPORT_PLUGIN2( BaseGNUDebugger, GNUDebugger )
