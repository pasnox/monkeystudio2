#ifndef XUPPLUGIN_H
#define XUPPLUGIN_H

#include "pluginsmanager/BasePlugin.h"

class Q_MONKEY_EXPORT XUPPlugin : virtual public BasePlugin
{
};

Q_DECLARE_INTERFACE( XUPPlugin, "org.monkeystudio.MonkeyStudio.XUPPlugin/1.0" )

#endif // XUPPLUGIN_H
