#include "ProjectsModel.h"
#include "ProjectsProxy.h"

ProjectsModel::ProjectsModel( QObject* o )
	: QStandardItemModel( o )
{
	// proxy for only show projects
	mProjects = new ProjectsProxy( this );
	mProjects->setNegateFilter( false );
	mProjects->addFilterRole( ProjectItem::ProjectType );
	mProjects->setFiltering( true );
	// proxy for only show projects/scopes
	mScopes = new ProjectsProxy( this );
	mScopes->setNegateFilter( false );
	mScopes->addFilterRole( ProjectItem::ProjectType );
	mScopes->addFilterRole( ProjectItem::ScopeType );
	mScopes->addFilterRole( ProjectItem::NestedScopeType );
	mScopes->setFiltering( true );
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
