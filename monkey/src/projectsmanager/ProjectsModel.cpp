#include "ProjectsModel.h"
#include "ProjectsProxy.h"

ProjectsModel::ProjectsModel( QObject* o )
	: QStandardItemModel( o )
{
	mProxy = new ProjectsProxy( this );
	mProxy->setNegateFilter( false );
	mProxy->addFilterRole( ProjectItem::ProjectType );
	mProxy->setFiltering( true );
}

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
