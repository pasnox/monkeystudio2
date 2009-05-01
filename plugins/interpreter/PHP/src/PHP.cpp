/****************************************************************************
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
****************************************************************************/
#include "PHP.h"
#include "PHPParser.h"

#include <QTabWidget>

PHP::PHP ()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "PHP" );
	mPluginInfos.Description = tr( "This plugin provide PHP interpreter and php parser." );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iInterpreter;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.FirstStartEnabled = true;

	// install parsers
	foreach ( QString s, availableParsers() )
	{
		MonkeyCore::consoleManager()->addParser( getParser( s ) );
	}
}

PHP::~PHP()
{
	// uninstall parsers
	foreach ( QString s, availableParsers() )
	{
		MonkeyCore::consoleManager()->removeParser( s );
	}
}

bool PHP::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		stateAction()->setChecked( true );
	}
	else if ( !b && isEnabled() )
	{
		stateAction()->setChecked( false );
	}
	
	return true;
}

QWidget* PHP::settingsWidget()
{
	QTabWidget* tw = new QTabWidget;
	tw->setAttribute( Qt::WA_DeleteOnClose );
	tw->addTab( interpreterSettingsWidget(), tr( "Interpret Command" ) );
	tw->addTab( cliToolSettingsWidget( this ), tr( "User Commands" ) );
	return tw;
}

pCommandList PHP::defaultCommands() const
{
	return pCommandList();
}

pCommandList PHP::userCommands() const
{
	// commands list
	pCommandList commands;

	// get settings object
	pSettings* settings = MonkeyCore::settings();

	// read user commands for this plugin
	int size = settings->beginReadArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < size; i++ )
	{
		settings->setArrayIndex( i );
		pCommand cmd;
		cmd.setText( settings->value( "Text" ).toString() );
		cmd.setCommand( settings->value( "Command" ).toString() );
		cmd.setArguments( settings->value( "Arguments" ).toString() );
		cmd.setWorkingDirectory( settings->value( "WorkingDirectory" ).toString() );
		cmd.setParsers( settings->value( "Parsers" ).toStringList() );
		cmd.setTryAllParsers( settings->value( "TryAll" ).toBool() );
		cmd.setSkipOnError( settings->value( "SkipOnError" ).toBool() );
		commands << cmd;
	}
	settings->endArray();

	// if no user commands get global ones
	if ( commands.isEmpty() )
	{
		commands << defaultCommands();
	}

	// return list
	return commands;
}

void PHP::setUserCommands( const pCommandList& commands ) const
{
	// get settings object
	pSettings* settings = MonkeyCore::settings();

	// remove old key
	settings->remove( settingsKey( "Commands" ) );

	// write user commands for this plugin
	settings->beginWriteArray( settingsKey( "Commands" ) );
	for ( int i = 0; i < commands.count(); i++ )
	{
		settings->setArrayIndex( i );
		const pCommand& cmd = commands[i];
		settings->setValue( "Text", cmd.text() );
		settings->setValue( "Command", cmd.command() );
		settings->setValue( "Arguments", cmd.arguments() );
		settings->setValue( "WorkingDirectory", cmd.workingDirectory() );
		settings->setValue( "Parsers", cmd.parsers() );
		settings->setValue( "TryAll", cmd.tryAllParsers() );
		settings->setValue( "SkipOnError", cmd.skipOnError() );
	}
	settings->endArray();
}

QStringList PHP::availableParsers() const
{
	return QStringList( mPluginInfos.Name );
}

pCommandParser* PHP::getParser( const QString& s )
{
	return s == mPluginInfos.Name ? new PHPParser( this ) : 0;
}

pCommand PHP::defaultInterpretCommand() const
{
		const QString mPHP = "php";
		return pCommand( "Interpret", mPHP, QString::null, false, availableParsers(), "$cpp$" );
}

pCommand PHP::interpretCommand() const
{
	// get settings object
	pSettings* settings = MonkeyCore::settings();
	pCommand cmd;

	cmd.setText( settings->value( settingsKey( "InterpretCommand/Text" ) ).toString() );
	cmd.setCommand( settings->value( settingsKey( "InterpretCommand/Command" ) ).toString() );
	cmd.setArguments( settings->value( settingsKey( "InterpretCommand/Arguments" ) ).toString() );
	cmd.setWorkingDirectory( settings->value( settingsKey( "InterpretCommand/WorkingDirectory" ) ).toString() );
	cmd.setParsers( settings->value( settingsKey( "InterpretCommand/Parsers" ) ).toStringList() );
	cmd.setTryAllParsers( settings->value( settingsKey( "InterpretCommand/TryAll" ), false ).toBool() );
	cmd.setSkipOnError( settings->value( settingsKey( "InterpretCommand/SkipOnError" ), false ).toBool() );

	// if no user commands get global ones
	if ( !cmd.isValid() )
	{
		cmd = defaultInterpretCommand();
	}

	return cmd;
}

void PHP::setInterpretCommand( const pCommand& cmd )
{
	pSettings* settings = MonkeyCore::settings();
	settings->setValue( settingsKey( "InterpretCommand/Text" ), cmd.text() );
	settings->setValue( settingsKey( "InterpretCommand/Command" ), cmd.command() );
	settings->setValue( settingsKey( "InterpretCommand/Arguments" ), cmd.arguments() );
	settings->setValue( settingsKey( "InterpretCommand/WorkingDirectory" ), cmd.workingDirectory() );
	settings->setValue( settingsKey( "InterpretCommand/Parsers" ), cmd.parsers() );
	settings->setValue( settingsKey( "InterpretCommand/TryAll" ), cmd.tryAllParsers() );
	settings->setValue( settingsKey( "InterpretCommand/SkipOnError" ), cmd.skipOnError() );
}

Q_EXPORT_PLUGIN2( InterpreterPHP, PHP )
