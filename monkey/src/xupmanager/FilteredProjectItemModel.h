#ifndef FILTEREDPROJECTITEMMODEL_H
#define FILTEREDPROJECTITEMMODEL_H

#include <fresh.h>

#include <QStandardItemModel>
#include <QHash>

#include "XUPItem.h"

class ProjectItemModel;

class Q_MONKEY_EXPORT FilteredProjectItem : public QStandardItem
{
public:
	FilteredProjectItem( XUPItem* it )
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
	
	XUPItem* item() const
	{ return mItem; }
	
	XUPItem* project() const
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
	XUPItem* mItem;
};

class Q_MONKEY_EXPORT FilteredProjectItemModel : public QStandardItemModel
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
	void addItemsRecursively( XUPItem*, FilteredProjectItem* );
	FilteredProjectItem* getProject( XUPItem* );
	FilteredProjectItem* createFolder( const QString& name, XUPItem* inItem );
	FilteredProjectItem* getFolder( XUPItem*, FilteredProjectItem* = 0, const QString& forceName = QString() );
	FilteredProjectItem* getVariable( XUPItem* );
	void addFilteredValue( XUPItem* );
	void addFilteredVariable( XUPItem* );
	void addFilteredProject( XUPItem* );
	
	ProjectItemModel* mSourceModel;
	QHash<XUPItem*, FilteredProjectItem*> mItems;

private slots:
	void rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end );
	void rowsInserted( const QModelIndex& parent, int start, int end );
};

#endif // FILTEREDPROJECTITEMMODEL_H
