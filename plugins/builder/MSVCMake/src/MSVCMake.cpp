/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Builder Plugins
** FileName  : MSVCMake.cpp
** Date      : 2008-01-14T00:52:27
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#include <QTabWidget>
#include "MSVCMake.h"
#include "MSVCMakeParser.h"

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
		MonkeyCore::consoleManager()->addParser( getParser( s ) );
}

MSVCMake::~MSVCMake()
{
	// uninstall parsers
	foreach ( QString s, availableParsers() )
		MonkeyCore::consoleManager()->removeParser( s );
}

bool MSVCMake::setEnabled( bool b)
{
	if ( b && !isEnabled() )
		mPluginInfos.Enabled = true;
	else if ( !b && isEnabled() )
		mPluginInfos.Enabled = false;
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
    pSettings* s = MonkeyCore::settings();
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
    pSettings* s = MonkeyCore::settings();
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
{ return QStringList( /*mPluginInfos->Name*/ ); }

pCommandParser* MSVCMake::getParser( const QString& )
{ return NULL/*s == mPluginInfos->Name ? new MSVCMakeParser : 0*/; }

pCommand MSVCMake::defaultBuildCommand() const
{ return pCommand( "Build", "nmake", "", false, availableParsers(), "$cpp$" ); }

pCommand MSVCMake::buildCommand() const
{
    // get settings object
    pSettings* s = MonkeyCore::settings();
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
    pSettings* s = MonkeyCore::settings();
    s->setValue( settingsKey( "BuildCommand/Text" ), c.text() );
    s->setValue( settingsKey( "BuildCommand/Command" ), c.command() );
    s->setValue( settingsKey( "BuildCommand/Arguments" ), c.arguments() );
    s->setValue( settingsKey( "BuildCommand/WorkingDirectory" ), c.workingDirectory() );
    s->setValue( settingsKey( "BuildCommand/Parsers" ), c.parsers() );
    s->setValue( settingsKey( "BuildCommand/TryAll" ), c.tryAllParsers() );
    s->setValue( settingsKey( "BuildCommand/SkipOnError" ), c.skipOnError() );
}

Q_EXPORT_PLUGIN2( BuilderMSVCMake, MSVCMake )

