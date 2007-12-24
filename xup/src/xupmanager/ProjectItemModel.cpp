#include "ProjectItemModel.h"
#include "ProjectItem.h"
#include "FilteredProjectItemModel.h"

ProjectItemModel::ProjectItemModel( QObject* o )
	: QStandardItemModel( o ), mFilteredModel( 0 )
{ setItemPrototype( new ProjectItem() ); }

ProjectItemModel::~ProjectItemModel()
{}

ProjectItem* ProjectItemModel::item( int r, int c ) const
{ return dynamic_cast<ProjectItem*>( QStandardItemModel::item( r, c ) ); }

ProjectItem* ProjectItemModel::itemFromIndex( const QModelIndex& i ) const
{ return dynamic_cast<ProjectItem*>( QStandardItemModel::itemFromIndex( i ) ); }

void ProjectItemModel::appendRow( ProjectItem* it )
{ QStandardItemModel::appendRow( it ); }

QList<ProjectItem*> ProjectItemModel::topLevelProjects( int c ) const
{
	QList<ProjectItem*> l;
	for ( int i = 0; i < rowCount(); i++ )
		l << item( i, c );
	return l;
}

FilteredProjectItemModel* ProjectItemModel::filteredModel()
{
	if ( !mFilteredModel )
		mFilteredModel = new FilteredProjectItemModel( this );
	return mFilteredModel;
}
