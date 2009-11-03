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
#include "Python.h"

#include <QTabWidget>

Python::Python ()
{
	// install parsers
	foreach ( QString s, availableParsers() )
	{
		MonkeyCore::consoleManager()->addParser( getParser( s ) );
	}
}

void Python::fillPluginInfos()
{
	mPluginInfos.Caption = tr( "Python" );
	mPluginInfos.Description = tr( "This plugin provide Python interpreter and python parser." );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, Michon Aurelien aka aurelien <aurelien.french@gmail.com>";
	mPluginInfos.Type = BasePlugin::iInterpreter;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.FirstStartEnabled = true;
	mPluginInfos.HaveSettingsWidget = true;
	mPluginInfos.Pixmap = pIconManager::pixmap( "python.png", ":/icons" );
}

Python::~Python()
{ // TODO move to uninstall
	// uninstall parsers
	foreach ( QString s, availableParsers() )
	{
		MonkeyCore::consoleManager()->removeParser( s );
	}
}

bool Python::install()
{
	return true;
}

bool Python::uninstall()
{
	return true;
}

QWidget* Python::settingsWidget()
{
	QTabWidget* tw = new QTabWidget;
	tw->setAttribute( Qt::WA_DeleteOnClose );
	tw->addTab( interpreterSettingsWidget(), tr( "Interpret Command" ) );
	tw->addTab( cliToolSettingsWidget(), tr( "User Commands" ) );
	return tw;
}

pCommandList Python::defaultCommands() const
{
	return pCommandList();
}

pCommandList Python::userCommands() const
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

void Python::setUserCommands( const pCommandList& commands ) const
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

QStringList Python::availableParsers() const
{
	return QStringList( infos().Name );
}

pCommand Python::defaultInterpretCommand() const
{
		const QString mPython = "python";
		return pCommand( "Interpret", mPython, QString::null, false, availableParsers(), "$cpp$" );
}

pCommand Python::interpretCommand() const
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

void Python::setInterpretCommand( const pCommand& cmd )
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

Q_EXPORT_PLUGIN2( InterpreterPython, Python )