#ifndef DEBUGGERPLUGIN_H
#define DEBUGGERPLUGIN_H
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT DebuggerPlugin : public BasePlugin
{
	Q_OBJECT
	//
};
//
Q_DECLARE_INTERFACE( DebuggerPlugin, "org.monkeystudio.MonkeyStudio.DebuggerPlugin/1.0" )
//
#endif // DEBUGGERPLUGIN_H
