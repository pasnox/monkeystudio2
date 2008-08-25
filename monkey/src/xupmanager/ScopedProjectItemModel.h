#ifndef SCOPEDPROJECTITEMMODEL_H
#define SCOPEDPROJECTITEMMODEL_H

#include <fresh.h>

#include <QSortFilterProxyModel>

class ProjectItemModel;
class XUPItem;
class XUPProjectItem;

class Q_MONKEY_EXPORT ScopedProjectItemModel : public QSortFilterProxyModel
{
	Q_OBJECT
	
public:
	ScopedProjectItemModel( ProjectItemModel* = 0 );

	void setFilteredProject( XUPProjectItem* project );
	XUPProjectItem* filteredProject() const;
	
	inline ProjectItemModel* model() const { return mSourceModel; }

protected:
	ProjectItemModel* mSourceModel;
	XUPProjectItem* mProject;

	virtual bool filterAcceptsRow( int sr, const QModelIndex& sp ) const;
};

#endif // SCOPEDPROJECTITEMMODEL_H
