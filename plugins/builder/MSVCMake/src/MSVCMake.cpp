#include <QTabWidget>
#include "MSVCMake.h"
#include "MSVCMakeParser.h"
#include "pConsoleManager.h"
#include "pMenuBar.h"

MSVCMake::MSVCMake ()
{
    // set plugin infos
    mPluginInfos.Caption = tr( "MSVCMake" );
    mPluginInfos.Description = tr( "Plugin for execute MSVC Make in console and parse it's output" );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBuilder;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.5.0";
    mPluginInfos.Enabled = false;
	// install parsers
	foreach ( QString s, availableParsers() )
		pConsoleManager::instance()->addParser( getParser( s ) );
}

MSVCMake::~MSVCMake ()
{
	// uninstall parsers
	foreach ( QString s, availableParsers() )
		pConsoleManager::instance()->removeParser( s );
}

bool MSVCMake::setEnabled( bool b)
{
    if ( b == mPluginInfos.Enabled )
        return true;
    mPluginInfos.Enabled = b;
     if ( b )
    {
        pMenuBar* mb = pMenuBar::instance();
        foreach ( pCommand c, pCommandList() << userCommands() )
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
        foreach ( QAction* a, mb->menu( "mBuilder/mUserCommands" )->actions() )
            if ( a->data().toString() == mPluginInfos.Name )
                delete a;
    }
    return true;
}

QWidget* MSVCMake::settingsWidget()
{
    QTabWidget* tw = new QTabWidget;
    tw->setAttribute( Qt::WA_DeleteOnClose );
    tw->addTab( builderSettingsWidget(), tr( "Build Command" ) );
    tw->addTab( cliToolSettingsWidget( this ), tr( "User Commands" ) );
    return tw;
}

pCommandList MSVCMake::defaultCommands() const
{ return pCommandList(); }

pCommandList MSVCMake::userCommands() const
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

void MSVCMake::setUserCommands( const pCommandList& l ) const
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

QStringList MSVCMake::availableParsers() const
{ return QStringList( /*mPluginInfos.Name*/ ); }

pCommandParser* MSVCMake::getParser( const QString& s )
{ return s == mPluginInfos.Name ? new MSVCMakeParser : 0; }

pCommand MSVCMake::defaultBuildCommand() const
{ return pCommand( "Build", "nmake", "", false, availableParsers(), "$cpp$" ); }

pCommand MSVCMake::buildCommand() const
{
    // get settings object
    QSettings* s = settings();
    pCommand c;
    c.setText( s->value( settingsKey( "BuildCommand/Text" ) ).toString() );
    c.setCommand( s->value( settingsKey( "BuildCommand/Command" ) ).toString() );
    c.setArguments( s->value( settingsKey( "BuildCommand/Arguments" ) ).toString() );
    c.setWorkingDirectory( s->value( settingsKey( "BuildCommand/WorkingDirectory" ) ).toString() );
    c.setParsers( s->value( settingsKey( "BuildCommand/Parsers" ) ).toStringList() );
    c.setTryAllParsers( s->value( settingsKey( "BuildCommand/TryAll" ), false ).toBool() );
    c.setSkipOnError( s->value( settingsKey( "BuildCommand/SkipOnError" ), false ).toBool() );
    // if no user commands get global ones
    if ( !c.isValid() )
        c = defaultBuildCommand();
    return c;
}

void MSVCMake::setBuildCommand( const pCommand& c )
{
    QSettings* s = settings();
    s->setValue( settingsKey( "BuildCommand/Text" ), c.text() );
    s->setValue( settingsKey( "BuildCommand/Command" ), c.command() );
    s->setValue( settingsKey( "BuildCommand/Arguments" ), c.arguments() );
    s->setValue( settingsKey( "BuildCommand/WorkingDirectory" ), c.workingDirectory() );
    s->setValue( settingsKey( "BuildCommand/Parsers" ), c.parsers() );
    s->setValue( settingsKey( "BuildCommand/TryAll" ), c.tryAllParsers() );
    s->setValue( settingsKey( "BuildCommand/SkipOnError" ), c.skipOnError() );
}

void MSVCMake::commandTriggered()
{
    pConsoleManager* cm = pConsoleManager::instance();
    pCommandList l = userCommands() << buildCommand();
    if ( QAction* a = qobject_cast<QAction*>( sender() ) )
        cm->addCommands( cm->recursiveCommandList( l, cm->getCommand( l, a->statusTip() ) ) );
}

Q_EXPORT_PLUGIN2( BuilderMSVCMake, MSVCMake )

