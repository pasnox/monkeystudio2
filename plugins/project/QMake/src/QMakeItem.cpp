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

#include "pMonkeyStudio.h"

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
		{
			// get parent value
			QString v = parent() ? parent()->getValue() : QString::null;
			// if file/path base variable
			if ( l3.contains( v, Qt::CaseInsensitive ) )
			{
				// if special case subdirs
				if ( v.toLower() == "subdirs" )
				{
					// get path to check in for pro
					v = canonicalFilePath( s );
					// get all pro file
					foreach ( QFileInfo f, pMonkeyStudio::getFiles( QDir( v ), "pro", false ) )
					{
						// check that value = filename
						if ( f.baseName().toLower() == s.toLower() )
						{
							// set full file path
							setFilePath( v.append( "/" ).append( f.fileName() ) );
							break;
						}
					}
				}
				// set full file path
				else
					setFilePath( s );
			}
			break;
		}
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

void QMakeItem::setFilePath( const QString& s )
{
	// set data
	setData( canonicalFilePath( s ), ProjectsModel::FilePathRole );
	
	// set text
	switch( getType() )
	{
		case ProjectsModel::ValueType:
			if ( parent() && parent()->getValue().toLower() != "subdirs" )
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
