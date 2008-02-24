#include "ScopedProjectItemModel.h"
#include "ProjectItemModel.h"
#include "XUPItem.h"

ScopedProjectItemModel::ScopedProjectItemModel( ProjectItemModel* m )
	: QSortFilterProxyModel( m ), mSourceModel( m )
{
	setSourceModel( m );
	setDynamicSortFilter( true );
	invalidateFilter();
}

bool ScopedProjectItemModel::filterAcceptsRow( int sr, const QModelIndex& sp ) const
{
	if ( XUPItem* it = mSourceModel->itemFromIndex( mSourceModel->index( sr, 0, sp ) ) )
		return it->isType( "project" ) || it->isType( "scope" );
	return true;
}
