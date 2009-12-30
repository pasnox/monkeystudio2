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
			SearchResultsModel::Result* result = index.isValid() ? static_cast<SearchResultsModel::Result*>( index.internalPointer() ) : 0;
			QString text = result->fileName;
			
			// index is a root parent
			if ( mParentsList.value( index.row() ) == result )
			{
				const int count = rowCount( index );
				text.append( QString( " (%1)" ).arg( count ) );
			}
			// index is a root parent child
			else
			{
				text = tr( "Line: %1, Column: %2 (offset %3) %4" ).arg( result->position.y() +1 ).arg( result->position.x() ).arg( result->offset ).arg( result->capture );
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
	
	SearchResultsModel::Result* result = parent.isValid() ? static_cast<SearchResultsModel::Result*>( parent.internalPointer() ) : 0;
	
	// parent is a root parent
	if ( result && mParentsList.value( parent.row() ) == result )
	{
		result = mResults.at( parent.row() ).at( row );
		return createIndex( row, column, result );
	}
	
	Q_ASSERT( !parent.isValid() );
	
	// parent is invalid
	return createIndex( row, column, mParentsList[ row ] );
}

QModelIndex SearchResultsModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return QModelIndex();
	}
	
	SearchResultsModel::Result* result = index.isValid() ? static_cast<SearchResultsModel::Result*>( index.internalPointer() ) : 0;
	
	// index is a root parent
	if ( result && mParentsList.value( index.row() ) == result )
	{
		return QModelIndex();
	}
	
	Q_ASSERT( index.isValid() );
	
	result = mParents[ result->fileName ];
	const int row = mParentsList.indexOf( result );
	
	// index is a root parent child
	return createIndex( row, index.column(), result );
}

int SearchResultsModel::rowCount( const QModelIndex& parent ) const
{
	// root parents
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
	// root parents
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
		const int pRow = mParentsList.indexOf( result );
		
		// update all children to same state as parent
		foreach ( SearchResultsModel::Result* r, mResults.at( pRow ) )
		{
			r->checkState = result->checkState;
		}
		
		const QModelIndex left = index.child( 0, 0 );
		const QModelIndex right = index.child( rowCount( index ) -1, columnCount( index ) -1 );
		emit dataChanged( left, right );
	}
	else
	{
		const int pRow = mParentsList.indexOf( pResult );
		int count = 0;
		int checkedCount = 0;
		
		foreach ( SearchResultsModel::Result* r, mResults.at( pRow ) )
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

const QList<SearchResultsModel::ResultList>& SearchResultsModel::results() const
{
	return mResults;
}

void SearchResultsModel::thread_reset()
{	
	foreach ( const SearchResultsModel::ResultList& results, mResults )
	{
		qDeleteAll( results );
	}
	mResults.clear();
	qDeleteAll( mParents );
	mParents.clear();
	mParentsList.clear();
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
		result->checkable = properties->mode & SearchAndReplaceV2::ModeFlagReplace;
		result->checkState = result->checkable ? Qt::Checked : Qt::Unchecked;
		
		beginInsertRows( QModelIndex(), mRowCount, mRowCount );
		mParents[ fileName ] = result;
		mParentsList << result;
		mRowCount++;
		mResults << results;
		endInsertRows();
	}
	else
	{
		const int pRow = mParentsList.indexOf( result );
		const int count = mResults.at( pRow ).count();
		const QModelIndex index = createIndex( pRow, 0, result );
		
		beginInsertRows( index, count, count +results.count() -1 );
		mResults[ pRow ] << results;
		endInsertRows();
		
		emit dataChanged( index, index );
	}
}

void SearchResultsModel::thread_resultsHandled( const QString& fileName, const SearchResultsModel::ResultList& results )
{
	SearchResultsModel::Result* pResult = mParents.value( fileName );
	
	Q_ASSERT( pResult );
	
	const int pRow = mParentsList.indexOf( pResult );
	SearchResultsModel::ResultList& children = mResults[ pRow ];
	const QModelIndex pIndex = createIndex( pRow, 0, pResult );
	
	foreach ( SearchResultsModel::Result* result, results )
	{
		const int index = children.indexOf( result );
		beginRemoveRows( pIndex, index, index );
		delete children.takeAt( index );
		endRemoveRows();
	}
	
	if ( children.isEmpty() )
	{
		beginRemoveRows( QModelIndex(), pRow, pRow );
		mResults.removeAt( pRow );
		mParentsList.removeAt( pRow );
		delete mParents.take( fileName );
		endRemoveRows();
	}
	else
	{
		pResult->checkState = Qt::Unchecked;
		emit dataChanged( pIndex, pIndex );
	}
	
	//qWarning() << mParents.count() << mParentsList.count() << mResults.count();
}
