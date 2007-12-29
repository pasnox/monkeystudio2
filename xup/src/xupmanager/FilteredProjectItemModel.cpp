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
		connect( mSourceModel, SIGNAL( columnsAboutToBeInserted( const QModelIndex&, int, int ) ), this, SLOT( columnsAboutToBeInserted( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( columnsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( columnsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( columnsInserted( const QModelIndex&, int, int ) ), this, SLOT( columnsInserted( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( columnsRemoved( const QModelIndex&, int, int ) ), this, SLOT( columnsRemoved( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( dataChanged( const QModelIndex&, const QModelIndex& ) ) );
		connect( mSourceModel, SIGNAL( headerDataChanged( Qt::Orientation, int, int ) ), this, SLOT( headerDataChanged( Qt::Orientation, int, int ) ) );
		connect( mSourceModel, SIGNAL( layoutAboutToBeChanged() ), this, SLOT( layoutAboutToBeChanged() ) );
		connect( mSourceModel, SIGNAL( layoutChanged() ), this, SLOT( layoutChanged() ) );
		connect( mSourceModel, SIGNAL( modelAboutToBeReset() ), this, SLOT( modelAboutToBeReset() ) );
		connect( mSourceModel, SIGNAL( modelReset() ), this, SLOT( modelReset() ) );
		connect( mSourceModel, SIGNAL( rowsAboutToBeInserted( const QModelIndex&, int, int ) ), this, SLOT( rowsAboutToBeInserted( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( rowsInserted( const QModelIndex&, int, int ) ) );
		connect( mSourceModel, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ), this, SLOT( rowsRemoved( const QModelIndex&, int, int ) ) );
	}
}

ProjectItemModel* FilteredProjectItemModel::sourceModel() const
{ return mSourceModel; }

QModelIndex FilteredProjectItemModel::mapFromSource( const QModelIndex& ) const
{
	qWarning( "mapFromSource" );
	return QModelIndex();
}

QModelIndex FilteredProjectItemModel::mapToSource( const QModelIndex& ) const
{
	qWarning( "mapToSource" );
	return QModelIndex();
}

FilteredProjectItem* FilteredProjectItemModel::getVariable( ProjectItem* it )
{
	// get project item
	FilteredProjectItem* pit = mItems.value( it->project() );
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
	FilteredProjectItem* vit = new FilteredProjectItem( it->clone() );
	vit->item()->setValue( "operator", "-" );
	vit->item()->setValue( "multiline", "-" );
	
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
	mItems[it->parent()->project()]->insertRow( ri +1, vit );
	
	return vit;
}

FilteredProjectItem* FilteredProjectItemModel::getFolder( ProjectItem* it, FilteredProjectItem* vit )
{
	if ( !vit )
		vit = getVariable( it->parent() );
	
	// get variable path
	const QString pn = QFileInfo( it->defaultInterpretedValue() ).path();
	
	// find already existing path item
	for ( int i = 0; i < vit->rowCount(); i++ )
	{
		FilteredProjectItem* cit = vit->child( i );
		if ( cit->item()->defaultValue() == pn )
			return cit;
	}
	
	// create item folder
	FilteredProjectItem* fit = new FilteredProjectItem( it->clone() );
	fit->item()->setValue( "type", "folder" );
	fit->item()->setValue( "icon", "folder" );
	fit->item()->setValue( "name", pn );
	
	// insert item
	vit->appendRow( fit );
	
	return fit;
}

void FilteredProjectItemModel::addVariable( ProjectItem* it )
{
	// get variable item
	FilteredProjectItem* vit = getVariable( it );
	
	bool b = XUPManager::fileVariables().contains( vit->item()->defaultValue() );
	
	// add values to vit
	foreach ( ProjectItem* cit, it->children( false, true ) )
	{
		if ( !cit->defaultValue().isEmpty() )
		{
			FilteredProjectItem* ccit = new FilteredProjectItem( cit );
			b ? getFolder( cit, vit )->appendRow( ccit ) : vit->appendRow( ccit );
			//mItems[cit] = ccit;
		}
	}
}

void FilteredProjectItemModel::projectInserted( ProjectItem* it )
{
	FilteredProjectItem* pit = new FilteredProjectItem( it );
	if ( it->parent() )
		mItems[it->parent()->project()]->appendRow( pit );
	else
		appendRow( pit );
	mItems[it] = pit;
	
	bool b = it->filteredVariables().isEmpty();
	foreach ( ProjectItem* cit, it->children( true, true ) )
		if ( cit->isType( "variable" ) && ( b || ( !b && it->filteredVariables().contains( cit->defaultValue() ) ) ) )
			addVariable( cit );
	
	foreach ( ProjectItem* cit, it->children( false, false ) )
		if ( cit->isType( "project" ) )
			projectInserted( cit );
}

void FilteredProjectItemModel::headerDataChanged( Qt::Orientation, int first, int last )
{
	qWarning( "headerDataChanged: %i, %i", first, last );
}

void FilteredProjectItemModel::layoutAboutToBeChanged()
{
	qWarning( "layoutAboutToBeChanged" );
}

void FilteredProjectItemModel::layoutChanged()
{
	qWarning( "layoutChanged" );
}

void FilteredProjectItemModel::modelAboutToBeReset()
{
	qWarning( "modelAboutToBeReset" );
}

void FilteredProjectItemModel::modelReset()
{
	qWarning( "modelReset" );
}

void FilteredProjectItemModel::rowsInserted( const QModelIndex& parent, int start, int end )
{
	for ( int i = start; i < end +1; i++ )
	{
		if ( ProjectItem* it = mSourceModel->itemFromIndex( mSourceModel->index( i, 0, parent ) ) )
		{
			if ( it->isProject() )
				projectInserted( it );
		}
	}
}

void FilteredProjectItemModel::rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end )
{
	for ( int i = start; i < end +1; i++ )
	{
		QModelIndex mi = mSourceModel->index( i, 0, parent );
		ProjectItem* it = mSourceModel->itemFromIndex( mi );
		if ( mItems.contains( it ) )
		{
			qWarning( "removing..." );
			removeRow( mItems.value( it )->row(), mItems.value( it )->index().parent() );
		}
	}
}

