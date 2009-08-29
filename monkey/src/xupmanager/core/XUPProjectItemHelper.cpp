#include "XUPProjectItemHelper.h"
#include "XUPProjectItem.h"

#include <QDebug>

XUPItem* XUPProjectItemHelper::projectCommandsScope( XUPProjectItem* project, bool create )
{
	XUPItem* settingsScope = project->projectSettingsScope( create );
	
	if ( !settingsScope )
	{
		return 0;
	}
	
	foreach ( XUPItem* child, settingsScope->childrenList() )
	{
		if ( child->type() == XUPItem::Scope && child->attribute( "name" ) == CommandsScopeName )
		{
			return child;
		}
	}
	
	if ( !create )
	{
		return 0;
	}
	
	XUPItem* commandsScope = settingsScope->addChild( XUPItem::Scope );
	commandsScope->setAttribute( "name", CommandsScopeName );
	commandsScope->setAttribute( "nested", "false" );
	return commandsScope;
}

void XUPProjectItemHelper::addCommandProperty( XUPItem* variableItem, const QString& value )
{
	XUPItem* valueItem = variableItem->addChild( XUPItem::Value );
	valueItem->setAttribute( "content", value );
}

void XUPProjectItemHelper::setProjectCommands( XUPProjectItem* project, const TypeCommandListMap& commands )
{
	XUPItem* commandsScope = projectCommandsScope( project, !commands.isEmpty() );
	
	if ( !commandsScope )
	{
		return;
	}
	
	// delete scope if no commands
	if ( commands.isEmpty() )
	{
		commandsScope->parent()->removeChild( commandsScope );
		return;
	}
	
	// clear existing commands
	foreach ( XUPItem* child, commandsScope->childrenList() )
	{
		commandsScope->removeChild( child );
	}
	
	// create new ones
	foreach ( const BasePlugin::Type& type, commands.keys() )
	{
		foreach ( const pCommand& command, commands[ type ] )
		{
			XUPItem* variable = commandsScope->addChild( XUPItem::Variable );
			variable->setAttribute( "name", CommandScopeName );
			variable->setAttribute( "operator", "=" );
			variable->setAttribute( "multiline", "true" );
			
			addCommandProperty( variable, QString::number( type ) );
			addCommandProperty( variable, command.text() );
			addCommandProperty( variable, command.command() );
			addCommandProperty( variable, command.arguments() );
			addCommandProperty( variable, command.workingDirectory() );
			addCommandProperty( variable, command.parsers().join( ";" ) );
			addCommandProperty( variable, command.skipOnError() ? "1" : "0" );
			addCommandProperty( variable, command.tryAllParsers() ? "1" : "0" );
		}
	}
}

TypeCommandListMap XUPProjectItemHelper::projectCommands( XUPProjectItem* project )
{
	TypeCommandListMap commands;
	XUPItem* commandsScope = projectCommandsScope( project, false );
	
	if ( commandsScope )
	{
		foreach ( XUPItem* commandVariable, commandsScope->childrenList() )
		{
			QVariantList values;
			
			foreach ( XUPItem* commandValue, commandVariable->childrenList() )
			{
				values << commandValue->attribute( "content" );
			}
			
			if ( values.count() != 8 )
			{
				Q_ASSERT( 0 );
				qWarning() << "Skip reading incomplete command";
				continue;
			}
			
			pCommand command;
			
			command.setText( values.at( 1 ).toString() );
			command.setCommand( values.at( 2 ).toString() );
			command.setArguments( values.at( 3 ).toString() );
			command.setWorkingDirectory( values.at( 4 ).toString() );
			command.setParsers( values.at( 5 ).toString().split( ";" ) );
			command.setSkipOnError( values.at( 6 ).toBool() );
			command.setTryAllParsers( values.at( 7 ).toBool() );
			
			commands[ (BasePlugin::Type)values.at( 0 ).toInt() ] << command;
		}
	}
	
	return commands;
}
