#ifndef XUPPROJECTMODELPROXY_H
#define XUPPROJECTMODELPROXY_H

#include <QSortFilterProxyModel>

class XUPProjectModel;

class XUPProjectModelProxy : public QSortFilterProxyModel
{
public:
	XUPProjectModelProxy( QObject* parent = 0 );
	
	virtual void setSourceModel( QAbstractItemModel* sourceModel );
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;

protected:
	XUPProjectModel* mSourceModel;
};

#endif // XUPPROJECTMODELPROXY_H
