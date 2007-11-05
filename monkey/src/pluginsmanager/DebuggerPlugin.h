/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : DebuggerPlugin.h
** Date      : 2007-11-04T22:45:41
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef DEBUGGERPLUGIN_H
#define DEBUGGERPLUGIN_H
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT DebuggerPlugin : public BasePlugin
{
};
//
Q_DECLARE_INTERFACE( DebuggerPlugin, "org.monkeystudio.MonkeyStudio.DebuggerPlugin/1.0" )
//
#endif // DEBUGGERPLUGIN_H
