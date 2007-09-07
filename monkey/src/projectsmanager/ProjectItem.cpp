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
	// set type
	setType( t );
	
	// append to parent if needed
	if ( i )
		i->appendRow( this );
}

ProjectItem::~ProjectItem()
{
	
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

ProjectItem* ProjectItem::itemFromIndex( const QModelIndex& i ) const
{ return model()->itemFromIndex( i ); }

ProjectsModel* ProjectItem::model() const
{ return reinterpret_cast<ProjectsModel*>( QStandardItem::model() ); }

ProjectItem* ProjectItem::parent() const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::parent() ); }

ProjectItem* ProjectItem::child( int i, int j ) const
{ return reinterpret_cast<ProjectItem*>( QStandardItem::child( i, j ) ); }

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
	return it ? it->getFilePath() : QString();
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
	return s;
}

ProjectItemList ProjectItem::getItemList( ProjectsModel::NodeType t, const QString& v, const QString& o, const QString& s ) const
{
	// project item
	ProjectItem* p = project();
	// temp list
	ProjectItemList l;
	foreach ( QModelIndex i, model()->match( p->index().child( 0, 0 ), ProjectsModel::ValueRole, v, -1, Qt::MatchFixedString | Qt::MatchRecursive ) )
	{
		// get item
		ProjectItem* it = const_cast<ProjectItem*>( itemFromIndex( i ) );
		// check same project
		if ( it->project() == p )
			// check same scope
			if ( isEqualScope( it->scope(), s ) )
				// check operator
				if ( ( !o.isEmpty() && it->getOperator() == o ) || o.isEmpty() )
					// check type
					if ( it->getType() == t )
						// add to list
						l << it;
	}
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
