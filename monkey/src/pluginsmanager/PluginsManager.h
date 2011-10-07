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
#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include <MonkeyExport.h>

#include "pluginsmanager/BasePlugin.h"
#include "ChildPlugin.h"
#include "DebuggerPlugin.h"
#include "pluginsmanager/CLIToolPlugin.h"

class XUPItem;
class pAbstractChild;
class PluginsMenu;

class Q_MONKEY_EXPORT PluginsManager : public QObject
{
    Q_OBJECT
    friend class MonkeyCore;
    
public:
    enum StateType { stAll = -1, stDisabled, stEnabled };

    void loadsPlugins();
    
    QList<BasePlugin*> plugins() const;
    template <class T>
    QList<T> plugins( PluginsManager::StateType state, const QString& name = QString::null, const QString& version = QString::null )
    {
        QList<T> plugins;
        
        foreach ( BasePlugin* bp, mPlugins ) {
            // plugin state
            if ( state == stAll || ( !bp->isEnabled() && state == stDisabled ) || ( bp->isEnabled() && state == stEnabled ) ) {
                // empty or good name
                if ( name.isEmpty() || bp->infos().Name == name ) {
                    // no version or good version
                    if ( version.isEmpty() || bp->infos().Version == version ) {
                        // good cast
                        if ( T plugin = qobject_cast<T>( bp ) ) {
                            plugins << plugin;
                        }
                    }
                }
            }
        }
        
        return plugins;
    }
    template <class T>
    T plugin( PluginsManager::StateType type, const QString& name,  const QString& version = QString::null )
    {
        if ( name.isEmpty() ) {
            return 0;
        }
        
        return plugins<T>( type, name, version ).value( 0 );
    }
    
    pAbstractChild* documentForFileName( const QString& fileName );
    QMap<QString, QStringList> childSuffixes() const;
    QString childFilters() const;
    
    inline PluginsMenu* menuHandler() const { return mMenuHandler; }
    
protected:
    PluginsMenu* mMenuHandler;
    QList<BasePlugin*> mPlugins;

    PluginsManager( QObject* = 0 );
    bool addPlugin( QObject* );
    void enableUserPlugins();
    
public slots:
    void manageRequested();
    void clearPlugins();
};

#endif // PLUGINSMANAGER_H
