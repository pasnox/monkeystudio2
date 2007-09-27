#ifndef CHILDPLUGIN_H
#define CHILDPLUGIN_H
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
#include <QStringList>
//
class AbstractProjectProxy;
//
class Q_MONKEY_EXPORT ChildPlugin : public BasePlugin
{
	Q_OBJECT
	//
public:
	virtual QStringList extensions() const
	{ return mExtensions; }
	virtual QStringList filters() const = 0;
	virtual bool addNewDocument( const QString&, AbstractProjectProxy* = 0 ) = 0;
	virtual bool openFile( const QString&, AbstractProjectProxy* = 0 ) = 0;
	//
protected:
	QStringList mExtensions;
	//
};
//
Q_DECLARE_INTERFACE( ChildPlugin, "org.monkeystudio.MonkeyStudio.ChildPlugin/1.0" )
//
#endif // CHILDPLUGIN_H
