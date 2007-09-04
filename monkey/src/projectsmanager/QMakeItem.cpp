/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox aka Azevedo Filipe <pasnox@gmail.com>
** Project   : QMakeItem
** FileName  : QMakeItem.cpp
** Date      : 2007-09-04T15:07:31
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "QMakeItem.h"

QMakeItem::QMakeItem( ProjectsModel::NodeType t, QMakeItem* i )
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

void QMakeItem::setType( ProjectsModel::NodeType t )
{
	setData( t, ProjectsModel::TypeRole );
}

ProjectsModel::NodeType QMakeItem::getType()
{
	return (ProjectsModel::NodeType)data( ProjectsModel::TypeRole ).toInt();
}

void QMakeItem::setOperator( const QString& s )
{
	setData( s, ProjectsModel::OperatorRole );
}

QString QMakeItem::getOperator() const
{
	return data( ProjectsModel::OperatorRole ).toString();
}

void QMakeItem::setValue( const QString& s )
{
	setData( s, ProjectsModel::ValueRole );
	setText( s );
}

QString QMakeItem::getValue() const
{
	return data( ProjectsModel::ValueRole ).toString();
}

void QMakeItem::setMultiLine( bool b )
{
	setData( b, ProjectsModel::MultiLineRole );
}

bool QMakeItem::getMultiLine() const
{
	return data( ProjectsModel::MultiLineRole ).toBool();
}

void QMakeItem::setModified( bool b )
{
	setData( b, ProjectsModel::ModifiedRole );
}

bool QMakeItem::getModified() const
{
	return data( ProjectsModel::ModifiedRole ).toBool();
}

void QMakeItem::setReadOnly( bool b )
{
	setData( b, ProjectsModel::ReadOnlyRole );
}

bool QMakeItem::getReadOnly() const
{
	return data( ProjectsModel::ReadOnlyRole ).toBool();
}

void QMakeItem::setComment( const QString& s )
{
	setData( s, ProjectsModel::CommentRole );
}

QString QMakeItem::getComment() const
{
	return data( ProjectsModel::CommentRole ).toString();
}

void QMakeItem::setFilePath( const QString& s )
{
	setData( s, ProjectsModel::FilePathRole );
}

QString QMakeItem::getFilePath() const
{
	return data( ProjectsModel::FilePathRole ).toString();
}
/*
void QMakeItem::appendRow( const QList<QMakeItem*>& )
{
}

void QMakeItem::appendRow( QMakeItem* )
{
}

void QMakeItem::appendRows( const QList<QMakeItem*>& )
{
}

void QMakeItem::insertColumn( int, const QList<QMakeItem*>& )
{
}

void QMakeItem::insertColumns( int, int )
{
}

void QMakeItem::insertRow( int, const QList<QMakeItem*>& )
{
}

void QMakeItem::insertRow( int, QMakeItem* )
{
}

void QMakeItem::insertRows( int, const QList<QMakeItem*>& )
{
}

void QMakeItem::insertRows( int, int )
{
}

void QMakeItem::removeColumn( int )
{
}

void QMakeItem::removeColumns( int, int )
{
}

void QMakeItem::removeRow( int )
{
}

void QMakeItem::removeRows( int, int )
{
}

QMakeItem* QMakeItem::child( int, int ) const
{
}

QMakeItem* QMakeItem::clone() const
{
}

QMakeItem* QMakeItem::parent() const
{
}

void QMakeItem::setChild( int, int, QMakeItem* )
{
}

void QMakeItem::setChild( int, QMakeItem* )
{
}

QMakeItem* QMakeItem::takeChild( int, int )
{
}

QList<QMakeItem*> QMakeItem::takeColumn( int )
{
}

QList<QMakeItem*> QMakeItem::takeRow( int )
{
}
*/
