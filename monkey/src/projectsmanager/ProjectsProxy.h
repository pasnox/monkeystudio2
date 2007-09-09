#ifndef PROJECTSPROXY_H
#define PROJECTSPROXY_H

#include "MonkeyExport.h"

#include <QSortFilterProxyModel>
#include <QStringList>

class ProjectsModel;

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
