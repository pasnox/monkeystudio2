/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : MonkeyProject.h
** Date      : 2007-11-04T22:53:34
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef MONKEYPROJECT_H
#define MONKEYPROJECT_H

#include "ProjectPlugin.h"

class MonkeyProjectItem;

class MonkeyProject : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )

public:
	MonkeyProject();
	ProjectItem* getProjectItem( const QString& );
	
	bool setEnabled( bool b);
};

#endif // MONKEYPROJECT_H
