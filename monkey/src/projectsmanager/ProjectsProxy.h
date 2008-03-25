/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : ProjectsProxy.h
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
#ifndef PROJECTSPROXY_H
#define PROJECTSPROXY_H

#include <fresh.h>

#include <QSortFilterProxyModel>
#include <QStringList>

class ProjectsModel;
class ProjectItem;

class Q_MONKEY_EXPORT ProjectsProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	
public:
	ProjectsProxy( ProjectsModel* );

	virtual ProjectsModel* projectsModel() const;
	virtual bool isFiltering() const;
	virtual bool isNegateFilter() const;
	virtual QList<int> filterRoles() const;
	virtual QStringList filterValues() const;
	virtual void refresh( ProjectItem* = 0 );
	
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	
	bool mFiltering;
	bool mNegateFilter;
	QList<int> mFilterRoles;
	QStringList mFilterValues;
	
public slots:
	virtual void setFiltering( bool );
	virtual void setNegateFilter( bool );
	virtual void addFilterRole( int );
	virtual void addFilterRoles( const QList<int>& );
	virtual void setFilterRoles( const QList<int>& );
	virtual void removeFilterRole( int );
	virtual void removeFilterRoles( const QList<int>& );
	virtual void addFilterValue( const QString& );
	virtual void addFilterValues( const QStringList& );
	virtual void setFilterValues( const QStringList& );
	virtual void removeFilterValue( const QString& );
	virtual void removeFilterValues( const QStringList& );
	
signals:
	// emit when filtering change
	void filteringChanged( bool );
	// emit when negate filter change
	void negateFilterChanged( bool );
	
};

#endif // PROJECTSPROXY_H
