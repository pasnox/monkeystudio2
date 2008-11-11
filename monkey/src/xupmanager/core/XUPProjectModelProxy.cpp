#include "XUPProjectModelProxy.h"
#include "XUPProjectModel.h"
#include "XUPItem.h"

XUPProjectModelProxy::XUPProjectModelProxy( QObject* parent )
	: QSortFilterProxyModel( parent )
{
	mSourceModel = 0;
}

void XUPProjectModelProxy::setSourceModel( QAbstractItemModel* sourceModel )
{
	mSourceModel = qobject_cast<XUPProjectModel*>( sourceModel );
	QSortFilterProxyModel::setSourceModel( mSourceModel );
}

Qt::ItemFlags XUPProjectModelProxy::flags( const QModelIndex& index ) const
{
	if ( !mSourceModel || !index.isValid() )
	{
		return 0;
	}
	
	QModelIndex idx = mapToSource( index );
	XUPItem* item = mSourceModel->itemFromIndex( idx );
	
	bool enabled = false;
	
	if ( item->type() == XUPItem::Project || item->type() == XUPItem::Scope )
	{
		enabled = true;
	}
	else if ( item->type() == XUPItem::Function && item->attribute( "name" ).toLower() != "include" )
	{
		enabled = true;
	}
	
	return enabled ? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::ItemFlags();
}
