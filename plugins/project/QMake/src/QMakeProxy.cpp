/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : QMakeProxy.cpp
** Date      : 2008-01-14T00:54:11
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
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

