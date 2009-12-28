#include "SearchResultsModel.h"

#include <QDebug>

SearchResultsModel::SearchResultsModel( /*SearchWidget::Properties* properties,*/ QObject* parent )
	: QAbstractItemModel( parent )
{
	//Q_ASSERT( properties );
	mProperties = 0;//properties;
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
			
			if ( !pIndex.isValid() )
			{
				const int count = rowCount( index );
				text.append( QString( " (%1)" ).arg( count ) );
			}
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
		return createIndex( row, column, _this->mResults[ result ][ row ] );
	}
	
	return createIndex( row, column, (_this->mParents.begin() +row).value() );
}

QModelIndex SearchResultsModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return QModelIndex();
	}
	
	SearchResultsModel::Result* result = static_cast<SearchResultsModel::Result*>( index.internalPointer() );
	
	if ( mParents.values().contains( result ) )
	{
		return QModelIndex();
	}
	
	SearchResultsModel* _this = const_cast<SearchResultsModel*>( this );
	result = _this->mParents[ result->fileName ];
	return createIndex( mParents.keys().indexOf( result->fileName ), index.column(), result );
}

int SearchResultsModel::rowCount( const QModelIndex& parent ) const
{
	if ( !parent.isValid() )
	{
		return mParents.count();
	}
	
	SearchResultsModel::Result* result = static_cast<SearchResultsModel::Result*>( parent.internalPointer() );
	
	return mResults[ result ].count();
}

bool SearchResultsModel::hasChildren( const QModelIndex& parent ) const
{
	if ( !parent.isValid() )
	{
		return !mParents.isEmpty();
	}
	
	SearchResultsModel::Result* result = static_cast<SearchResultsModel::Result*>( parent.internalPointer() );
	
	return !mResults[ result ].isEmpty();
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
	emit reset();
}

void SearchResultsModel::thread_resultsAvailable( const QString& fileName, const SearchResultsModel::ResultList& results )
{
	SearchResultsModel::Result* result = mParents.value( fileName );
	
	if ( !result )
	{
		result = new SearchResultsModel::Result( fileName );
		mParents[ fileName ] = result;
	}
	
	mResults[ result ] << results;
	emit reset();
}
