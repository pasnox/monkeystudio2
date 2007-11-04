/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : CompilerPlugin.h
** Date      : 2007-11-04T22:45:29
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef COMPILERPLUGIN_H
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
