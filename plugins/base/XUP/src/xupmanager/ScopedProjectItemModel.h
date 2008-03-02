#ifndef SCOPEDPROJECTITEMMODEL_H
#define SCOPEDPROJECTITEMMODEL_H

#include <QSortFilterProxyModel>

class ProjectItemModel;
class XUPItem;

class ScopedProjectItemModel : public QSortFilterProxyModel
{
	Q_OBJECT
	
public:
	ScopedProjectItemModel( ProjectItemModel* = 0 );

	void setFilteredProject( XUPItem* project );
	XUPItem* filteredProject() const;

protected:
	ProjectItemModel* mSourceModel;
	XUPItem* mProject;

	virtual bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const;
};

#endif // SCOPEDPROJECTITEMMODEL_H
