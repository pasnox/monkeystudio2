/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : BuilderPlugin.h
** Date      : 2007-11-04T22:45:27
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
