/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : ProjectsProxy.h
** Date      : 2007-11-04T22:46:00
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef PROJECTSPROXY_H
#define PROJECTSPROXY_H

#include "MonkeyExport.h"

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
