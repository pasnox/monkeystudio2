#ifndef CLITOOLPLUGIN_H
#define CLITOOLPLUGIN_H

#include "MonkeyExport.h"
#include "pCommand.h"
#include "BasePlugin.h"
#include "UICommandsEditor.h"
#include "pCommandParser.h"

class Q_MONKEY_EXPORT CLIToolPlugin
{
public:
	virtual ~CLIToolPlugin() {}
	virtual pCommandList globalCommands() const { return pCommandList(); }
	virtual pCommandList userCommands() const { return pCommandList(); }
	virtual void setUserCommands( const pCommandList& ) const {}
	virtual QWidget* commandsEditorWidget( BasePlugin* p )
	{ return new UICommandsEditor( p, globalCommands(), userCommands(), QApplication::activeWindow() ); }
	virtual QStringList availableParsers() const { return QStringList(); }
	virtual pCommandParser* getParser( const QString& ) = 0;	
	virtual void commandTriggered() {}
	
};

Q_DECLARE_INTERFACE( CLIToolPlugin, "org.monkeystudio.MonkeyStudio.CLIToolPlugin/1.0" )

#endif // CLITOOLPLUGIN_H
