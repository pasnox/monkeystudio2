#ifndef SCOPEDPROJECTITEMMODEL_H
#define SCOPEDPROJECTITEMMODEL_H

#include <fresh.h>

#include <QSortFilterProxyModel>

class ProjectItemModel;
class XUPItem;

class Q_MONKEY_EXPORT ScopedProjectItemModel : public QSortFilterProxyModel
{
	Q_OBJECT
	
public:
	ScopedProjectItemModel( ProjectItemModel* = 0 );

	void setFilteredProject( XUPItem* project );
	XUPItem* filteredProject() const;

protected:
	ProjectItemModel* mSourceModel;
	XUPItem* mProject;

	virtual bool filterAcceptsRow( int sr, const QModelIndex& sp ) const;
};

#endif // SCOPEDPROJECTITEMMODEL_H
