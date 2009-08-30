#ifndef XUPPROJECTITEMHELPER_H
#define XUPPROJECTITEMHELPER_H

#include "BasePlugin.h"
#include "pCommand.h"

class XUPItem;
class XUPProjectItem;

typedef QList<BasePlugin::Type> BasePluginTypeList;
typedef QMap<BasePlugin::Type, pCommandList> TypeCommandListMap;

namespace XUPProjectItemHelper
{
	static const QString CommandsScopeName = "Commands";
	static const QString CommandScopeName = "Command";
	
	XUPItem* projectCommandsScope( XUPProjectItem* project, bool create );
	void addCommandProperty( XUPItem* variableItem, const QString& value );
	void setProjectCommands( XUPProjectItem* project, const TypeCommandListMap& commands );
	TypeCommandListMap projectCommands( XUPProjectItem* project );
	void installProjectCommands( XUPProjectItem* project );
};

#endif // XUPPROJECTITEMHELPER_H
