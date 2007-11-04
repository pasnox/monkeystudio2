/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QMakeProxy.cpp
** Date      : 2007-11-04T22:54:26
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "QMakeProxy.h"
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

bool isDirectParent( ProjectItem* it, ProjectItem* pit )
{
	bool b = false;
	for ( int i = 0; i < it->rowCount(); i++ )
	{
		ProjectItem* cit = it->child( i );
		b = cit == pit;
		if ( !b && cit->hasChildren() )
			b = isDirectParent( cit, pit );
		if ( b )
			return true;
	}
	return b;
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
	// return state
	return it->project() == mProject ? b : isDirectParent( it, mProject );
}

void QMakeProxy::setFiltering( bool b )
{
	if ( b == mFiltering )
		return;
	mFiltering = b;
	emit filteringChanged( mFiltering );
	invalidateFilter();
}

