/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : ProjectsProxy.cpp
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
#include "ProjectsProxy.h"
#include "ProjectsModel.h"
#include "ProjectItem.h"

ProjectsProxy::ProjectsProxy( ProjectsModel* m )
	: QSortFilterProxyModel( m ), mFiltering( false ), mNegateFilter( true )
{
	// set model
	setSourceModel( m );
	
	// set role to filter on
	setFilterRole( ProjectItem::TypeRole );
	
	// set sort filter
	setSortRole( ProjectItem::OriginalViewRole );
	
	// dynamic sort/filter items
	setDynamicSortFilter( true );
}

ProjectsModel* ProjectsProxy::projectsModel() const
{ 	return qobject_cast<ProjectsModel*>( sourceModel() ); }

bool ProjectsProxy::filterAcceptsRow( int r, const QModelIndex& i ) const
{
	if ( !mFiltering )
		return true;
	ProjectItem* it = projectsModel()->itemFromIndex( sourceModel()->index( r, 0, i ) );
	int f = it->data( filterRole() ).toInt();
	QString s = it->getValue();
	// check role
	bool b = mNegateFilter ? !mFilterRoles.contains( f ) : mFilterRoles.contains( f );
	// check child visibility
	if ( !b && !( !mFilterValues.isEmpty() && f == ProjectItem::VariableType && !mFilterValues.contains( s, Qt::CaseInsensitive ) ) )
		for ( int j = 0; j < it->rowCount(); j++ )
			if ( filterAcceptsRow( j, it->index() ) )
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
	setSortRole( mFiltering ? ProjectItem::FilteredViewRole : ProjectItem::OriginalViewRole );
	emit filteringChanged( mFiltering );
	refresh();
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

void ProjectsProxy::refresh( ProjectItem* it )
{
	if ( it )
		it->refresh();
	else
		foreach ( it, projectsModel()->projects( true ) )
			it->refresh();
	sort( -1 );
	sort( 0, Qt::AscendingOrder );
}
