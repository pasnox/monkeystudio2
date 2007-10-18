#include "QMakeProxy.h"
#include "ProjectsModel.h"
#include "ProjectItem.h"
#include "UIQMakeProjectSettings.h"
#include "UISettingsQMake.h"

QMakeProxy::QMakeProxy( ProjectsModel* m, ProjectItem* i )
	: ProjectsProxy( m )
{
	Q_ASSERT( i != 0 );
	mProject = i;
}

bool QMakeProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( !mFiltering )
		return true;
	ProjectItem* it = projectsModel()->itemFromIndex( sourceModel()->index( r, 0, i ) );
	int f = it->data( filterRole() ).toInt();
	// check role
	bool b = mNegateFilter ? !mFilterRoles.contains( f ) : mFilterRoles.contains( f );
	// check child visibility
	if ( !b )
		for ( int j = 0; j < it->rowCount(); j++ )
			if ( filterAcceptsRow( j, it->index() ) )
				return true;

	return b & it->project() == mProject;
}

void QMakeProxy::setFiltering( bool b )
{
	if ( b == mFiltering )
		return;
	mFiltering = b;
	emit filteringChanged( mFiltering );
	invalidateFilter();
}

