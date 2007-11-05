/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : CLIToolPlugin.h
** Date      : 2007-11-04T22:45:28
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
	virtual QStringList availableParsers() const = 0;
	virtual pCommandParser* getParser( const QString& ) = 0;	
	virtual void commandTriggered() = 0;
	
};

Q_DECLARE_INTERFACE( CLIToolPlugin, "org.monkeystudio.MonkeyStudio.CLIToolPlugin/1.0" )

#endif // CLITOOLPLUGIN_H
