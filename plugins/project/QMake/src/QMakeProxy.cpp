#include "QMakeProxy.h"
#include "ProjectsModel.h"
#include "ProjectItem.h"
#include "UIQMakeProjectSettings.h"
#include "UISettingsQMake.h"

QMakeProxy::QMakeProxy( ProjectsModel* m, bool b, ProjectItem* i )
	: ProjectsProxy( m ), mProjectViewMode( b ), mProject( i ), mParentProject( mProject->parent() ? mProject->parent()->project() : 0 )
{
	Q_ASSERT( i != 0 );
	if ( b )
	{
		setFilterRoles( QList<int>() << ProjectsModel::ScopeType << ProjectsModel::NestedScopeType << ProjectsModel::ScopeEndType );
		setNegateFilter( false );
	}
	setFiltering( true );
}

bool QMakeProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	QModelIndex index = sourceModel()->index( r, 0, i );
	if ( !mProjectViewMode )
	{
		ProjectItem* j = mProject->project( i );
		bool b = ProjectsProxy::filterAcceptsRow( r, i );
		if ( b && j != mProject && j != mParentProject )
			b = false;
		return b;
	}
	else
	{
		if ( !mFiltering )
			return true;
		int t = index.data( ProjectsModel::TypeRole ).toInt();
		if ( t == ProjectsModel::ValueType )
			return true;
		else if ( t == ProjectsModel::ProjectType )
			return true;
		else if ( t == ProjectsModel::VariableType )
		{
			QStringList l = UISettingsQMake::readFilters();
			if ( l.isEmpty() )
				l = UISettingsQMake::defaultFilters();
			QVariant v = index.data( ProjectsModel::ValueRole );
			int i = l.indexOf( v.toString(), Qt::CaseInsensitive );
			if ( i != -1 )
			{
				/*
				if ( index.data( ProjectsModel::FilterRole ).toInt() != i )
					sourceModel()->setData( index, i, ProjectsModel::FilterRole );
				*/
				return true;
			}
		}
		else if ( mFilterRoles.contains( t ) )
		{
			for ( int j = 0; j < sourceModel()->rowCount( index ); j++ )
				if ( filterAcceptsRow( j, index ) )
					return true;
		}
		return false;
	}
}
//
void QMakeProxy::setFiltering( bool b )
{
	if ( b == mFiltering )
		return;
	mFiltering = b;
	emit filteringChanged( mFiltering );
	if ( mProjectViewMode )
	{
		if ( mFiltering )
			sort( 0, Qt::AscendingOrder );
		else
		{
			sort( -1 );
			//reset();
		}
	}
	filterChanged();
}
