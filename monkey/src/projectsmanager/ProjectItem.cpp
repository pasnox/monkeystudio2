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

ProjectItem::ProjectItem( ProjectsModel::NodeType t, ProjectItem* i )
{
	// set type
	setType( t );
	
	// append to parent if needed
	if ( i )
		i->appendRow( this );
	
	/*
	FirstType = 0, // first type
	EmptyType, // empty line
	FileType, // a file
	CommentType, // comment line
	NestedScopeType, // single line scope
	ScopeType, // multi line scope
	ScopeEndType, // end of a multi scope line
	VariableType, // variable
	ValueType, // value
	FunctionType, // function
	IncludeType, // include
	ProjectType, // project
	LastType // last type
	*/
}

QList<QStandardItem*> projectToStandardItemList( const QList<ProjectItem*> l )
{
	QList<QStandardItem*> ll;
	foreach ( ProjectItem* it, l )
		ll << reinterpret_cast<QStandardItem*>( it );
	return ll;
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
{
	insertRow( rowCount(), i );
}

void ProjectItem::insertRow( int r, ProjectItem* i )
{
	// check scope & function item
	ProjectItem* it = reinterpret_cast<ProjectItem*>( i );
	if ( it && ( getType() == ProjectsModel::NestedScopeType || getType() == ProjectsModel::ScopeType || getType() == ProjectsModel::FunctionType ) && it->getType() != ProjectsModel::ScopeEndType )
		r--;
	
	// default insert
	QStandardItem::insertRow( r, i );
}


/*
void ProjectItem::appendRow( const QList<ProjectItem*>& l )
{ QStandardItem::appendRow( projectToStandardItemList( l ) ); }

void ProjectItem::appendRow( ProjectItem* i )
{ QStandardItem::appendRow( i ); }

void ProjectItem::appendRows( const QList<ProjectItem*>& l )
{ QStandardItem::appendRows( projectToStandardItemList( l ) ); }

void ProjectItem::insertColumn( int i, const QList<ProjectItem*>& l )
{ QStandardItem::insertColumn( i, projectToStandardItemList( l ) ); }

void ProjectItem::insertColumns( int i, int c )
{ QStandardItem::insertColumns( i, c ); }

void ProjectItem::insertRow( int i, const QList<ProjectItem*>& l )
{ QStandardItem::insertRow( i, projectToStandardItemList( l ) ); }

void ProjectItem::insertRow( int r, ProjectItem* i )
{ QStandardItem::insertRow( r, i ); }

void ProjectItem::insertRows( int i, const QList<ProjectItem*>& l )
{ QStandardItem::insertRows( i, projectToStandardItemList( l ) ); }

void ProjectItem::insertRows( int r, int c )
{ QStandardItem::insertRows( r, c ); }

void ProjectItem::removeColumn( int i )
{ QStandardItem::removeColumn( i ); }

void ProjectItem::removeColumns( int i, int c )
{ QStandardItem::removeColumns( i, c ); }

void ProjectItem::removeRow( int i )
{ QStandardItem::removeRow( i ); }

void ProjectItem::removeRows( int r, int c )
{ QStandardItem::removeRows( r, c ); }

template <class T> T ProjectItem::child( int r, int c ) const
{ return reinterpret_cast<T>( QStandardItem::child( r, c ) ); }

template <class T> T ProjectItem::clone() const
{ return reinterpret_cast<T>( QStandardItem::clone() ); }

template <class T> T ProjectItem::parent() const
{ return reinterpret_cast<T>( QStandardItem::parent() ); }

void ProjectItem::setChild( int r, int c, ProjectItem* i )
{ QStandardItem::setChild( r, c, i ); }

void ProjectItem::setChild( int r, ProjectItem* i )
{ QStandardItem::setChild( r, i ); }

template <class T> T ProjectItem::takeChild( int r, int c )
{ return reinterQStandardItem::takeChild( r, c ); }

template <class T> QList<T> ProjectItem::takeColumn( int i )
{ return QStandardItem::takeColumn( i ); }

template <class T> QList<T> ProjectItem::takeRow( int i )
{ return QStandardItem::takeRow( i ); }
*/

