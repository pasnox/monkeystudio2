#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "BasePlugin.h"

#include <QApplication>

//class AbstractProjectProxy;

class Q_MONKEY_EXPORT PluginsManager : public QObject, public QSingleton<PluginsManager>
{
	Q_OBJECT
	friend class QSingleton<PluginsManager>;
	
public:
	void loadsPlugins( const QString& = QString() );
	QList<BasePlugin*> plugins() const;
	BasePlugin* plugin( const QString&, BasePlugin::Type = BasePlugin::iAll,  const QString& = QString::null );
	
/*
	// loading files using childs plugins
	bool childPluginOpenFile( const QString&, AbstractProjectProxy* = 0 );
	QStringList childsFilters() const;
*/

private:
	QList<BasePlugin*> mPlugins;
	PluginsManager( QObject* = QApplication::instance() );
	bool addPlugin( QObject* );
	void enableUserPlugins();
	
/*
public slots:
	void manageRequested();
*/
};

#endif // PLUGINSMANAGER_H
