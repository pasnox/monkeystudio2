#include "ProjectItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"

ProjectItem::ProjectItem( const QDomElement e, const QString& s, bool b, ProjectItem* bit )
{
	//setBuddy( bit );
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

ProjectItem* ProjectItem::child( int r, int c ) const
{ return dynamic_cast<ProjectItem*>( QStandardItem::child( r, c ) ); }

ProjectItem* ProjectItem::parent() const
{ return dynamic_cast<ProjectItem*>( QStandardItem::parent() ); }

ProjectItemModel* ProjectItem::model() const
{ return dynamic_cast<ProjectItemModel*>( QStandardItem::model() ); }

ProjectItem* ProjectItem::clone( bool b ) const
{ return b ? new ProjectItem( domElement().cloneNode( false ).toElement(), projectFilePath(), modified()/*, buddy()*/ ) : new ProjectItem; }

void ProjectItem::appendRow( ProjectItem* it )
{ insertRow( rowCount(), it ); }

void ProjectItem::insertRow( int i, ProjectItem* it )
{
	QStandardItem::insertRow( i, it );
	if ( it->isProject() )
		connect( it, SIGNAL( modifiedChanged( ProjectItem*, bool ) ), this, SIGNAL( modifiedChanged( ProjectItem*, bool ) ) );
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
		XUPManager::updateItem( this );
	}
}

QDomElement ProjectItem::domElement() const
{ return mDomElement; }

QDomDocument ProjectItem::toDomDocument()
{ return XUPManager::toDomDocument( this ); }

QString ProjectItem::interpretedVariable( const QString&, const ProjectItem*, const QString& s ) const
{ return s; }

QString ProjectItem::defaultInterpretedValue() const
{ return defaultValue(); }

QString ProjectItem::valueName() const
{ return XUPManager::valueName( value( "type" ) ); }

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
		XUPManager::updateItem( this );
		setModified( true );
	}
}

QString ProjectItem::value( const QString& n, const QString& v ) const
{
	//const ProjectItem* it = mBuddy ? mBuddy : this;
	if ( n == "text" )
		return /*it->*/mDomElement.firstChild().toText().data();
	else if ( n == "type" )
		return /*it->*/mDomElement.tagName();
	return /*it->*/mDomElement.attribute( n, v );
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
			emit modifiedChanged( this, mModified );
	}
}
/*
ProjectItem* ProjectItem::buddy() const
{ return mBuddy; }

void ProjectItem::setBuddy( ProjectItem* it )
{
	if ( mBuddy != it )
		mBuddy = it;
}

QVariant ProjectItem::data( int r ) const
{ return mBuddy ? mBuddy->data( r ) : QStandardItem::data( r ); }
*/

void ProjectItem::checkChildrenProjects()
{}

bool ProjectItem::loadProject( const QString& s, const QString& v )
{
	if ( XUPManager::loadXUP( this, s, v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		checkChildrenProjects();
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
		if ( ( XUPManager::fileVariables().contains( v ) || XUPManager::pathVariables().contains( v ) ) && !defaultValue().isEmpty() )
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
