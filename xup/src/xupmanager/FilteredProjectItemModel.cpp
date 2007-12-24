#include "FilteredProjectItemModel.h"
#include "ProjectItemModel.h"
#include "ProjectItem.h"

FilteredProjectItemModel::FilteredProjectItemModel( ProjectItemModel* m )
	: QStandardItemModel( m ), mSourceModel( 0 )
{ setSourceModel( m ); }

FilteredProjectItemModel::~FilteredProjectItemModel()
{}

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
}

QModelIndex FilteredProjectItemModel::mapToSource( const QModelIndex& ) const
{
	qWarning( "mapToSource" );
}

void FilteredProjectItemModel::addVariable( ProjectItem* v, ProjectItem* p )
{
	// get variable name
	const QString vn = v->defaultValue();
	// find already existing variable item
	ProjectItem* vit = 0;
	foreach ( ProjectItem* cit, p->children( false, true ) )
	{
		if ( cit->defaultValue() == vn )
		{
			vit = cit;
			break;
		}
	}
	// if not exist, create it
	if ( !vit )
	{
		vit = v->clone();
		vit->setValue( "operator", "-" );
		p->appendRow( vit );
		mItems[v->index()] = vit;
	}
	// add values to vit
	foreach ( ProjectItem* cit, v->children( false, true ) )
	{
		if ( !cit->defaultValue().isEmpty() )
		{
			ProjectItem* ccit = cit->clone( false );
			ccit->setBuddy( cit->buddy() ? cit->buddy() : cit );
			vit->appendRow( ccit );
			mItems[cit->index()] = ccit;
		}
	}
}

void FilteredProjectItemModel::projectInserted( ProjectItem* it, ProjectItem* p )
{
	// clone project item
	ProjectItem* pit = it->clone( false );
	pit->setBuddy( it->buddy() ? it->buddy() : it );
	if ( p )
		p->appendRow( pit );
	else
		appendRow( pit );
	mItems[it->index()] = pit;
	
	bool b = it->filteredVariables().isEmpty();
	foreach ( ProjectItem* cit, it->children( true, true ) )
		if ( cit->isType( "variable" ) && ( b || ( !b && it->filteredVariables().contains( cit->defaultValue() ) ) ) )
			addVariable( cit, pit );
	
	foreach ( ProjectItem* cit, it->children( false, false ) )
		if ( cit->isType( "project" ) )
			projectInserted( cit, pit );
}

void FilteredProjectItemModel::headerDataChanged( Qt::Orientation orientation, int first, int last )
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
		if ( mItems.contains( mi ) )
		{
			qWarning( "removing..." );
			removeRow( mItems.value( mi )->row(), mItems.value( mi )->index().parent() );
		}
	}
}

