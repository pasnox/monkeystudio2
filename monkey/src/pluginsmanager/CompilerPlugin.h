#ifndef COMPILERPLUGIN_H
#define COMPILERPLUGIN_H

#include "BasePlugin.h"
#include "MonkeyExport.h"
#include "UICommandsEditor.h"
#include "pCommand.h"

class ProjectItem;

class Q_MONKEY_EXPORT CompilerPlugin : public BasePlugin
{
public:
	virtual pCommandList globalCommands() const = 0;
	virtual pCommandList userCommands() const = 0;
	virtual void setUserCommands( const pCommandList& ) const = 0;
	virtual QWidget* settingsWidget() { return new UICommandsEditor( globalCommands(), userCommands(), QApplication::activeWindow() ); };

};

Q_DECLARE_INTERFACE( CompilerPlugin, "org.monkeystudio.MonkeyStudio.CompilerPlugin/1.0" )

#endif // COMPILERPLUGIN_H
