#include "pOpenedFileModel.h"
#include "pWorkspace.h"
#include "pAbstractChild.h"

#include <QDebug>

struct OpeningOrderSorter
{
	OpeningOrderSorter( const QList<pAbstractChild*>& documents )
	{
		originalDocuments = documents;
	}
	
	bool operator()( pAbstractChild* left, pAbstractChild* right ) const
	{
		return originalDocuments.indexOf( left ) < originalDocuments.indexOf( right );
	}
	
	QList<pAbstractChild*> originalDocuments;
};

struct FileNameSorter
{
	bool operator()( pAbstractChild* left, pAbstractChild* right ) const
	{
		return left->fileName().toLower() < right->fileName().toLower();
	}
};

struct URLSorter
{
	bool operator()( pAbstractChild* left, pAbstractChild* right ) const
	{
		return left->filePath().toLower() < right->filePath().toLower();
	}
};

struct SuffixesSorter
{
	bool operator()( pAbstractChild* left, pAbstractChild* right ) const
	{
		const QFileInfo leftInfos( left->filePath() );
		const QString leftBaseName = leftInfos.baseName().toLower();
		const QString leftSuffix = leftInfos.completeSuffix().toLower();
		const QFileInfo rightInfos( right->filePath() );
		const QString rightBaseName = rightInfos.baseName().toLower();
		const QString rightSuffix = rightInfos.completeSuffix().toLower();
		
		if ( leftSuffix == rightSuffix )
		{
			return leftBaseName < rightBaseName;
		}
		
		return leftSuffix < rightSuffix;
	}
};

pOpenedFileModel::pOpenedFileModel( pWorkspace* workspace )
	: QAbstractItemModel( workspace )
{
	Q_ASSERT( workspace );
	mWorkspace = workspace;
	mSortMode = pOpenedFileModel::OpeningOrder;
	mTransparentIcon = QIcon( ":/file/icons/file/transparent.png" );
	
	connect( workspace, SIGNAL( documentOpened( pAbstractChild* ) ), this, SLOT( documentOpened( pAbstractChild* ) ) );
	connect( workspace, SIGNAL( documentClosed( pAbstractChild* ) ), this, SLOT( documentClosed( pAbstractChild* ) ) );
}

pOpenedFileModel::~pOpenedFileModel()
{
}

int pOpenedFileModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return 1;
}

int pOpenedFileModel::rowCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? 0 : mDocuments.count();
}

bool pOpenedFileModel::hasChildren( const QModelIndex& parent ) const
{
	return parent.isValid() ? false : !mDocuments.isEmpty();
}

QVariant pOpenedFileModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( section == 0 && orientation == Qt::Horizontal )
	{
		switch ( role )
		{
			case Qt::DecorationRole:
				break;
			case Qt::DisplayRole:
				return tr( "Opened Files" );
				break;
			default:
				break;
		}
	}
	
	return QVariant();
}

QVariant pOpenedFileModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() )
	{
		return QVariant();
	}
	
	switch ( role )
	{
		case Qt::DecorationRole:
		{
			QIcon icon = mDocuments.at( index.row() )->windowIcon();
			
			if ( icon.isNull() )
			{
				icon = mTransparentIcon;
			}
			
			return icon;
			break;
		}
		case Qt::DisplayRole:
			return mDocuments.at( index.row() )->fileName();
			break;
		case Qt::ToolTipRole:
			return mDocuments.at( index.row() )->filePath();
			break;
		default:
			break;
	}
	
	return QVariant();
}

QModelIndex pOpenedFileModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( parent.isValid() || column > 0 || row >= mDocuments.count() )
	{
		return QModelIndex();
	}
	
	return createIndex( row, column, mDocuments.at( row ) );
}

QModelIndex pOpenedFileModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

pOpenedFileModel::SortMode pOpenedFileModel::sortMode() const
{
	return mSortMode;
}

void pOpenedFileModel::setSortMode( pOpenedFileModel::SortMode mode )
{
	if ( mSortMode != mode )
	{
		mSortMode = mode;
		sortDocuments();
	}
}

void pOpenedFileModel::sortDocuments()
{
	if ( mSortMode == pOpenedFileModel::Custom )
	{
		return;
	}
	
	emit layoutAboutToBeChanged();
	const QModelIndexList pOldIndexes = persistentIndexList();
	QModelIndexList pIndexes;
	QMap<int, pAbstractChild*> documentsMapping;
	QMap<int, int> mapping;
	
	for ( int i = 0; i < pOldIndexes.count(); i++ )
	{
		const QModelIndex& index = pOldIndexes.at( i );
		const int row = index.row();
		documentsMapping[ row ] = mDocuments.at( row );
		mapping[ row ] = row;
	}
	
	switch ( mSortMode )
	{
		case pOpenedFileModel::OpeningOrder:
		{
			OpeningOrderSorter functor( mWorkspace->documents() );
			qSort( mDocuments.begin(), mDocuments.end(), functor );
			break;
		}
		case pOpenedFileModel::FileName:
		{
			FileNameSorter functor;
			qSort( mDocuments.begin(), mDocuments.end(), functor );
			break;
		}
		case pOpenedFileModel::URL:
		{
			URLSorter functor;
			qSort( mDocuments.begin(), mDocuments.end(), functor );
			break;
		}
		case pOpenedFileModel::Suffixes:
		{
			SuffixesSorter functor;
			qSort( mDocuments.begin(), mDocuments.end(), functor );
			break;
		}
		case pOpenedFileModel::Custom:
			break;
	}
	
	for ( int i = 0; i < pOldIndexes.count(); i++ )
	{
		const QModelIndex& pIndex = pOldIndexes.at( i );
		const int row = pIndex.row();
		pAbstractChild* document = documentsMapping[ row ];
		const int index = mDocuments.indexOf( document );
		mapping[ row ] = index;
	}
	
	for ( int i = 0; i < pOldIndexes.count(); i++ )
	{
		const QModelIndex& pIndex = pOldIndexes.at( i );
		const int row = pIndex.row();
		const int index = mapping[ row ];
		
		if ( pOldIndexes.at( i ).isValid() )
			pIndexes << createIndex( index, pIndex.column(), mDocuments.at( index ) );
		else
			pIndexes << QModelIndex();
	}
	
	changePersistentIndexList( pOldIndexes, pIndexes );
	emit layoutChanged();
}

void pOpenedFileModel::documentOpened( pAbstractChild* document )
{
	Q_ASSERT( !mDocuments.contains( document ) );
	const int index = mDocuments.count();
	beginInsertRows( QModelIndex(), index, index );
	mDocuments << document;
	endInsertRows();
	sortDocuments();
}

void pOpenedFileModel::documentClosed( pAbstractChild* document )
{
	const int index = mDocuments.indexOf( document );
	beginRemoveRows( QModelIndex(), index, index );
	mDocuments.removeOne( document );
	endRemoveRows();
}
