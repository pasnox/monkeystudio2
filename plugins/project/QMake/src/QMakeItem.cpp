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

#include <QObject>

using namespace pMonkeyStudio;

QMakeItem::QMakeItem( ProjectsModel::NodeType t, ProjectItem* i )
	: ProjectItem()
{
	mBuffer.clear();
	setType( t );
	setReadOnly( false );
	if ( i )
		i->appendRow( this );
}

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
		case ProjectsModel::FolderType:
			setIcon( QIcon( ":/icons/icons/folder.png" ) );
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
				setText( l2.value( l1.indexOf( getValue(), Qt::CaseInsensitive ) ) );
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
	// cancel is file not exists
	if ( !QFile::exists( canonicalFilePath( s ) ) )
		return;
	
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

QString QMakeItem::getIndent() const //
{
	return QString();
	/*
	// if scope end, need take parent indent
	if ( const_cast<ProjectItem*>( this )->getType() == ProjectsModel::ScopeEndType )
		return parent()->getIndent();
	// if first value of a variable, no indent
	if ( const_cast<ProjectItem*>( this )->getType() == ProjectsModel::ValueType && isFirst() )
		return QString();
	// if not value, and parent is nestedscope, no indent
	if ( parent() && parent()->getType() == ProjectsModel::NestedScopeType )
		return QString();
	// default indent
	int i = -1;
	// count parent
	ProjectItem* p = project();
	ProjectItem* it = const_cast<ProjectItem*>( this );
	while ( it && ( it = it->parent() ) )
		if ( it->project() == p )
			i++;
	// return indent
	return QString().fill( '\t', i );
	*/
}

QString QMakeItem::getEol() const
{
	switch ( eolMode() )
	{
		case QsciScintilla::EolWindows:
			return QString( "\r\n" );
			break;
		case QsciScintilla::EolUnix:
			return QString( "\r" );
			break;
		case QsciScintilla::EolMac:
			return QString( "\n" );
			break;
		default:
			return QString( "\r\n" );
	}
}

bool QMakeItem::isFirst() const
{
	return row() == 0;
}

bool QMakeItem::isLast() const
{
	if ( model() )
		return model()->rowCount( index().parent() ) -1 == row();
	else if ( parent() )
		return parent()->rowCount() -1 == row();
	Q_ASSERT( 0 );
	return false;
}

QString QMakeItem::scope() const
{
	QString s;
	ProjectItem* j = const_cast<QMakeItem*>( this );
	ProjectItem* p = project();
	while ( j && j != p )
	{
		if ( j->getType() == ProjectsModel::ScopeType || j->getType() == ProjectsModel::NestedScopeType )
			s.prepend( QString( "%1%2" ).arg( j->getValue(), j->getOperator().isEmpty() ? ":" : j->getOperator() ) );
		j = j->parent();
	}
	s.chop( 1 );
	return s;
}

QString QMakeItem::checkScope( const QString& s ) const
{
	QString ss = s.trimmed();
	if ( ss.endsWith( ':' ) )
		ss.chop( 1 );
	return ss;
}

bool QMakeItem::isEqualScope( const QString& s1, const QString& s2 ) const
{
	if ( s2 == "*" )
		return true;
	return checkScope( QString( s1 ).replace( '/', ':' ) ) == checkScope( QString( s2 ).replace( '/', ':' ) );
}

void QMakeItem::appendRow( ProjectItem* i )
{
	insertRow( rowCount(), i );
}

void QMakeItem::insertRow( int r, ProjectItem* it )
{
	// check scope & function item for not be able to append items after scope end
	if ( it &&  it->getType() != ProjectsModel::ScopeEndType && ( getType() == ProjectsModel::NestedScopeType || getType() == ProjectsModel::ScopeType ) )
		r--;
	// if there is subproject, we need to create items before subproject
	if ( hasChildren() )
	{
		ProjectItem* p = project();
		ProjectItem* i = 0;
		while ( ( i = child( r -1 ) ) && i->project() != p )
			r--;
	}
	// default insert
	QStandardItem::insertRow( r, it );
	qWarning( qPrintable( QString( "Inserted item: %1, %2, %3, %4 - %5, parent: %6" ).arg( it->getValue() ).arg( it->getType() ).arg( it->scope() ).arg( it->row() ).arg( it->column() ).arg( getValue() ) ) );
}

bool QMakeItem::swapRow( int i, int j )
{
	if ( -1 < i < rowCount() && -1 < j < rowCount() && i != i )
	{
		QList<QStandardItem*> ii;
		QList<QStandardItem*> ij;
		if ( i > j )
		{
			ii = takeRow( i );
			ij = takeRow( j );
		}
		else
		{
			ij = takeRow( j );
			ii = takeRow( i );
		}
		QStandardItem::insertRow( i, ij );
		QStandardItem::insertRow( j, ii );
		return true;
	}
	return false;
}

bool QMakeItem::moveRowUp( int i )
{
	ProjectItem* it = child( i );
	bool b = it && it->getType() != ProjectsModel::ScopeEndType && i > 0;
	if ( b )
		QStandardItem::insertRow( i -1, takeRow( i ) );
	return b;
}

bool QMakeItem::moveRowDown( int i )
{
	ProjectItem* it = child( i +1 );
	bool b = it && it->getType() != ProjectsModel::ScopeEndType && i < rowCount() -1;
	if ( b )
		QStandardItem::insertRow( i +1, takeRow( i ) );
	return b;
}

bool QMakeItem::moveUp()
{
	return parent() ? parent()->moveRowUp( row() ) : false;
}

bool QMakeItem::moveDown()
{
	return parent()  ? parent()->moveRowDown( row() ) : false;
}

void QMakeItem::remove()
{
	ProjectItem* it = parent();
	if ( !( it && ( it->getType() == ProjectsModel::ScopeType || it->getType() == ProjectsModel::NestedScopeType ) && it->rowCount() == 2 ) )
		it = this;
	qWarning( qPrintable( QString( "Removing item: %1, %2, %3, %4 - %5, parent: %6" ).arg( it->getValue() ).arg( it->getType() ).arg( it->scope() ).arg( it->row() ).arg( it->column() ).arg( it->parent() ? it->parent()->getValue() : "no parent" ) ) );
	if ( it->parent() )
		it->parent()->removeRow( it->row() );
	else if ( model() )
		model()->removeRow( it->row(), it->index().parent() );
}

void QMakeItem::close()
{
	saveAll( true );
	remove();
}

void QMakeItem::save( bool b )
{
	ProjectItem* p = project();
	if ( p->getReadOnly() || !p->getModified() )
		return;
	if ( !b || ( b && question( QObject::tr( "Save Project..." ), QObject::tr( "The project [%1] has been modified, save it ?" ).arg( name() ) ) ) )
		writeProject();
}

void QMakeItem::saveAll( bool b )
{
	// save current project
	save( b );
	// save all children projects
	foreach ( ProjectItem* it, childrenProjects() )
		it->save( b );
}

void QMakeItem::debug()
{
	QString s;
	foreach ( ProjectItem* it, projectItems() )
		s.append( QString( "%1(%2)%3%4%5" ).arg( QString().fill( ' ', it->parentCount() ) ).arg( it->getType() ).arg( it->getValue() ).arg( it->getOperator() ).arg( it->getEol() ) );
	qWarning( qPrintable( s ) );
}

ProjectItem* QMakeItem::getItemScope( const QString& s, bool c ) const
{
	if ( s.isEmpty() )
		return project();
	ProjectItem* sit = 0;
	foreach ( ProjectItem* it, projectScopes() )
	{
		// good scope
		if ( isEqualScope( it->scope(), s ) )
		{
			if ( it->getType() == ProjectsModel::NestedScopeType )
			{
				it->setType( ProjectsModel::ScopeType );
				it->setOperator( QString::null );
			}
			return it;
		}
		// else try found nearest scope
		else if  ( c && checkScope( s ).startsWith( it->scope() ) )
		{
			if ( it->getType() == ProjectsModel::ScopeType || ( it->getType() == ProjectsModel::NestedScopeType && s[it->scope().length()] == ':' ) )
				if ( !sit || ( sit && sit->scope().length() < it->scope().length() ) )
					sit = it;
		}
	}
	
	// not found and not want create
	if ( !c )
		return 0;
	
	// get nearest scope and remove it from full scope to create
	QString p = s;
	if ( sit )
		p.remove( sit->scope() );
	
	// remove trailing operator
	if ( p.startsWith( ':' ) )
		p.remove( 0, 1 );
	
	// split nested : scope
	QStringList l = p.split( ':', QString::SkipEmptyParts );
	
	// create each scope
	foreach ( QString scope, l )
	{
		sit = new QMakeItem( ProjectsModel::NestedScopeType, sit ? sit : project() );
		sit ->setValue( scope );
		sit ->setOperator( ":" );
		(void) new QMakeItem( ProjectsModel::ScopeEndType, sit );
	}
	
	// return full created scope
	return sit;
}

QStringList QMakeItem::getListValues( const QString& v, const QString& o, const QString& s ) const
{
	QStringList l;
	foreach ( ProjectItem* it, getItemListValues( v, o, s ) )
		l << it->getValue();
	return l;
}

QString QMakeItem::getStringValues( const QString& v, const QString& o, const QString& s ) const
{
	return getListValues( v, o, s ).join( " " );
}

void QMakeItem::setListValues( const QStringList& vl, const QString& v, const QString& o, const QString& s )
{
	ProjectItem* it = getItemVariable( v, o, s );
	// create index if it not exists
	if ( !it )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		it = new QMakeItem( ProjectsModel::VariableType, getItemScope( s, true ) );
		it->setValue( v );
		it->setOperator( o );
	}
	// set values
	QStringList l = vl;
	// check all child of variable and remove from model/list unneeded index/value
	foreach ( ProjectItem* c, it->children( false, true ) )
	{
		if ( c->getType() == ProjectsModel::ValueType )
		{
			if ( l.contains( c->getValue() ) )
				l.removeAll( c->getValue() );
			else if ( !l.value( 0 ).isEmpty() )
			{
				c->setValue( l.value( 0 ) );
				l.removeFirst();
			}
			else
				c->remove();
		}
	}
	// add require index
	if ( it )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = new QMakeItem( ProjectsModel::ValueType, it );
				cItem->setValue( e );
			}
		}
		// if variable is empty, remove it
		if ( !it->rowCount() )
			it->remove();
	}
}

void QMakeItem::setStringValues( const QString& val, const QString& v, const QString& o, const QString& s )
{
	setListValues( val.isEmpty() ? QStringList() : QStringList( val ), v, o, s );
}

void QMakeItem::addListValues( const QStringList& vl, const QString& v, const QString& o, const QString& s )
{
	ProjectItem* it = getItemVariable( v, o, s );
	// create index if it not exists
	if ( !it )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		it = new QMakeItem( ProjectsModel::VariableType, getItemScope( s, true ) );
		it->setValue( v );
		it->setOperator( o );
	}
	// add values
	QStringList l = vl;
	// check all values of variable and remove from list already existing values
	foreach ( ProjectItem* c, it->children( false, true ) )
		if ( c->getType() == ProjectsModel::ValueType )
			if ( l.contains( c->getValue() ) )
				l.removeAll( c->getValue() );
	// add require values
	if ( it )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = new QMakeItem( ProjectsModel::ValueType, it );
				cItem->setValue( e );
			}
		}
	}
}

void QMakeItem::addStringValues( const QString& val, const QString& v, const QString& o, const QString& s )
{
	addListValues( QStringList( val ), v, o, s );
}

void QMakeItem::redoLayout( ProjectItem* it ) //
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

#include <QTextEdit>
void QMakeItem::writeProject() //
{
	// clear buffer
	mBuffer.clear();
	// make sure we got a project item
	if ( ProjectItem* p = project() )
	{
		// write items
		foreach ( ProjectItem* it, p->children() )
			if ( it->project() == p )
				writeItem( it );
		// set project unmodified
		p->setModified( false );
		// append project file to buffer to check
		mBuffer.prepend( canonicalFilePath() +"\n\n" );
		
		QTextEdit* l = new QTextEdit;
		l->setPlainText( mBuffer );
		l->show();
	}
	else
		qWarning( qPrintable( "Can't write project: " +canonicalFilePath() ) );
}

void QMakeItem::writeItem( ProjectItem* it ) //
{
	// cancel if no item
	if ( !it )
		return;
	// write to buffer
	switch ( it->getType() )
	{
		case ProjectsModel::EmptyType:
		{
			mBuffer.append( it->getIndent() ).append( it->getEol() );
			break;
		}
		case ProjectsModel::FolderType:
		{
			break;
		}
		case ProjectsModel::CommentType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getEol() );
			break;
		}
		case ProjectsModel::NestedScopeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getOperator() );
			break;
		}
		case ProjectsModel::ScopeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( " {" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
			break;
		}
		case ProjectsModel::ScopeEndType:
		{
			if ( it->parent()->getType() == ProjectsModel::ScopeType )
			{
				// parent sibling
				ProjectItem* p = it->parent()->parent()->child( it->parent()->row() +1, 0 );
				if ( p && p->getType() != ProjectsModel::ScopeType && p->getType() != ProjectsModel::NestedScopeType && p->getValue().toLower() != "else" )
					p = 0;
				mBuffer.append( it->getIndent() ).append( "}" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( p ? " " : it->getEol() );
				//mBuffer.append( it->getIndent() ).append( "}" ).append().append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
			}
			break;
		}
		case ProjectsModel::VariableType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( "\t" ).append( it->getOperator() ).append( " " );
			break;
		}
		case ProjectsModel::ValueType:
		{
			if ( it->parent()->getMultiLine() )
				mBuffer.append( it->getIndent() ).append( it->getValue() ).append( !it->isLast() ? " \\" : "" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
			else
				mBuffer.append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->isLast() ? it->getEol() : " " );
			break;
		}
		case ProjectsModel::FunctionType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
			break;
		}
		case ProjectsModel::IncludeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
			break;
		}
		case ProjectsModel::ProjectType:
		{
			break;
		}
		default:
			break;
	}
	// write children
	ProjectItem* p = it->project();
	foreach ( ProjectItem* cit, it->children() )
		if ( cit->project() == p )
			writeItem( cit );
}
