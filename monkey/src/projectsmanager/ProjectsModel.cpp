#include "ProjectsModel.h"
#include "ProjectItem.h"

ProjectsModel::ProjectsModel( QObject* o )
	: QStandardItemModel( o )
{}

ProjectItem* ProjectsModel::item( int r, int c ) const
{ return reinterpret_cast<ProjectItem*>( QStandardItemModel::item( r, c ) ); }

ProjectItem* ProjectsModel::itemFromIndex( const QModelIndex& i ) const
{ return reinterpret_cast<ProjectItem*>( QStandardItemModel::itemFromIndex( i ) ); }

ProjectItem* ProjectsModel::takeItem( int r, int c )
{ return reinterpret_cast<ProjectItem*>( QStandardItemModel::takeItem( r, c ) ); }

ProjectItemList ProjectsModel::projects( bool b )
{
	ProjectItemList l;
	for ( int i = 0; i < rowCount(); i++ )
		if ( ProjectItem* it = item( i ) )
			if ( it->isProject() )
			{
				l << it;
				if ( b )
					l << it->childrenProjects( true );
			}
	return l;
}
