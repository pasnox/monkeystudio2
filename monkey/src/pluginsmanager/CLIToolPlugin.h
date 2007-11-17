#ifndef CLITOOLPLUGIN_H
#define CLITOOLPLUGIN_H

#include "MonkeyExport.h"
#include "pCommand.h"
#include "BasePlugin.h"
#include "UICLIToolSettings.h"
#include "pCommandParser.h"

class Q_MONKEY_EXPORT CLIToolPlugin
{
public:
	virtual ~CLIToolPlugin() {}
	virtual pCommandList defaultCommands() const = 0;
	virtual pCommandList userCommands() const = 0;
	virtual void setUserCommands( const pCommandList& ) const = 0;
	virtual QWidget* cliToolSettingsWidget( BasePlugin* p )
	{ return new UICLIToolSettings( p, defaultCommands(), userCommands(), QApplication::activeWindow() ); }
	//FIXME Temporary commented, because I not sure, it's need. For Gcc parsers is separate plugin
	/*virtual QStringList availableParsers() const = 0;
	virtual pCommandParser* getParser( const QString& ) = 0;	*/
	virtual void commandTriggered() = 0;
	
};

Q_DECLARE_INTERFACE( CLIToolPlugin, "org.monkeystudio.MonkeyStudio.CLIToolPlugin/1.0" )

#endif // CLITOOLPLUGIN_H
