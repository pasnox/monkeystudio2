#ifndef PLOCALEMODEL_H
#define PLOCALEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class pLocaleModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    pLocaleModel( QObject* parent = 0 );
    virtual ~pLocaleModel();
    
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& child ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    QModelIndex localeToIndex( const QString& locale ) const;
    QString indexToLocale( const QModelIndex& index ) const;
    
    bool isCheckable() const;
    void setCheckable( bool checkable );
    
    QStringList checkedLocales() const;
    void setCheckedLocales( const QStringList& locales, bool checked = true );
    
    static QString localeDisplayText( const QString& name );
    
protected:
    typedef QHash<int, QVariant> IntVariantMap;
    mutable QStringList mLocales; // top level locales
    mutable QHash<QString, QStringList> mChildrenLocales; // children locales of a top level locale
    QHash<QString, pLocaleModel::IntVariantMap> mData;
    bool mIsCheckable;
    
    void populate();
    void debug( const QModelIndex& root = QModelIndex(), int prof = 0 ) const;
};

#endif // PLOCALEMODEL_H
