#include "FilteredProjectItemModel.h"
#include "ProjectItemModel.h"
#include "ProjectItem.h"
#include "XUPManager.h"

#include <QFileInfo>

FilteredProjectItemModel::FilteredProjectItemModel( ProjectItemModel* m )
	: QStandardItemModel( m ), mSourceModel( 0 )
{ setSourceModel( m ); }

FilteredProjectItemModel::~FilteredProjectItemModel()
{}

FilteredProjectItem* FilteredProjectItemModel::item( int r, int c ) const
{ return dynamic_cast<FilteredProjectItem*>( QStandardItemModel::item( r, c ) ); }

FilteredProjectItem* FilteredProjectItemModel::itemFromIndex( const QModelIndex& i ) const
{ return dynamic_cast<FilteredProjectItem*>( QStandardItemModel::itemFromIndex( i ) ); }

void FilteredProjectItemModel::appendRow( FilteredProjectItem* it )
{ QStandardItemModel::appendRow( it ); }

void FilteredProjectItemModel::setSourceModel( ProjectItemModel* m )
{
	if ( mSourceModel != m )
	{
		mSourceModel = m;
		// connections
		connect( mSourceModel, SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( rowsInserted( const QModelIndex&, int, int ) ) );
	}
}

ProjectItemModel* FilteredProjectItemModel::sourceModel() const
{ return mSourceModel; }

QModelIndex FilteredProjectItemModel::mapFromSource( const QModelIndex& i ) const
{
	foreach ( ProjectItem* it, mItems.keys() )
		if ( it->index() == i )
			return mItems.value( it )->index();
	return QModelIndex();
}

QModelIndex FilteredProjectItemModel::mapToSource( const QModelIndex& i ) const
{
	foreach ( FilteredProjectItem* it, mItems.values() )
		if ( it->index() == i )
			return mItems.key( it )->index();
	return QModelIndex();
}

void FilteredProjectItemModel::addItemsRecursively( ProjectItem* it, FilteredProjectItem* pit )
{
	foreach ( ProjectItem* cit, it->children( false, true ) )
	{
		FilteredProjectItem* ncit = new FilteredProjectItem( cit );
		if ( cit->hasChildren() )
			addItemsRecursively( cit, ncit );
		pit->appendRow( ncit );
		mItems[cit] = ncit;
	}
}

FilteredProjectItem* FilteredProjectItemModel::getProject( ProjectItem* it )
{
	// return already exists if possible
	if ( mItems.contains( it ) )
		return mItems.value( it );
	
	// create and add
	FilteredProjectItem* pit = new FilteredProjectItem( it );
	if ( it->parent() )
		mItems[it->parent()->project()]->appendRow( pit );
	else
		appendRow( pit );
	mItems[it] = pit;
	
	// return item
	return pit;
}

FilteredProjectItem* FilteredProjectItemModel::getVariable( ProjectItem* it )
{
	// get project item
	FilteredProjectItem* pit = getProject( it->project() );
	Q_ASSERT( pit );
	
	// get variable name
	const QString vn = it->defaultValue();
	
	// find already existing variable item
	for ( int i = 0; i < pit->rowCount(); i++ )
	{
		FilteredProjectItem* cit = pit->child( i );
		if ( cit->item()->defaultValue() == vn )
			return cit;
	}
	
	// create item variable
	FilteredProjectItem* vit = new FilteredProjectItem( it );
	
	// calculate row to insert to
	int r = it->filteredVariables().indexOf( vn );
	int ri = -1;
	for ( int i = 0; i < pit->rowCount(); i++ )
	{
		FilteredProjectItem* cit = pit->child( i );
		const int ci = it->filteredVariables().indexOf( cit->item()->defaultValue() );
		if ( ci < r )
			ri = cit->row();
		else
			break;
	}
	
	// insert item
	getProject( it->parent()->project() )->insertRow( ri +1, vit );
	mItems[it] = vit;
	
	return vit;
}

FilteredProjectItem* FilteredProjectItemModel::getFolder( ProjectItem* it, FilteredProjectItem* vit )
{
	// get variable item
	if ( !vit )
		vit = getVariable( it->parent() );
	
	// get variable path
	const QString pn = QFileInfo( it->relativeFilePath() ).path();
	
	// if file is at root, don't create folder
	if ( pn == "." )
		return vit;
	
	// find already existing path item
	for ( int i = 0; i < vit->rowCount(); i++ )
	{
		FilteredProjectItem* cit = vit->child( i );
		if ( cit->item()->defaultValue() == pn )
			return cit;
	}
	
	// create item folder
	FilteredProjectItem* fit = new FilteredProjectItem( it->clone( false ) );
	fit->item()->setDomElement( it->domElement().ownerDocument().createElement( "folder" ) );
	fit->item()->setValue( "name", pn );
	
	// calculate row to insert to
	int ri = 0;
	for ( int i = 0; i < vit->rowCount(); i++ )
	{
		FilteredProjectItem* cit = vit->child( i );
		if ( cit->item()->isType( "folder" ) )
		{
			int r = QString::localeAwareCompare( pn, cit->item()->defaultValue() );
			if ( r < 0 )
			{
				ri = cit->row();
				break;
			}
			else
				ri = cit->row() +1;
		}
	}
	
	// insert item
	vit->insertRow( ri, fit );
	
	return fit;
}

void FilteredProjectItemModel::addFilteredValue( ProjectItem* it )
{
	// get variable item
	FilteredProjectItem* vit = getVariable( it->parent() );
	
	// check file based
	bool b = it->fileVariables().contains( vit->item()->defaultValue() );
	
	if ( !it->defaultValue().isEmpty() )
	{
		FilteredProjectItem* cit = new FilteredProjectItem( it );
		b ? getFolder( it, vit )->appendRow( cit ) : vit->appendRow( cit );
		mItems[it] = cit;
	}
}

void FilteredProjectItemModel::addFilteredVariable( ProjectItem* it )
{
	// get variable item
	FilteredProjectItem* vit = getVariable( it );
	
	// check if is file based
	bool b = it->fileVariables().contains( vit->item()->defaultValue() );
	
	// add values to vit
	foreach ( ProjectItem* cit, it->children( false, true ) )
	{
		if ( !cit->defaultValue().isEmpty() )
		{
			FilteredProjectItem* ccit = new FilteredProjectItem( cit );
			b ? getFolder( cit, vit )->appendRow( ccit ) : vit->appendRow( ccit );
			mItems[cit] = ccit;
		}
	}
}

void FilteredProjectItemModel::addFilteredProject( ProjectItem* it )
{
	// get project
	getProject( it );
	
	// add recursive variable
	foreach ( ProjectItem* cit, it->children( true, true ) )
		if ( cit->isType( "variable" ) && it->filteredVariables().contains( cit->defaultValue() ) )
			addFilteredVariable( cit );
	
	// add recursive project
	foreach ( ProjectItem* cit, it->children( false, false ) )
		if ( cit->isProject() )
			addFilteredProject( cit );
}

void FilteredProjectItemModel::rowsInserted( const QModelIndex& parent, int start, int end )
{
	for ( int i = start; i < end +1; i++ )
	{
		if ( ProjectItem* it = mSourceModel->itemFromIndex( mSourceModel->index( i, 0, parent ) ) )
		{
			// if filtered
			if ( !it->filteredVariables().isEmpty() )
			{
				if ( it->isProject() )
					addFilteredProject( it );
				else if ( it->isType( "variable" ) )
					addFilteredVariable( it );
				else if ( it->isType( "value" ) )
					addFilteredValue( it );
			}
			else
			{
				if ( it->isProject() )
					addItemsRecursively( it, getProject( it ) );
				else if ( it->isType( "variable" ) )
					addItemsRecursively( it, getProject( it->project() ) );
				else if ( it->isType( "value" ) )
					addItemsRecursively( it, getVariable( it->parent() ) );
			}
		}
	}
}

void FilteredProjectItemModel::rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end )
{
	FilteredProjectItem* folder = 0;
	for ( int i = start; i < end +1; i++ )
	{
		// get item
		QModelIndex mi = mSourceModel->index( i, 0, parent );
		ProjectItem* it = mSourceModel->itemFromIndex( mi );
		if ( it )
		{
			// keep parent
			if ( !folder )
			{
				if ( FilteredProjectItem* fi = itemFromIndex( mapFromSource( mi ) ) )
					if ( fi->parent() && fi->parent()->item()->isType( "folder" ) )
						folder = fi->parent();
			}
			// check if items are in the hash, and remove them
			foreach ( ProjectItem* cit, it->children( true, false ) )
				mItems.remove( cit );
			// remove item from filtered model
			removeRow( mItems.value( it )->row(), mItems.value( it )->index().parent() );
			mItems.remove( it );
		}
	}
	// check if parent is folder and is empty to remove it
	if ( folder && !folder->hasChildren() )
		removeRow( folder->row(), folder->index().parent() );
}

