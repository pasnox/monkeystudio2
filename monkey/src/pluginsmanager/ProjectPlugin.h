#ifndef PROJECTPLUGIN_H
#define PROJECTPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"
#include "ProjectItem.h"

#include <QHash>
#include <QStringList>

class Q_MONKEY_EXPORT ProjectPlugin : public BasePlugin
{
	Q_OBJECT
	
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
