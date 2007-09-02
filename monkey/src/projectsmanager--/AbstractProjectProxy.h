#ifndef ABSTRACTPROJECTPROXY_H
#define ABSTRACTPROJECTPROXY_H
//
#include <QSortFilterProxyModel>
#include <QHash>
//
#include "MonkeyExport.h"
//
class AbstractProjectModel;
class AbstractProjectProxy;
typedef QHash<int, AbstractProjectProxy*> QHashProxys; // id, proxy
//
class Q_MONKEY_EXPORT AbstractProjectProxy : public QSortFilterProxyModel
{
	Q_OBJECT
	//
public:
	AbstractProjectProxy( AbstractProjectModel* );
	~AbstractProjectProxy();
	int id() const;
	static AbstractProjectProxy* byId( int );
	static QHashProxys all();
	static AbstractProjectProxy* getProxyByProject( AbstractProjectModel* );
	virtual AbstractProjectModel* project() const;
	virtual bool isFiltering() const;
	virtual bool isNegateFilter() const;
	virtual QList<int> filterRoles() const;
	//
protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
	static int mUniqueId;
	static QHashProxys mProxysList;
	int mId;
	bool mFiltering;
	bool mNegateFilter;
	QList<int> mFilterRoles;
	//
public slots:
	virtual void setFiltering( bool );
	virtual void setNegateFilter( bool );
	virtual void addFilterRole( int );
	virtual void setFilterRoles( const QList<int>& );
	//
	virtual void doubleClicked( const QModelIndex& ) = 0;
 	virtual void clicked( const QModelIndex& ) = 0;
	virtual void customContextMenuRequested( const QPoint& ) = 0;
	virtual void projectSettings( const QModelIndex& = QModelIndex() ) = 0;
	//
signals:
	// emit when filtering change
	void filteringChanged( bool );
	// emit when negate filter change
	void negateFilterChanged( bool );
	// emit when a file request to be open
	void fileOpenRequested( const QString&, AbstractProjectProxy* );
    //emit when file selected in the TreeView
    void fileSelected ( QString absPath );
    //emit when project selected in the TreeView
    void projectSelected ( AbstractProjectModel* );
	//
};
//
#endif // ABSTRACTPROJECTPROXY_H
