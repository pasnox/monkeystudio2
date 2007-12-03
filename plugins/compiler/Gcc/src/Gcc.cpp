#include "Gcc.h"
#include "pMenuBar.h"

#include <QTabWidget>

Gcc::Gcc()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Gcc" );
	mPluginInfos.Description = tr( "Plugin for execute Gcc in console" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>, Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iCompiler;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Enabled = false;
	
	// install parsers
	foreach ( QString s, availableParsers() )
		pConsoleManager::instance()->addParser( getParser( s ) );
}

Gcc::~Gcc()
{
	// uninstall parsers
	foreach ( QString s, availableParsers() )
		pConsoleManager::instance()->removeParser( s );
}

bool Gcc::setEnabled( bool b)
{
	if ( b == mPluginInfos.Enabled )
		return true;
	mPluginInfos.Enabled = b;
 	if ( b )
	{
		pMenuBar* mb = pMenuBar::instance();
		
		// compile command
		pCommand c = compileCommand();
		QAction* a = mb->action( QString( "mBuilder/mBuild/%1" ).arg( c.text() ), c.text() );
		a->setData( mPluginInfos.Name );
		a->setStatusTip( c.text() );
		connect( a, SIGNAL( triggered() ), this, SLOT( commandTriggered() ) );
		
		// user commands
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
		pMenuBar* mb = pMenuBar::instance();
		foreach ( QAction* a, mb->menu( "mBuilder/mUserCommands" )->actions() << mb->menu( "mBuilder/mBuild" )->actions() )
			if ( a->data().toString() == mPluginInfos.Name )
				delete a;
	}
	return true;
}

QWidget* Gcc::settingsWidget()
{
    QTabWidget* tw = new QTabWidget;
    tw->setAttribute( Qt::WA_DeleteOnClose );
    tw->addTab( compilerSettingsWidget(), tr( "Compile Command" ) );
    tw->addTab( cliToolSettingsWidget( this ), tr( "User Commands" ) );
    return tw;
}

pCommand Gcc::defaultCompileCommand() const
{ return pCommand( "Compile Current File", "gcc", "-w \"$cf$\"", false, QStringList( "GccParser" ), "$cfp$" ); }

pCommand Gcc::compileCommand() const
{
	// get settings object
    QSettings* s = settings();
    pCommand c;
    c.setText( s->value( settingsKey( "CompileCommand/Text" ) ).toString() );
    c.setCommand( s->value( settingsKey( "CompileCommand/Command" ) ).toString() );
    c.setArguments( s->value( settingsKey( "CompileCommand/Arguments" ) ).toString() );
    c.setWorkingDirectory( s->value( settingsKey( "CompileCommand/WorkingDirectory" ) ).toString() );
    c.setParsers( s->value( settingsKey( "CompileCommand/Parsers" ) ).toStringList() );
    c.setTryAllParsers( s->value( settingsKey( "CompileCommand/TryAll" ), false ).toBool() );
    c.setSkipOnError( s->value( settingsKey( "CompileCommand/SkipOnError" ), false ).toBool() );
    // if no user commands get global ones
    if ( !c.isValid() )
        c = defaultCompileCommand();
    return c;
}

void Gcc::setCompileCommand( const pCommand& c )
{
	QSettings* s = settings();
    s->setValue( settingsKey( "CompileCommand/Text" ), c.text() );
    s->setValue( settingsKey( "CompileCommand/Command" ), c.command() );
    s->setValue( settingsKey( "CompileCommand/Arguments" ), c.arguments() );
    s->setValue( settingsKey( "CompileCommand/WorkingDirectory" ), c.workingDirectory() );
    s->setValue( settingsKey( "CompileCommand/Parsers" ), c.parsers() );
    s->setValue( settingsKey( "CompileCommand/TryAll" ), c.tryAllParsers() );
    s->setValue( settingsKey( "CompileCommand/SkipOnError" ), c.skipOnError() );
}

pCommandList Gcc::defaultCommands() const
{ return pCommandList(); }

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

void Gcc::commandTriggered()
{
	pConsoleManager* cm = pConsoleManager::instance();
	pCommandList l = userCommands() << compileCommand();
	if ( QAction* a = qobject_cast<QAction*>( sender() ) )
		cm->addCommands( cm->recursiveCommandList( l, cm->getCommand( l, a->statusTip() ) ) );
}

Q_EXPORT_PLUGIN2( CompilerGcc, Gcc )
