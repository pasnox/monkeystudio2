#include "SearchResultsModel.h"
#include "SearchThread.h"

#include <QDebug>

SearchResultsModel::SearchResultsModel( SearchThread* searchThread, QObject* parent )
	: QAbstractItemModel( parent )
{
	Q_ASSERT( searchThread );
	mRowCount = 0;
	mSearchThread = searchThread;
	
	// connections
	connect( mSearchThread, SIGNAL( reset() ), this, SLOT( thread_reset() ) );
	connect( mSearchThread, SIGNAL( resultsAvailable( const QString&, const SearchResultsModel::ResultList& ) ), this, SLOT( thread_resultsAvailable( const QString&, const SearchResultsModel::ResultList& ) ) );
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
		{
			return data( index, Qt::DisplayRole );
		}
		case Qt::CheckStateRole:
		{
			if ( flags( index ) & Qt::ItemIsUserCheckable )
			{
				SearchResultsModel::Result* result = static_cast<SearchResultsModel::Result*>( index.internalPointer() );
				return result->checkState;
			}
		}
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

Qt::ItemFlags SearchResultsModel::flags( const QModelIndex& index ) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags( index );
	SearchWidget::Properties* properties = mSearchThread->properties();
	
	if ( properties->mode & SearchAndReplaceV2::ModeFlagReplace )
	{
		flags |= Qt::ItemIsUserCheckable;
	}
	
	return flags;
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

bool SearchResultsModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	const QModelIndex pIndex = index.parent();
	const bool isParent = !pIndex.isValid();
	SearchResultsModel::Result* pResult = pIndex.isValid() ? static_cast<SearchResultsModel::Result*>( pIndex.internalPointer() ) : 0;
	SearchResultsModel::Result* result = static_cast<SearchResultsModel::Result*>( index.internalPointer() );
	bool ok = false;
	
	switch ( role )
	{
		case Qt::CheckStateRole:
		{
			result->checkState = Qt::CheckState( value.toInt() );
			emit dataChanged( index, index );
			ok = true;
		}
	}
	
	if ( isParent )
	{
		// update all children to same state as parent
		foreach ( SearchResultsModel::Result* r, mResults.at( result->row ) )
		{
			r->checkState = result->checkState;
		}
		
		const QModelIndex left = index.child( 0, 0 );
		const QModelIndex right = index.child( rowCount( index ) -1, columnCount( index ) -1 );
		emit dataChanged( left, right );
	}
	else
	{
		int count = 0;
		int checkedCount = 0;
		
		foreach ( SearchResultsModel::Result* r, mResults.at( pResult->row ) )
		{
			count++;
			
			if ( r->checkState == Qt::Checked )
			{
				checkedCount++;
			}
		}
		
		// update parent
		if ( checkedCount == 0 )
		{
			pResult->checkState = Qt::Unchecked;
		}
		else if ( checkedCount == count )
		{
			pResult->checkState = Qt::Checked;
		}
		else
		{
			pResult->checkState = Qt::PartiallyChecked;
		}
		
		emit dataChanged( pIndex, pIndex );
	}
	
	return ok;
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
	SearchWidget::Properties* properties = mSearchThread->properties();
	
	if ( !result )
	{
		result = new SearchResultsModel::Result( fileName );
		result->row = mRowCount;
		result->checkable = properties->mode & SearchAndReplaceV2::ModeFlagReplace;
		result->checkState = result->checkable ? Qt::Checked : Qt::Unchecked;
		
		beginInsertRows( QModelIndex(), mRowCount, mRowCount );
		mParents[ fileName ] = result;
		mParentsRows[ mRowCount ] = result;
		mRowCount++;
		mResults << results;
		endInsertRows();
	}
	else
	{
		const int count = mResults.at( result->row ).count();
		const QModelIndex index = createIndex( result->row, 0, result );
		
		beginInsertRows( index, count, count +results.count() -1 );
		mResults[ result->row ] << results;
		endInsertRows();
	}
}
