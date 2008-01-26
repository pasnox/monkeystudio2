#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H

#include <QStandardItemModel>

class ProjectItem;
class FilteredProjectItemModel;

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
	// return the filtered model
	FilteredProjectItemModel* filteredModel();

private:
	FilteredProjectItemModel* mFilteredModel;
};

#endif // PROJECTITEMMODEL_H
