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
    
    //cmd.setUserData( QVariant::fromValue( &mCommands ) );
    cmd.setProject( this );
    cmd.setSkipOnError( false );
    const pCommand cmdInterpret = cmd;
    
    // get qt version
    QString mainFile = relativeFilePath( XUPProjectItemHelper::projectSettingsValue( this, "MAIN_FILE" ) );
    
    if ( mainFile.isEmpty() ) {
        mainFile = relativeFilePath( findFile( "main.py" ).value( 0 ).absoluteFilePath() );
    }
    
    // available commands
    if ( ip ) {
        // execute project
        cmd = cmdInterpret;
        cmd.setText( tr( "Run" ) );
        cmd.setCommand( QString( "%1 \"%2\"" ).arg( cmd.command() ).arg( mainFile ) );
        cmd.setParsers( QStringList() );
        cmd.setTryAllParsers( false );
        addCommand( "mInterpreter", cmd );
    }
    
    // pyuic4 on current file
    cmd = cmdInterpret;
    cmd.setText( tr( "Generate current form implementation" ) );
    cmd.setCommand( "pyuic4 \"$cf$\" -o \"$cf$.py\"" );
    cmd.setWorkingDirectory( "$cfp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // pyrcc4 on current file
    cmd = cmdInterpret;
    cmd.setText( tr( "Generate current resource implementation" ) );
    cmd.setCommand( "pyrcc4 \"$cf$\" -o \"$cf$.py\"" );
    cmd.setWorkingDirectory( "$cfp$" );
    cmd.setParsers( QStringList() );
    cmd.setSkipOnError( false );
    cmd.setTryAllParsers( false );
    addCommand( "mInterpreter", cmd );
    
    // pylupdate4 on current file
    cmd = cmdInterpret;
    cmd.setText( tr( "Generate current project translation files" ) );
    cmd.setCommand( "pylupdate4 \"$cp$\"" );
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
