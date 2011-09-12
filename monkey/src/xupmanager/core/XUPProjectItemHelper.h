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
