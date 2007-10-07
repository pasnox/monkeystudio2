/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : MonkeyProject plugin
** FileName  : MonkeyProject.h
** Date      : 2007-09-28
** License   : GPL
** Comment   : MonkeyProject plugin class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef NOPROJECT_H
#define NOPROJECT_H

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

#endif // NOPROJECT_H
