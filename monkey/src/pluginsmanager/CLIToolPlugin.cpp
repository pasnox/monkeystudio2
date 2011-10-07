/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#include "pluginsmanager/CLIToolPlugin.h"
#include "pluginsmanager/ui/UICLIToolSettings.h"
#include "coremanager/MonkeyCore.h"
#include "settingsmanager/Settings.h"

#include <QApplication>

QWidget* CLIToolPlugin::settingsWidget() const
{
    return new UICLIToolSettings( const_cast<CLIToolPlugin*>( this ), QApplication::activeWindow() );
}

pCommand CLIToolPlugin::command() const
{
    // get settings object
    pSettings* settings = MonkeyCore::settings();
    pCommand cmd;
    
    cmd.setName( settings->value( settingsKey( "command/Name" ) ).toString() );
    cmd.setText( settings->value( settingsKey( "command/Text" ) ).toString() );
    cmd.setCommand( settings->value( settingsKey( "command/Command" ) ).toString() );
    cmd.setWorkingDirectory( settings->value( settingsKey( "command/WorkingDirectory" ) ).toString() );
    cmd.setParsers( settings->value( settingsKey( "command/Parsers" ) ).toStringList() );
    cmd.setTryAllParsers( settings->value( settingsKey( "command/TryAll" ), false ).toBool() );
    cmd.setSkipOnError( settings->value( settingsKey( "command/SkipOnError" ), false ).toBool() );
    
    // if no user commands get global ones
    if ( !cmd.isValid() )
    {
        cmd = defaultCommand();
    }
    
    return cmd;
}

void CLIToolPlugin::setCommand( const pCommand& cmd )
{
    pSettings* settings = MonkeyCore::settings();
    
    settings->setValue( settingsKey( "command/Name" ), cmd.name() );
    settings->setValue( settingsKey( "command/Text" ), cmd.text() );
    settings->setValue( settingsKey( "command/Command" ), cmd.command() );
    settings->setValue( settingsKey( "command/WorkingDirectory" ), cmd.workingDirectory() );
    settings->setValue( settingsKey( "command/Parsers" ), cmd.parsers() );
    settings->setValue( settingsKey( "command/TryAll" ), cmd.tryAllParsers() );
    settings->setValue( settingsKey( "command/SkipOnError" ), cmd.skipOnError() );
}

QStringList CLIToolPlugin::availableParsers() const
{
    return QStringList();
}
