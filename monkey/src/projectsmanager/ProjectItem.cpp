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
#include <QTextCodec>

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

ProjectsModel* ProjectItem::model() const
{ return reinterpret_cast<ProjectsModel*>( QStandardItem::model() ); }

ProjectItem* ProjectItem::parent() const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::parent() ); }

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

ProjectItem* ProjectItem::project( const QModelIndex& i ) const
{
	// get project item
	ProjectItem* it = i.isValid() ? model()->itemFromIndex( i ) : const_cast<ProjectItem*>( this );
	
	// check reucursively parent if needed
	while ( it && it->getType() != ProjectsModel::ProjectType )
		it = it->parent();
	
	// return item
	return it;
}

QString ProjectItem::canonicalFilePath() const
{
	// get project item
	ProjectItem* it = project();
	
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

QModelIndexList ProjectItem::getIndexList( const QModelIndex& i, ProjectsModel::NodeType t, const QString& v, const QString& o, const QString& s ) const
{
	ProjectItem* p = project( i );
	QModelIndexList l;
	foreach ( QModelIndex index, model()->match( p->index().child( 0, 0 ), ProjectsModel::ValueRole, v, -1, Qt::MatchFixedString | Qt::MatchRecursive ) )
		if ( project( index ) == p )
			if ( isEqualScope( scopeOf( index ), s ) )
				if ( ( !o.isEmpty() && index.data( ProjectsModel::OperatorRole ).toString() == o ) || o.isEmpty() )
					if ( index.data( ProjectsModel::TypeRole ).toInt() == t )
						l << index;
	return l;
}

QModelIndex ProjectItem::getScope( const QModelIndex& i, const QString& s, bool c ) const
{
	// get project item
	ProjectItem* sItem = project( i );
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
			QModelIndex fItem = ( getIndexList( sItem->index(), ProjectsModel::ScopeType, s1, s2, sScope ) << getIndexList( sItem->index(), ProjectsModel::NestedScopeType, s1, s2, sScope ) ).value( 0 );
			// create scope
			if ( ( !fItem.isValid() || project( fItem ) != sItem ) )
			{
				if ( c )
				{
					sItem = new ProjectItem( l.at( 2 ).trimmed().isEmpty() ? ProjectsModel::ScopeType : ProjectsModel::NestedScopeType, sItem );
					sItem->setValue( s1 );
					sItem->setOperator( s2 );
					(void) new ProjectItem( ProjectsModel::ScopeEndType, sItem );
				}
				else
					return sItem->index();
			}
			else
				sItem = model()->itemFromIndex( fItem );
			pos += r.matchedLength();
		}
		// if scope is nested, made it ScopeType
		if ( sItem && sItem->getType() == ProjectsModel::NestedScopeType )
		{
			sItem->setType( ProjectsModel::ScopeType );
			sItem->setOperator( QString::null );
		}
	}
	return sItem->index();
}

QString ProjectItem::scopeOf( const QModelIndex& i ) const
{
	if ( !i.isValid() )
		return QString();
	QString s;
	ProjectItem* j = model()->itemFromIndex( i );
	ProjectItem* p = project( i );
	while ( j && j != p )
	{
		if ( j->getType() == ProjectsModel::ScopeType || j->getType() == ProjectsModel::NestedScopeType )
			s.prepend( QString( "%1%2" ).arg( j->getValue(), j->getOperator().isEmpty() ? ":" : j->getOperator() ) );
		j = j->parent();
	}
	return s;
}

QString ProjectItem::checkScope( const QString& s ) const
{
	// remove trailing : as it s the default operator
	QString ss = s.trimmed();
	if ( !ss.isEmpty() && ss.endsWith( ':' ) )
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

QModelIndexList ProjectItem::getIndexListValues( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	QModelIndexList l = getIndexList( i, ProjectsModel::VariableType, v, o, s );
	QModelIndexList ll;
	foreach ( QModelIndex index, l )
		for ( int j = 0; j < model()->rowCount( index ); j++ )
			if ( index.child( j, 0 ).data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ValueType )
				ll << index.child( j, 0 );
	return ll;
}

QModelIndex ProjectItem::getIndexVariable( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	return getIndexList( i, ProjectsModel::VariableType, v, o, s ).value ( 0 );
}

QStringList ProjectItem::getListValues( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	QStringList l;
	foreach ( QModelIndex index, getIndexList( i, ProjectsModel::VariableType, v, o, s ) )
		for ( int j = 0; j < model()->rowCount( index ); j++ )
			if ( index.child( j, 0 ).data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ValueType )
				l << index.child( j, 0 ).data( ProjectsModel::ValueRole ).toString();
	return l;
}

QString ProjectItem::getStringValues( const QString& v, const QModelIndex& i, const QString& o, const QString& s ) const
{
	return getListValues( v, i, o, s ).join( " " );
}

void ProjectItem::setListValues( const QStringList& vl, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	ProjectItem* p = project( i );
	QModelIndex it = getIndexVariable( v, p->index(), o, s );
	// create index if it not exists
	if ( !it.isValid() )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		ProjectItem* vItem = new ProjectItem( ProjectsModel::VariableType, model()->itemFromIndex( getScope( p->index(), s, true ) ) );
		vItem->setValue( v );
		vItem->setOperator( o );
		it = vItem->index();
	}
	// set values
	QStringList l = vl;
	// check all child of variable and remove from model/list unneeded index/value
	for ( int j = model()->rowCount( it ) -1; j > -1; j-- )
	{
		QModelIndex c = it.child( j, 0 );
		if ( c.data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ValueType )
		{
			QString d = c.data( ProjectsModel::ValueRole ).toString();
			if ( l.contains( d ) )
				l.removeAll( d );
			else
				delete model()->itemFromIndex( c.parent() )->takeChild( c.row() );
		}
	}
	// add require index
	ProjectItem* pItem = model()->itemFromIndex( it );
	if ( pItem )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = new ProjectItem( ProjectsModel::ValueType, pItem );
				cItem->setValue( e );
			}
		}
		// if variable is empty, remove it
		if ( !pItem->rowCount() )
			delete pItem->parent()->takeChild( pItem->row() );
	}
}

void ProjectItem::setStringValues( const QString& val, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	setListValues( val.isEmpty() ? QStringList() : QStringList( val ), v, i, o, s );
}

void ProjectItem::addListValues( const QStringList& vl, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	ProjectItem* p = project( i );
	QModelIndex it = getIndexVariable( v, p->index(), o, s );
	// create index if it not exists
	if ( !it.isValid() )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		ProjectItem* vItem = new ProjectItem( ProjectsModel::VariableType, model()->itemFromIndex( getScope( p->index(), s, true ) ) );
		vItem->setValue( v );
		vItem->setOperator( o );
		it = vItem->index();
	}
	// add values
	QStringList l = vl;
	// check all values of variable and remove from list already existing values
	for ( int j = model()->rowCount( it ) -1; j > -1; j-- )
	{
		QModelIndex c = it.child( j, 0 );
		if ( c.data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ValueType )
		{
			QString d = c.data( ProjectsModel::ValueRole ).toString();
			if ( l.contains( d ) )
				l.removeAll( d );
		}
	}
	// add require values
	ProjectItem* pItem = model()->itemFromIndex( it );
	if ( pItem )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = new ProjectItem( ProjectsModel::ValueType, pItem );
				cItem->setValue( e );
			}
		}
	}
}

void ProjectItem::addStringValues( const QString& val, const QString& v, const QModelIndex& i, const QString& o, const QString& s )
{
	addListValues( QStringList( val ), v, i, o, s );
}

QTextCodec* ProjectItem::codec( const QModelIndex& i ) const
{
	QTextCodec* c = QTextCodec::codecForName( getStringValues( "ENCODING", i ).toAscii() );
	if ( !c )
		c = QTextCodec::codecForName( "UTF-8" );
	return c;
}
