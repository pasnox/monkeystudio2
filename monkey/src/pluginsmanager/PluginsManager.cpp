#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"

#include <QPluginLoader>

PluginsManager::PluginsManager( QObject* p )
	: QObject( p )
{}

QList<BasePlugin*> PluginsManager::plugins() const
{ return mPlugins; }

BasePlugin* PluginsManager::plugin( const QString& n, BasePlugin::Type t, const QString& v )
{
	// for each plugin
	foreach ( BasePlugin* bp, mPlugins )
		// good name
		if ( bp->infos().Name == n )
			// good type or type = iAll
			if ( t == BasePlugin::iAll || bp->infos().Type == t )
				// no version or good version
				if ( v.isNull() || bp->infos().Version == v )
					return bp;
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
				qWarning( qPrintable( tr( "Failed to load plugin ( %1 ): Error: %2" ).arg( s, l.errorString() ) ) );
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
		bp->setEnabled( true );
		continue;
		
		// check in settings if we must install this plugin
		if ( !pSettings::instance()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() )
			qWarning( qPrintable( tr( "User wantn't to intall plugin: %1" ).arg( bp->infos().Name ) ) );
		// if not enabled, enable it
		else if ( !bp->isEnabled() )
		{
			if ( bp->setEnabled( true ) )
				qWarning( qPrintable( tr( "Successfully installed plugin: %1" ).arg( bp->infos().Name ) ) );
			else
				qWarning( qPrintable( tr( "Unsuccessfully installed plugin: %1" ).arg( bp->infos().Name ) ) );
		}
		else
			qWarning( qPrintable( tr( "Already enabled plugin: %1" ).arg( bp->infos().Name ) ) );
	}
}

/*
bool PluginsManager::childPluginOpenFile( const QString& s, AbstractProjectProxy* p )
{
	QString mExtension = QFileInfo( s ).completeSuffix();
	foreach ( BasePlugin* bp, mPlugins )
	{
		if ( bp->infos().Type == BasePlugin::iChild )
		{
			ChildPlugin* cp = (ChildPlugin*)bp;
			if ( cp && cp->infos().Installed && cp->extensions().contains( mExtension, Qt::CaseInsensitive ) )
				return cp->openFile( s, p );
		}
	}
	return QDesktopServices::openUrl( s );
}

QStringList PluginsManager::childsFilters() const
{
	QStringList l;
	foreach ( BasePlugin* bp, mPlugins )
	{
		if ( bp->infos().Type == BasePlugin::iChild )
		{
			ChildPlugin* cp = (ChildPlugin*)bp;
			if ( cp && cp->infos().Installed )
				l << cp->filters();
		}
	}
	return l;
}
//
void PluginsManager::manageRequested()
{
	UIPluginsSettings::self( this, qApp->activeWindow() )->exec(); 
}
*/
