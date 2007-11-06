/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : BuilderPlugin
** FileName  : BuilderPlugin.h
** Date      : 2007-10-09T19:48:10
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef BUILDERPLUGIN_H
#define BUILDERPLUGIN_H

#include "CLIToolPlugin.h"
#include "UIBuilderSettings.h"

class Q_MONKEY_EXPORT BuilderPlugin : public BasePlugin, public CLIToolPlugin
{	
public:
	virtual pCommand defaultBuildCommand() const = 0;
	virtual pCommand buildCommand() const = 0;
	virtual void setBuildCommand( const pCommand& ) = 0;
	virtual QWidget* builderSettingsWidget() { return new UIBuilderSettings( this, QApplication::activeWindow() ); }

};

Q_DECLARE_INTERFACE( BuilderPlugin, "org.monkeystudio.MonkeyStudio.BuilderPlugin/1.0" )

#endif // BUILDERPLUGIN_H
