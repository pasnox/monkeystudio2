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
#ifndef PLUGINSMENU_H
#define PLUGINSMENU_H

#include "MonkeyExport.h"

#include <QMenu>

#include "pluginsmanager/BasePlugin.h"

class PluginsManager;

class Q_MONKEY_EXPORT PluginsMenu : public QObject
{
    Q_OBJECT
    
public:
    PluginsMenu( PluginsManager* manager );
    virtual ~PluginsMenu();
    
    QMenu* menu() const;
    void setMenu( QMenu* menu );
    
    void addPlugin( BasePlugin* plugin );

protected:
    PluginsManager* mManager;
    QMenu* mMenu;
    QAction* mManageDialogAction;
    QMap<BasePlugin::Type, QMenu*> mTypeMenus;
    QMap<BasePlugin*, QMenu*> mMenus;
    
    void initPluginMenusActions( BasePlugin* plugin, BasePlugin::Type type );

protected slots:
    void actionEnable_triggered( bool checked );
    void actionNeverEnable_triggered( bool checked );
    void actionConfigure_triggered();
    void actionAbout_triggered();
};

#endif // PLUGINSMENU_H
