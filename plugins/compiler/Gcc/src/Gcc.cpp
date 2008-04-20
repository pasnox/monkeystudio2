/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Compiler Plugins
** FileName  : Gcc.cpp
** Date      : 2008-01-14T00:53:21
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
#include "Gcc.h"

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
		MonkeyCore::consoleManager()->addParser( getParser( s ) );
}

Gcc::~Gcc()
{
	// uninstall parsers
	foreach ( QString s, availableParsers() )
		MonkeyCore::consoleManager()->removeParser( s );
}

bool Gcc::setEnabled( bool b)
{
	if ( b && !isEnabled() )
		mPluginInfos.Enabled = true;
	else if ( !b && isEnabled() )
		mPluginInfos.Enabled = false;
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
    pSettings* s = MonkeyCore::settings();
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
	pSettings* s = MonkeyCore::settings();
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

void Gcc::setUserCommands( const pCommandList& l ) const
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

Q_EXPORT_PLUGIN2( CompilerGcc, Gcc )
