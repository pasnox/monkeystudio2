#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"
#include "UIPluginsSettings.h"

#include <QPluginLoader>

PluginsManager::PluginsManager( QObject* p )
	: QObject( p )
{}

QList<BasePlugin*> PluginsManager::plugins() const
{ return mPlugins; }

template <class T>
QList<T> PluginsManager::plugins( const QString& n, BasePlugin::Type t, const QString& v )
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
						if ( qobject_cast<T>( bp ) )
							l << qobject_cast<T>( bp );
	// return list
	return l;
}

template <class T>
T PluginsManager::plugin( const QString& n, BasePlugin::Type t, const QString& v )
{
	return plugins<T>( n, t, v ).value( 0 );
}

ProjectPlugin* PluginsManager::projectPluginForFileName( const QString& s )
{
	foreach ( ProjectPlugin* pp, plugins<ProjectPlugin*>( "", BasePlugin::iProject ) )
	{
		// get plugin suffixes
		QHash<QString, QStringList> l = pp->suffixes();
		// file suffixe
		const QString sf = QFileInfo( s ).suffix().prepend( "*." );
		const QString f = QFileInfo( s ).fileName();
		bool b = false;
		// check all suffixes
		foreach ( QString k, l.keys() )
		{
			// check normal extension: ie *.ext
			b = l.value( k ).contains( sf, Qt::CaseInsensitive );
			// if not found try regexp expression: ie *makefile or Makefile*
			if ( !b )
				foreach ( QString sx, l.value( k ) )
					if ( ( b = QRegExp( sx, Qt::CaseInsensitive, QRegExp::Wildcard ).exactMatch( f ) ) )
						break;
			// if suffixe match
			if ( b )
				return pp;
		}
	}
	// not found
	return 0;
}

void PluginsManager::loadsPlugins( const QString& s )
{
	// get application path
	QDir d( s.isEmpty() ? qApp->applicationDirPath() : s );
#if defined( Q_OS_WIN )
	// move up if in debug/release folder
	if ( d.dirName().toLower() == "debug" || d.dirName().toLower() == "release" )
		d.cdUp();
#elif defined( Q_OS_MAC )
	// move up 3 times if in MacOS folder
	if ( d.dirName() == "MacOS" )
	{
		d.cdUp();
		d.cdUp();
		d.cdUp();
	}
#endif
	
	// go to plugins directory
	d.cd( "plugins" );
	
	// loads static plugins
	foreach ( QObject* o, QPluginLoader::staticInstances() )
		if ( !addPlugin( o ) )
			qWarning( qPrintable( tr( "Failed to load static plugin" ) ) );
		
	// load all plugins
	foreach ( QFileInfo f, pMonkeyStudio::getFiles( d ) )
	{
		QPluginLoader l( f.absoluteFilePath() );
		if ( !addPlugin( l.instance() ) )
		{
			// try unload it and reload it in case of old one in memory
			l.unload();
			l.load();
			
			// if can t load it, check next
			if ( !addPlugin( l.instance() ) )			
				qWarning( qPrintable( tr( "Failed to load plugin ( %1 ): Error: %2" ).arg( f.absoluteFilePath(), l.errorString() ) ) );
		}
	}
		
	// installs user requested plugins
	enableUserPlugins();
}

bool PluginsManager::addPlugin( QObject* o )
{
	// try to cast instance to BasePlugin
	BasePlugin* bp = qobject_cast<BasePlugin*>( o );
	
	// if not return
	if ( !bp )
		return false;
	
	// show plugin infos
	qWarning( qPrintable( tr( "Found plugin: %1, type: %2" ).arg( bp->infos().Name ).arg( bp->infos().Type ) ) );
	
	// add it to plugins list
	mPlugins << bp;
	
	// return
	return true;
}

void PluginsManager::enableUserPlugins()
{
	foreach ( BasePlugin* bp, mPlugins )
	{		
		// check in settings if we must install this plugin
		if ( !pSettings::instance()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() )
			qWarning( qPrintable( tr( "User wantn't to intall plugin: %1" ).arg( bp->infos().Name ) ) );
		// if not enabled, enable it
		else if ( !bp->isEnabled() )
		{
			if ( bp->setEnabled( true ) )
				qWarning( qPrintable( tr( "Successfully enabled plugin: %1" ).arg( bp->infos().Name ) ) );
			else
				qWarning( qPrintable( tr( "Unsuccessfully enabled plugin: %1" ).arg( bp->infos().Name ) ) );
		}
		else
			qWarning( qPrintable( tr( "Already enabled plugin: %1" ).arg( bp->infos().Name ) ) );
	}
}

void PluginsManager::manageRequested()
{
	UIPluginsSettings::instance()->exec(); 
}
