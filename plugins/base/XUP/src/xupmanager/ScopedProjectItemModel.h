#ifndef SCOPEDPROJECTITEMMODEL_H
#define SCOPEDPROJECTITEMMODEL_H

#include <QSortFilterProxyModel>

class ProjectItemModel;

class ScopedProjectItemModel : public QSortFilterProxyModel
{
	Q_OBJECT
	
public:
	ScopedProjectItemModel( ProjectItemModel* = 0 );

protected:
	ProjectItemModel* mSourceModel;

	virtual bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const;
};

#endif // SCOPEDPROJECTITEMMODEL_H
