#ifndef COMPILERPLUGIN_H
#define COMPILERPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"

class Q_MONKEY_EXPORT CompilerPlugin : public BasePlugin
{
public:
	// initialize compiler configuration getting parameter, and sending string configuration
	virtual QString setUp( const QString& ) const = 0;

};

Q_DECLARE_INTERFACE( CompilerPlugin, "org.monkeystudio.MonkeyStudio.CompilerPlugin/1.0" )

#endif // COMPILERPLUGIN_H
