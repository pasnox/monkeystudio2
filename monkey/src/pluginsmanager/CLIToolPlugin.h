#ifndef CLITOOLPLUGIN_H
#define CLITOOLPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"
#include "pCommand.h"
#include "pCommandParser.h"

#include <QString>

class ConsoleCommand;
	
/*
Normal way to execute some console command (make tool, compiler, utility, etc.):
1) Get poiter to the CLIToolPlugin for a command
   		Example  GNUMake plugin
2) If it's exist, get it's command by using getCommand
				 command = GNUMake::getCommand ()
3) Add to the command line actions that you need to process
				 command.command() = command.command() +" clean all"
4) Execute command, using Console manager
*/

class Q_MONKEY_EXPORT CLIToolPlugin
{
public:
	virtual ~CLIToolPlugin() {}
	// Tool name
	virtual QString name() = 0;
	// Command for executing tool
	virtual QString getCommand() = 0;
	// will show to user dialog, where he can configure command line switches, 
	// and, when user will accept, switches string will be returned
	virtual QString getSwitches ( const QString = QString::null ) = 0;
    // returns pointer to the console output parser for output of console utility
    virtual pCommandParser* getParser() = 0;
	
};

Q_DECLARE_INTERFACE( CLIToolPlugin, "org.monkeystudio.MonkeyStudio.CLIToolPlugin/1.0" )

#endif // CLITOOLPLUGIN_H
