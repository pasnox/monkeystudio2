#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H
//
#include <QObject>
#include <QPointer>
#include <QDir>
//
#include "BasePlugin.h"
#include "MonkeyExport.h"
//
class AbstractProjectProxy;
//
class Q_MONKEY_EXPORT PluginsManager : public QObject
{
	Q_OBJECT
	//
public:
	static PluginsManager* self( QObject* = 0 );
	~PluginsManager();
	//
	void loadsPlugins( const QString& = QString() );
	void loadsPlugins( QDir );
	bool addPlugin( QObject* );
	void installPlugins();
	QList<BasePlugin*> plugins() const;
	BasePlugin* plugin( const QString&, BasePlugin::Type = BasePlugin::iAll,  const QString& = QString::null );
	// loading files using childs plugins
	bool childPluginOpenFile( const QString&, AbstractProjectProxy* = 0 );
	QStringList childsFilters() const;
	//
private:
	PluginsManager( QObject* = 0 );
	//
	static QPointer<PluginsManager> mSelf;
	QList<BasePlugin*> mPlugins;
	//
public slots:
	void manageRequested();
	//
};
//
#endif // PLUGINSMANAGER_H
