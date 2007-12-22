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
#include "ProjectsModel.h"

#include <QFileInfo>
#include <QDir>
#include <QTextCodec>

void ProjectItem::init()
{
	mBuilder = 0;
	mCompiler = 0;
	mDebugger = 0;
	mInterpreter = 0;
	mBuffer.clear();
	setReadOnly( false );
}

ProjectItem::ProjectItem( ProjectItem::NodeType t, ProjectItem* i )
{
	init();
	setType( t );
	if ( i )
		i->appendRow( this );
}

int ProjectItem::type() const
{ return QStandardItem::UserType; }

void ProjectItem::setPlugin( ProjectPlugin* p )
{
	if ( isProject() )
		mPlugin = p;
}

ProjectPlugin* ProjectItem::plugin() const
{ return isProject() ? mPlugin : 0; }

void ProjectItem::setData( const QVariant& v, int r )
{
	// check read only
	if ( parent() && getReadOnly() )
		return;
	// set data
	QStandardItem::setData( v, r );
	// set modified state
	if ( r != ProjectItem::ModifiedRole && r != ProjectItem::ReadOnlyRole && r != ProjectItem::FilteredViewRole && r != ProjectItem::OriginalViewRole && r != ProjectItem::FilePathRole )
		setModified( true );
}

void ProjectItem::setType( ProjectItem::NodeType t )
{ setData( t, ProjectItem::TypeRole ); }

ProjectItem::NodeType ProjectItem::getType() const
{ return (ProjectItem::NodeType)data( ProjectItem::TypeRole ).toInt(); }

void ProjectItem::setOperator( const QString& s )
{ setData( s, ProjectItem::OperatorRole ); }

QString ProjectItem::getOperator() const
{ return data( ProjectItem::OperatorRole ).toString(); }

void ProjectItem::setValue( const QString& s )
{ setData( s, ProjectItem::ValueRole ); }

QString ProjectItem::getValue() const
{ return data( ProjectItem::ValueRole ).toString(); }

void ProjectItem::setMultiLine( bool b )
{ setData( b, ProjectItem::MultiLineRole ); }

bool ProjectItem::getMultiLine() const
{ return data( ProjectItem::MultiLineRole ).toBool(); }

void ProjectItem::setComment( const QString& s )
{ setData( s, ProjectItem::CommentRole ); }

QString ProjectItem::getComment() const
{ return data( ProjectItem::CommentRole ).toString(); }

void ProjectItem::setFilePath( const QString& s )
{ setData( s, ProjectItem::FilePathRole ); }

QString ProjectItem::getFilePath() const
{ return data( ProjectItem::FilePathRole ).toString(); }

void ProjectItem::setFilteredView( int i )
{ setData( i, ProjectItem::FilteredViewRole ); }

int ProjectItem::getFilteredView() const
{ return data( ProjectItem::FilteredViewRole ).toInt(); }

void ProjectItem::setOriginalView( int i )
{ setData( i, ProjectItem::OriginalViewRole ); }

int ProjectItem::getOriginalView() const
{ return data( ProjectItem::OriginalViewRole ).toInt(); }

void ProjectItem::setModified( bool b )
{
	// if project, and want set no modified, need update all items of the project
	if ( isProject() && !b )
		foreach ( ProjectItem* it, projectItems() )
			it->setModified( false );

	// set modified state if needed
	if ( getModified() == b )
		return;
	setData( b, ProjectItem::ModifiedRole );
	
	if ( isProject() )
		emit modifiedChanged( b );
	else if ( ProjectItem* p  = project() )
		if ( b && !p->getModified() )
			p->setModified( true );
}

bool ProjectItem::getModified() const
{ return data( ProjectItem::ModifiedRole ).toBool(); }

void ProjectItem::setReadOnly( bool b )
{
	if ( isProject() )
		foreach ( ProjectItem* it, projectItems() )
			it->setReadOnly( b );
	
	setData( b, ProjectItem::ReadOnlyRole );
	
	if ( !isProject() )
		if ( ProjectItem* p  = project() )
			if ( !b && p->getReadOnly() )
				p->setReadOnly( false );
}

bool ProjectItem::getReadOnly() const
{ return data( ProjectItem::ReadOnlyRole ).toBool(); }

bool ProjectItem::isEmpty() const
{ return getType() == ProjectItem::EmptyType; }

bool ProjectItem::isFolder() const
{ return getType() == ProjectItem::FolderType; }

bool ProjectItem::isComment() const
{ return getType() == ProjectItem::CommentType; }

bool ProjectItem::isNested() const
{ return getType() == ProjectItem::NestedScopeType; }

bool ProjectItem::isScope( bool b ) const
{ return b ? getType() == ProjectItem::ScopeType || getType() == ProjectItem::NestedScopeType : getType() == ProjectItem::ScopeType; }

bool ProjectItem::isScopeEnd() const
{ return getType() == ProjectItem::ScopeEndType; }

bool ProjectItem::isVariable() const
{ return getType() == ProjectItem::VariableType; }

bool ProjectItem::isValue() const
{ return getType() == ProjectItem::ValueType; }

bool ProjectItem::isFunction() const
{ return getType() == ProjectItem::FunctionType; }

bool ProjectItem::isInclude() const
{ return getType() == ProjectItem::IncludeType; }

bool ProjectItem::isProject() const
{ return getType() == ProjectItem::ProjectType; }

QString ProjectItem::quotedString( const QString& s ) const
{ return s.contains( ' ' ) && !s.startsWith( '"' ) && !s.endsWith( '"' ) ? QString( "\"%1\"" ).arg( s ) : s; }

ProjectsModel* ProjectItem::model() const
{ return dynamic_cast<ProjectsModel*>( QStandardItem::model() ); }

ProjectItem* ProjectItem::parent() const
{ return dynamic_cast<ProjectItem*>( QStandardItem::parent() ); }

int ProjectItem::parentCount() const
{
	if ( isProject() || !parent() )
		return 0;
	int i = 1;
	ProjectItem* it = parent();
	while ( it->parent() )
	{
		it = it->parent();
		i++;
		if ( it->isProject() )
			return i;
	}
	return i;
}

ProjectItem* ProjectItem::child( int i, int j ) const
{ return dynamic_cast<ProjectItem*>( QStandardItem::child( i, j ) ); }

ProjectItemList ProjectItem::children( bool r, bool s ) const
{
	ProjectItemList l;
	for ( int i = 0; i < rowCount(); i++ )
	{
		ProjectItem* ci = child( i, 0 );
		if ( !s || !ci->isProject() )
		{
			l << ci;
			if ( r && ci->rowCount() )
				l << ci->children( r, s );
		}
	}
	return l;
}

void ProjectItem::appendRow( ProjectItem* i )
{ insertRow( rowCount(), i ); }

bool ProjectItem::swapRow( int i, int j )
{
	if ( -1 < i < rowCount() && -1 < j < rowCount() && i != j )
	{
		QList<QStandardItem*> ii = takeRow( qMax( i, j ) );
		QList<QStandardItem*> ij = takeRow( qMin( i, j ) );
		QStandardItem::insertRow( qMin( i, j ), ii );
		QStandardItem::insertRow( qMax( i, j ), ij );
		setModified( true );
		return true;
	}
	return false;
}

bool ProjectItem::moveRowUp( int i )
{
	ProjectItem* it = child( i );
	bool b = it && !it->isScopeEnd() && i > 0;
	if ( b )
		QStandardItem::insertRow( i -1, takeRow( i ) );
	return b;
}

bool ProjectItem::moveRowDown( int i )
{
	ProjectItem* it = child( i +1 );
	bool b = it && !it->isScopeEnd() && i < rowCount() -1;
	if ( b )
		QStandardItem::insertRow( i +1, takeRow( i ) );
	return b;
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
		model()->removeRow( row(), index().parent() );
	else
		Q_ASSERT( 0 );
}

void ProjectItem::close()
{
	// only project item can call this
	if ( !isProject() )
		return;
	
	// close subprojects first
	while ( ProjectItem* p = childrenProjects( false ).value( 0 ) )
		p->close();
	
	// tell we will close the project
	emit aboutToClose();
	
	// ask to save project if needed
	save( true );
	
	// tell project is closed
	emit closed();
	
	// remove it from model
	remove();
}

void ProjectItem::saveAll( bool b )
{
	// save current project
	save( b );
	// save all children projects
	foreach ( ProjectItem* it, childrenProjects() )
		it->save( b );
}

void ProjectItem::addExistingFile( const QString& f, ProjectItem* s, const QString& o )
{ addExistingFiles( QStringList( f ), s, o ); }

void ProjectItem::refresh()
{ redoLayout(); }

ProjectItem* ProjectItem::project() const
{
	if ( isProject() )
		return const_cast<ProjectItem*>( this );
	ProjectItem* it = parent();
	while ( it && !it->isProject() )
		it = it->parent();
	return it;
}

ProjectItem* ProjectItem::parentProject() const
{
	ProjectItem* p = project();
	return p && p->parent() ? p->parent()->project() : 0;
}

ProjectItemList ProjectItem::childrenProjects( bool b ) const
{ return match( ProjectItem::ProjectType, "*", !b ); }

ProjectItemList ProjectItem::projectItems( bool b ) const
{ return project()->children( true, b ); }

QString ProjectItem::canonicalFilePath() const
{ return QFileInfo( project()->getValue() ).canonicalFilePath(); }

QString ProjectItem::canonicalFilePath( const QString& s ) const
{
	QString fp = s;
	if ( QFileInfo( fp ).isRelative() )
		fp = canonicalPath().append( "/" ).append( fp );
	if ( QFile::exists( fp ) )
		return QFileInfo( fp ).canonicalFilePath();
	return QString();
	return QFileInfo( fp ).absoluteFilePath();
}

QString ProjectItem::canonicalPath() const
{ return QFileInfo( canonicalFilePath() ).absolutePath(); }

QString ProjectItem::canonicalPath( const QString& s ) const
{ return QFileInfo( canonicalFilePath( s ) ).path(); }

QString ProjectItem::relativeFilePath( const QString& s ) const
{ return QDir( canonicalPath() ).relativeFilePath( s ); }

QString ProjectItem::relativePath( const QString& s ) const
{ return QFileInfo( relativeFilePath( s ) ).path(); }

QString ProjectItem::fileName( const QString& s )
{ return QFileInfo( s ).fileName(); }

QString ProjectItem::completeBaseName( const QString& s )
{ return QFileInfo( s ).completeBaseName(); }

QString ProjectItem::name() const
{ return QFileInfo( canonicalFilePath() ).completeBaseName(); }

ProjectItemList ProjectItem::match( int r, const QVariant& v, bool b ) const
{
	ProjectItemList l;
	const QString s = v.toString();
	foreach ( ProjectItem* it, projectItems( b ) )
		if ( it->getType() == r )
			if ( it->getValue() == s || s == "*" )
				l << it;
	return l;
}

ProjectItemList ProjectItem::getItemList( ProjectItem* s, ProjectItem::NodeType t, const QString& v, const QString& o ) const
{
	ProjectItemList l;
	foreach ( ProjectItem* it, match( t, v, true ) )
		if ( it->parent() == s )
			if ( it->getOperator() == o || o == "*" )
				l << it;
	return l;
}

ProjectItemList ProjectItem::getAllItemList( ProjectItem::NodeType t, const QString& v, const QString& o ) const
{
	ProjectItemList l;
	foreach ( ProjectItem* it, match( t, v, true ) )
		if ( it->getOperator() == o || o == "*" )
			l << it;
	return l;
}
