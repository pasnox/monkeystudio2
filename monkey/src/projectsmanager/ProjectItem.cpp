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
#include "pMonkeyStudio.h"

#include <QFileInfo>
#include <QDir>
#include <QTextCodec>

ProjectItem::ProjectItem( ProjectsModel::NodeType t, ProjectItem* i )
{
	// set type
	setType( t );
	// set readonly
	setReadOnly( false );
	// append to parent if needed
	if ( i )
		i->appendRow( this );
}

ProjectItem::~ProjectItem()
{}

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

QString ProjectItem::getIndent() const
{
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
}

QString ProjectItem::getEol() const
{
	switch ( pMonkeyStudio::eolMode() )
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
	}
}

bool ProjectItem::isFirst() const
{ return parent() ? parent()->child( 0, 0 ) == this : true; }

bool ProjectItem::isLast() const
{ return parent() ? parent()->rowCount() -1 == row() : true; }

ProjectsModel* ProjectItem::model() const
{ return reinterpret_cast<ProjectsModel*>( QStandardItem::model() ); }

ProjectItem* ProjectItem::parent() const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::parent() ); }

ProjectItem* ProjectItem::child( int i, int j ) const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::child( i, j ) ); }

ProjectItemList ProjectItem::children( bool b ) const
{
	ProjectItemList l;
	for ( int i = 0; i < rowCount(); i++ )
	{
		ProjectItem* ci = child( i, 0 );
		l << ci;
		if ( b && ci->rowCount() )
			l << ci->children( true );
	}
	return l;
}

ProjectItem* ProjectItem::clone( ProjectsModel::NodeType t, ProjectItem* p ) const
{ return new ProjectItem( t, p ); }

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

bool ProjectItem::swapRow( int i, int j )
{
	// temp row list
	QList<QStandardItem*> ii;
	QList<QStandardItem*> ij;
	// if get the rows
	if ( !( ii = takeRow( i ) ).isEmpty() && !( ij = takeRow( j ) ).isEmpty() )
	{
		// reinsert them
		QStandardItem::insertRow( i, ij );
		QStandardItem::insertRow( j, ii );
	}
	// return true if they are not empty
	return !ii.isEmpty() && !ij.isEmpty();
}

bool ProjectItem::moveRowUp( int i )
{
	// we can't move up the scope end item
	ProjectItem* it = child( i, 0 );
	if ( !it || ( it && it->getType() == ProjectsModel::ScopeEndType ) )
		return false;
	// if valid, move it
	if ( i > 0 )
		QStandardItem::insertRow( i -1, takeRow( i ) );
	return i > 0;
}

bool ProjectItem::moveRowDown( int i )
{
	// if in a scope we can t go to very last item as it s the scope end
	ProjectItem* it = child( i +1, 0 );
	if ( !it || ( it && it->getType() == ProjectsModel::ScopeEndType ) )
		return false;
	// if valid, move it
	if ( i < rowCount() -1 )
		QStandardItem::insertRow( i +1, takeRow( i ) );
	return i < rowCount() -1;
}

bool ProjectItem::moveUp()
{ return parent() ? parent()->moveRowUp( row() ) : false; }

bool ProjectItem::moveDown()
{ return parent()  ? parent()->moveRowDown( row() ) : false; }

void ProjectItem::remove()
{
	if ( parent() )
		parent()->removeRow( row() );
	else if ( model() )
		model()->removeRow( row() );
	else
		delete this;
}

ProjectItem* ProjectItem::project() const
{
	// get project item
	ProjectItem* it = const_cast<ProjectItem*>( this );
	// check reucursively parent if needed
	while ( it && it->getType() != ProjectsModel::ProjectType )
		it = it->parent();
	// return item
	return it;
}

ProjectItemList ProjectItem::projectItems() const
{
	ProjectItem* p = project();
	ProjectItemList l;
	foreach ( ProjectItem* it, p->children( true ) )
		if ( it->project() == p )
			l << it;
	return l;
}

ProjectItem* ProjectItem::parentProject() const
{
	ProjectItem* p = project();
	return p && p->parent() ? p->parent()->project() : 0;
}

ProjectItemList ProjectItem::childrenProjects( bool b ) const
{
	ProjectItemList l;
	foreach ( ProjectItem* it, children( b) )
		if ( it->getType() == ProjectsModel::ProjectType )
			l << it;
	return l;
}

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
	QStringList l( "" ); // root scope
	foreach ( ProjectItem* it, projectItems() )
	{
		if ( it->getType() == ProjectsModel::ScopeType || it->getType() == ProjectsModel::NestedScopeType )
		{
			const QString s = it->scope();
			if ( !l.contains( s ) )
				l << s;
		}
	}
	return l;
}

ProjectItemList ProjectItem::match( ProjectItem* i, int r, const QVariant& v ) const
{
	ProjectItemList l;
	const QString s = v.toString().toLower();
	foreach ( ProjectItem* it, i->projectItems() )
		if ( it->getValue().toLower() == s || s == "*" )
			l << it;
	return l;
}

QString ProjectItem::checkScope( const QString& s ) const
{
	// remove trailing : as it s the default operator
	QString ss = s.trimmed();
	if ( ss.endsWith( ':' ) )
		ss.chop( 1 );
	return ss;
}

bool ProjectItem::isEqualScope( const QString& s1, const QString& s2 ) const
{
	if ( s2 == "*" )
		return true;
	QString ss1( s1.toLower() ), ss2( s2.toLower() );
	ss1.replace( '/', ':' );
	ss2.replace( '/', ':' );
	return checkScope( ss1 ) == checkScope( ss2 );
}

QString ProjectItem::scope() const
{
	QString s;
	ProjectItem* j = const_cast<ProjectItem*>( this );
	ProjectItem* p = project();
	while ( j && j != p )
	{
		if ( j->getType() == ProjectsModel::ScopeType || j->getType() == ProjectsModel::NestedScopeType )
			s.prepend( QString( "%1%2" ).arg( j->getValue(), j->getOperator().isEmpty() ? ":" : j->getOperator() ) );
		j = j->parent();
	}
	s.chop( 1 ); // remove trailing operator
	return s;
}

ProjectItemList ProjectItem::getItemList( ProjectsModel::NodeType t, const QString& v, const QString& o, const QString& s ) const
{
	// temp list
	ProjectItemList l;
	foreach ( ProjectItem* it, match( project(), ProjectsModel::ValueRole, v ) )
		// check same scope
		if ( isEqualScope( it->scope(), s ) )
			// check operator
			if ( ( !o.isEmpty() && it->getOperator() == o ) || o.isEmpty() )
				// check type
				if ( it->getType() == t )
					// add to list
					l << it;
	// return list
	return l;
}

ProjectItem* ProjectItem::getItemScope( const QString& s, bool c ) const
{
	// get project item
	ProjectItem* sItem = project();
	// it there is scope
	if ( !s.trimmed().isEmpty() )
	{
		// get item scope and separeted operator
		static QRegExp r( "([^:|]+)(\\:|\\|)?" );
		int pos = 0;
		QString cScope = checkScope( s );
		// create scope is there are not existing
		while ( ( pos = r.indexIn( cScope, pos ) ) != -1 )
		{
			QStringList l = r.capturedTexts();
			QString s1 = l.at( 1 );
			QString s2 = l.at( 2 );
			QString sScope = cScope.left( pos ).append( s1 );
			// search existing scope
			ProjectItem* fItem = ( sItem->getItemList( ProjectsModel::ScopeType, s1, s2, sScope ) << sItem->getItemList( ProjectsModel::NestedScopeType, s1, s2, sScope ) ).value( 0 );
			// create scope
			if ( ( !fItem || fItem->project() != sItem ) )
			{
				if ( c )
				{
					sItem = clone( l.at( 2 ).trimmed().isEmpty() ? ProjectsModel::ScopeType : ProjectsModel::NestedScopeType, sItem );
					sItem->setValue( s1 );
					sItem->setOperator( s2 );
					(void) clone( ProjectsModel::ScopeEndType, sItem );
				}
				else
					return sItem;
			}
			else
				sItem = fItem;
			pos += r.matchedLength();
		}
		// if scope is nested, made it ScopeType
		if ( sItem && sItem->getType() == ProjectsModel::NestedScopeType )
		{
			sItem->setType( ProjectsModel::ScopeType );
			sItem->setOperator( QString::null );
		}
	}
	// return scope item
	return sItem;
}

ProjectItemList ProjectItem::getItemListValues( const QString& v, const QString& o, const QString& s ) const
{
	ProjectItemList l = getItemList( ProjectsModel::VariableType, v, o, s );
	ProjectItemList ll;
	foreach ( ProjectItem* it, l )
		for ( int i = 0; i < it->rowCount(); i++ )
			if ( it->child( i, 0 )->getType() == ProjectsModel::ValueType )
				ll << it->child( i, 0 );
	return ll;
}

ProjectItem* ProjectItem::getItemVariable( const QString& v, const QString& o, const QString& s ) const
{ return getItemList( ProjectsModel::VariableType, v, o, s ).value ( 0 ); }

QStringList ProjectItem::getListValues( const QString& v, const QString& o, const QString& s ) const
{
	QStringList l;
	foreach ( ProjectItem* it, getItemList( ProjectsModel::VariableType, v, o, s ) )
		for ( int i = 0; i < it->rowCount(); i++ )
			if ( it->child( i, 0 )->getType() == ProjectsModel::ValueType )
				l << it->child( i, 0 )->getValue();
	return l;
}

QString ProjectItem::getStringValues( const QString& v, const QString& o, const QString& s ) const
{ return getListValues( v, o, s ).join( " " ); }

void ProjectItem::setListValues( const QStringList& vl, const QString& v, const QString& o, const QString& s )
{
	ProjectItem* p = project();
	ProjectItem* it = getItemVariable( v, o, s );
	// create index if it not exists
	if ( !it )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		it = clone( ProjectsModel::VariableType, getItemScope( s, true ) );
		it->setValue( v );
		it->setOperator( o );
	}
	// set values
	QStringList l = vl;
	// check all child of variable and remove from model/list unneeded index/value
	for ( int j = it->rowCount() -1; j > -1; j-- )
	{
		ProjectItem* c = it->child( j, 0 );
		if ( c->getType() == ProjectsModel::ValueType )
		{
			if ( l.contains( c->getValue() ) )
				l.removeAll( c->getValue() );
			else
				it->removeRow( c->row() );
		}
	}
	// add require index
	if ( it )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = clone( ProjectsModel::ValueType, it );
				cItem->setValue( e );
			}
		}
		// if variable is empty, remove it
		if ( !it->rowCount() )
		{
			if ( it->parent() )
				it->parent()->removeRow( it->row() );
			else if ( it->model() )
				model()->removeRow( it->row() );
			else
				delete it;
		}
	}
}

void ProjectItem::setStringValues( const QString& val, const QString& v, const QString& o, const QString& s )
{ setListValues( val.isEmpty() ? QStringList() : QStringList( val ), v, o, s ); }

void ProjectItem::addListValues( const QStringList& vl, const QString& v, const QString& o, const QString& s )
{
	ProjectItem* p = project();
	ProjectItem* it = getItemVariable( v, o, s );
	// create index if it not exists
	if ( !it )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		it = clone( ProjectsModel::VariableType, getItemScope( s, true ) );
		it->setValue( v );
		it->setOperator( o );
	}
	// add values
	QStringList l = vl;
	// check all values of variable and remove from list already existing values
	for ( int j = it->rowCount() -1; j > -1; j-- )
	{
		ProjectItem* c = it->child( j, 0 );
		if ( c->getType() == ProjectsModel::ValueType )
			if ( l.contains( c->getValue() ) )
				l.removeAll( c->getValue() );
	}
	// add require values
	if ( it )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = clone( ProjectsModel::ValueType, it );
				cItem->setValue( e );
			}
		}
	}
}

void ProjectItem::addStringValues( const QString& val, const QString& v, const QString& o, const QString& s )
{ addListValues( QStringList( val ), v, o, s ); }

QString ProjectItem::canonicalFilePath() const
{
	// get project item
	ProjectItem* it = project();
	// return project absolutepath
	return it ? it->getFilePath() : QString();
}

QString ProjectItem::canonicalFilePath( const QString& s ) const
{
	// file path
	QString fp = s;
	// check if relative or absolute
	if ( QFileInfo( fp ).isRelative() )
		fp = canonicalPath().append( "/" ).append( fp );
	// return canonical file path if file exists, exists
	if ( QFile::exists( fp ) )
		return QFileInfo( fp ).canonicalFilePath();
	// else return absolute file path, may be not exists
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
