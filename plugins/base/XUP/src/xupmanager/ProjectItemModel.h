#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H

#include <QStandardItemModel>

class XUPItem;
class FilteredProjectItemModel;
class ScopedProjectItemModel;

class ProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	
public:
	ProjectItemModel( QObject* = 0 );
	~ProjectItemModel();
	
	// return item for row / column
	XUPItem* item( int, int = 0 ) const;
	// return item from index
	XUPItem* itemFromIndex( const QModelIndex& ) const;
	// append row item
	void appendRow( XUPItem* );
	// return all top level proejcts for column
	QList<XUPItem*> topLevelProjects( int = 0 ) const;
	// return the filtered model, ie a view that is beautifull for user
	FilteredProjectItemModel* filteredModel();
	// return a scoped view of the project, ie a view that only contains project and scope items
	ScopedProjectItemModel* scopedModel();

private:
	FilteredProjectItemModel* mFilteredModel;
	ScopedProjectItemModel* mScopedModel;
};

#endif // PROJECTITEMMODEL_H
