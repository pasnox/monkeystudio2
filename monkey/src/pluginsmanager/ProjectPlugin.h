/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : ProjectPlugin.h
** Date      : 2007-11-04T22:45:46
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef PROJECTPLUGIN_H
#define PROJECTPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"
#include "ProjectItem.h"

#include <QHash>
#include <QStringList>

class Q_MONKEY_EXPORT ProjectPlugin : public BasePlugin
{
public:
	// the suffixes this project can manage
	virtual QHash<QString, QStringList> suffixes() const
	{ return mSuffixes; }
	// return a new project item of this type of project
	virtual ProjectItem* getProjectItem( const QString& ) = 0;
	
protected:
	QHash<QString, QStringList> mSuffixes;
	
};

Q_DECLARE_INTERFACE( ProjectPlugin, "org.monkeystudio.MonkeyStudio.ProjectPlugin/1.0" )

#endif // PROJECTPLUGIN_H
