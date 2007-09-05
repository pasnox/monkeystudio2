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

ProjectItem::ProjectItem( ProjectsModel::NodeType t, ProjectItem* i )
{
	// set type
	setType( t );
	
	// append to parent if needed
	if ( i )
		i->appendRow( this );
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

void ProjectItem::setModified( bool b )
{ setData( b, ProjectsModel::ModifiedRole ); }

bool ProjectItem::getModified() const
{ return data( ProjectsModel::ModifiedRole ).toBool(); }

void ProjectItem::setReadOnly( bool b )
{ setData( b, ProjectsModel::ReadOnlyRole ); }

bool ProjectItem::getReadOnly() const
{ return data( ProjectsModel::ReadOnlyRole ).toBool(); }

void ProjectItem::setComment( const QString& s )
{ setData( s, ProjectsModel::CommentRole ); }

QString ProjectItem::getComment() const
{ return data( ProjectsModel::CommentRole ).toString(); }

void ProjectItem::setFilePath( const QString& s )
{ setData( s, ProjectsModel::FilePathRole ); }

QString ProjectItem::getFilePath() const
{ return data( ProjectsModel::FilePathRole ).toString(); }

void ProjectItem::appendRow( ProjectItem* i )
{ insertRow( rowCount(), i ); }

void ProjectItem::insertRow( int r, ProjectItem* i )
{
	// check scope & function item for not be able to append items after scope end
	ProjectItem* it = reinterpret_cast<ProjectItem*>( i );
	if ( it && ( getType() == ProjectsModel::NestedScopeType || getType() == ProjectsModel::ScopeType || getType() == ProjectsModel::FunctionType ) && it->getType() != ProjectsModel::ScopeEndType )
		r--;
	
	// default insert
	QStandardItem::insertRow( r, i );
}

ProjectItem* ProjectItem::parent() const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::parent() ); }

QString ProjectItem::canonicalFilePath() const
{
	// get project item
	ProjectItem* it = const_cast<ProjectItem*>( this );
	
	// check reucursively parent if needed
	while ( it && it->getType() != ProjectsModel::ProjectType )
		it = it->parent();
	
	// return project absolutepath
	if ( it )
		return it->getFilePath();
	
	// return nothing
	return QString();
}

QString ProjectItem::canonicalFilePath( const QString& s ) const
{
	// file path
	QString fp = s;
	
	// check if relative or absolute
	if ( QFileInfo( s ).isRelative() )
		fp = canonicalPath().append( "/" ).append( s );
	
	// return canonical file path if file exists, exists
	if ( QFile::exists( fp ) )
		return QFileInfo( fp ).canonicalFilePath();
	
	// else return absolute file path, may be not exists
	return QFileInfo( fp ).absoluteFilePath();
}

QString ProjectItem::canonicalPath() const
{ return QFileInfo( canonicalFilePath() ).absolutePath(); }

QString ProjectItem::fileName( const QString& s )
{ return QFileInfo( s ).fileName(); }

QString ProjectItem::completeBaseName( const QString& s )
{ return QFileInfo( s ).completeBaseName(); }
