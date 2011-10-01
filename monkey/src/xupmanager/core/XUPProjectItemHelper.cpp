#include "xupmanager/core/XUPProjectItemHelper.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/XUPDynamicFolderItem.h"
#include "pMonkeyStudio.h"

#include <QDir>
#include <QDebug>

QString settingsKey( const QString& key )
{
    const QString prefix = QString( "%1." ).arg( XUPProjectItemHelper::SettingsScopeName );
    QString k = key;
    
    if ( !k.startsWith( prefix, Qt::CaseInsensitive ) ) {
        k.prepend( prefix );
    }
    
    return k;
}

void XUPProjectItemHelper::setProjectSettingsValues( XUPProjectItem* project, const QString& key, const QStringList& _values )
{
    XUPItem* variable = project->getVariable( project, settingsKey( key ) );
    QStringList values = _values;
    
    if ( !variable && values.isEmpty() ) {
        return;
    }
    
    if ( !variable ) {
        variable = project->addChild( XUPItem::Variable );
        variable->setAttribute( "name", settingsKey( key ) );
    }
    
    foreach ( XUPItem* item, variable->childrenList() ) {
        if ( item->type() != XUPItem::Value ) {
            continue;
        }
        
        const int index = values.indexOf( item->content() );
        
        if ( index == -1 ) {
            variable->removeChild( item );
        }
        else {
            values.removeAt( index );
        }
    }
    
    foreach ( const QString& value, values ) {
        XUPItem* item = variable->addChild( XUPItem::Value );
        item->setContent( value );
    }
}

QStringList XUPProjectItemHelper::projectSettingsValues( XUPProjectItem* project, const QString& key, const QStringList& defaultValues )
{
    XUPItem* variable = project->getVariable( project, settingsKey( key ) );
    QStringList values;
    
    if ( variable ) {
        foreach ( XUPItem* item, variable->childrenList() ) {
            if ( item->type() == XUPItem::Value ) {
                values << item->content();
            }
        }
    }
    
    return values.isEmpty() ? defaultValues : values;
}

void XUPProjectItemHelper::setProjectSettingsValue( XUPProjectItem* project, const QString& key, const QString& value )
{
    XUPProjectItemHelper::setProjectSettingsValues( project, key, QStringList( value ) );
}

QString XUPProjectItemHelper::projectSettingsValue( XUPProjectItem* project, const QString& key, const QString& defaultValue )
{
    const QStringList values = XUPProjectItemHelper::projectSettingsValues( project, key );
    return values.isEmpty() ? defaultValue : values.join( " " );
}

void XUPProjectItemHelper::addCommandProperty( XUPItem* variableItem, const QString& value )
{
    XUPItem* valueItem = variableItem->addChild( XUPItem::Value );
    valueItem->setContent( value );
}

void XUPProjectItemHelper::setProjectCommands( XUPProjectItem* project, const MenuCommandListMap& commands )
{
    bool emptyCommands = true;
    
    foreach ( const QString& menu, commands.keys() ) {
        if ( commands[ menu ].isEmpty() ) {
            continue;
        }
        
        emptyCommands = false;
        break;
    }
    
    XUPItem* commandsScope = project;
    
    if ( !commandsScope ) {
        return;
    }
    
    // clear existing commands
    foreach ( XUPItem* child, commandsScope->childrenList() ) {
        if ( child->type() != XUPItem::Variable || child->attribute( "name" ) != XUPProjectItemHelper::CommandScopeName ) {
            continue;
        }
        
        commandsScope->removeChild( child );
    }
    
    // create new ones
    foreach ( const QString& menu, commands.keys() ) {
        foreach ( const pCommand& command, commands[ menu ] ) {
            XUPItem* variable = commandsScope->addChild( XUPItem::Variable );
            variable->setAttribute( "name", CommandScopeName );
            
            addCommandProperty( variable, menu );
            addCommandProperty( variable, command.name() );
            addCommandProperty( variable, command.text() );
            addCommandProperty( variable, command.command() );
            addCommandProperty( variable, command.workingDirectory() );
            addCommandProperty( variable, command.parsers().join( ";" ) );
            addCommandProperty( variable, command.skipOnError() ? "1" : "0" );
            addCommandProperty( variable, command.tryAllParsers() ? "1" : "0" );
        }
    }
}

MenuCommandListMap XUPProjectItemHelper::projectCommands( XUPProjectItem* project )
{
    MenuCommandListMap commands;
    XUPItem* commandsScope = project;
    
    if ( commandsScope ) {
        foreach ( XUPItem* commandVariable, commandsScope->childrenList() ) {
            if ( commandVariable->attribute( "name" ) != XUPProjectItemHelper::CommandScopeName ) {
                continue;
            }
            
            QVariantList values;
            
            foreach ( XUPItem* commandValue, commandVariable->childrenList() ) {
                values << commandValue->content();
            }
            
            if ( values.count() != 8 ) {
                qWarning() << "Skip reading incomplete command";
                Q_ASSERT( 0 );
                continue;
            }
            
            pCommand command;
            
            command.setName( values.at( 1 ).toString() );
            command.setText( values.at( 2 ).toString() );
            command.setCommand( values.at( 3 ).toString() );
            command.setWorkingDirectory( values.at( 4 ).toString() );
            command.setParsers( values.at( 5 ).toString().split( ";", QString::SkipEmptyParts ) );
            command.setSkipOnError( values.at( 6 ).toBool() );
            command.setTryAllParsers( values.at( 7 ).toBool() );
            command.setUserData( Qt::Unchecked );
            
            commands[ values.at( 0 ).toString() ] << command;
        }
    }
    
    return commands;
}

XUPItem* XUPProjectItemHelper::projectDynamicFolderSettingsItem( XUPProjectItem* project, bool create )
{
    foreach ( XUPItem* child, project->childrenList() )
    {
        if ( child->type() == XUPItem::Variable && child->attribute( "name" ) == DynamicFolderSettingsName )
        {
            return child;
        }
    }
    
    if ( !create )
    {
        return 0;
    }
    
    XUPItem* dynamicFolderSettingsItem = project->addChild( XUPItem::Variable );
    dynamicFolderSettingsItem->setAttribute( "name", DynamicFolderSettingsName );
    return dynamicFolderSettingsItem;
}

void XUPProjectItemHelper::addDynamicFolderSettingsProperty( XUPItem* dynamicFolderSettingsItem, const QString& value )
{
    XUPItem* valueItem = dynamicFolderSettingsItem->addChild( XUPItem::Value );
    valueItem->setContent( value );
}

XUPDynamicFolderSettings XUPProjectItemHelper::projectDynamicFolderSettings( XUPProjectItem* project )
{
    XUPDynamicFolderSettings folder;
    XUPItem* dynamicFolderSettingsItem = projectDynamicFolderSettingsItem( project, false );
    
    if ( dynamicFolderSettingsItem )
    {
        QVariantList values;
        
        foreach ( XUPItem* valueItem, dynamicFolderSettingsItem->childrenList() )
        {
            values << valueItem->content();
        }
        
        if ( values.count() != 3 )
        {
            qWarning() << "Skip reading incomplete dynamic folder settings";
            Q_ASSERT( 0 );
            return folder;
        }
        
        folder.Active = values.at( 0 ).toBool();
        folder.AbsolutePath = project->filePath( values.at( 1 ).toString() );
        folder.FilesPatterns = values.at( 2 ).toString().split( ";", QString::SkipEmptyParts );
        
        if ( folder.AbsolutePath.isEmpty() && folder.Active )
        {
            folder.AbsolutePath = project->path();
        }
    }
    
    return folder;
}

void XUPProjectItemHelper::setProjectDynamicFolderSettings( XUPProjectItem* project, const XUPDynamicFolderSettings& folder )
{
    XUPItem* dynamicFolderSettingsItem = projectDynamicFolderSettingsItem( project, !folder.isNull() );
    
    if ( dynamicFolderSettingsItem )
    {
        // clear existing values
        foreach ( XUPItem* child, dynamicFolderSettingsItem->childrenList() )
        {
            dynamicFolderSettingsItem->removeChild( child );
        }
        
        addDynamicFolderSettingsProperty( dynamicFolderSettingsItem, folder.Active ? "1" : "0" );
        addDynamicFolderSettingsProperty( dynamicFolderSettingsItem, folder.AbsolutePath );
        addDynamicFolderSettingsProperty( dynamicFolderSettingsItem, folder.FilesPatterns.join( ";" ) );
    }
}

XUPDynamicFolderItem* XUPProjectItemHelper::projectDynamicFolderItem( XUPProjectItem* project, bool create )
{
    foreach ( XUPItem* child, project->childrenList() ) {
        if ( child->type() == XUPItem::DynamicFolder ) {
            return static_cast<XUPDynamicFolderItem*>( child );
        }
    }
    
    if ( !create ) {
        return 0;
    }
    
    XUPDynamicFolderItem* dynamicFolderItem = static_cast<XUPDynamicFolderItem*>( project->addChild( XUPItem::DynamicFolder ) );
    dynamicFolderItem->setAttribute( "name", DynamicFolderName );
    return dynamicFolderItem;
}

QDomDocument XUPProjectItemHelper::stripDynamicFolderFiles( const QDomDocument& document )
{
    QDomDocument doc = document.cloneNode().toDocument();
    const QDomNodeList nodesToRemove = doc.elementsByTagName( "dynamicfolder" );
    
    for ( int i = 0; i < nodesToRemove.count(); i++ ) {
        const QDomNode& node = nodesToRemove.at( i );
        node.parentNode().removeChild( node );
    }
    
    return doc;
}
