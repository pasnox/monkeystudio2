#include "Gcc.h"
#include "GccParser.h"
#include "pMenuBar.h"

Gcc::Gcc()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Gcc" );
	mPluginInfos.Description = tr( "Plugin for execute Gcc in console and parse it's output" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>, Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iCompiler;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Enabled = false;
}

bool Gcc::setEnabled( bool b)
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
			QAction* a = mb->action( QString( "mBuilder/mUserCommands/%1" ).arg( c.text() ), c.text() );
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
		foreach ( QAction* a, mb->menu( "mBuilder/mUserCommands" )->actions() )
			if ( a->data().toString() == mPluginInfos.Name )
				delete a;
	}
	return true;
}

QWidget* Gcc::settingsWidget()
{ return cliToolSettingsWidget( this ); }

pCommandList Gcc::defaultCommands() const
{ return pCommandList() << pCommand( "Build Current File", "gcc", "$cf$", false, availableParsers(), "$cfp$" ); }

pCommandList Gcc::userCommands() const
{
	// commands list
	pCommandList l;
	// get settings object
	QSettings* s = settings();
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
		c.setTryAllParsers( s->value( "TryAll" ).toBool() );
		c.setSkipOnError( s->value( "SkipOnError" ).toBool() );
		l << c;
	}
	s->endArray();
	// if no user commands get global ones
	if ( l.isEmpty() )
		l << defaultCommands();
	// return list
	return l;
}

void Gcc::setUserCommands( const pCommandList& l ) const
{
	// get settings object
	QSettings* s = settings();
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
		s->setValue( "TryAll", c.tryAllParsers() );
		s->setValue( "SkipOnError", c.skipOnError() );
	}
	s->endArray();
}

QStringList Gcc::availableParsers() const
{ return QStringList( mPluginInfos.Name ); }

pCommandParser* Gcc::getParser( const QString& s )
{ return s == mPluginInfos.Name ? new GccParser : 0; }

void Gcc::commandTriggered()
{
	pConsoleManager* cm = pConsoleManager::instance();
	pCommandList l = userCommands();
	if ( QAction* a = qobject_cast<QAction*>( sender() ) )
		cm->addCommands( cm->recursiveCommandList( l, cm->getCommand( l, a->statusTip() ) ) );
}

Q_EXPORT_PLUGIN2( CompilerGcc, Gcc )

