/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : ProjectsModel.cpp
** Date      : 2007-11-04T22:45:57
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "ProjectsModel.h"
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
