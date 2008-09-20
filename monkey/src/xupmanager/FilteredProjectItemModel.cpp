#include "FilteredProjectItemModel.h"
#include "ProjectItemModel.h"
#include "XUPItem.h"

#include <QDir>
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
	foreach ( XUPItem* it, mItems.keys() )
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

void FilteredProjectItemModel::addItemsRecursively( XUPItem* it, FilteredProjectItem* pit )
{
	foreach ( XUPItem* cit, it->children( false, true ) )
	{
		FilteredProjectItem* ncit = new FilteredProjectItem( cit );
		if ( cit->hasChildren() )
			addItemsRecursively( cit, ncit );
		pit->appendRow( ncit );
		mItems[cit] = ncit;
	}
}

FilteredProjectItem* FilteredProjectItemModel::getProject( XUPItem* it )
{
	// return already exists if possible
	if ( mItems.contains( it ) )
		return mItems.value( it );
	// create and add
	FilteredProjectItem* pit = new FilteredProjectItem( it );
	if ( it != it->topProjectForInclude() )
		getFolder( it, mItems[it->parent()->topProjectForInclude()], tr( "Includes Projects" ) )->appendRow( pit );
	else if ( it->parent() )
		mItems[it->parent()->project()]->appendRow( pit );
	else
		appendRow( pit );
	mItems[it] = pit;
	
	// return item
	return pit;
}

FilteredProjectItem* FilteredProjectItemModel::getVariable( XUPItem* it )
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
	int r = it->project()->filteredVariables().indexOf( vn );
	int ri = -1;
	for ( int i = 0; i < pit->rowCount(); i++ )
	{
		FilteredProjectItem* cit = pit->child( i );
		const int ci = it->project()->filteredVariables().indexOf( cit->item()->defaultValue() );
		if ( cit->item()->isProject() )
			continue;
		else if ( ci < r )
			ri = cit->row();
		else
			break;
	}
	
	// insert item
	getProject( it->parent()->project() )->insertRow( ri +1, vit );
	mItems[it] = vit;
	
	return vit;
}

FilteredProjectItem* FilteredProjectItemModel::createFolder( const QString& name, XUPItem* inItem )
{
	FilteredProjectItem* fit = new FilteredProjectItem(new XUPItem());
	fit->item()->setDomElement( inItem->domElement().ownerDocument().createElement( "folder" ) );
	fit->item()->setValue( "name", name );
	return fit;
}

FilteredProjectItem* FilteredProjectItemModel::getFolder( XUPItem* it, FilteredProjectItem* vit, const QString& forceName )
{
	// get variable item
	if ( !vit )
		vit = getVariable( it->parent() );
	
	// get variable path
	const QString pn = forceName.isEmpty() ? QFileInfo( QDir( it->project()->projectPath() ).relativeFilePath( it->filePath() ) ).path() : forceName;
	
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
	FilteredProjectItem* fit = createFolder( pn, it );
	
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

void FilteredProjectItemModel::addFilteredValue( XUPItem* it )
{
	// get parent variable
	XUPItem* parent = it->parent();
	// check if we need to show this value
	if ( !parent->project()->filteredVariables().contains( parent->defaultValue() ) )
		return;
	
	// get variable item
	FilteredProjectItem* vit = getVariable( parent );
	
	// check file based
	bool b = it->project()->fileVariables().contains( parent->defaultValue() );
	
	if ( !it->defaultValue().isEmpty() )
	{
		FilteredProjectItem* cit = new FilteredProjectItem( it );
		b ? getFolder( it, vit )->appendRow( cit ) : vit->appendRow( cit );
		mItems[it] = cit;
	}
}

void FilteredProjectItemModel::addFilteredVariable( XUPItem* it )
{
	// check we need to show it
	if ( !it->project()->filteredVariables().contains( it->defaultValue() ) )
		return;
	
	// add values to vit
	foreach ( XUPItem* cit, it->children( false, true ) )
		if ( cit->isType( "value" ) )
			addFilteredValue( cit );
}

void FilteredProjectItemModel::addFilteredProject( XUPItem* it )
{
	// get project
	getProject( it->project() );
	
	// add recursive variable
	foreach ( XUPItem* cit, it->children( true, false ) )
	{
		if ( cit->isProject() )
			getProject( cit );
		else if ( cit->isType( "variable" ) )
			addFilteredVariable( cit );
	}
}

void FilteredProjectItemModel::rowsInserted( const QModelIndex& parent, int start, int end )
{
	for ( int i = start; i < end +1; i++ )
	{
		if ( XUPItem* it = mSourceModel->itemFromIndex( mSourceModel->index( i, 0, parent ) ) )
		{
			// if filtered
			if ( !it->project()->filteredVariables().isEmpty() )
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
		XUPItem* it = mSourceModel->itemFromIndex( mi );
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
			foreach ( XUPItem* cit, it->children( true, false ) )
				mItems.remove( cit );
			// remove item from filtered model if needed
			if ( mItems.value( it ) )
				removeRow( mItems.value( it )->row(), mItems.value( it )->index().parent() );
			mItems.remove( it );
		}
	}
	// check if parent is folder and is empty to remove it
	if ( folder && !folder->hasChildren() )
		removeRow( folder->row(), folder->index().parent() );
}

