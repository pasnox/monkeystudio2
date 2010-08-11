#ifndef XUPPROJECTITEMHELPER_H
#define XUPPROJECTITEMHELPER_H

#include "MonkeyExport.h"
#include "BasePlugin.h"
#include "pCommand.h"

#include <QDomDocument>

class XUPItem;
class XUPProjectItem;

typedef QList<BasePlugin::Type> BasePluginTypeList;
typedef QMap<BasePlugin::Type, pCommandList> TypeCommandListMap;

namespace XUPProjectItemHelper
{
	static const QString CommandsScopeName = "Commands";
	static const QString CommandScopeName = "Command";
	
	Q_MONKEY_EXPORT XUPItem* projectCommandsScope( XUPProjectItem* project, bool create );
	Q_MONKEY_EXPORT void addCommandProperty( XUPItem* variableItem, const QString& value );
	Q_MONKEY_EXPORT void setProjectCommands( XUPProjectItem* project, const TypeCommandListMap& commands );
	Q_MONKEY_EXPORT TypeCommandListMap projectCommands( XUPProjectItem* project );
	Q_MONKEY_EXPORT void installProjectCommands( XUPProjectItem* project );
};

#endif // XUPPROJECTITEMHELPER_H
