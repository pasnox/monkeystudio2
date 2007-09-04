#ifndef PROJECTPLUGIN_H
#define PROJECTPLUGIN_H

#include "MonkeyExport.h"
#include "BasePlugin.h"

#include <QHash>
#include <QStringList>
#include <QStandardItem>

class Q_MONKEY_EXPORT ProjectPlugin : public BasePlugin
{
	Q_OBJECT
	
public:
	// the suffixes this project can manage
	virtual QHash<QString, QStringList> suffixes() const
	{ return mSuffixes; }
	// the function that open a project
	virtual QStandardItem* openProject( const QString&, QStandardItem* = 0 ) = 0;
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
	QHash<QString, QStringList> mSuffixes;
	
};

Q_DECLARE_INTERFACE( ProjectPlugin, "org.monkeystudio.MonkeyStudio.ProjectPlugin/1.0" )

#endif // PROJECTPLUGIN_H
