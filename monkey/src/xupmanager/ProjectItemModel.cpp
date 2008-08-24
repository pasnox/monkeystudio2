#include "ProjectItemModel.h"
#include "XUPItem.h"
#include "XUPProjectItem.h"
#include "FilteredProjectItemModel.h"
#include "ScopedProjectItemModel.h"

ProjectItemModel::ProjectItemModel( QObject* o )
	: QStandardItemModel( o ), mFilteredModel( 0 ), mScopedModel( 0 )
{ setItemPrototype( new XUPItem() ); }

ProjectItemModel::~ProjectItemModel()
{}

XUPItem* ProjectItemModel::item( int r, int c ) const
{ return dynamic_cast<XUPItem*>( QStandardItemModel::item( r, c ) ); }

XUPItem* ProjectItemModel::itemFromIndex( const QModelIndex& i ) const
{ return dynamic_cast<XUPItem*>( QStandardItemModel::itemFromIndex( i ) ); }

void ProjectItemModel::appendRow( XUPItem* it )
{ QStandardItemModel::appendRow( it ); }

QList<XUPProjectItem*> ProjectItemModel::topLevelProjects( int c ) const
{
	QList<XUPProjectItem*> l;
	for ( int i = 0; i < rowCount(); i++ )
		l << dynamic_cast<XUPProjectItem*> (item( i, c ));
	return l;
}

FilteredProjectItemModel* ProjectItemModel::filteredModel()
{
	if ( !mFilteredModel )
		mFilteredModel = new FilteredProjectItemModel( this );
	return mFilteredModel;
}

ScopedProjectItemModel* ProjectItemModel::scopedModel()
{
	if ( !mScopedModel )
		mScopedModel = new ScopedProjectItemModel( this );
	return mScopedModel;
}
