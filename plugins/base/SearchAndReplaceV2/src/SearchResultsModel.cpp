#include "SearchResultsModel.h"

#include <QDebug>

SearchResultsModel::SearchResultsModel( /*SearchWidget::Properties* properties,*/ QObject* parent )
	: QAbstractItemModel( parent )
{
	//Q_ASSERT( properties );
	mProperties = 0;//properties;
	mRowCount = 0;
}

SearchResultsModel::~SearchResultsModel()
{
}

int SearchResultsModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

QVariant SearchResultsModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() )
	{
		return QVariant();
	}
	
	switch ( role )
	{
		case Qt::DisplayRole:
		{
			const QModelIndex pIndex = index.parent();
			SearchResultsModel::Result* result = static_cast<SearchResultsModel::Result*>( index.internalPointer() );
			QString text = result->fileName;
			
			// parent
			if ( !pIndex.isValid() )
			{
				const int count = rowCount( index );
				text.append( QString( " (%1)" ).arg( count ) );
			}
			// child
			else
			{
				text = tr( "At position %1: %2" ).arg( result->position ).arg( result->capture );
			}
			
			return text;
		}
		case Qt::ToolTipRole:
			return data( index, Qt::DisplayRole );
	}
	
	return QVariant();
}

QModelIndex SearchResultsModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( row >= rowCount( parent ) || column >= columnCount( parent ) )
	{
		return QModelIndex();
	}
	
	SearchResultsModel* _this = const_cast<SearchResultsModel*>( this );
	SearchResultsModel::Result* result = parent.isValid() ? static_cast<SearchResultsModel::Result*>( parent.internalPointer() ) : 0;
	
	if ( result )
	{
		// child
		const int pRow = result->row;
		return createIndex( row, column, _this->mResults.at( pRow ).at( row ) );
	}
	
	// parent
	return createIndex( row, column, mParentsRows[ row ] );
}

QModelIndex SearchResultsModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return QModelIndex();
	}
	
	SearchResultsModel::Result* result = static_cast<SearchResultsModel::Result*>( index.internalPointer() );
	
	if ( result->row != -1 )
	{
		return QModelIndex();
	}
	
	SearchResultsModel* _this = const_cast<SearchResultsModel*>( this );
	result = _this->mParents[ result->fileName ];
	return createIndex( result->row, index.column(), result );
}

int SearchResultsModel::rowCount( const QModelIndex& parent ) const
{
	// root
	if ( !parent.isValid() )
	{
		return mRowCount;
	}
	
	return parent.parent().isValid() ? 0 : mResults.at( parent.row() ).count();
}

bool SearchResultsModel::hasChildren( const QModelIndex& parent ) const
{
	// root
	if ( !parent.isValid() )
	{
		return mRowCount != 0;
	}
	
	return parent.parent().isValid() ? false : !mResults.at( parent.row() ).isEmpty();
}

void SearchResultsModel::thread_reset()
{	
	foreach ( const SearchResultsModel::ResultList& results, mResults )
	{
		qDeleteAll( results );
	}
	mResults.clear();
	mParentsRows.clear();
	qDeleteAll( mParents );
	mParents.clear();
	mRowCount = 0;
	emit reset();
}

void SearchResultsModel::thread_resultsAvailable( const QString& fileName, const SearchResultsModel::ResultList& results )
{
	SearchResultsModel::Result* result = mParents[ fileName ];
	
	if ( !result )
	{
		result = new SearchResultsModel::Result( fileName );
		result->row = mRowCount;
		mParents[ fileName ] = result;
		mParentsRows[ mRowCount ] = result;
		mRowCount++;
		
		mResults << results;
	}
	else
	{
		mResults[ result->row ] << results;
	}
	
	emit reset();
}
