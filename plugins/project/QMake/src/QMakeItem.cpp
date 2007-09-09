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
#include "UISettingsQMake.h"

QMakeItem::QMakeItem( ProjectsModel::NodeType t, QMakeItem* i )
	: ProjectItem()
{
	// set type
	setType( t );
	
	// set readonly
	setReadOnly( false );
	
	// append to parent if needed
	if ( i )
		i->appendRow( this );
}

ProjectItem* QMakeItem::clone( ProjectsModel::NodeType t, ProjectItem* p ) const
{ return new QMakeItem( t, reinterpret_cast<QMakeItem*>( p ) ); }

void QMakeItem::setType( ProjectsModel::NodeType t )
{
	// set data
	setData( t, ProjectsModel::TypeRole );
	
	// no modified
	setModified( false );
	
	// update icon
	switch( t )
	{
		case ProjectsModel::EmptyType:
			setIcon( QIcon( ":/icons/icons/line.png" ) );
			break;
		case ProjectsModel::FileType:
			setIcon( QIcon( ":/icons/icons/file.png" ) );
			break;
		case ProjectsModel::CommentType:
			setIcon( QIcon( ":/icons/icons/comment.png" ) );
			break;
		case ProjectsModel::NestedScopeType:
			setIcon( QIcon( ":/icons/icons/scope.png" ) );
			break;
		case ProjectsModel::ScopeType:
			setIcon( QIcon( ":/icons/icons/scope.png" ) );
			break;
		case ProjectsModel::ScopeEndType:
			setIcon( QIcon( ":/icons/icons/scope_end.png" ) );
			break;
		case ProjectsModel::VariableType:
			setIcon( QIcon( ":/icons/icons/variable.png" ) );
			break;
		case ProjectsModel::ValueType:
			setIcon( QIcon( ":/icons/icons/value.png" ) );
			break;
		case ProjectsModel::FunctionType:
			setIcon( QIcon( ":/icons/icons/function.png" ) );
			break;
		case ProjectsModel::IncludeType:
			setIcon( QIcon( ":/icons/icons/include.png" ) );
			break;
		case ProjectsModel::ProjectType:
			setIcon( QIcon( ":/icons/icons/project.png" ) );
			break;
		default:
			setIcon( QIcon() );
			break;
	}
}

ProjectsModel::NodeType QMakeItem::getType()
{ return (ProjectsModel::NodeType)data( ProjectsModel::TypeRole ).toInt(); }

void QMakeItem::setOperator( const QString& s )
{ setData( s, ProjectsModel::OperatorRole ); }

QString QMakeItem::getOperator() const
{ return data( ProjectsModel::OperatorRole ).toString(); }

void QMakeItem::setValue( const QString& s )
{
	// set data
	setData( s, ProjectsModel::ValueRole );
	
	// set text
	setText( s );
	
	// variables name to changes
	QStringList l1 = UISettingsQMake::readFilters();
	QStringList l2 = UISettingsQMake::readFiltersToolTips();
	QStringList l3 = UISettingsQMake::readPathFiles();
	
	// update text
	switch( getType() )
	{
		case ProjectsModel::VariableType:
			if ( l1.contains( getValue(), Qt::CaseInsensitive ) )
				setText( l2.at( l1.indexOf( getValue(), Qt::CaseInsensitive ) ) );
			break;
		case ProjectsModel::ValueType:
			if ( l3.contains( parent() ? parent()->getValue() : "", Qt::CaseInsensitive ) )
				setFilePath( s );
			break;
		case ProjectsModel::IncludeType:
			setFilePath( "" );
			break;
		case ProjectsModel::ProjectType:
			setFilePath( s );
			break;
		case ProjectsModel::ScopeType:
		case ProjectsModel::NestedScopeType:
			if ( s.toLower() == "win32" )
				setIcon( QIcon( ":/icons/icons/windows.png" ) );
			else if ( s.toLower() == "unix" )
				setIcon( QIcon( ":/icons/icons/unix.png" ) );
			else if ( s.toLower() == "mac" )
				setIcon( QIcon( ":/icons/icons/mac.png" ) );
			break;
		default:
			break;
	}
}

QString QMakeItem::getValue() const
{ return data( ProjectsModel::ValueRole ).toString(); }

void QMakeItem::setMultiLine( bool b )
{ setData( b, ProjectsModel::MultiLineRole ); }

bool QMakeItem::getMultiLine() const
{ return data( ProjectsModel::MultiLineRole ).toBool(); }

void QMakeItem::setModified( bool b )
{
	setData( b, ProjectsModel::ModifiedRole );
}

bool QMakeItem::getModified() const
{ return data( ProjectsModel::ModifiedRole ).toBool(); }

void QMakeItem::setReadOnly( bool b )
{ setData( b, ProjectsModel::ReadOnlyRole ); }

bool QMakeItem::getReadOnly() const
{ return data( ProjectsModel::ReadOnlyRole ).toBool(); }

void QMakeItem::setComment( const QString& s )
{ setData( s, ProjectsModel::CommentRole ); }

QString QMakeItem::getComment() const
{ return data( ProjectsModel::CommentRole ).toString(); }

void QMakeItem::setFilePath( const QString& s )
{
	// set data
	setData( canonicalFilePath( s ), ProjectsModel::FilePathRole );
	
	// set text
	switch( getType() )
	{
		case ProjectsModel::ValueType:
			setText( fileName( s ) );
			setToolTip( getFilePath() );
			break;
		case ProjectsModel::ProjectType:
			setText( completeBaseName( s ) );
			setToolTip( getFilePath() );
			break;
		default:
			break;
	}
}

QString QMakeItem::getFilePath() const
{ return data( ProjectsModel::FilePathRole ).toString(); }

void QMakeItem::redoLayout( ProjectItem* it )
{
	// get correct item
	it = it ? it : project();
	
	// variables name to changes
	QStringList l1 = UISettingsQMake::readFilters();
	
	// layout it child
	for ( int i = 0; i < it->rowCount(); i++ )
	{
		// get child
		ProjectItem* c = it->child( i );
		
		// chld value
		int j = l1.indexOf( c->getValue(), Qt::CaseInsensitive );
		
		// change it s filter role if needed
		if ( c->getType() == ProjectsModel::VariableType )
		{
			// set child filter
			c->setFilteredView( j );
			
			// if it is scope/nestedscope change it s filter too
			if ( it->getType() == ProjectsModel::ScopeType || it->getType() == ProjectsModel::NestedScopeType )
				it->setFilteredView( j );
		}
		
		// set original view
		c->setOriginalView( i );
		
		// do child layout if needed
		if ( c->rowCount() )
			redoLayout( c );
	}
}
