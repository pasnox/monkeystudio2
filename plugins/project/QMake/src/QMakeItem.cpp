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

ProjectsModel::NodeType QMakeItem::type()
{
	return (ProjectsModel::NodeType)data( QMakeItem::TypeRole ).toInt();
}

void QMakeItem::setCaption( const QString& s )
{
	setData( s, ProjectsModel::CaptionRole );
}

QString QMakeItem::caption() const
{
	return data( QMakeItem::CaptionRole ).toString();
}

void QMakeItem::setOperator( const QString& s )
{
	setData( s, ProjectsModel::OperatorRole );
}

QString QMakeItem::operator() const
{
	return data( QMakeItem::OperatorRole ).toString();
}

void QMakeItem::setValue( const QString& s )
{
	setData( s, ProjectsModel::ValueRole );
}

QString QMakeItem::value() const
{
	return data( QMakeItem::ValueRole ).toString();
}

void QMakeItem::setMultiLine( bool b )
{
	setData( b, ProjectsModel::MultiLineRole );
}

bool QMakeItem::multiLine() const
{
	return data( QMakeItem::MultiLineRole ).toBool();
}

void QMakeItem::setComment( const QString& s )
{
	setData( s, ProjectsModel::CommentRole );
}

QString QMakeItem::comment() const
{
	return data( QMakeItem::CommentRole ).toString();
}

void QMakeItem::setFilePath( const QString& s )
{
	setData( s, ProjectsModel::FilePathRole );
}

QString QMakeItem::filePath() const
{
	return data( QMakeItem::FilePathRole ).toString();
}

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

QMakeItem* QMakeItem::child( int, int = 0 ) const
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

QMakeItem* QMakeItem::takeChild( int, int = 0 )
{
}

QList<QMakeItem*> QMakeItem::takeColumn( int )
{
}

QList<QMakeItem*> QMakeItem::takeRow( int )
{
}

