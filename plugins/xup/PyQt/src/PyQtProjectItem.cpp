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
#include <QSettings>
#include <QProcessEnvironment>
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
    
    pyQtBinaryFilePath( PyQtProjectItem::Python );
    
    // pyuic4 on current file
    cmd = cmdInterpret;
    cmd.setName( "generate_form" );
    cmd.setText( tr( "Generate current form implementation" ) );
    cmd.setCommand( QString( "%1 $cf$ -o $cf$.py" ).arg( pyQtBinaryFilePath( PyQtProjectItem::Uic ) ) );
    cmd.setWorkingDirectory( "$cfp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // pyrcc4 on current file
    cmd = cmdInterpret;
    cmd.setName( "generate_resource" );
    cmd.setText( tr( "Generate current resource implementation" ) );
    cmd.setCommand( QString( "%1 $cf$ -o $cf$.py" ).arg( pyQtBinaryFilePath( PyQtProjectItem::Rcc ) ) );
    cmd.setWorkingDirectory( "$cfp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // pylupdate4 on current source file
    cmd = cmdInterpret;
    cmd.setName( "update_translation" );
    cmd.setText( tr( "Update current source translation file" ) );
    cmd.setCommand( QString( "%1 $cf$ -ts translations.ts" ).arg( pyQtBinaryFilePath( PyQtProjectItem::LUpdate ) ) );
    cmd.setWorkingDirectory( "$cpp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // lrelease on current file
    cmd = cmdInterpret;
    cmd.setName( "generate_translation" );
    cmd.setText( tr( "Generate current translation qm file" ) );
    cmd.setCommand( QString( "%1 $cf$ -qm translation.qm" ).arg( pyQtBinaryFilePath( PyQtProjectItem::LRelease ) ) );
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

QString PyQtProjectItem::pyQtFilePath() const
{
#if defined( Q_OS_WIN )
    // system scope
    {
        const QSettings settings( "HKEY_LOCAL_MACHINE\\Software\\PyQt4", QSettings::NativeFormat );
        const QStringList versions = settings.childGroups();
        
        foreach ( const QString& version, versions ) {
            const QString installPath = settings.value( QString( "%1/InstallPath/." ).arg( version ) ).toString();
            
            if ( !installPath.isEmpty() && QFile::exists( installPath ) ) {
                return QDir::toNativeSeparators( QDir::cleanPath( installPath ) );
            }
        }
    }
    
    // user scope
    {
        const QSettings settings( "HKEY_CURRENT_USER\\Software\\PyQt4", QSettings::NativeFormat );
        const QStringList versions = settings.childGroups();
        
        foreach ( const QString& version, versions ) {
            const QString installPath = settings.value( QString( "%1/InstallPath/." ).arg( version ) ).toString();
            
            if ( !installPath.isEmpty() && QFile::exists( installPath ) ) {
                return QDir::toNativeSeparators( QDir::cleanPath( installPath ) );
            }
        }
    }
#endif
    
    return QString::null;
}

QString PyQtProjectItem::pyQtBinaryFilePath( PyQtProjectItem::Binary binary ) const
{
#if defined( Q_OS_WIN )
    const QString path = quotedValue( pyQtFilePath() );
    
    switch ( binary ) {
        case PyQtProjectItem::Python:
            return QString( "%1\\pythonw.exe" ).arg( path );
        case PyQtProjectItem::LRelease:
            return QString( "%1\\Lib\\site-packages\\PyQt4\\lrelease.exe" ).arg( path );
        case PyQtProjectItem::LUpdate:
            return QString( "%1\\Lib\\site-packages\\PyQt4\\pylupdate4.exe" ).arg( path );
        case PyQtProjectItem::Rcc:
            return QString( "%1\\Lib\\site-packages\\PyQt4\\pyrcc4.exe" ).arg( path );
        case PyQtProjectItem::Uic:
            return QString( "%1 /C %2\\Lib\\site-packages\\PyQt4\\pyuic4.bat" )
                .arg( QProcessEnvironment::systemEnvironment().value( "COMSPEC" ) )
                .arg( path )
            ;
    }
#else
    switch ( binary ) {
        case PyQtProjectItem::Python:
            return "python";
        case PyQtProjectItem::LRelease:
            return "lrelease";
        case PyQtProjectItem::LUpdate:
            return "pylupdate4";
        case PyQtProjectItem::Rcc:
            return "pyrcc4";
        case PyQtProjectItem::Uic:
            return "pyuic4";
    }
#endif
    
    return QString::null;
}
