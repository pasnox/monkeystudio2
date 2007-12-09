#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"
#include "pMenuBar.h"
#include "UIPluginsSettings.h"

#include <QPluginLoader>

PluginsManager::PluginsManager( QObject* p )
	: QObject( p )
{
	mBuilder = 0;
	mCompiler = 0;
	mDebugger = 0;
	mInterpreter = 0;
}

QList<BasePlugin*> PluginsManager::plugins() const
{ return mPlugins; }

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
		if ( !d.exists( "plugins" ) )
		{
			d.cdUp();
			d.cdUp();
		}
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

ProjectItem* PluginsManager::projectItem( const QString& s )
{
	foreach ( ProjectPlugin* pp, plugins<ProjectPlugin*>( PluginsManager::stEnabled ) )
		foreach ( QString k, pp->suffixes().keys() )
			if ( QDir::match( pp->suffixes().value( k ), s ) )
				return pp->getProjectItem( s );
	return 0;
}

pAbstractChild* PluginsManager::openChildFile( const QString& s, const QPoint& p )
{
	foreach ( ChildPlugin* cp, plugins<ChildPlugin*>( PluginsManager::stEnabled ) )
		if ( cp->canOpen( s ) )
			return cp->openFile( s, p );
	return 0;
}

QHash<QString, QStringList> PluginsManager::childSuffixes() const
{
	QHash<QString, QStringList> l;
	foreach ( ChildPlugin* cp, const_cast<PluginsManager*>( this )->plugins<ChildPlugin*>( PluginsManager::stEnabled ) )
		foreach ( QString k, cp->suffixes().keys() )
			l[k] << cp->suffixes().value( k );
	return l;
}

QString PluginsManager::childFilters() const
{
	QString f;
	QHash<QString, QStringList> l = childSuffixes();
	foreach ( QString k, l.keys() )
		f += QString( "%1 (%2);;" ).arg( k ).arg( l.value( k ).join( " " ) );
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );
	return f;
}

void PluginsManager::setCurrentBuilder( BuilderPlugin* b )
{
	// if same cancel
	if ( mBuilder == b )
		return;
	
	// disabled all builder
	foreach ( BuilderPlugin* bp, plugins<BuilderPlugin*>( PluginsManager::stAll ) )
		bp->setEnabled( false );
	
	// enabled the one we choose
	mBuilder = b;
	if ( mBuilder )
		mBuilder->setEnabled( true );
	
	// enable menu according to current builder
	pMenuBar::instance()->menu( "mBuilder" )->setEnabled( mBuilder || mCompiler );
}

BuilderPlugin* PluginsManager::currentBuilder()
{ return mBuilder; }

void PluginsManager::setCurrentCompiler( CompilerPlugin* c )
{
	// if same cancel
	if ( mCompiler == c )
		return;
	
	// disabled all builder
	foreach ( CompilerPlugin* cp, plugins<CompilerPlugin*>( PluginsManager::stAll ) )
		cp->setEnabled( false );
	
	// enabled the one we choose
	mCompiler = c;
	if ( mCompiler )
		mCompiler->setEnabled( true );
	
	// enable menu according to current compiler
	pMenuBar::instance()->menu( "mBuilder" )->setEnabled( mCompiler || mBuilder );
}

CompilerPlugin* PluginsManager::currentCompiler()
{ return mCompiler; }

void PluginsManager::setCurrentDebugger( DebuggerPlugin* d )
{
	// if same cancel
	if ( mDebugger == d )
		return;
	
	// disabled all debugger
	foreach ( DebuggerPlugin* dp, plugins<DebuggerPlugin*>( PluginsManager::stAll ) )
		dp->setEnabled( false );
	
	// enabled the one we choose
	mDebugger = d;
	if ( mDebugger )
		mDebugger->setEnabled( true );
	
	// enable menu according to current debugger
	pMenuBar::instance()->menu( "mDebugger" )->setEnabled( mDebugger );
}

DebuggerPlugin* PluginsManager::currentDebugger()
{ return mDebugger; }
	
void PluginsManager::setCurrentInterpreter( InterpreterPlugin* i )
{
	// if same cancel
	if ( mInterpreter == i )
		return;
	
	// disabled all debugger
	foreach ( InterpreterPlugin* ip, plugins<InterpreterPlugin*>( PluginsManager::stAll ) )
		ip->setEnabled( false );
	
	// enabled the one we choose
	mInterpreter = i;
	if ( mInterpreter )
		mInterpreter->setEnabled( true );
	
	// enable menu according to current interpreter
	pMenuBar::instance()->menu( "mInterpreter" )->setEnabled( mInterpreter );
}

InterpreterPlugin* PluginsManager::currentInterpreter()
{ return mInterpreter; }

void PluginsManager::manageRequested()
{ UIPluginsSettings::instance()->exec(); }
