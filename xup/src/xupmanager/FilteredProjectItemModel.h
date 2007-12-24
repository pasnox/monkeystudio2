#ifndef FILTEREDPROJECTITEMMODEL_H
#define FILTEREDPROJECTITEMMODEL_H

#include <QStandardItemModel>
#include <QHash>

class ProjectItemModel;
class ProjectItem;

class FilteredProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	
public:
	FilteredProjectItemModel( ProjectItemModel* = 0 );
	~FilteredProjectItemModel();
	void setSourceModel( ProjectItemModel* );
	ProjectItemModel* sourceModel() const;
	QModelIndex mapFromSource( const QModelIndex& ) const;
	QModelIndex mapToSource( const QModelIndex& ) const;

private:
	void addVariable( ProjectItem*, ProjectItem* );
	void projectInserted( ProjectItem*, ProjectItem* = 0 );
	ProjectItemModel* mSourceModel;
	QHash<QModelIndex, ProjectItem*> mItems;

private slots:
	//void columnsAboutToBeInserted( const QModelIndex& parent, int start, int end );
	//void columnsAboutToBeRemoved( const QModelIndex& parent, int start, int end );
	//void columnsInserted( const QModelIndex& parent, int start, int end );
	//void columnsRemoved( const QModelIndex& parent, int start, int end );
	//void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
	void headerDataChanged( Qt::Orientation orientation, int first, int last );
	void layoutAboutToBeChanged();
	void layoutChanged();
	void modelAboutToBeReset();
	void modelReset();
	//void rowsAboutToBeInserted( const QModelIndex& parent, int start, int end );
	void rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end );
	void rowsInserted( const QModelIndex& parent, int start, int end );
	//void rowsRemoved( const QModelIndex& parent, int start, int end );
};

#endif // FILTEREDPROJECTITEMMODEL_H
