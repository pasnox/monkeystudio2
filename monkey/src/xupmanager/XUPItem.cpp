#include "XUPItem.h"
#include "ProjectItemModel.h"
#include "XUPIO.h"
#include "../coremanager/MonkeyCore.h"
#include "../pluginsmanager/PluginsManager.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

XUPItemInfos XUPItem::mXUPItemInfos;
const QString mProjectSettingsScope = "ProjectSettings";

XUPItem::XUPItem( const QDomElement e, const QString& s, bool b )
{
	if ( !mXUPItemInfos.Registered )
		registerItem();
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

void XUPItem::registerItem()
{
	mXUPItemInfos.Registered = true;
	mXUPItemInfos.TextTypes << "comment" << "value" << "emptyline" << "function";
	mXUPItemInfos.FileVariables << "FILES";
	mXUPItemInfos.VariableLabels["FILES"] = tr( "Project Files" );
	mXUPItemInfos.FilteredVariables << "FILES";
	mXUPItemInfos.Operators << "=" << "+=" << "-=" << "*=" << "~=";
	mXUPItemInfos.Suffixes[tr( "Monkey Studio Project" )] = QStringList( "*.xup" );
}

QStringList XUPItem::operators() const
{ return mXUPItemInfos.Operators; }

void XUPItem::registerOperator( const QString& s )
{
	if ( !mXUPItemInfos.Operators.contains( s ) )
		mXUPItemInfos.Operators << s;
}

QStringList XUPItem::filteredVariables() const
{ return mXUPItemInfos.FilteredVariables; }

void XUPItem::registerFilteredVariables( const QString& s )
{
	if ( !mXUPItemInfos.FilteredVariables.contains( s ) )
		mXUPItemInfos.FilteredVariables << s;
}

QStringList XUPItem::textTypes() const
{ return mXUPItemInfos.TextTypes; }

void XUPItem::registerTextType( const QString& s )
{
	if ( !mXUPItemInfos.TextTypes.contains( s ) )
		mXUPItemInfos.TextTypes << s;
}

QStringList XUPItem::fileVariables() const
{ return mXUPItemInfos.FileVariables; }

void XUPItem::registerFileVariables( const QString& s )
{
	if ( !mXUPItemInfos.FileVariables.contains( s ) )
		mXUPItemInfos.FileVariables << s;
}

QStringList XUPItem::pathVariables() const
{ return mXUPItemInfos.PathVariables; }

void XUPItem::registerPathVariables( const QString& s )
{
	if ( !mXUPItemInfos.PathVariables.contains( s ) )
		mXUPItemInfos.PathVariables << s;
}

QHash<QString, QStringList> XUPItem::suffixes() const
{ return mXUPItemInfos.Suffixes; }

void XUPItem::registerSuffixes( const QString& l, const QStringList& s )
{ mXUPItemInfos.Suffixes[l] = s; }

QHash<QString, QString> XUPItem::variableLabels() const
{ return mXUPItemInfos.VariableLabels; }

void XUPItem::registerVariableLabels( const QString& v, const QString& l )
{ mXUPItemInfos.VariableLabels[v] = l; }

QHash<QString, QIcon> XUPItem::variableIcons() const
{ return mXUPItemInfos.VariableIcons; }

void XUPItem::registerVariableIcons( const QString& v, const QIcon& i )
{ mXUPItemInfos.VariableIcons[v] = i; }

QHash<QString, QStringList> XUPItem::variableSuffixes() const
{ return mXUPItemInfos.VariableSuffixes; }

void XUPItem::registerVariableSuffixes( const QString& n, const QStringList& s )
{ mXUPItemInfos.VariableSuffixes[n] = s; }

QIcon XUPItem::getIcon( const QString& o, const QString& d ) const
{ return QIcon( QFile::exists( o ) ? o : QString( ":/items/%1.png" ).arg( d ) ); }

QString XUPItem::valueName( const QString& s ) const
{
	if ( textTypes().contains( s ) )
		return "text";
	return "name";
}

QStringList XUPItem::files( bool a )
{
	// files list
	QStringList l;
	
	// check valid item
	if ( !isType( "variable" ) || !fileVariables().contains( defaultValue() ) )
		return l;
	
	// check recurs items from vit
	foreach ( XUPItem* cit, children( false, false ) )
		if ( cit->isType( "value" ) )
				l << ( a ? cit->filePath() : cit->relativeFilePath() );
	
	// return list
	return l;
}

QVariant XUPItem::data( int role ) const
{
	// decoration role
	if ( role == Qt::DecorationRole )
	{
		QIcon icn = getIcon( value( "icon" ), value( "type" ) );
		if ( isType( "variable" ) )
		{
			icn = QFile::exists( value( "icon" ) ) ? QIcon( value( "icon" ) ) : variableIcons().value( value( "name" ) );
			if ( icn.isNull() )
				icn = getIcon( QString(), value( "type" ) );
		}
		return QVariant( icn );
	}
	// display role
	else if ( role == Qt::DisplayRole )
	{
		QString txt = defaultValue();
		if ( isType( "emptyline" ) )
			txt = tr( "%1 Empty Line(s)" ).arg( defaultValue() );
		else if ( isType( "variable" ) )
		{
			if ( variableLabels().contains( defaultValue() ) )
				txt = variableLabels().value( defaultValue() );
		}
		else if ( isType( "value" ) )
		{
			if ( parent() )
			{
				if ( fileVariables().contains( parent()->defaultValue() ) )
					txt = QFileInfo( defaultInterpretedValue() ).fileName();
				else if ( pathVariables().contains( parent()->defaultValue() ) )
					txt = relativeFilePath( defaultInterpretedValue() );
			}
		}
		return QVariant( txt );
	}
	// tooltip role
	else if ( role == Qt::ToolTipRole )
	{
		QString tt;
		QString cmt = value( "comment" );
		if ( cmt.isEmpty() )
			cmt = tr( "no comment" );
		if ( isType( "project" ) )
			tt = tr( "<b>Project</b><br />%1" ).arg( projectFilePath() );
		else if ( isType( "comment" ) )
			tt = tr( "<b>Comment</b><br />%1" ).arg( defaultValue() );
		else if ( isType( "emptyline" ) )
			tt = tr( "<b>Empty Line(s)</b><br />%1" ).arg( defaultValue() );
		else if ( isType( "variable" ) )
			tt = tr( "<b>Variable</b><br />%1" ).arg( defaultValue() );
		else if ( isType( "value" ) )
			tt = QString( "<b>Value</b><br />%1 (%2, %3, %4)" ).arg( defaultValue() ).arg( parent() ? parent()->value( "operator", "=" ) : QString( "no parent" ) ).arg( parent() ? ( QVariant( parent()->value( "multiline", "false" ) ).toBool() ? tr( "multiline" ) : tr( "singleline" ) ) : tr( "no parent" ) ).arg( cmt );
		else if ( isType( "function" ) )
			tt = tr( "<b>Function</b><br />%1 (%2)" ).arg( defaultValue() ).arg( cmt );
		else if ( isType( "scope" ) )
			tt = tr( "<b>Scope</b><br />%1 (%2, %3)" ).arg( defaultValue() ).arg( QVariant( value( "nested", "false" ) ).toBool() ? tr( "(nested)" ) : tr( "(not nested)" ) ).arg( cmt );
		else if ( isType( "folder" ) )
			tt = tr( "<b>Folder</b><br />%1" ).arg( defaultValue() );
		return QVariant( tt );
	}
	// default values
	return QStandardItem::data( role );
}

XUPItem* XUPItem::child( int r, int c ) const
{ return dynamic_cast<XUPItem*>( QStandardItem::child( r, c ) ); }

XUPItem* XUPItem::parent() const
{ return dynamic_cast<XUPItem*>( QStandardItem::parent() ); }

ProjectItemModel* XUPItem::model() const
{ return dynamic_cast<ProjectItemModel*>( QStandardItem::model() ); }

XUPItem* XUPItem::clone( bool b ) const
{ return b ? new XUPItem( domElement(), projectFilePath(), modified() ) : new XUPItem; }

void XUPItem::appendRow( XUPItem* it )
{ insertRow( rowCount(), it ); }

void XUPItem::insertRow( int i, XUPItem* it )
{
	if ( it )
	{
		QStandardItem::insertRow( i, it );
		if ( XUPItem* pit = project() )
		{
			if ( it->isProject() )
			{
				connect( it, SIGNAL( aboutToClose( XUPItem* ) ), pit, SIGNAL( aboutToClose( XUPItem* ) ) );
				connect( it, SIGNAL( closed( XUPItem* ) ), pit, SIGNAL( closed( XUPItem* ) ) );
				connect( it, SIGNAL( installCommandRequested( const pCommand&, const QString& ) ), pit, SIGNAL( installCommandRequested( const pCommand&, const QString& ) ) );
				connect( it, SIGNAL( uninstallCommandRequested( const pCommand&, const QString& ) ), pit, SIGNAL( uninstallCommandRequested( const pCommand&, const QString& ) ) );
			}
			else
			{
				connect( it, SIGNAL( modifiedChanged( XUPItem*, bool ) ), pit, SIGNAL( modifiedChanged( XUPItem*, bool ) ) );
				setModified( true );
			}
		}
	}
}

QList<XUPItem*> XUPItem::children( bool r, bool s ) const
{
	QList<XUPItem*> l;
	for ( int i = 0; i < rowCount(); i++ )
	{
		if ( XUPItem* cit = child( i ) )
		{
			if ( !s || ( s && !cit->isProject() ) )
			{
				l << cit;
				if ( r )
					l << cit->children( r, s );
			}
		}
	}
	return l;
}

void XUPItem::remove()
{
	bool b = isProject();
	XUPItem* pit = b ? 0 : project();
	// remove node
	mDomElement.parentNode().removeChild( mDomElement );
	// remove item from model
	if ( model() )
		model()->removeRow( row(), index().parent() );
	else
		deleteLater();
	// update project modified state
	if ( !b && pit )
		pit->setModified( true );
}

void XUPItem::setDomElement( const QDomElement& e )
{
	if ( mDomElement != e )
		mDomElement = e;
}

QDomElement XUPItem::domElement() const
{ return mDomElement; }

void XUPItem::setDomDocument( const QDomDocument& d )
{ mDocument = d; }

QDomDocument XUPItem::domDocument() const
{ return mDocument; }

QString XUPItem::interpretedVariable( const QString&, const XUPItem*, const QString& s ) const
{ return s; }

QString XUPItem::defaultInterpretedValue() const
{ return defaultValue(); }

QString XUPItem::valueName() const
{ return valueName( value( "type" ) ); }

void XUPItem::setValue( const QString& n, const QString& v )
{
	if ( value( n ) != v )
	{
		if ( n == "text" )
		{
			if ( !mDomElement.childNodes().count() )
				mDomElement.appendChild( mDomElement.ownerDocument().createTextNode( v ) );
			else
				mDomElement.firstChild().toText().setData( v );
		}
		else if ( n == "type" )
			mDomElement.setTagName( v );
		else
			mDomElement.setAttribute( n, v );
		setModified( true );
	}
}

QString XUPItem::value( const QString& n, const QString& v ) const
{
	if ( n == "text" )
		return mDomElement.firstChild().toText().data();
	else if ( n == "type" )
		return mDomElement.tagName();
	return mDomElement.attribute( n, v );
}

QString XUPItem::defaultValue( const QString& v ) const
{ return value( valueName(), v ); }

bool XUPItem::modified() const
{
	if ( isProject() )
		foreach ( XUPItem* it, children( true, true ) )
			if ( it->modified() )
				return true;
	return mModified;
}

void XUPItem::setModified( bool b, bool e )
{
	if ( isProject() && b && !modified() )
	{
		mModified = true;
		if ( e )
			emit modifiedChanged( this, true );
	}
	else if ( isProject() && !b && modified() )
	{
		foreach ( XUPItem* it, children( true, true ) )
			if ( it->modified() )
				it->setModified( false, false );
		mModified = false;
		if ( e )
			emit modifiedChanged( this, false );
	}
	else if ( mModified != b )
	{
		mModified = b;
		if ( e )
			emit modifiedChanged( project(), mModified );
	}
}

void XUPItem::checkChildrenProjects()
{}

bool XUPItem::isProjectContainer() const
{ return false; }

bool XUPItem::loadProject( const QString& s, const QString& v )
{
	if ( XUPIO::loadXUP( this, s, v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		return true;
	}
	return false;
}

bool XUPItem::saveProject( const QString& s, const QString& v )
{
	if ( XUPIO::saveXUP( this, s.isEmpty() ? projectFilePath() : s, v ) )
	{
		mProjectFilePath = s.isEmpty() ? projectFilePath() : s;
		setModified( false );
		return true;
	}
	return false;
}

void XUPItem::closeProject()
{
	// close child project
	foreach ( XUPItem* it, children( false, false ) )
		if ( it->isProject() )
			it->closeProject();
	// tell we will close the project
	emit aboutToClose( this );
	// emit closed
	emit closed( this );
}

void XUPItem::addFiles( const QStringList& files, XUPItem* scope, const QString& op )
{
	// abort if no files
	if ( files.isEmpty() )
		return;
	
	// set scope
	if ( !scope )
		scope = this;
	
	// get variable item
	XUPItem* vit = 0;
	foreach ( XUPItem* cit, scope->children( false, true ) )
	{
		if ( cit->isType( "variable" ) && cit->defaultValue() == "FILES" && cit->value( "operator", "=" ) == op )
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
		vit = clone( false );
		vit->setDomElement( mDomElement.ownerDocument().createElement( "variable" ) );
		scope->domElement().appendChild( vit->domElement() );
		vit->setValue( vit->valueName(), "FILES" );
		vit->setValue( "operator", op );
		vit->setValue( "multiline", "true" );
	}
	
	// get all files
	QStringList existingFiles = vit->files();
	
	// add files
	foreach ( QString f, files )
	{
		QString fp = filePath( f );
		if ( !existingFiles.contains( fp ) )
		{
			XUPItem* it = clone( false );
			it->setDomElement( mDomElement.ownerDocument().createElement( "value" ) );
			vit->domElement().appendChild( it->domElement() );
			it->setValue( it->valueName(), relativeFilePath( fp ) );
			vit->appendRow( it );
		}
	}
	
	// append var item only at last will prevent multiple call of addFilteredValue from filtered view
	if ( !exists )
		scope->appendRow( vit );
}

void XUPItem::removeFiles( const QStringList& files, XUPItem* scope, const QString& op )
{
	// abort if no files
	if ( files.isEmpty() )
		return;
	
	// set scope
	if ( !scope )
		scope = this;
	
	// get variable item
	XUPItem* vit = 0;
	foreach ( XUPItem* cit, scope->children( false, true ) )
	{
		if ( cit->isType( "variable" ) && cit->defaultValue() == "FILES" && cit->value( "operator" ) == op )
		{
			vit = cit;
			break;
		}
	}
	
	// if not variable, cancel
	if ( !vit )
		return;
	
	// check each child
	foreach ( XUPItem* cit, vit->children( false, false ) )
		if ( cit->isType( "value" ) && files.contains( cit->filePath() ) )
			cit->remove();
	
	// remove variable item if emptyline
	if ( !vit->hasChildren() )
		vit->remove();
}

QStringList XUPItem::variableValues() const
{
	QStringList l;
	if ( isType( "variable" ) )
		foreach ( XUPItem* cit, children( false, true ) )
			if ( cit->isType( "value" ) )
				l << cit->defaultValue();
	return l;
}

QString XUPItem::projectFilePath() const
{
	if ( XUPItem* pi = project() )
		return pi->mProjectFilePath;
	return QString();
}

QString XUPItem::projectPath() const
{ return QFileInfo( projectFilePath() ).absolutePath(); }

QString XUPItem::filePath( const QString& s ) const
{
	if ( s.isEmpty() && isType( "value" ) )
	{
		const QString v = parent()->defaultValue();
		if ( ( fileVariables().contains( v ) || pathVariables().contains( v ) ) && !defaultValue().isEmpty() )
		{
			QString div = defaultInterpretedValue();
			QFileInfo fi( div );
			if ( fi.isRelative() )
				fi.setFile( projectPath().append( "/%1" ).arg( div ) );
			return fi.absoluteFilePath();
		}
	}
	else if ( !s.isEmpty() )
	{
		QFileInfo fi( s );
		if ( fi.isRelative() )
			fi.setFile( projectPath().append( "/%1" ).arg( s ) );
		return fi.absoluteFilePath();
	}
	return s;
}

QString XUPItem::relativeFilePath( const QString& s ) const
{ return QDir( projectPath() ).relativeFilePath( filePath( s ) ); }

bool XUPItem::isProject() const
{ return value( "type" ) == "project"; }

bool XUPItem::isType( const QString& s ) const
{ return value( "type" ) == s; }

XUPItem* XUPItem::project() const
{
	if ( isProject() )
		return const_cast<XUPItem*>( this );
	XUPItem* it = const_cast<XUPItem*>( this );
	while ( ( it = it->parent() )  )
		if ( it->isProject() )
			return const_cast<XUPItem*>( it );
	return 0;
}

XUPItem* XUPItem::topLevelProject() const
{
	XUPItem* it = project();
	while ( it && it->parent() )
		it = it->parent()->project();
	return it;
}

XUPItem* XUPItem::topProjectForInclude() const
{
	if ( XUPItem* it = project() )
	{
		if ( XUPItem* pit = it->parent() )
			if ( pit->isType( "function" ) && pit->defaultValue().startsWith( "include" ) )
				return pit->topProjectForInclude();
		return it;
	}
	return 0;
}

XUPItem* XUPItem::scope( const QString& scopeName, XUPItem* fromScope, bool create ) const
{
	// set the scope to search
	if ( fromScope == 0 )
		fromScope = const_cast<XUPItem*>( this );
	// check each child
	foreach ( XUPItem* cit, fromScope->children( false, true ) )
		if ( cit->isType( "scope" ) && cit->defaultValue() == scopeName )
			return cit;
	// create item if needed
	XUPItem* s = 0;
	if ( create )
	{
		// create scope
		s = clone( false );
		s->setDomElement( mDomElement.ownerDocument().createElement( "scope" ) );
		fromScope->domElement().appendChild( s->domElement() );
		s->setValue( s->valueName(), scopeName );
		// append it to fromScope
		fromScope->appendRow( s );
	}
	// return scope
	return s;
}

QStringList XUPItem::projectSettingsValues( const QString& variable, const QStringList& defaultValues ) const
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

void XUPItem::setProjectSettingsValues( const QString& variable, const QStringList& values )
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
		vit->remove();
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
}

void XUPItem::addProjectSettingsValues( const QString& variable, const QStringList& values )
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
}

BuilderPlugin* XUPItem::builder( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<BuilderPlugin*>( PluginsManager::stAll, projectSettingsValue( "BUILDER", plugin ) ); }

CompilerPlugin* XUPItem::compiler( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<CompilerPlugin*>( PluginsManager::stAll, projectSettingsValue( "COMPILER", plugin ) ); }

DebuggerPlugin* XUPItem::debugger( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<DebuggerPlugin*>( PluginsManager::stAll, projectSettingsValue( "DEBUGGER", plugin ) ); }

InterpreterPlugin* XUPItem::interpreter( const QString& plugin ) const
{ return MonkeyCore::pluginsManager()->plugin<InterpreterPlugin*>( PluginsManager::stAll, projectSettingsValue( "INTERPRETER", plugin ) ); }

void XUPItem::addCommand( const pCommand& cmd, const QString& mnu )
{
	if ( cmd.isValid() )
	{
		emit installCommandRequested( cmd, mnu );
		mCommands.insert( mnu, cmd );
	}
}

void XUPItem::installCommands()
{
	// get plugins
	BuilderPlugin* bp = builder();
	CompilerPlugin* cp = compiler();
	DebuggerPlugin* dp = debugger();
	InterpreterPlugin* ip = interpreter();
	
	// build command
	if ( bp )
	{
		pCommand cmd = bp->buildCommand();
		if ( cp )
			cmd.addParsers( cp->compileCommand().parsers() );
		cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
		cmd.setProject( this );
		addCommand( cmd, "mBuilder/mBuild" );
	}
	
	// compile file command
	if ( cp )
	{
		pCommand cmd = cp->compileCommand();
		cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
		cmd.setProject( this );
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
			addCommand( cmd, "mBuilder/mUserCommands" );
		}
	}
	
	// install debugger user command
	// install interpreter user command
}

void XUPItem::uninstallCommands()
{
	foreach ( const pCommand& cmd, mCommands.values() )
		emit uninstallCommandRequested( cmd, mCommands.key( cmd ) );
	mCommands.clear();
}
