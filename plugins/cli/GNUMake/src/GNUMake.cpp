#include "GNUMake.h"
#include "GNUMakeParser.h"
#include "pConsoleManager.h"

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
 		pConsoleManager::instance()->addParser( new GNUMakeParser() );
 	else
 		pConsoleManager::instance()->removeParser( mPluginInfos.Name );
	return true;
}

pCommandList GNUMake::globalCommands() const
{
	return pCommandList()
		<< pCommand( "Build", "make", QString::null, false, QStringList( mPluginInfos.Name ) )
		<< pCommand( "Build Release", "make", "release", false, QStringList( mPluginInfos.Name ) )
		<< pCommand( "Build Debug", "make", "debug", false, QStringList( mPluginInfos.Name ) )
		<< pCommand( "Clean", "make", "clean", false, QStringList( mPluginInfos.Name ) )
		<< pCommand( "Distclean", "make", "distclean", false, QStringList( mPluginInfos.Name ) ); 
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

void GNUMake::setUserCommands( const pCommandList& ) const
{
}

QString GNUMake::name()
{ return mPluginInfos.Name; }

QString GNUMake::getCommand ()
{ return QString ("make");  }

QString GNUMake::getSwitches( QString s )
{ return s; }

pCommandParser* GNUMake::getParser()
{ return new GNUMakeParser(); }

Q_EXPORT_PLUGIN2( CompilerGNUMake, GNUMake )

