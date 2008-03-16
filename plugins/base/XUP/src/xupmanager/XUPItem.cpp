#include "XUPItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"

XUPItemInfos XUPItem::mXUPItemInfos;

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

void XUPItem::updateItem()
{
	// set icon
	setIcon( getIcon( value( "icon" ), value( "type" ) ) );
	// set caption
	setText( defaultValue() );
	// get element tagname
	QString tn = value( "type" );
	// get comment
	QString c = value( "comment" );
	if ( c.isEmpty() )
		c = tr( "no comment" );
	// set visual datas
	if ( tn == "project" )
	{
		// set ToolTip
		setToolTip( tr( "<b>Project</b><br />%1" ).arg( projectFilePath() ) );
	}
	else if ( tn == "comment" )
	{
		// set ToolTip
		setToolTip( tr( "<b>Comment</b><br />%1" ).arg( defaultValue() ) );
	}
	else if ( tn == "emptyline" )
	{
		// set caption
		setText( tr( "%1 Empty Line(s)" ).arg( defaultValue() ) );
		// set ToolTip
		setToolTip( tr( "<b>Empty Line(s)</b><br />%1" ).arg( defaultValue() ) );
	}
	else if ( tn == "variable" )
	{
		// set icon
		QIcon i = QFile::exists( value( "icon" ) ) ? QIcon( value( "icon" ) ) : variableIcons().value( value( "name" ) );
		if ( i.isNull() )
			i = getIcon( QString(), value( "type" ) );
		setIcon( i );
		// set caption
		if ( variableLabels().contains( defaultValue() ) )
			setText( variableLabels().value( defaultValue() ) );
		// set ToolTip
		setToolTip( tr( "<b>Variable</b><br />%1" ).arg( defaultValue() ) );
	}
	else if ( tn == "value" )
	{
		// set caption
		XUPItem* pit = parent();
		if ( pit && fileVariables().contains( pit->defaultValue() ) )
			setText( QFileInfo( defaultValue() ).fileName() );
		// set ToolTip
		setToolTip( QString( "<b>Value</b><br />%1 (%2, %3, %4)" ).arg( defaultValue() ).arg( pit ? pit->value( "operator", "=" ) : QString( "no parent" ) ).arg( pit ? ( QVariant( pit->value( "multiline", "false" ) ).toBool() ? tr( "multiline" ) : tr( "singleline" ) ) : tr( "no parent" ) ).arg( c ) );
	}
	else if ( tn == "function" )
	{
		// set ToolTip
		setToolTip( tr( "<b>Function</b><br />%1 (%2)" ).arg( defaultValue() ).arg( c ) );
	}
	else if ( tn == "scope" )
	{
		// set ToolTip
		setToolTip( tr( "<b>Scope</b><br />%1 (%2, %3)" ).arg( defaultValue() ).arg( QVariant( value( "nested", "false" ) ).toBool() ? tr( "(nested)" ) : tr( "(not nested)" ) ).arg( c ) );
	}
	else if ( tn == "folder" )
	{
		// set ToolTip
		setToolTip( tr( "<b>Folder</b><br />%1" ).arg( defaultValue() ) );
	}
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
			}
			else
			{
				connect( it, SIGNAL( modifiedChanged( XUPItem*, bool ) ), pit, SIGNAL( modifiedChanged( XUPItem*, bool ) ) );
				setModified( true );
			}
		}
		it->updateItem();
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
	// update proejct modified state
	if ( !b && pit )
		pit->setModified( true );
}

void XUPItem::setDomElement( const QDomElement& e )
{
	if ( mDomElement != e )
	{
		mDomElement = e;
		updateItem();
	}
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
		updateItem();
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
	if ( XUPManager::loadXUP( this, s, v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		updateItem();
		return true;
	}
	return false;
}

bool XUPItem::saveProject( const QString& s, const QString& v )
{
	if ( XUPManager::saveXUP( this, s.isEmpty() ? projectFilePath() : s, v ) )
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
	// tell we will close the proejct
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

QString XUPItem::projectFilePath() const
{
	if ( XUPItem* pi = project() )
		return pi->mProjectFilePath;
	return QString();
}

QString XUPItem::projectPath() const
{ return QFileInfo( projectFilePath() ).absolutePath(); }

QString XUPItem::filePath( const QString& s )
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

QString XUPItem::relativeFilePath( const QString& s )
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
