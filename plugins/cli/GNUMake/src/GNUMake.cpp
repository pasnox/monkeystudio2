#include "GNUMake.h"
#include "GNUMakeParser.h"
#include "GccParser.h"
#include "pConsoleManager.h"
#include "UIProjectsManager.h"
#include "pMenuBar.h"

GNUMake::GNUMake ()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "GNUMake" );
	mPluginInfos.Description = tr( "Plugin for execute GNU Make in console and parse it's output" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>, Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iCompiler;
	mPluginInfos.Name = "GNUMake";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

bool GNUMake::setEnabled( bool b)
{
	if ( b == mPluginInfos.Enabled )
		return true;
	mPluginInfos.Enabled = b;
 	if ( b )
	{
		foreach ( QString s, availableParsers() )
			pConsoleManager::instance()->addParser( getParser( s ) );
		
		pMenuBar* mb = pMenuBar::instance();
		foreach ( pCommand c, userCommands() )
		{
			QAction* a = mb->action( QString( "mBuild/%1" ).arg( c.text() ), c.text() );
			a->setData( mPluginInfos.Name );
			a->setStatusTip( c.text() );
			connect( a, SIGNAL( triggered() ), this, SLOT( commandTriggered() ) );
		}
	}
 	else
	{
		foreach ( QString s, availableParsers() )
			pConsoleManager::instance()->removeParser( s );
		
		pMenuBar* mb = pMenuBar::instance();
		foreach ( QAction* a, mb->menu( "mBuild" )->actions() )
			if ( a->data().toString() == mPluginInfos.Name )
				delete a;
	}
	return true;
}

pCommandList GNUMake::globalCommands() const
{
#ifdef Q_OS_WIN
	const QString mMake = "mingw32-make";
#else
	const QString mMake = "make";
#endif
	return pCommandList()
		<< pCommand( "Build", mMake, QString::null, false, availableParsers(), "$cpp$" )
		<< pCommand( "Build Release", mMake, "release", false, availableParsers(), "$cpp$" )
		<< pCommand( "Build Debug", mMake, "debug", false, availableParsers(), "$cpp$" )
		<< pCommand( "Clean", mMake, "clean", false, availableParsers(), "$cpp$" )
		<< pCommand( "Distclean", mMake, "distclean", false, availableParsers(), "$cpp$" ); 
}

pCommandList GNUMake::userCommands() const
{
	// commands list
	pCommandList l;
	// get settings object
	QSettings* s = settingsObject();
	// read user commands for this plugin
	int size = s->beginReadArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < size; i++ )
	{
		s->setArrayIndex( i );
		pCommand c;
		c.setText( s->value( "Text" ).toString() );
		c.setCommand( s->value( "Command" ).toString() );
		c.setArguments( s->value( "Arguments" ).toString() );
		c.setWorkingDirectory( s->value( "WorkingDirectory" ).toString() );
		c.setParsers( s->value( "Parsers" ).toStringList() );
		l << c;
	}
	s->endArray();
	// if no user commands get global ones
	if ( l.isEmpty() )
		l << globalCommands();
	// return list
	return l;
}

void GNUMake::setUserCommands( const pCommandList& l ) const
{
	// get settings object
	QSettings* s = settingsObject();
	// remove old key
	s->remove( settingsKey( "Commands" ) );
	// write user commands for this plugin
	s->beginWriteArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < l.count(); i++ )
	{
		s->setArrayIndex( i );
		const pCommand& c = l[i];
		s->setValue( "Text", c.text() );
		s->setValue( "Command", c.command() );
		s->setValue( "Arguments", c.arguments() );
		s->setValue( "WorkingDirectory", c.workingDirectory() );
		s->setValue( "Parsers", c.parsers() );
	}
	s->endArray();
}

QStringList GNUMake::availableParsers() const
{ return QStringList() << "GNUMake" << "Gcc"; }

pCommandParser* GNUMake::getParser( const QString& s )
{
	if ( s == "GNUMake" )
		return new GNUMakeParser;
	else if ( s == "Gcc" )
		return new GccParser;
	return 0;
}

void GNUMake::commandTriggered()
{
	if ( QAction* a = qobject_cast<QAction*>( sender() ) )
	{
		foreach ( pCommand c, userCommands() )
		{
			if ( c.text() == a->statusTip() )
			{
				if ( c.workingDirectory().contains( "$cpp$" ) )
					c.setWorkingDirectory( c.workingDirectory().replace( "$cpp$", UIProjectsManager::instance()->currentProject()->canonicalPath() ) );
				pConsoleManager::instance()->addCommand( c );
				return;
			}
		}
	}
}

Q_EXPORT_PLUGIN2( CompilerGNUMake, GNUMake )

