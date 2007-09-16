/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox aka Azevedo Filipe <pasnox@gmail.com>
** Project   : ProjectItem
** FileName  : ProjectItem.cpp
** Date      : 2007-09-04T15:07:31
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "ProjectItem.h"

#include <QFileInfo>
#include <QDir>
#include <QTextCodec>

ProjectItem::ProjectItem( ProjectsModel::NodeType t, ProjectItem* i )
{
	// clear buffer
	mBuffer.clear();
	// set type
	setType( t );
	// set readonly
	setReadOnly( false );
	// append to parent if needed
	if ( i )
		i->appendRow( this );
}

void ProjectItem::setData( const QVariant& v, int r )
{
	// check read only
	if ( getReadOnly() )
		return;
	// set data
	QStandardItem::setData( v, r );
	// set modified state
	if ( r != ProjectsModel::ModifiedRole )
		setModified( true );
}

void ProjectItem::setType( ProjectsModel::NodeType t )
{ setData( t, ProjectsModel::TypeRole ); }

ProjectsModel::NodeType ProjectItem::getType()
{ return (ProjectsModel::NodeType)data( ProjectsModel::TypeRole ).toInt(); }

void ProjectItem::setOperator( const QString& s )
{ setData( s, ProjectsModel::OperatorRole ); }

QString ProjectItem::getOperator() const
{ return data( ProjectsModel::OperatorRole ).toString(); }

void ProjectItem::setValue( const QString& s )
{ setData( s, ProjectsModel::ValueRole ); }

QString ProjectItem::getValue() const
{ return data( ProjectsModel::ValueRole ).toString(); }

void ProjectItem::setMultiLine( bool b )
{ setData( b, ProjectsModel::MultiLineRole ); }

bool ProjectItem::getMultiLine() const
{ return data( ProjectsModel::MultiLineRole ).toBool(); }

void ProjectItem::setComment( const QString& s )
{ setData( s, ProjectsModel::CommentRole ); }

QString ProjectItem::getComment() const
{ return data( ProjectsModel::CommentRole ).toString(); }

void ProjectItem::setFilePath( const QString& s )
{ setData( s, ProjectsModel::FilePathRole ); }

QString ProjectItem::getFilePath() const
{ return data( ProjectsModel::FilePathRole ).toString(); }

void ProjectItem::setFilteredView( int i )
{ setData( i, ProjectsModel::FilteredViewRole ); }

int ProjectItem::getFilteredView() const
{ return data( ProjectsModel::FilteredViewRole ).toInt(); }

void ProjectItem::setOriginalView( int i )
{ setData( i, ProjectsModel::OriginalViewRole ); }

int ProjectItem::getOriginalView() const
{ return data( ProjectsModel::OriginalViewRole ).toInt(); }

void ProjectItem::setModified( bool b )
{
	// if item project and not modified
	if ( project() == this && !b )
	{
		// set unmodified to all items
		foreach ( ProjectItem* it, projectItems() )
			it->setModified( b );
	}
	else
		setData( b, ProjectsModel::ModifiedRole );
}

bool ProjectItem::getModified() const
{
	// if project, modified is true if one of its items is modified
	if ( project() == this )
	{
		foreach ( ProjectItem* it, projectItems() )
			if ( it->getModified() )
				return true;
	}
	// return default item value
	return data( ProjectsModel::ModifiedRole ).toBool();
}

void ProjectItem::setReadOnly( bool b )
{ setData( b, ProjectsModel::ReadOnlyRole ); }

bool ProjectItem::getReadOnly() const
{ return data( ProjectsModel::ReadOnlyRole ).toBool(); }

ProjectsModel* ProjectItem::model() const
{ return reinterpret_cast<ProjectsModel*>( QStandardItem::model() ); }

ProjectItem* ProjectItem::parent() const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::parent() ); }

int ProjectItem::parentCount() const
{
	ProjectItem* p = project();
	ProjectItem* it = const_cast<ProjectItem*>( this );
	int i = 0;
	while ( it && it != p )
	{
		i++;
		it = it->parent();
	}
	return i;
}

ProjectItem* ProjectItem::child( int i, int j ) const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::child( i, j ) ); }

ProjectItemList ProjectItem::children( bool r, bool s ) const
{
	ProjectItem* p = s ? project() : 0;
	ProjectItemList l;
	for ( int i = 0; i < rowCount(); i++ )
	{
		ProjectItem* ci = child( i, 0 );
		if ( !s || s && ci->project() == p )
		{
			l << ci;
			if ( r && ci->rowCount() )
				l << ci->children( r, s );
		}
	}
	return l;
}

ProjectItem* ProjectItem::project() const
{
	ProjectItem* it = const_cast<ProjectItem*>( this );
	while ( it && it->getType() != ProjectsModel::ProjectType )
		it = it->parent();
	return it;
}

ProjectItem* ProjectItem::parentProject() const
{
	ProjectItem* p = project();
	return p && p->parent() ? p->parent()->project() : 0;
}

ProjectItemList ProjectItem::childrenProjects( bool b ) const
{
	ProjectItemList l;
	foreach ( ProjectItem* it, children( b ) )
		if ( it->getType() == ProjectsModel::ProjectType )
			l << it;
	return l;
}

ProjectItemList ProjectItem::projectItems() const
{ return project()->children( true, true ); }

ProjectItemList ProjectItem::projectScopes() const
{
	ProjectItemList l;
	foreach ( ProjectItem* it, projectItems() )
		if ( it->getType() == ProjectsModel::ScopeType || it->getType() == ProjectsModel::NestedScopeType )
			l << it;
	return l;
}

QStringList ProjectItem::projectScopesList() const
{
	QStringList l( "" );
	foreach ( ProjectItem* it, projectScopes() )
	{
		const QString s = it->scope();
		if ( !l.contains( s ) )
			l << s;
	}
	return l;
}

QString ProjectItem::canonicalFilePath() const
{ return project()->getFilePath(); }

QString ProjectItem::canonicalFilePath( const QString& s ) const
{
	QString fp = s;
	if ( QFileInfo( fp ).isRelative() )
		fp = canonicalPath().append( "/" ).append( fp );
	if ( QFile::exists( fp ) )
		return QFileInfo( fp ).canonicalFilePath();
	return QFileInfo( fp ).absoluteFilePath();
}

QString ProjectItem::canonicalPath() const
{ return QFileInfo( canonicalFilePath() ).absolutePath(); }

QString ProjectItem::canonicalPath( const QString& s ) const
{ return canonicalFilePath( s ); }

QString ProjectItem::relativeFilePath( const QString& s ) const
{ return QDir( canonicalPath() ).relativeFilePath( s ); }

QString ProjectItem::fileName( const QString& s )
{ return QFileInfo( s ).fileName(); }

QString ProjectItem::completeBaseName( const QString& s )
{ return QFileInfo( s ).completeBaseName(); }

QString ProjectItem::name() const
{ return QFileInfo( canonicalFilePath() ).completeBaseName(); }

ProjectItemList ProjectItem::match( int r, const QVariant& v ) const
{
	ProjectItemList l;
	const QString s = v.toString();
	foreach ( ProjectItem* it, projectItems() )
		if ( it->getValue() == s || s == "*" )
			l << it;
	return l;
}

ProjectItemList ProjectItem::getItemList( ProjectsModel::NodeType t, const QString& v, const QString& o, const QString& s ) const
{
	ProjectItemList l;
	foreach ( ProjectItem* it, match( ProjectsModel::ValueRole, v ) )
		if ( isEqualScope( it->scope(), s ) )
			if ( ( !o.isEmpty() && it->getOperator() == o ) || o.isEmpty() )
				if ( it->getType() == t )
					l << it;
	return l;
}

ProjectItemList ProjectItem::getItemListValues( const QString& v, const QString& o, const QString& s ) const
{
	ProjectItemList l;
	foreach ( ProjectItem* it, getItemList( ProjectsModel::VariableType, v, o, s ) )
		for ( int i = 0; i < it->rowCount(); i++ )
			if ( it->child( i, 0 )->getType() == ProjectsModel::ValueType )
				l << it->child( i, 0 );
	return l;
}

ProjectItem* ProjectItem::getItemVariable( const QString& v, const QString& o, const QString& s ) const
{ return getItemList( ProjectsModel::VariableType, v, o, s ).value ( 0 ); }
