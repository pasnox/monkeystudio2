#include "ProjectsProxy.h"
#include "ProjectsModel.h"

ProjectsProxy::ProjectsProxy( ProjectsModel* m )
	: QSortFilterProxyModel( m ), mFiltering( false ), mNegateFilter( true )
{
	// set model
	setSourceModel( m );
	
	// set role to filter on
	setFilterRole( ProjectsModel::TypeRole );
	
	// set sort filter
	setSortRole( ProjectsModel::OriginalViewRole );
	
	// set dynamic filter allowed
	setDynamicSortFilter( true );
}

ProjectsModel* ProjectsProxy::projectsModel() const
{ 	return qobject_cast<ProjectsModel*>( sourceModel() ); }

bool ProjectsProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( !mFiltering )
		return true;
	QModelIndex index = sourceModel()->index( r, 0, i );
	int f = index.data( filterRole() ).toInt();
	QString s = index.data( ProjectsModel::ValueRole ).toString();
	// check role
	bool b = mNegateFilter ? !mFilterRoles.contains( f ) : mFilterRoles.contains( f );
	// check child visibility
	if ( !b && !( !mFilterValues.isEmpty() && f == ProjectsModel::VariableType && !mFilterValues.contains( s, Qt::CaseInsensitive ) ) )
		for ( int j = 0; j < sourceModel()->rowCount( index ); j++ )
			if ( filterAcceptsRow( j, index ) )
				return true;

	return b;
}

bool ProjectsProxy::isFiltering() const
{ return mFiltering; }

void ProjectsProxy::setFiltering( bool b )
{
	if ( b == mFiltering )
		return;
	mFiltering = b;
	setSortRole( mFiltering ? ProjectsModel::FilteredViewRole : ProjectsModel::OriginalViewRole );
	emit filteringChanged( mFiltering );
	invalidateFilter();
}

bool ProjectsProxy::isNegateFilter() const
{ return mNegateFilter; }

void ProjectsProxy::setNegateFilter( bool b )
{
	if ( b == mNegateFilter )
		return;
	mNegateFilter = b;
	emit negateFilterChanged( mNegateFilter );
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::addFilterRole( int r )
{
	if ( mFilterRoles.contains( r ) )
		return;
	mFilterRoles << r;
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::addFilterRoles( const QList<int>& r )
{
	foreach ( int i, r )
		if ( !mFilterRoles.contains( i ) )
			mFilterRoles << i;
	if ( mFiltering )
		invalidateFilter();
}

QList<int> ProjectsProxy::filterRoles() const
{ return mFilterRoles; }

void ProjectsProxy::setFilterRoles( const QList<int>& r )
{
	if ( mFilterRoles == r )
		return;
	mFilterRoles = r;
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::removeFilterRole( int r )
{
	if ( !mFilterRoles.contains( r ) )
		return;
	mFilterRoles.removeAll( r );
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::removeFilterRoles( const QList<int>& r )
{
	foreach ( int i, r )
		if ( mFilterRoles.contains( i ) )
			mFilterRoles.removeAll( i );
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::addFilterValue( const QString& v )
{
	if ( mFilterValues.contains( v ) )
		return;
	mFilterValues << v;
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::addFilterValues( const QStringList& v )
{
	foreach ( QString s, v )
		if ( !mFilterValues.contains( s ) )
			mFilterValues << s;
	if ( mFiltering )
		invalidateFilter();
}

QStringList ProjectsProxy::filterValues() const
{ return mFilterValues; }

void ProjectsProxy::setFilterValues( const QStringList& v )
{
	if ( mFilterValues == v )
		return;
	mFilterValues = v;
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::removeFilterValue( const QString& v )
{
	if ( !mFilterValues.contains( v ) )
		return;
	mFilterValues.removeAll( v );
	if ( mFiltering )
		invalidateFilter();
}

void ProjectsProxy::removeFilterValues( const QStringList& v )
{
	foreach ( QString s, v )
		if ( mFilterValues.contains( s ) )
			mFilterValues.removeAll( s );
	if ( mFiltering )
		invalidateFilter();
}
