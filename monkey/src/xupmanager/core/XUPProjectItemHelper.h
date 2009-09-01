#ifndef XUPPROJECTITEMHELPER_H
#define XUPPROJECTITEMHELPER_H

#include "BasePlugin.h"
#include "pCommand.h"

#include <QDomDocument>

class XUPItem;
class XUPProjectItem;

typedef QList<BasePlugin::Type> BasePluginTypeList;
typedef QMap<BasePlugin::Type, pCommandList> TypeCommandListMap;

struct XUPDynamicFolderSettings
{
	XUPDynamicFolderSettings()
	{
		Active = false;
	}
	
	bool isNull() const
	{
		return !Active && AbsolutePath.isEmpty() && FilesPatterns.isEmpty();
	}
	
	bool Active;
	QString AbsolutePath;
	QStringList FilesPatterns;
};

namespace XUPProjectItemHelper
{
	static const QString CommandsScopeName = "Commands";
	static const QString CommandScopeName = "Command";
	static const QString DynamicFolderSettingsName = "DynamicFolder.Settings";
	static const QString DynamicFolderName = "DynamicFolder";
	
	XUPItem* projectCommandsScope( XUPProjectItem* project, bool create );
	void addCommandProperty( XUPItem* variableItem, const QString& value );
	void setProjectCommands( XUPProjectItem* project, const TypeCommandListMap& commands );
	TypeCommandListMap projectCommands( XUPProjectItem* project );
	void installProjectCommands( XUPProjectItem* project );
	
	XUPItem* projectDynamicFolderSettingsItem( XUPProjectItem* project, bool create );
	void addDynamicFolderSettingsProperty( XUPItem* dynamicFolderItem, const QString& value );
	XUPDynamicFolderSettings projectDynamicFolderSettings( XUPProjectItem* project );
	void setProjectDynamicFolderSettings( XUPProjectItem* project, const XUPDynamicFolderSettings& folder );
	
	XUPItem* projectDynamicFolderItem( XUPProjectItem* project, bool create );
	void addDynamicFolderProperty( XUPItem* dynamicFolderItem, const QString& value );
	void updateDynamicFolder( XUPProjectItem* project, const QString& path );
	
	QDomDocument stripDynamicFolderFiles( const QDomDocument& document );
};

#endif // XUPPROJECTITEMHELPER_H
