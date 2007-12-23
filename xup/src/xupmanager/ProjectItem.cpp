#include "ProjectItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"

ProjectItem::ProjectItem( const QDomElement& e, const QString& s, bool b )
{
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
{ return b ? new ProjectItem( domElement(), projectFilePath(), modified() ) : new ProjectItem; }

void ProjectItem::appendRow( ProjectItem* it )
{
	QStandardItem::appendRow( it );
	if ( it->value( "type" ) != "project" )
		connect( it, SIGNAL( modifiedChanged( ProjectItem*, bool ) ), this, SIGNAL( modifiedChanged( ProjectItem*, bool ) ) );
}

QList<ProjectItem*> ProjectItem::children( bool r, bool s ) const
{
	QList<ProjectItem*> l;
	for ( int i = 0; i < rowCount(); i++ )
	{
		ProjectItem* cit = child( i );
		if ( !s || ( s && !model()->isProject( cit ) ) )
		{
			l << cit;
			if ( r )
				l << cit->children( r, s );
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
	if ( model()->isProject( this ) )
		foreach ( ProjectItem* it, children( true, true ) )
			if ( it->modified() )
				return true;
	return mModified;
}

void ProjectItem::setModified( bool b, bool e )
{
	if ( model()->isProject( this ) && b && !modified() )
	{
		mModified = true;
		if ( e )
			emit modifiedChanged( this, true );
	}
	else if ( model()->isProject( this ) && !b && modified() )
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

QString ProjectItem::projectFilePath() const
{
	if ( ProjectItem* pi = model()->project( this ) )
		return pi->mProjectFilePath;
	return QString();
}

QString ProjectItem::projectPath() const
{ return QFileInfo( projectFilePath() ).canonicalPath(); }

QString ProjectItem::filePath( const QString& s )
{
	if ( s.isEmpty() && model()->isType( this, "value" ) )
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
