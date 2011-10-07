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
#include "PyQtProjectItem.h"
#include <xupmanager/core/XUPProjectItemHelper.h>

#include <xupmanager/gui/XUPPageEditor.h>
#include <pluginsmanager/CLIToolPlugin.h>
#include <maininterface/UIMain.h>
#include <pluginsmanager/PluginsManager.h>

#include <QDir>
#include <QDebug>

PyQtProjectItem::PyQtProjectItem()
    : XUPProjectItem()
{
}

QString PyQtProjectItem::projectType() const
{
    return PLUGIN_NAME;
}

void PyQtProjectItem::installCommands()
{
    // get plugins
    CLIToolPlugin* ip = interpreter();
    
    // temp command
    pCommand cmd;

    // build command
    if ( ip ) {
        cmd = ip->command();
    }
    
    cmd.setProject( this );
    cmd.setSkipOnError( false );
    const pCommand cmdInterpret = cmd;
    
    // available commands
    if ( ip ) {
        // execute project
        cmd = cmdInterpret;
        cmd.setName( "run" );
        cmd.setText( tr( "Run" ) );
        cmd.setCommand( QString( "%1 $target$" ).arg( cmd.command() ) );
        cmd.setParsers( QStringList() );
        cmd.setTryAllParsers( false );
        addCommand( "mInterpreter", cmd );
    }
    
    // pyuic4 on current file
    cmd = cmdInterpret;
    cmd.setName( "generate_form" );
    cmd.setText( tr( "Generate current form implementation" ) );
    cmd.setCommand( "pyuic4 $cf$ -o $cf$.py" );
    cmd.setWorkingDirectory( "$cfp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // pyrcc4 on current file
    cmd = cmdInterpret;
    cmd.setName( "generate_resource" );
    cmd.setText( tr( "Generate current resource implementation" ) );
    cmd.setCommand( "pyrcc4 $cf$ -o $cf$.py" );
    cmd.setWorkingDirectory( "$cfp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // pylupdate4 on current file
    cmd = cmdInterpret;
    cmd.setName( "generate_translation" );
    cmd.setText( tr( "Generate current project translation files" ) );
    cmd.setCommand( "pylupdate4 $cp$" );
    cmd.setWorkingDirectory( "$cpp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // install defaults commands
    XUPProjectItem::installCommands();
}

CLIToolPlugin* PyQtProjectItem::interpreter() const
{
    return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "Python" );
}
