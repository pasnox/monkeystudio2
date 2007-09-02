#ifndef PROJECTPLUGIN_H
#define PROJECTPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"

#include <QStringList>

class Q_MONKEY_EXPORT ProjectPlugin : public BasePlugin
{
	Q_OBJECT
	
public:
	// the extensions this project can manage
	virtual QStringList extensions() const
	{ return mExtensions; }
	// the filters list to give to openfile dialog
	virtual QStringList filters() const
	{ return QStringList(); }
	// the function that open a project
	virtual bool openProject( const QString& ) = 0;
	// the compiler this project use
	virtual QString compiler() const
	{ return QString::null; }
	// the debugger this project use
	virtual QString debugger() const
	{ return QString::null; }
	// the interpreter this project use
	virtual QString interpreter()
	{ return QString::null; }
	
protected:
	QStringList mExtensions;
	
};

Q_DECLARE_INTERFACE( ProjectPlugin, "org.monkeystudio.MonkeyStudio.ProjectPlugin/1.0" )

#endif // PROJECTPLUGIN_H
