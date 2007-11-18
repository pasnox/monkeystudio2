#ifndef COMPILERPLUGIN_H
#define COMPILERPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"
#include "pCommand.h"
#include "UICompilerSettings.h"

class Q_MONKEY_EXPORT CompilerPlugin : public BasePlugin
{
public:
	virtual pCommand defaultCompileCommand() const = 0;
	virtual pCommand compileCommand() const = 0;
	virtual void setCompileCommand( const pCommand& ) = 0;
	virtual QWidget* compilerSettingsWidget() { return new UICompilerSettings( this, QApplication::activeWindow() ); }
	// initialize compiler configuration getting parameter, and sending string configuration
	virtual QString setUp( const QString& ) const = 0;

};

Q_DECLARE_INTERFACE( CompilerPlugin, "org.monkeystudio.MonkeyStudio.CompilerPlugin/1.0" )

#endif // COMPILERPLUGIN_H
