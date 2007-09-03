#include "AbstractProjectProxy.h"
#include "AbstractProjectModel.h"
//
#include <QTreeView>
//
int AbstractProjectProxy::mUniqueId = 0;
QHashProxys AbstractProjectProxy::mProxysList = QHashProxys();
//
AbstractProjectProxy::AbstractProjectProxy( AbstractProjectModel* m )
	: QSortFilterProxyModel( m ), mId( mUniqueId++ ), mFiltering( false ), mNegateFilter( true )
{
	mProxysList[ mId ] = this;
	setDynamicSortFilter( true );
	setFilterRole( AbstractProjectModel::TypeRole );
	setSourceModel( m );
}
AbstractProjectProxy::~AbstractProjectProxy()
{
	if ( mProxysList.contains( mId ) )
		mProxysList.remove( mId );
}
//
int AbstractProjectProxy::id() const
{
	return mId;
}
//
AbstractProjectProxy* AbstractProjectProxy::byId( int i )
{
	return mProxysList.contains( i ) ? mProxysList.value( i ) : 0;
}
//
QHashProxys AbstractProjectProxy::all()
{
	return mProxysList;
}
//
AbstractProjectProxy* AbstractProjectProxy::getProxyByProject( AbstractProjectModel* p )
{
	return p ? p->findChild<AbstractProjectProxy*>() : 0;
}
//
AbstractProjectModel* AbstractProjectProxy::project() const
{
	return qobject_cast<AbstractProjectModel*>( sourceModel() );
}
//
bool AbstractProjectProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( !mFiltering )
		return true;
	QModelIndex index = sourceModel()->index( r, 0, i );
	int f = index.data( filterRole() ).toInt();
	bool b = mNegateFilter ? !mFilterRoles.contains( f ) : mFilterRoles.contains( f );
	if ( !b )
		for ( int j = 0; j < sourceModel()->rowCount( index ); j++ )
			if ( filterAcceptsRow( j, index ) )
				return true;
	return b;
}
//
bool AbstractProjectProxy::isFiltering() const
{
	return mFiltering;
}
//
void AbstractProjectProxy::setFiltering( bool b )
{
	if ( b == mFiltering )
		return;
	mFiltering = b;
	emit filteringChanged( mFiltering );
	filterChanged();
}
//
bool AbstractProjectProxy::isNegateFilter() const
{
	return mNegateFilter;
}
//
void AbstractProjectProxy::setNegateFilter( bool b )
{
	if ( b == mNegateFilter )
		return;
	mNegateFilter = b;
	emit negateFilterChanged( mNegateFilter );
	if ( mFiltering )
		filterChanged();
}
//
void AbstractProjectProxy::addFilterRole( int r )
{
	if ( mFilterRoles.contains( r ) )
		return;
	mFilterRoles << r;
	if ( mFiltering )
		filterChanged();
}
//
QList<int> AbstractProjectProxy::filterRoles() const
{
	return mFilterRoles;
}
//
void AbstractProjectProxy::setFilterRoles( const QList<int>& r )
{
	if ( mFilterRoles == r )
		return;
	mFilterRoles = r;
	if ( mFiltering )
		filterChanged();
}
