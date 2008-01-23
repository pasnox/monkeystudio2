#include "ProjectItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"

ProjectItem::ProjectItem( const QDomElement e, const QString& s, bool b )
{
	mFilteredVariables << "FORMS" << "HEADERS" << "SOURCES";
	mFileVariables = mFilteredVariables;
	// remove top
	mTextTypes << "comment" << "value" << "emptyline" << "function";
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

ProjectItem::~ProjectItem()
{}

QStringList ProjectItem::filteredVariables() const
{ return mFilteredVariables; }

void ProjectItem::registerFilteredVariables( const QString& s )
{
	if ( !mFilteredVariables.contains( s ) )
		mFilteredVariables << s;
}

QStringList ProjectItem::textTypes() const
{ return mTextTypes; }

void ProjectItem::registerTextType( const QString& s )
{
	if ( !mTextTypes.contains( s ) )
		mTextTypes << s;
}

QStringList ProjectItem::fileVariables() const
{ return mFileVariables; }

void ProjectItem::registerFileVariables( const QString& s )
{
	if ( !mFileVariables.contains( s ) )
		mFileVariables << s;
}

QStringList ProjectItem::pathVariables() const
{ return mPathVariables; }

void ProjectItem::registerPathVariables( const QString& s )
{
	if ( !mPathVariables.contains( s ) )
		mPathVariables << s;
}

QHash<QString, QString> ProjectItem::variableLabels() const
{ return mVariableLabels; }

void ProjectItem::registerVariableLabels( const QString& v, const QString& l )
{ mVariableLabels[v] = l; }

QHash<QString, QIcon> ProjectItem::variableIcons() const
{ return mVariableIcons; }

void ProjectItem::registerVariableIcons( const QString& v, const QIcon& i )
{ mVariableIcons[v] = i; }

QIcon ProjectItem::getIcon( const QString& o, const QString& d ) const
{ return QIcon( QFile::exists( o ) ? o : QString( ":/items/%1.png" ).arg( d ) ); }

QString ProjectItem::valueName( const QString& s ) const
{
	if ( mTextTypes.contains( s ) )
		return "text";
	return "name";
}

void ProjectItem::updateItem()
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
		QIcon i = QFile::exists( value( "icon" ) ) ? QIcon( value( "icon" ) ) : mVariableIcons.value( value( "name" ) );
		if ( i.isNull() )
			i = getIcon( QString(), value( "type" ) );
		setIcon( i );
		// set caption
		if ( mVariableLabels.contains( defaultValue() ) )
			setText( mVariableLabels.value( defaultValue() ) );
		// set ToolTip
		setToolTip( tr( "<b>Variable</b><br />%1" ).arg( defaultValue() ) );
	}
	else if ( tn == "value" )
	{
		// set caption
		ProjectItem* pit = parent();
		if ( pit && mFileVariables.contains( pit->defaultValue() ) )
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

ProjectItem* ProjectItem::child( int r, int c ) const
{ return dynamic_cast<ProjectItem*>( QStandardItem::child( r, c ) ); }

ProjectItem* ProjectItem::parent() const
{ return dynamic_cast<ProjectItem*>( QStandardItem::parent() ); }

ProjectItemModel* ProjectItem::model() const
{ return dynamic_cast<ProjectItemModel*>( QStandardItem::model() ); }

ProjectItem* ProjectItem::clone( bool b ) const
{ return b ? new ProjectItem( domElement(), projectFilePath(), modified() ) : new ProjectItem; }

void ProjectItem::appendRow( ProjectItem* it )
{ insertRow( rowCount(), it ); }

void ProjectItem::insertRow( int i, ProjectItem* it )
{
	QStandardItem::insertRow( i, it );
	if ( ProjectItem* pit = project() )
	{
		connect( it, SIGNAL( modifiedChanged( ProjectItem*, bool ) ), pit, SIGNAL( modifiedChanged( ProjectItem*, bool ) ) );
		if ( it->isProject() )
			connect( it, SIGNAL( aboutToClose( ProjectItem* ) ), pit, SIGNAL( aboutToClose( ProjectItem* ) ) );
	}
}

QList<ProjectItem*> ProjectItem::children( bool r, bool s ) const
{
	QList<ProjectItem*> l;
	for ( int i = 0; i < rowCount(); i++ )
	{
		if ( ProjectItem* cit = child( i ) )
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

void ProjectItem::setDomElement( const QDomElement& e )
{
	if ( mDomElement != e )
	{
		mDomElement = e;
		updateItem();
	}
}

QDomElement ProjectItem::domElement() const
{ return mDomElement; }

void ProjectItem::setDomDocument( const QDomDocument& d )
{ mDocument = d; }

QDomDocument ProjectItem::domDocument() const
{ return mDocument; }

QString ProjectItem::interpretedVariable( const QString&, const ProjectItem*, const QString& s ) const
{ return s; }

QString ProjectItem::defaultInterpretedValue() const
{ return defaultValue(); }

QString ProjectItem::valueName() const
{ return valueName( value( "type" ) ); }

void ProjectItem::setValue( const QString& n, const QString& v )
{
	if ( value( n ) != v )
	{
		if ( n == "text" )
			mDomElement.firstChild().toText().setData( v );
		else if ( n == "type" )
			mDomElement.setTagName( v );
		else
			mDomElement.setAttribute( n, v );
		updateItem();
		setModified( true );
	}
}

QString ProjectItem::value( const QString& n, const QString& v ) const
{
	if ( n == "text" )
		return mDomElement.firstChild().toText().data();
	else if ( n == "type" )
		return mDomElement.tagName();
	return mDomElement.attribute( n, v );
}

QString ProjectItem::defaultValue( const QString& v ) const
{ return value( valueName(), v ); }

bool ProjectItem::modified() const
{
	if ( isProject() )
		foreach ( ProjectItem* it, children( true, true ) )
			if ( it->modified() )
				return true;
	return mModified;
}

void ProjectItem::setModified( bool b, bool e )
{
	if ( isProject() && b && !modified() )
	{
		mModified = true;
		if ( e )
			emit modifiedChanged( this, true );
	}
	else if ( isProject() && !b && modified() )
	{
		foreach ( ProjectItem* it, children( true, true ) )
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

void ProjectItem::checkChildrenProjects()
{}

bool ProjectItem::loadProject( const QString& s, const QString& v )
{
	if ( XUPManager::loadXUP( this, s, v ) )
	{
		mProjectFilePath = s;
		updateItem();
		setModified( false );
		return true;
	}
	return false;
}

bool ProjectItem::saveProject( const QString& s, const QString& v )
{
	if ( XUPManager::saveXUP( this, s.isEmpty() ? projectFilePath() : s, v ) )
	{
		mProjectFilePath = s.isEmpty() ? projectFilePath() : s;
		setModified( false );
		return true;
	}
	return false;
}

void ProjectItem::closeProject()
{
	// close child project
	foreach ( ProjectItem* it, children( false, false ) )
		if ( it->isProject() )
			it->closeProject();
	// tell we will close the proejct
	emit aboutToClose( this );
	// remove it from model
	if ( model() )
		model()->removeRow( row(), index().parent() );
	else
		deleteLater();
}

QString ProjectItem::projectFilePath() const
{
	if ( ProjectItem* pi = project() )
		return pi->mProjectFilePath;
	return QString();
}

QString ProjectItem::projectPath() const
{ return QFileInfo( projectFilePath() ).canonicalPath(); }

QString ProjectItem::filePath( const QString& s )
{
	if ( s.isEmpty() && isType( "value" ) )
	{
		const QString v = parent()->defaultValue();
		if ( ( fileVariables().contains( v ) || pathVariables().contains( v ) ) && !defaultValue().isEmpty() )
		{
			QFileInfo fi( projectPath().append( "/%1" ).arg( defaultInterpretedValue() ) );
			return fi.exists() ? fi.canonicalFilePath() : fi.absoluteFilePath();
		}
	}
	else if ( !s.isEmpty() )
	{
		QFileInfo fi( projectPath().append( "/%1" ).arg( s ) );
		return fi.exists() ? fi.canonicalFilePath() : fi.absoluteFilePath();
	}
	return s;
}

QString ProjectItem::relativeFilePath( const QString& s )
{ return QDir( projectPath() ).relativeFilePath( filePath( s ) ); }

bool ProjectItem::isProject() const
{ return value( "type" ) == "project"; }

bool ProjectItem::isType( const QString& s ) const
{ return value( "type" ) == s; }

ProjectItem* ProjectItem::project() const
{
	if ( isProject() )
		return const_cast<ProjectItem*>( this );
	ProjectItem* it = const_cast<ProjectItem*>( this );
	while ( ( it = it->parent() )  )
		if ( it->isProject() )
			return const_cast<ProjectItem*>( it );
	return 0;
}

ProjectItem* ProjectItem::topLevelProject() const
{
	ProjectItem* it = project();
	while ( it && it->parent() && ( it == it->project() ) )
		if ( it->isProject() )
			return const_cast<ProjectItem*>( it );
	return const_cast<ProjectItem*>( it );
}
