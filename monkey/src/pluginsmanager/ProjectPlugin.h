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
	// the function that open a project
	virtual ProjectItem* openProject( const QString&, ProjectItem* = 0 ) = 0;
	// the compiler this project use
	virtual QString compiler( ProjectItem* ) const
	{ return "GNUMake"; }
	// the debugger this project use
	virtual QString debugger( ProjectItem* ) const
	{ return "GNUDebugger"; }
	// the interpreter this project use
	virtual QString interpreter( ProjectItem* )
	{ return QString::null; }
	// edit the project settings
	virtual void editSettings( ProjectItem* ) = 0;
	
protected:
	QHash<QString, QStringList> mSuffixes;
	
};

Q_DECLARE_INTERFACE( ProjectPlugin, "org.monkeystudio.MonkeyStudio.ProjectPlugin/1.0" )

#endif // PROJECTPLUGIN_H
