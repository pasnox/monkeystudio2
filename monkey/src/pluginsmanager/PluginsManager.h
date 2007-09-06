#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "BasePlugin.h"
#include "ProjectPlugin.h"

#include <QApplication>

class Q_MONKEY_EXPORT PluginsManager : public QObject, public QSingleton<PluginsManager>
{
	Q_OBJECT
	friend class QSingleton<PluginsManager>;
	
public:
	void loadsPlugins( const QString& = QString() );

	QList<BasePlugin*> plugins() const;

	template <class T>
	QList<T> plugins( const QString& n, BasePlugin::Type t = BasePlugin::iAll,  const QString& v = QString::null )
	{
		// temporary list
		QList<T> l;
		// for each plugin
		foreach ( BasePlugin* bp, mPlugins )
			// plugin must be enabled
			if ( bp->isEnabled() )
				// empty or good name
				if ( n.isEmpty() || bp->infos().Name == n )
					// good type or type = iAll
					if ( t == BasePlugin::iAll || bp->infos().Type == t )
						// no version or good version
						if ( v.isEmpty() || bp->infos().Version == v )
							// good cast
							if ( T p = qobject_cast<T>( bp ) )
								l << p;
		// return list
		return l;
	}
	
	template <class T>
	T plugin( const QString& n, BasePlugin::Type t = BasePlugin::iAll,  const QString& v = QString::null )
	{ return plugins<T>( n, t, v ).value( 0 ); }
	
	ProjectPlugin* projectPluginForFileName( const QString& );
	
	void setCurrentCompiler( const QString& );
	const QString currentCompiler();
	
	void setCurrentDebugger( const QString& );
	const QString currentDebugger();
	
	void setCurrentInterpreter( const QString& );
	const QString currentInterpreter();
	
private:
	QList<BasePlugin*> mPlugins;
	PluginsManager( QObject* = QApplication::instance() );
	bool addPlugin( QObject* );
	void enableUserPlugins();
	
public slots:
	void manageRequested();

};

#endif // PLUGINSMANAGER_H
