#ifndef FILTEREDPROJECTITEMMODEL_H
#define FILTEREDPROJECTITEMMODEL_H

#include <QStandardItemModel>
#include <QHash>

#include "ProjectItem.h"

class ProjectItemModel;

class FilteredProjectItem : public QStandardItem
{
public:
	FilteredProjectItem( ProjectItem* it )
	{
		mItem = it;
		Q_ASSERT( mItem );
	}
	
	virtual QVariant data( int r = Qt::UserRole +1 ) const
	{ return mItem->data( r ); }
	
	virtual FilteredProjectItem* child( int r, int c = 0 ) const
	{ return dynamic_cast<FilteredProjectItem*>( QStandardItem::child( r, c ) ); }
	
	virtual void appendRow( FilteredProjectItem* it )
	{ insertRow( rowCount(), it ); }
	
	virtual void insertRow( int i, FilteredProjectItem* it )
	{ QStandardItem::insertRow( i, it ); }
	
	virtual FilteredProjectItem* parent() const
	{ return dynamic_cast<FilteredProjectItem*>( QStandardItem::parent() ); }
	
	ProjectItem* item() const
	{ return mItem; }
	
	ProjectItem* project() const
	{
		if ( mItem->isProject() )
			return mItem;
		else if ( mItem->isType( "value" ) )
			return mItem->project();
		else if ( mItem->isType( "folder" ) )
			return parent()->parent()->project();
		else
			return parent()->project();
	}
	
protected:
	ProjectItem* mItem;
};

class FilteredProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	
public:
	FilteredProjectItemModel( ProjectItemModel* = 0 );
	~FilteredProjectItemModel();

	FilteredProjectItem* item( int, int = 0 ) const;
	FilteredProjectItem* itemFromIndex( const QModelIndex& ) const;
	void appendRow( FilteredProjectItem* );

	void setSourceModel( ProjectItemModel* );
	ProjectItemModel* sourceModel() const;
	QModelIndex mapFromSource( const QModelIndex& ) const;
	QModelIndex mapToSource( const QModelIndex& ) const;

private:
	void addItemsRecursively( ProjectItem*, FilteredProjectItem* );
	FilteredProjectItem* getProject( ProjectItem* );
	FilteredProjectItem* getFolder( ProjectItem*, FilteredProjectItem* = 0 );
	FilteredProjectItem* getVariable( ProjectItem* );
	void addFilteredValue( ProjectItem* );
	void addFilteredVariable( ProjectItem* );
	void addFilteredProject( ProjectItem* );
	
	ProjectItemModel* mSourceModel;
	QHash<ProjectItem*, FilteredProjectItem*> mItems;

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
