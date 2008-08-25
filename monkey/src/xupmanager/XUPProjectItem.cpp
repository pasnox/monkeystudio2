#include "../coremanager/MonkeyCore.h"
#include "../pluginsmanager/PluginsManager.h"
#include "XUPIO.h"

#include "XUPProjectItem.h"


QString XUPProjectItem::mProjectSettingsScope = "ProjectSettings";
XUPItemInfos XUPProjectItem::mXUPItemInfos;

XUPProjectItem::XUPProjectItem( const QDomElement e, const QString& s, bool b )
{
#if 0
	if ( !mXUPItemInfos.Registered )
		registerItem();
#endif
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

const XUPItemInfos& XUPProjectItem::itemInfos() const
{ return mXUPItemInfos; }

QStringList XUPProjectItem::operators() const
{ return mXUPItemInfos.Operators; }

void XUPProjectItem::registerOperator( const QString& s )
{
	if ( !mXUPItemInfos.Operators.contains( s ) )
		mXUPItemInfos.Operators << s;
}

QStringList XUPProjectItem::filteredVariables() const
{ return mXUPItemInfos.FilteredVariables; }

void XUPProjectItem::registerFilteredVariables( const QString& s )
{
	if ( !mXUPItemInfos.FilteredVariables.contains( s ) )
		mXUPItemInfos.FilteredVariables << s;
}

QStringList XUPProjectItem::textTypes() const
{ return mXUPItemInfos.TextTypes; }

void XUPProjectItem::registerTextType( const QString& s )
{
	if ( !mXUPItemInfos.TextTypes.contains( s ) )
		mXUPItemInfos.TextTypes << s;
}

QStringList XUPProjectItem::fileVariables() const
{ return mXUPItemInfos.FileVariables; }

void XUPProjectItem::registerFileVariables( const QString& s )
{
	if ( !mXUPItemInfos.FileVariables.contains( s ) )
		mXUPItemInfos.FileVariables << s;
}

QStringList XUPProjectItem::pathVariables() const
{ return mXUPItemInfos.PathVariables; }

void XUPProjectItem::registerPathVariables( const QString& s )
{
	if ( !mXUPItemInfos.PathVariables.contains( s ) )
		mXUPItemInfos.PathVariables << s;
}

QHash<QString, QStringList> XUPProjectItem::suffixes() const
{ return mXUPItemInfos.Suffixes; }

void XUPProjectItem::registerSuffixes( const QString& l, const QStringList& s )
{ mXUPItemInfos.Suffixes[l] = s; }

QHash<QString, QString> XUPProjectItem::variableLabels() const
{ return mXUPItemInfos.VariableLabels; }

void XUPProjectItem::registerVariableLabels( const QString& v, const QString& l )
{ mXUPItemInfos.VariableLabels[v] = l; }

QHash<QString, QIcon> XUPProjectItem::variableIcons() const
{ return mXUPItemInfos.VariableIcons; }

void XUPProjectItem::registerVariableIcons( const QString& v, const QIcon& i )
{ mXUPItemInfos.VariableIcons[v] = i; }

QHash<QString, QStringList> XUPProjectItem::variableSuffixes() const
{ return mXUPItemInfos.VariableSuffixes; }

void XUPProjectItem::registerVariableSuffixes( const QString& n, const QStringList& s )
{ mXUPItemInfos.VariableSuffixes[n] = s; }

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
		vit = scope->clone();
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
		XUPItem* it = vit->clone();
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
		vit = scope->clone();
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
			XUPItem* it = vit->clone();
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

bool XUPProjectItem::loadProject( const QString& s, const QString& v )
{
	if ( XUPIO::loadXUP( this, s, v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		return true;
	}
	return false;
}

bool XUPProjectItem::saveProject( const QString& s, const QString& v )
{
	if ( XUPIO::saveXUP( this, s.isEmpty() ? projectFilePath() : s, v ) )
	{
		mProjectFilePath = s.isEmpty() ? projectFilePath() : s;
		setModified( false );
		return true;
	}
	return false;
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
