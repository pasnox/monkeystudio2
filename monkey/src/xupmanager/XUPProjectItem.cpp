#include "../coremanager/MonkeyCore.h"
#include "../pluginsmanager/PluginsManager.h"

#include "XUPProjectItem.h"

QString XUPProjectItem::mProjectSettingsScope = "ProjectSettings";

XUPProjectItem::XUPProjectItem( const QDomElement e, const QString& s, bool b )
{
	if ( !mXUPItemInfos.Registered )
		registerItem();
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

XUPProjectItem* XUPProjectItem::clone( bool b ) const
{ return b ? new XUPProjectItem( domElement(), projectFilePath(), modified() ) : new XUPProjectItem; }

QStringList XUPProjectItem::projectSettingsValues( const QString& variable, const QStringList& defaultValues ) const
{
	// get the most top level project
	if ( XUPItem* project = topLevelProject() )
		// get project settings scope
		if ( XUPItem* scope = project->scope( mProjectSettingsScope, project, false ) )
			// check for the variable in children
			foreach ( XUPItem* cit, scope->children( false, true ) )
				if ( cit->isType( "variable" ) && cit->defaultValue() == variable )
					return cit->variableValues();
	// else return default list
	return defaultValues;
}

void XUPProjectItem::setProjectSettingsValues( const QString& variable, const QStringList& values )
{
	// abort if no files
	if ( variable.isEmpty() )
		return;
	
	// get top level project
	XUPItem* project = topLevelProject();
	// get project settings scope
	XUPItem* scope = project ? project->scope( mProjectSettingsScope, project, !values.isEmpty() ) : 0;
	if ( !scope )
		return;
	
	// get variable item
	XUPItem* vit = 0;
	foreach ( XUPItem* cit, scope->children( false, true ) )
	{
		if ( cit->isType( "variable" ) && cit->defaultValue() == variable )
		{
			vit = cit;
			break;
		}
	}
	
	// check variable exists or not
	bool exists = vit;
	
	// remove variable if needed
	if ( exists && values.isEmpty() )
	{
		// remova variable
		vit->remove();
		// update scope nested if needed
		if ( scope->isType( "scope" ) )
			scope->setValue( "nested", scope->rowCount() > 1 ? "false" : "true" );
		return;
	}
	
	// if not exists, and value is empty, do nothing
	if ( !exists && values.isEmpty() )
		return;
	
	// create variable if needed
	if ( !exists )
	{
		vit = scope->clone( false );
		vit->setDomElement( scope->domElement().ownerDocument().createElement( "variable" ) );
		scope->domElement().appendChild( vit->domElement() );
		vit->setValue( vit->valueName(), variable );
		vit->setValue( "multiline", "true" );
	}
	
	// if same value, return
	if ( vit->variableValues() == values )
		return;
	// clear values
	while ( vit->hasChildren() )
		vit->child( 0 )->remove();
	// set values
	foreach ( const QString& value, values )
	{
		// create item value
		XUPItem* it = vit->clone( false );
		it->setDomElement( vit->domElement().ownerDocument().createElement( "value" ) );
		vit->domElement().appendChild( it->domElement() );
		it->setValue( it->valueName(), value );
		vit->appendRow( it );
	}
	
	// append var item only at last will prevent multiple call of addFilteredValue from filtered view
	if ( !exists )
		scope->appendRow( vit );
	
	// update scope nested if needed
	if ( scope->isType( "scope" ) )
		scope->setValue( "nested", scope->rowCount() > 1 ? "false" : "true" );
}

void XUPProjectItem::addProjectSettingsValues( const QString& variable, const QStringList& values )
{
	// abort if no files or values
	if ( variable.isEmpty() || values.isEmpty() )
		return;
	
	// get top level project
	XUPItem* project = topLevelProject();
	// get project settings scope
	XUPItem* scope = project ? project->scope( mProjectSettingsScope, project, !values.isEmpty() ) : 0;
	if ( !scope )
		return;
	
	// get variable item
	XUPItem* vit = 0;
	foreach ( XUPItem* cit, scope->children( false, true ) )
	{
		if ( cit->isType( "variable" ) && cit->defaultValue() == variable )
		{
			vit = cit;
			break;
		}
	}
	
	// check variable exists or not
	bool exists = vit;
	
	// create variable if needed
	if ( !exists )
	{
		vit = scope->clone( false );
		vit->setDomElement( scope->domElement().ownerDocument().createElement( "variable" ) );
		scope->domElement().appendChild( vit->domElement() );
		vit->setValue( vit->valueName(), variable );
		vit->setValue( "multiline", "true" );
	}
	
	// get existing values in variable
	const QStringList existingValues = vit->variableValues();
	// set values
	foreach ( const QString& value, values )
	{
		// create item value if needed
		if ( !existingValues.contains( value ) )
		{
			XUPItem* it = vit->clone( false );
			it->setDomElement( vit->domElement().ownerDocument().createElement( "value" ) );
			vit->domElement().appendChild( it->domElement() );
			it->setValue( it->valueName(), value );
			vit->appendRow( it );
		}
	}
	
	// append var item only at last will prevent multiple call of addFilteredValue from filtered view
	if ( !exists )
		scope->appendRow( vit );
	
	// update scope nested if needed
	if ( scope->isType( "scope" ) )
		scope->setValue( "nested", scope->rowCount() > 1 ? "false" : "true" );
}

void XUPProjectItem::closeProject()
{
	// close child project
	foreach ( XUPItem* it, children( false, false ) )
		if ( it->isProject() )
			dynamic_cast<XUPProjectItem*> (it)->closeProject();
	// tell we will close the project
	emit aboutToClose( this );
	// emit closed
	emit closed( this );
}

BuilderPlugin* XUPProjectItem::builder( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<BuilderPlugin*>( PluginsManager::stAll, projectSettingsValue( "BUILDER", plugin ) ); }

CompilerPlugin* XUPProjectItem::compiler( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<CompilerPlugin*>( PluginsManager::stAll, projectSettingsValue( "COMPILER", plugin ) ); }

DebuggerPlugin* XUPProjectItem::debugger( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<DebuggerPlugin*>( PluginsManager::stAll, projectSettingsValue( "DEBUGGER", plugin ) ); }

InterpreterPlugin* XUPProjectItem::interpreter( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<InterpreterPlugin*>( PluginsManager::stAll, projectSettingsValue( "INTERPRETER", plugin ) ); }

void XUPProjectItem::addCommand( const pCommand& cmd, const QString& mnu )
{
	if ( cmd.isValid() )
	{
		emit installCommandRequested( cmd, mnu );
		mCommands.insert( mnu, cmd );
	}
}

void XUPProjectItem::installCommands()
{
	// get plugins
	BuilderPlugin* bp = builder();
	CompilerPlugin* cp = compiler();
	/*
	DebuggerPlugin* dp = debugger();
	InterpreterPlugin* ip = interpreter();
	*/
	
	// build command
	if ( bp )
	{
		pCommand cmd = bp->buildCommand();
		if ( cp )
			cmd.addParsers( cp->compileCommand().parsers() );
		cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
		cmd.setProject( this );
		cmd.setSkipOnError( false );
		addCommand( cmd, "mBuilder/mBuild" );
	}
	
	// compile file command
	if ( cp )
	{
		pCommand cmd = cp->compileCommand();
		cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
		cmd.setProject( this );
		cmd.setSkipOnError( false );
		addCommand( cmd, "mBuilder/mBuild" );
	}
	
	// install builder user command
	if ( bp )
	{
		foreach ( pCommand cmd, bp->userCommands() )
		{
			if ( cp )
				cmd.addParsers( cp->compileCommand().parsers() );
			cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder/mUserCommands" );
		}
	}
	
	// install compiler user command
	if ( cp )
	{
		foreach ( pCommand cmd, cp->userCommands() )
		{
			cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder/mUserCommands" );
		}
	}
	
	// install debugger user command
	// install interpreter user command
}

void XUPProjectItem::uninstallCommands()
{
	foreach ( const pCommand& cmd, mCommands.values() )
		emit uninstallCommandRequested( cmd, mCommands.key( cmd ) );
	mCommands.clear();
}
