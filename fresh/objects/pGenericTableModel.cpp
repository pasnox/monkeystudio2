#include "pGenericTableModel.h"

#include <QDebug>

pGenericTableModel::pGenericTableModel( QObject* parent )
    : QAbstractTableModel( parent )
{
    mColumnCount = 0;
    mRowCount = 0;
}

pGenericTableModel::~pGenericTableModel()
{
}

int pGenericTableModel::columnCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? mColumnCount : 0;
}

int pGenericTableModel::rowCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? mRowCount : 0;
}

QVariant pGenericTableModel::data( const QModelIndex& index, int role ) const
{
    switch ( role ) {
        case pGenericTableModel::ItemFlagsRole:
            return QVariant::fromValue( flags( index ) );
        default: {
            QVariant value = indexInternalData( index ).value( role );
            
            if ( value.isNull() && role == Qt::DisplayRole ) {
                value = indexInternalData( index ).value( Qt::EditRole );
            }
            
            return value;
        }
    }
}

QVariant pGenericTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    return headerInternalData( section, orientation ).value( role );
}

bool pGenericTableModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    pGenericTableModel::IntVariantMap* map = indexInternalData( index );
    
    if ( !map ) {
        return false;
    }
    
    if ( value.isNull() ) {
        if ( !map->contains( role ) ) {
            return true;
        }
        
        map->remove( role );
    }
    else {
        if ( map->value( role ) == value ) {
            return true;
        }
        
        (*map)[ role ] = value;
    }
    
    emit dataChanged( index, index );
    return true;
}

bool pGenericTableModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant& value, int role )
{
    pGenericTableModel::IntVariantMap* map = headerInternalData( section, orientation );
    
    if ( !map ) {
        return false;
    }
    
    (*map)[ role ] = value;
    emit headerDataChanged( orientation, section, section );
    return true;
}

Qt::ItemFlags pGenericTableModel::flags( const QModelIndex& index ) const
{
    const QVariant variant = indexInternalData( index ).value( pGenericTableModel::ItemFlagsRole );
    return variant.isNull() ? QAbstractTableModel::flags( index ) : variant.value<Qt::ItemFlags>();
}

void pGenericTableModel::sort( int column, Qt::SortOrder order )
{
    Q_UNUSED( column );
    Q_UNUSED( order );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
}

bool pGenericTableModel::insertColumns( int column, int count, const QModelIndex& parent )
{
    Q_UNUSED( column );
    Q_UNUSED( count );
    Q_UNUSED( parent );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
    return false;
}

bool pGenericTableModel::removeColumns( int column, int count, const QModelIndex& parent )
{
    Q_UNUSED( column );
    Q_UNUSED( count );
    Q_UNUSED( parent );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
    return false;
}

bool pGenericTableModel::insertRows( int row, int count, const QModelIndex& parent )
{
    if ( parent != QModelIndex() || row < 0 || row > mRowCount || count <= 0 || mColumnCount == 0 ) {
        return false;
    }
    
    beginInsertRows( parent, row, row +count -1 );
    mRowCount += count;
    foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
        if ( point.second >= row ) {
            mData[ pGenericTableModel::Point( point.first, point.second +count ) ] = mData.take( point );
        }
    }
    endInsertRows();
    return true;
}

bool pGenericTableModel::removeRows( int row, int count, const QModelIndex& parent )
{
    if ( parent != QModelIndex() || row < 0 || row >= mRowCount || row +count -1 >= mRowCount ) {
        return false;
    }
    
    beginRemoveRows( parent, row, row +count -1 );
    mRowCount -= count;
    foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
        if ( point.second >= row && point.second < row +count ) {
            mData.remove( point );
        }
        else if ( point.second >= row +count ) {
            mData[ pGenericTableModel::Point( point.first, point.second -count ) ] = mData.take( point );
        }
    }
    endRemoveRows();
    return true;
}

void pGenericTableModel::setColumnCount( int count )
{
    if ( count == mColumnCount ) {
        return;
    }
    
    if ( count > mColumnCount ) {
        const int diff = count -mColumnCount;
        beginInsertColumns( QModelIndex(), mColumnCount, diff -1 );
        mColumnCount = count;
        endInsertColumns();
    }
    else {
        beginRemoveColumns( QModelIndex(), count, mColumnCount -1 );
        mColumnCount = count;
        foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
            if ( point.first >= mColumnCount ) {
                mData.remove( point );
            }
        }
        foreach ( const pGenericTableModel::Point& point, mHeaderData.keys() ) {
            if ( point.first == Qt::Horizontal && point.second >= mColumnCount ) {
                mHeaderData.remove( point );
            }
        }
        endRemoveColumns();
    }
}

void pGenericTableModel::setRowCount( int count )
{
    if ( count == mRowCount ) {
        return;
    }
    
    if ( count > mRowCount ) {
        beginInsertRows( QModelIndex(), mRowCount, count -1 );
        mRowCount = count;
        endInsertRows();
    }
    else {
        beginRemoveRows( QModelIndex(), count, mRowCount -1 );
        mRowCount = count;
        foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
            if ( point.second >= mRowCount ) {
                mData.remove( point );
            }
        }
        foreach ( const pGenericTableModel::Point& point, mHeaderData.keys() ) {
            if ( point.first == Qt::Vertical && point.second >= mRowCount ) {
                mHeaderData.remove( point );
            }
        }
        endRemoveRows();
    }
}

bool pGenericTableModel::swapColumns( int fromColumn, int toColumn )
{
    Q_UNUSED( fromColumn );
    Q_UNUSED( toColumn );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
    return false;
}

bool pGenericTableModel::swapRows( int fromRow, int toRow )
{
    if ( fromRow < 0 || fromRow >= mRowCount || toRow < 0 || toRow >= mRowCount ) {
        return false;
    }
    
    if ( fromRow == toRow ) {
        return true;
    }
    
    emit layoutAboutToBeChanged();
    
    const QModelIndexList oldIndexes = persistentIndexList();
    QModelIndexList newIndexes;
    pGenericTableModel::PointIntVariantMap fromData;
    pGenericTableModel::PointIntVariantMap toData;
    
    // keep backup data
    foreach( const pGenericTableModel::Point& point, mData.keys() ) {
        if ( point.second == fromRow ) {
            fromData[ point ] = mData.take( point );
        }
        else if ( point.second == toRow ) {
            toData[ point ] = mData.take( point );
        }
    }
    
    // move from data
    foreach( const pGenericTableModel::Point& point, fromData.keys() ) {
        mData[ pGenericTableModel::Point( point.first, toRow ) ] = fromData.take( point );
    }
    
    // move to data
    foreach( const pGenericTableModel::Point& point, toData.keys() ) {
        mData[ pGenericTableModel::Point( point.first, fromRow ) ] = toData.take( point );
    }
    
    // update persistent indexes
    foreach ( const QModelIndex& index, oldIndexes ) {
        if ( index.row() == fromRow ) {
            newIndexes << this->index( toRow, index.column(), index.parent() );
        }
        else if ( index.row() == toRow ) {
            newIndexes << this->index( fromRow, index.column(), index.parent() );
        }
        else {
            newIndexes << index;
        }
    }
    
    changePersistentIndexList( oldIndexes, newIndexes );
    
    emit layoutChanged();
    
    return true;
}

pGenericTableModel::IntVariantMap* pGenericTableModel::indexInternalData( const QModelIndex& index )
{
    return index.isValid() ? &mData[ pGenericTableModel::Point( index.column(), index.row() ) ] : 0;
}

pGenericTableModel::IntVariantMap pGenericTableModel::indexInternalData( const QModelIndex& index ) const
{
    return mData.value( pGenericTableModel::Point( index.column(), index.row() ) );
}

pGenericTableModel::IntVariantMap* pGenericTableModel::headerInternalData( int section, Qt::Orientation orientation )
{
    switch ( orientation ) {
        case Qt::Horizontal:
            return section >= 0 && section < mColumnCount ? &mHeaderData[ pGenericTableModel::Point( (int)orientation, section ) ] : 0;
        case Qt::Vertical:
            return section >= 0 && section < mRowCount ? &mHeaderData[ pGenericTableModel::Point( (int)orientation, section ) ] : 0;
    }
    
    return 0;
}

pGenericTableModel::IntVariantMap pGenericTableModel::headerInternalData( int section, Qt::Orientation orientation ) const
{
    return mHeaderData.value( pGenericTableModel::Point( (int)orientation, section ) );
}

void pGenericTableModel::clear( bool onlyData )
{
    if ( rowCount() == 0 && columnCount() == 0 ) {
        return;
    }
    
    if ( onlyData ) {
        beginResetModel();
        mData.clear();
        endResetModel();
    }
    else {
        beginRemoveColumns( QModelIndex(), 0, mColumnCount -1 );
        mColumnCount = 0;
        mHeaderData.clear();
        endRemoveColumns();
        
        beginRemoveRows( QModelIndex(), 0, mRowCount -1 );
        mRowCount = 0;
        mData.clear();
        endRemoveRows();
    }
}

QModelIndexList pGenericTableModel::checkedIndexes() const
{
    QModelIndexList indexes;
    
    foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
        const pGenericTableModel::IntVariantMap& map = mData[ point ];
        
        if ( map.value( Qt::CheckStateRole ).toInt() != Qt::Unchecked ) {
            indexes << index( point.second, point.first );
        }
    }
    
    return indexes;
}

QList<int> pGenericTableModel::checkedRows() const
{
    QList<int> rows;
    
    foreach ( const pGenericTableModel::Point& point, mData.keys() ) {
        const pGenericTableModel::IntVariantMap& map = mData[ point ];
        
        if ( map.value( Qt::CheckStateRole ).toInt() != Qt::Unchecked ) {
            rows << point.second;
        }
    }
    
    return rows;
}
