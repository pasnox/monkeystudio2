#include "PluginsManager.h"
#include "ChildPlugin.h"
#include "WorkspacePlugin.h"
#include "CompilerPlugin.h"
#include "DebuggerPlugin.h"
#include "ProjectPlugin.h"
#include "Workspace.h"
#include "UIPluginsSettings.h"
#include "Settings.h"
//
#include <QPluginLoader>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
//
QPointer<PluginsManager> PluginsManager::mSelf = 0L;
//
PluginsManager* PluginsManager::self( QObject* p )
{
	if ( !mSelf )
		mSelf = new PluginsManager( p );
	return mSelf;
}
//
PluginsManager::PluginsManager( QObject* p )
	: QObject( p )
{
}
//
PluginsManager::~PluginsManager()
{
}
//
void PluginsManager::loadsPlugins( const QString& s )
{
	// dor application path
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
	// go into plugins dir
	d.cd( "plugins" );
	// loads static plugins
	foreach ( QObject* o, QPluginLoader::staticInstances() )
		if ( !addPlugin( o ) )
			qWarning( qPrintable( tr( "Failed to load static plugin" ) ) );
	// check all subdirs to load plugins
	loadsPlugins( d );
	// installs user requested plugins
	installPlugins();
}
//
void PluginsManager::loadsPlugins( QDir d )
{
	// looking fodlers to load
	foreach ( QString s, d.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) )
	{
		d.cd( s );
		loadsPlugins( d );
		d.cdUp();
	}
	// looking files to load
	foreach ( QString s, d.entryList( QDir::Files ) )
	{
		QPluginLoader l( d.absoluteFilePath( s ) );
		if ( !addPlugin( l.instance() ) )
		{
			// try unload it and reload it in case of old one in memory
			l.unload();
			l.load();
			if ( !addPlugin( l.instance() ) )			
				qWarning( qPrintable( tr( "Failed to load plugin ( %1 ): Error: %2" ).arg( s, l.errorString() ) ) );
		}
	}
}
//
bool PluginsManager::addPlugin( QObject* o )
{
	BasePlugin* bp = qobject_cast<BasePlugin*>( o );
	if ( !bp )
		return false;
	// initialize plugin
	bp->initialize( Workspace::self() );
	// show plugin infos
	qWarning( qPrintable( tr( "Found plugin: %1, type: %2" ).arg( bp->infos().Name ).arg( bp->infos().Type ) ) );
	// add it to plugins list
	mPlugins << bp;
	//
	return true;
}
//
void PluginsManager::installPlugins()
{
	foreach ( BasePlugin* bp, mPlugins )
	{
		// check in settings if we must install this plugin
		if ( !Settings::current()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() )
			qWarning( qPrintable( tr( "User wantn't to intall plugin: %1" ).arg( bp->infos().Name ) ) );
		// if not installed, install it
		else if ( !bp->infos().Installed )
		{
			if ( bp->install() )
				qWarning( qPrintable( tr( "Successfully installed plugin: %1" ).arg( bp->infos().Name ) ) );
			else
				qWarning( qPrintable( tr( "Unsuccessfully installed plugin: %1" ).arg( bp->infos().Name ) ) );
		}
		else
			qWarning( qPrintable( tr( "Already installed plugin: %1" ).arg( bp->infos().Name ) ) );
	}
}
//
QList<BasePlugin*> PluginsManager::plugins() const
{
	return mPlugins;
}
//
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
//
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
//
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
