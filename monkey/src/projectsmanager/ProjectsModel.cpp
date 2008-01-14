/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : ProjectsModel.cpp
** Date      : 2008-01-14T00:37:03
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
	// connections
	connect( this, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( rowsChanged( const QModelIndex&, int, int ) ) );
	connect( this, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ), this, SLOT( rowsChanged( const QModelIndex&, int, int ) ) );
}

void ProjectsModel::rowsChanged( const QModelIndex& i, int, int )
{
	if ( ProjectItem* it = itemFromIndex( i ) )
		if ( !it->isProject() )
			it->project()->setModified( true );
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
