#include "ProjectItemModel.h"
#include "ProjectItem.h"

ProjectItemModel::ProjectItemModel( QObject* o )
	: QStandardItemModel( o )
{ setItemPrototype( new ProjectItem() ); }

ProjectItemModel::~ProjectItemModel()
{}

ProjectItem* ProjectItemModel::item( int r, int c ) const
{ return dynamic_cast<ProjectItem*>( QStandardItemModel::item( r, c ) ); }

ProjectItem* ProjectItemModel::itemFromIndex( const QModelIndex& i ) const
{ return dynamic_cast<ProjectItem*>( QStandardItemModel::itemFromIndex( i ) ); }

void ProjectItemModel::appendRow( ProjectItem* it )
{ QStandardItemModel::appendRow( it ); }

bool ProjectItemModel::isProject( const ProjectItem* it ) const
{ return it ? it->value( "type" ) == "project" : false; }

bool ProjectItemModel::isProject( const QModelIndex& i ) const
{ return isProject( itemFromIndex( i ) ); }

bool ProjectItemModel::isType( const ProjectItem* it, const QString& s ) const
{ return it ? it->value( "type" ) == s : false; }

bool ProjectItemModel::isType( const QModelIndex& i, const QString& s ) const
{ return isType( itemFromIndex( i ), s ); }

ProjectItem* ProjectItemModel::project( const ProjectItem* it ) const
{
	if ( it )
	{
		if ( isProject( it ) )
			return const_cast<ProjectItem*>( it );
		while ( it && ( it = it->parent() )  )
			if ( isProject( it ) )
				return const_cast<ProjectItem*>( it );
	}
	return 0;
}

ProjectItem* ProjectItemModel::project( const QModelIndex& i ) const
{ return project( itemFromIndex( i ) ); }

ProjectItem* ProjectItemModel::topLevelProject( const ProjectItem* it ) const
{
	it = project( it );
	while ( it && it->parent() && ( it == project( it ) ) )
		if ( isProject( it ) )
			return const_cast<ProjectItem*>( it );
	return const_cast<ProjectItem*>( it );
}

ProjectItem* ProjectItemModel::topLevelProject( const QModelIndex& i ) const
{ return topLevelProject( itemFromIndex( i ) ); }

QList<ProjectItem*> ProjectItemModel::topLevelProjects( int c ) const
{
	QList<ProjectItem*> l;
	for ( int i = 0; i < rowCount(); i++ )
		l << item( i, c );
	return l;
}

void ProjectItemModel::closeProject( const ProjectItem* it )
{
	if ( it )
	{
		if ( it->parent() )
			qDeleteAll( it->parent()->takeRow( it->row() ) );
		else if ( it->model() )
			qDeleteAll( it->model()->takeRow( it->row() ) );
		else
			delete it;
	}
}

void ProjectItemModel::closeProject( const QModelIndex& i )
{ closeProject( itemFromIndex( i ) ); }
