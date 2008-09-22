#include "XUPItem.h"
#include "XUPProjectItem.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>


QIcon XUPItem::getIcon( const QString& o, const QString& d ) const
{ return QIcon( QFile::exists( o ) ? o : QString( ":/items/%1.png" ).arg( d ) ); }

QVariant XUPItem::data( int role ) const
{
	// decoration role
	if ( role == Qt::DecorationRole )
	{
		QIcon icn = getIcon( value( "icon" ), value( "type" ) );
		if ( isType( "variable" ) )
		{
			icn = QFile::exists( value( "icon" ) ) ? QIcon( value( "icon" ) ) : project()->variableIcons().value( value( "name" ) );
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
			if ( project()->variableLabels().contains( defaultValue() ) )
				txt = project()->variableLabels().value( defaultValue() );
		}
		else if ( isType( "value" ) )
		{
			if ( parent() )
			{
				if ( project()->fileVariables().contains( parent()->defaultValue() ) )
					txt = QFileInfo( defaultInterpretedValue() ).fileName();
				else if ( project()->pathVariables().contains( parent()->defaultValue() ) )
					txt = project()->relativeFilePath( defaultInterpretedValue() ); // TODO review. Do it's correct?
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
/*		if ( isType( "project" ) )
			tt = tr( "<b>Project</b><br />%1" ).arg( projectFilePath() );
*/		else if ( isType( "comment" ) )
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

void XUPItem::appendRow( XUPItem* it )
{ insertRow( rowCount(), it ); }

void XUPItem::insertRow( int i, XUPItem* it )
{
	if ( it )
	{
		QStandardItem::insertRow( i, it );
		if ( XUPItem* pit = project() )
		{
			connect( it, SIGNAL( modifiedChanged( XUPItem*, bool ) ), pit, SIGNAL( modifiedChanged( XUPItem*, bool ) ) );
			if ( it->isProject() )
			{
				connect( it, SIGNAL( aboutToClose( XUPProjectItem* ) ), pit, SIGNAL( aboutToClose( XUPProjectItem* ) ) );
				connect( it, SIGNAL( closed( XUPProjectItem* ) ), pit, SIGNAL( closed( XUPProjectItem* ) ) );
				connect( it, SIGNAL( installCommandRequested( const pCommand&, const QString& ) ), pit, SIGNAL( installCommandRequested( const pCommand&, const QString& ) ) );
				connect( it, SIGNAL( uninstallCommandRequested( const pCommand&, const QString& ) ), pit, SIGNAL( uninstallCommandRequested( const pCommand&, const QString& ) ) );
			}
			else
				setModified( true );
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

QStringList XUPItem::files( bool a )
{
	// files list
	QStringList l;
	
	// check valid item
	if ( !isType( "variable" ) || !project()->fileVariables().contains( defaultValue() ) )
		return l;
	
	// check recurs items from vit
	foreach ( XUPItem* cit, children( false, false ) )
		if ( cit->isType( "value" ) )
				l << ( a ? cit->filePath() : project()->relativeFilePath (cit->filePath()) ); //TODO review. Do it's right ?
	
	// return list
	return l;
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
		vit = new XUPItem();
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
			XUPItem* it = new XUPItem();
			it->setDomElement( mDomElement.ownerDocument().createElement( "value" ) );
			vit->domElement().appendChild( it->domElement() );
			it->setValue( it->valueName(), project()->relativeFilePath( fp ) );
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
	
	// update scope nested if needed
	if ( scope->isType( "scope" ) )
		scope->setValue( "nested", scope->rowCount() > 1 ? "false" : "true" );
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

QString XUPItem::valueName( const QString& s ) const
{
	if ( project() && project()->textTypes().contains( s ) ) //FIXME Do it's right?
		return "text";
	return "name";
}

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
	if ( mModified != b )
	{
		mModified = b;
		if ( e )
			emit modifiedChanged( project(), mModified );
	}
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

void XUPItem::setVariableValues( const QStringList& values )
{
	// abort if no variable
	if ( !isType( "variable" ) )
		return;
	
	// remove variable if needed
	if ( values.isEmpty() )
	{
		// get scope
		XUPItem* scope = parent();
		// remove variable
		remove();
		// update scope nested if needed
		if ( scope->isType( "scope" ) )
			scope->setValue( "nested", scope->rowCount() > 1 ? "false" : "true" );
		return;
	}
	
	// if same value, return
	if ( variableValues() == values )
		return;
	
	// clear values
	while ( hasChildren() )
		child( 0 )->remove();
	
	// set values
	foreach ( const QString& value, values )
	{
		// create item value
		XUPItem* it = new XUPItem();
		it->setDomElement( domElement().ownerDocument().createElement( "value" ) );
		domElement().appendChild( it->domElement() );
		it->setValue( it->valueName(), value );
		appendRow( it );
	}
}

void XUPItem::addVariableValues( const QStringList& values )
{
	// abort if no files or no value
	if ( !isType( "variable" ) || values.isEmpty() )
		return;
	
	// get existing values in variable
	const QStringList existingValues = variableValues();
	
	// set values
	foreach ( const QString& value, values )
	{
		// create item value if needed
		if ( !existingValues.contains( value ) )
		{
			XUPItem* it = new XUPItem();
			it->setDomElement( domElement().ownerDocument().createElement( "value" ) );
			domElement().appendChild( it->domElement() );
			it->setValue( it->valueName(), value );
			appendRow( it );
		}
	}
}

void XUPItem::removeVariableValues( const QStringList& values )
{
	// abort if no files or no value
	if ( !isType( "variable" ) || values.isEmpty() )
		return;
	for ( int i = rowCount() -1; i > -1; i-- )
		if ( values.contains( child( i )->defaultValue() ) )
			child( i )->remove();
	// auto remove variable if no longer values
	if ( rowCount() == 0 )
		remove();
}

QString XUPItem::filePath( const QString& s ) const
{
	if ( s.isEmpty() && isType( "value" ) )
	{
		const QString v = parent()->defaultValue();
		if ( ( project()->fileVariables().contains( v ) || project()->pathVariables().contains( v ) ) && !defaultValue().isEmpty() )
		{
			QString div = defaultInterpretedValue();
			QFileInfo fi( div );
			if ( fi.isRelative() )
				fi.setFile( project()->projectPath().append( "/%1" ).arg( div ) );
			return fi.absoluteFilePath();
		}
	}
	else if ( !s.isEmpty() )
	{
		QFileInfo fi( s );
		if ( fi.isRelative() )
			fi.setFile( project()->projectPath().append( "/%1" ).arg( s ) );
		return fi.absoluteFilePath();
	}
	else if ( isProject() )
		return project()->projectFilePath();
	return s;
}

bool XUPItem::isProject() const
{ return value( "type" ) == "project"; }

bool XUPItem::isType( const QString& s ) const
{ return value( "type" ) == s; }

XUPProjectItem* XUPItem::project() const
{
	XUPItem* it = const_cast<XUPItem*>( this );
	while (it->parent())
		it = it->parent();
	if ( it->isProject() )
		return dynamic_cast<XUPProjectItem*>( it );
	return 0;
}

XUPProjectItem* XUPItem::topLevelProject() const
{
	XUPItem* it = const_cast<XUPItem*> (this);
	while ( it && it->parent() )
		it = it->parent();
	return dynamic_cast<XUPProjectItem*> (it);
}

XUPProjectItem* XUPItem::topProjectForInclude() const
{
	if ( XUPProjectItem* it = project() )
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
		s = new XUPItem();
		s->setDomElement( mDomElement.ownerDocument().createElement( "scope" ) );
		fromScope->domElement().appendChild( s->domElement() );
		s->setValue( s->valueName(), scopeName );
		// append it to fromScope
		fromScope->appendRow( s );
		// update scope nested if needed
		if ( fromScope->isType( "scope" ) )
			fromScope->setValue( "nested", fromScope->rowCount() > 1 ? "false" : "true" );
	}
	// return scope
	return s;
}

XUPItem* XUPItem::variable( const QString& variableName, const QString& operatorName, XUPItem* fromScope, bool create ) const
{
	// set the scope to search
	if ( fromScope == 0 )
		fromScope = const_cast<XUPItem*>( this );
	// check each child
	foreach ( XUPItem* cit, fromScope->children( false, true ) )
		if ( cit->isType( "variable" ) && cit->defaultValue() == variableName && cit->value( "operator", "=" ) == operatorName )
			return cit;
	// create item if needed
	XUPItem* v = 0;
	if ( create )
	{
		// create scope
		v = new XUPItem();
		v->setDomElement( mDomElement.ownerDocument().createElement( "variable" ) );
		fromScope->domElement().appendChild( v->domElement() );
		v->setValue( v->valueName(), variableName );
		v->setValue( "operator", operatorName );
		// append it to fromScope
		fromScope->appendRow( v );
		// update scope nested if needed
		if ( fromScope->isType( "scope" ) )
			fromScope->setValue( "nested", fromScope->rowCount() > 1 ? "false" : "true" );
	}
	// return scope
	return v;
}
