#include "ScopedProjectItemModel.h"
#include "ProjectItemModel.h"
#include "XUPItem.h"

ScopedProjectItemModel::ScopedProjectItemModel( ProjectItemModel* m )
	: QSortFilterProxyModel( m ), mSourceModel( m ), mProject( 0 )
{
	setSourceModel( m );
	setDynamicSortFilter( true );
	invalidateFilter();
}

void ScopedProjectItemModel::setFilteredProject( XUPItem* project )
{
	mProject = project;
	invalidate();
}

XUPItem* ScopedProjectItemModel::filteredProject() const
{ return mProject; }

bool ScopedProjectItemModel::filterAcceptsRow( int sr, const QModelIndex& sp ) const
{
	if ( XUPItem* it = mSourceModel->itemFromIndex( mSourceModel->index( sr, 0, sp ) ) )
	{
		if ( mProject )
			return ( it->children( true, false ).contains( mProject ) || it == mProject ) ||
				( it->project() == mProject && it->isType( "project" ) || it->isType( "scope" ) );
		else
			return it->isType( "project" ) || it->isType( "scope" );
	}
	return true;
}
