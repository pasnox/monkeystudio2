/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Builder Plugins
** FileName  : GNUMake.cpp
** Date      : 2008-01-14T00:52:24
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
#include "GNUMake.h"
#include "GNUMakeParser.h"

#include <QTabWidget>

GNUMake::GNUMake ()
{
    // set plugin infos
    mPluginInfos.Caption = tr( "GNUMake" );
    mPluginInfos.Description = tr( "Plugin for execute GNU Make in console and parse it's output" );
    mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>, Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBuilder;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.5.0";
    mPluginInfos.Enabled = false;
	// install parsers
	foreach ( QString s, availableParsers() )
		MonkeyCore::consoleManager()->addParser( getParser( s ) );
}

GNUMake::~GNUMake()
{
	// uninstall parsers
	foreach ( QString s, availableParsers() )
		MonkeyCore::consoleManager()->removeParser( s );
}

bool GNUMake::setEnabled( bool b)
{
	if ( b && !isEnabled() )
		mPluginInfos.Enabled = true;
	else if ( !b && isEnabled() )
		mPluginInfos.Enabled = false;
	return true;
}

QWidget* GNUMake::settingsWidget()
{
    QTabWidget* tw = new QTabWidget;
    tw->setAttribute( Qt::WA_DeleteOnClose );
    tw->addTab( builderSettingsWidget(), tr( "Build Command" ) );
    tw->addTab( cliToolSettingsWidget( this ), tr( "User Commands" ) );
    return tw;
}

pCommandList GNUMake::defaultCommands() const
{ return pCommandList(); }

pCommandList GNUMake::userCommands() const
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

void GNUMake::setUserCommands( const pCommandList& l ) const
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

QStringList GNUMake::availableParsers() const
{ return QStringList( mPluginInfos.Name ); }

pCommandParser* GNUMake::getParser( const QString& s )
{ return s == mPluginInfos.Name ? new GNUMakeParser(this) : 0; }

pCommand GNUMake::defaultBuildCommand() const
{
#ifdef Q_OS_WIN
    const QString mMake = "mingw32-make";
#else
    const QString mMake = "make";
#endif
    return pCommand( "Build", mMake, "-w", false, availableParsers(), "$cpp$" );
}

pCommand GNUMake::buildCommand() const
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

void GNUMake::setBuildCommand( const pCommand& c )
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

Q_EXPORT_PLUGIN2( BuilderGNUMake, GNUMake )
