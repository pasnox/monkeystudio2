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
#ifndef XUPPROJECTITEMHELPER_H
#define XUPPROJECTITEMHELPER_H

#include <MonkeyExport.h>

#include "consolemanager/pCommand.h"

#include <QDomDocument>

class XUPItem;
class XUPProjectItem;
class XUPDynamicFolderItem;

typedef QMap<QString, pCommand::List> MenuCommandListMap;

struct Q_MONKEY_EXPORT XUPDynamicFolderSettings
{
    XUPDynamicFolderSettings() {
        Active = false;
    }
    
    bool isNull() const {
        return !Active
            && AbsolutePath.isEmpty()
            && FilesPatterns.isEmpty()
            ;
    }
    
    bool Active;
    QString AbsolutePath;
    QStringList FilesPatterns;
};

namespace XUPProjectItemHelper
{
    static const QString SettingsScopeName = "XUP";
    static const QString CommandScopeName = QString( "%1.Command" ).arg( SettingsScopeName );
    static const QString DynamicFolderName = QString( "%1.DynamicFolder" ).arg( SettingsScopeName );
    static const QString DynamicFolderSettingsName = QString( "%1.Settings" ).arg( DynamicFolderName );
    
    Q_MONKEY_EXPORT void setProjectSettingsValues( XUPProjectItem* project, const QString& key, const QStringList& values );
    Q_MONKEY_EXPORT QStringList projectSettingsValues( XUPProjectItem* project, const QString& key, const QStringList& defaultValues = QStringList() );
    Q_MONKEY_EXPORT void setProjectSettingsValue( XUPProjectItem* project, const QString& key, const QString& value );
    Q_MONKEY_EXPORT QString projectSettingsValue( XUPProjectItem* project, const QString& key, const QString& defaultValue = QString::null );
    
    Q_MONKEY_EXPORT void addCommandProperty( XUPItem* variableItem, const QString& value );
    Q_MONKEY_EXPORT void setProjectCommands( XUPProjectItem* project, const MenuCommandListMap& commands );
    Q_MONKEY_EXPORT MenuCommandListMap projectCommands( XUPProjectItem* project );
    
    Q_MONKEY_EXPORT XUPItem* projectDynamicFolderSettingsItem( XUPProjectItem* project, bool create );
    Q_MONKEY_EXPORT void addDynamicFolderSettingsProperty( XUPItem* dynamicFolderItem, const QString& value );
    Q_MONKEY_EXPORT XUPDynamicFolderSettings projectDynamicFolderSettings( XUPProjectItem* project );
    Q_MONKEY_EXPORT void setProjectDynamicFolderSettings( XUPProjectItem* project, const XUPDynamicFolderSettings& folder );
    
    Q_MONKEY_EXPORT XUPDynamicFolderItem* projectDynamicFolderItem( XUPProjectItem* project, bool create );
    Q_MONKEY_EXPORT QDomDocument stripDynamicFolderFiles( const QDomDocument& document );
};

#endif // XUPPROJECTITEMHELPER_H
