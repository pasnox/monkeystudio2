#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H

#include <QStandardItemModel>

class ProjectItem;
class FilteredProjectItemModel;
class ScopedProjectItemModel;

class ProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	
public:
	ProjectItemModel( QObject* = 0 );
	~ProjectItemModel();
	
	// return item for row / column
	ProjectItem* item( int, int = 0 ) const;
	// return item from index
	ProjectItem* itemFromIndex( const QModelIndex& ) const;
	// append row item
	void appendRow( ProjectItem* );
	// return all top level proejcts for column
	QList<ProjectItem*> topLevelProjects( int = 0 ) const;
	// return the filtered model, ie a view that is beautifull for user
	FilteredProjectItemModel* filteredModel();
	// return a scoped view of the project, ie a view that only contains project and scope items
	ScopedProjectItemModel* scopedModel();

private:
	FilteredProjectItemModel* mFilteredModel;
	ScopedProjectItemModel* mScopedModel;
};

#endif // PROJECTITEMMODEL_H
