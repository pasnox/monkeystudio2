#ifndef PGENERICTABLEMODEL_H
#define PGENERICTABLEMODEL_H

#include "MonkeyExport.h"

#include <QAbstractTableModel>
#include <QPoint>

class Q_MONKEY_EXPORT pGenericTableModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    enum ExtendedRole {
        ItemFlagsRole = Qt::UserRole,
        ExtendedUserRole = pGenericTableModel::ItemFlagsRole +1
    };
    
    pGenericTableModel( QObject* parent = 0 );
    virtual ~pGenericTableModel();
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    virtual bool setHeaderData( int section, Qt::Orientation orientation, const QVariant& value, int role = Qt::EditRole );
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );
    virtual bool insertColumns( int column, int count, const QModelIndex& parent = QModelIndex() );
    virtual bool removeColumns( int column, int count, const QModelIndex& parent = QModelIndex() );
    virtual bool insertRows( int row, int count, const QModelIndex& parent = QModelIndex() );
    virtual bool removeRows( int row, int count, const QModelIndex& parent = QModelIndex() );
    
    void setColumnCount( int count );
    void setRowCount( int count );
    bool swapColumns( int fromColumn, int toColumn );
    bool swapRows( int fromRow, int toRow );
    void clear( bool onlyData );
    
    QModelIndexList checkedIndexes( int column = -1 ) const;
    QList<int> checkedRows( int column = -1 ) const;
    QStringList checkedStringList( int column = -1 ) const;
    void clearCheckStates( int column = -1 );
    
protected:
    typedef QMap<int, QVariant> IntVariantMap; // represent an index
    typedef QMap<QPoint, pGenericTableModel::IntVariantMap> PointIntVariantMap; // represent an index data
    
    int mColumnCount;
    int mRowCount;
    pGenericTableModel::PointIntVariantMap mHeaderData;
    pGenericTableModel::PointIntVariantMap mData;
    
    pGenericTableModel::IntVariantMap* indexInternalData( const QModelIndex& index );
    pGenericTableModel::IntVariantMap indexInternalData( const QModelIndex& index ) const;
    
    pGenericTableModel::IntVariantMap* headerInternalData( int section, Qt::Orientation orientation );
    pGenericTableModel::IntVariantMap headerInternalData( int section, Qt::Orientation orientation ) const;
};

bool operator<( const QPoint& left, const QPoint& right );

Q_DECLARE_METATYPE( Qt::ItemFlags )

#endif // PGENERICTABLEMODEL_H
