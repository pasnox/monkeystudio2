#include "MSVC.h"
#include "MSVCParser.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"

#include <QTabWidget>

MSVC::MSVC()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "MSVC" );
	mPluginInfos.Description = tr( "Plugin for execute MSVC in console" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iCompiler;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Enabled = false;
	
	// install parsers
	foreach ( QString s, availableParsers() )
		MonkeyCore::consoleManager()->addParser( getParser( s ) );
}

MSVC::~MSVC()
{
	// uninstall parsers
	foreach ( QString s, availableParsers() )
		MonkeyCore::consoleManager()->removeParser( s );
}

bool MSVC::setEnabled( bool b)
{
	if ( b == mPluginInfos.Enabled )
		return true;
	mPluginInfos.Enabled = b;
 	if ( b )
	{
		pMenuBar* mb = MonkeyCore::menuBar();
		
		// compile command
		pCommand c = compileCommand();
		QAction* a = mb->action( QString( "mBuilder/mBuild/%1" ).arg( c.text() ), c.text() );
		a->setData( mPluginInfos.Name );
		a->setStatusTip( c.text() );
		connect( a, SIGNAL( triggered() ), this, SLOT( commandTriggered() ) );
		
		// user command
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
		pMenuBar* mb = MonkeyCore::menuBar();
		foreach ( QAction* a, mb->menu( "mBuilder/mUserCommands" )->actions() << mb->menu( "mBuilder/mBuild" )->actions() )
			if ( a->data().toString() == mPluginInfos.Name )
				delete a;
	}
	return true;
}

QWidget* MSVC::settingsWidget()
{
    QTabWidget* tw = new QTabWidget;
    tw->setAttribute( Qt::WA_DeleteOnClose );
    tw->addTab( compilerSettingsWidget(), tr( "Compile Command" ) );
    tw->addTab( cliToolSettingsWidget( this ), tr( "User Commands" ) );
    return tw;
}

pCommand MSVC::defaultCompileCommand() const
{ return pCommand( "Compile Current File", "cl", "$cf$", false, availableParsers(), "$cfp$" ); }

pCommand MSVC::compileCommand() const
{
	// get settings object
    pSettings s;
    pCommand c;
    c.setText( s.value( settingsKey( "CompileCommand/Text" ) ).toString() );
    c.setCommand( s.value( settingsKey( "CompileCommand/Command" ) ).toString() );
    c.setArguments( s.value( settingsKey( "CompileCommand/Arguments" ) ).toString() );
    c.setWorkingDirectory( s.value( settingsKey( "CompileCommand/WorkingDirectory" ) ).toString() );
    c.setParsers( s.value( settingsKey( "CompileCommand/Parsers" ) ).toStringList() );
    c.setTryAllParsers( s.value( settingsKey( "CompileCommand/TryAll" ), false ).toBool() );
    c.setSkipOnError( s.value( settingsKey( "CompileCommand/SkipOnError" ), false ).toBool() );
    // if no user commands get global ones
    if ( !c.isValid() )
        c = defaultCompileCommand();
    return c;
}

void MSVC::setCompileCommand( const pCommand& c )
{
	pSettings s;
    s.setValue( settingsKey( "CompileCommand/Text" ), c.text() );
    s.setValue( settingsKey( "CompileCommand/Command" ), c.command() );
    s.setValue( settingsKey( "CompileCommand/Arguments" ), c.arguments() );
    s.setValue( settingsKey( "CompileCommand/WorkingDirectory" ), c.workingDirectory() );
    s.setValue( settingsKey( "CompileCommand/Parsers" ), c.parsers() );
    s.setValue( settingsKey( "CompileCommand/TryAll" ), c.tryAllParsers() );
    s.setValue( settingsKey( "CompileCommand/SkipOnError" ), c.skipOnError() );
}

pCommandList MSVC::defaultCommands() const
{ return pCommandList(); }

pCommandList MSVC::userCommands() const
{
	// commands list
	pCommandList l;
	// get settings object
	pSettings s;
	// read user commands for this plugin
	int size = s.beginReadArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < size; i++ )
	{
		s.setArrayIndex( i );
		pCommand c;
		c.setText( s.value( "Text" ).toString() );
		c.setCommand( s.value( "Command" ).toString() );
		c.setArguments( s.value( "Arguments" ).toString() );
		c.setWorkingDirectory( s.value( "WorkingDirectory" ).toString() );
		c.setParsers( s.value( "Parsers" ).toStringList() );
		c.setTryAllParsers( s.value( "TryAll" ).toBool() );
		c.setSkipOnError( s.value( "SkipOnError" ).toBool() );
		l << c;
	}
	s.endArray();
	// if no user commands get global ones
	if ( l.isEmpty() )
		l << defaultCommands();
	// return list
	return l;
}

void MSVC::setUserCommands( const pCommandList& l ) const
{
	// get settings object
	pSettings s;
	// remove old key
	s.remove( settingsKey( "Commands" ) );
	// write user commands for this plugin
	s.beginWriteArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < l.count(); i++ )
	{
		s.setArrayIndex( i );
		const pCommand& c = l[i];
		s.setValue( "Text", c.text() );
		s.setValue( "Command", c.command() );
		s.setValue( "Arguments", c.arguments() );
		s.setValue( "WorkingDirectory", c.workingDirectory() );
		s.setValue( "Parsers", c.parsers() );
		s.setValue( "TryAll", c.tryAllParsers() );
		s.setValue( "SkipOnError", c.skipOnError() );
	}
	s.endArray();
}

QStringList MSVC::availableParsers() const
{ return QStringList()/* "MSVCParser" )*/; }

pCommandParser* MSVC::getParser( const QString& )
{
/*	if ( s == "MSVCParser" )
		return new MSVCParser;*/
	return 0;
}

void MSVC::commandTriggered()
{
	pConsoleManager* cm = MonkeyCore::consoleManager();
	pCommandList l = userCommands();
	if ( QAction* a = qobject_cast<QAction*>( sender() ) )
		cm->addCommands( cm->recursiveCommandList( l, cm->getCommand( l, a->statusTip() ) ) );
}

Q_EXPORT_PLUGIN2( CompilerMSVC, MSVC )
