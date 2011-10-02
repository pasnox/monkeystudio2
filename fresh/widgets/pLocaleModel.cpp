#include "pLocaleModel.h"
#include "pIconManager.h"

#include <QLocale>
#include <QDebug>

#define pLocaleModelColumnCount 1

bool caseInsensitiveLessThanStringLocale( const QString& left, const QString& right )
{
    return pLocaleModel::localeDisplayText( left ).toLower() < pLocaleModel::localeDisplayText( right ).toLower();
}

pLocaleModel::pLocaleModel( QObject* parent )
    : QAbstractItemModel( parent )
{
    mIsCheckable = false;
    
    populate();
    //debug();
}

pLocaleModel::~pLocaleModel()
{
}

QModelIndex pLocaleModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( parent.isValid() ) {
        const QString parentLocale = indexToLocale( parent );
        QStringList* list = mChildrenLocales.contains( parentLocale ) ? &mChildrenLocales[ parentLocale ] : 0;
        
        if ( !list || row < 0 || row >= list->count() || column < 0 || column >= pLocaleModelColumnCount ) {
            return QModelIndex();
        }
        
        return createIndex( row, column, &(*list)[ row ] );
    }
    
    if ( row < 0 || row >= mLocales.count() || column < 0 || column >= pLocaleModelColumnCount ) {
        return QModelIndex();
    }
    
    return createIndex( row, column, &mLocales[ row ] );
}

QModelIndex pLocaleModel::parent( const QModelIndex& child ) const
{
    const QLocale childLocale( indexToLocale( child ) );
    const QLocale parentLocale( childLocale.language() );
    return parentLocale == childLocale ? QModelIndex() : localeToIndex( parentLocale.name() );
}

int pLocaleModel::rowCount( const QModelIndex& parent ) const
{
    const QString locale = indexToLocale( parent );
    return parent == QModelIndex() ? mLocales.count() : mChildrenLocales.value( locale ).count();
}

int pLocaleModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent );
    return pLocaleModelColumnCount;
}

QVariant pLocaleModel::data( const QModelIndex& index, int role ) const
{
    if ( index.isValid() ) {
        const QString name = indexToLocale( index );
        const QString countryCode = name.section( '_', 1 );
        
        switch ( role ) {
            case Qt::DecorationRole:
                return pIconManager::icon( QString( "%1.png" ).arg( countryCode.toLower() ), ":/country-flags" );
            case Qt::DisplayRole:
                return pLocaleModel::localeDisplayText( name );
            case Qt::ToolTipRole:
                return name;
            case Qt::CheckStateRole: {
                if ( !mIsCheckable ) {
                    return QVariant();
                }
                
                Qt::CheckState state = Qt::CheckState( mData.value( name ).value( role, Qt::Unchecked ).toInt() );
                
                if ( state == Qt::Unchecked ) {
                    const QStringList locales = mChildrenLocales.value( name );
                    bool hasChildrenChecked = false;
                
                    foreach ( const QString& locale, locales ) {
                        if ( mData.value( locale ).value( Qt::CheckStateRole ).toInt() == Qt::Checked ) {
                            hasChildrenChecked = true;
                            break; // foreach
                        }
                    }
                    
                    if ( hasChildrenChecked ) {
                        state = Qt::PartiallyChecked;
                    }
                }
                
                return state;
            }
            default:
                break;
        }
        
        return mData.value( name ).value( role );
    }
    
    return QVariant();
}

Qt::ItemFlags pLocaleModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    
    if ( mIsCheckable ) {
        flags |= Qt::ItemIsUserCheckable;
    }
    
    return flags;
}

bool pLocaleModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if ( !index.isValid() ) {
        return false;
    }
    
    switch ( role ) {
        case Qt::DecorationRole:
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return false;
        default:
            break;
    }
    
    const QString name = indexToLocale( index );
    
    if ( !value.isNull() ) {
        mData[ name ][ role ] = value;
    }
    else if ( mData.value( name ).contains( role ) ) {
        mData[ name ].remove( role );
        
        if ( mData[ name ].isEmpty() ) {
            mData.remove( name );
        }
    }
    
    emit dataChanged( index, index );
    
    // update parent partially check state
    if ( role == Qt::CheckStateRole ) {
        const QModelIndex parent = index.parent();
        emit dataChanged( parent, parent );
    }
    
    return true;
}

QModelIndex pLocaleModel::localeToIndex( const QString& locale ) const
{
    int row = mLocales.indexOf( locale );
    
    if ( row != -1 ) {
        return createIndex( row, 0, &mLocales[ row ] );
    }
    
    const QString parentLocale = QLocale( QLocale( locale ).name().section( '_', 0, 0 ) ).name();
    row = mChildrenLocales.value( parentLocale ).indexOf( locale );
    QStringList* list = row != -1 ? &mChildrenLocales[ parentLocale ] : 0;
    
    if ( !list || row < 0 || row >= list->count() ) {
        return QModelIndex();
    }
    
    return createIndex( row, 0, &(*list)[ row ] );
}

QString pLocaleModel::indexToLocale( const QModelIndex& index ) const
{
    return index.isValid() ? *static_cast<QString*>( index.internalPointer() ) : QString::null;
}

bool pLocaleModel::isCheckable() const
{
    return mIsCheckable;
}

void pLocaleModel::setCheckable( bool checkable )
{
    mIsCheckable = checkable;
    emit dataChanged( index( 0, 0 ), index( rowCount() -1, columnCount() -1 ) );
}

QStringList pLocaleModel::checkedLocales() const
{
    QStringList names;
    
    foreach ( const QString& name, mData.keys() ) {
        if ( mData[ name ].value( Qt::CheckStateRole ).toInt() == Qt::Checked ) {
            names << name;
        }
    }
    
    return names;
}

void pLocaleModel::setCheckedLocales( const QStringList& locales, bool checked )
{
    foreach ( QString name, locales ) {
        // fix not complet locale ( only language and not country )
        if ( name.count( "_" ) == 0 ) {
            name = QLocale( name ).name();
        }
        
        if ( checked ) {
            mData[ name ][ Qt::CheckStateRole ] = Qt::Checked;
        }
        else if ( mData.value( name ).contains( Qt::CheckStateRole ) ) {
            mData[ name ].remove( Qt::CheckStateRole );
            
            if ( mData[ name ].isEmpty() ) {
                mData.remove( name );
            }
        }
    }
    
    emit dataChanged( index( 0, 0 ), index( rowCount() -1, columnCount() -1 ) );
}

QString pLocaleModel::localeDisplayText( const QString& name )
{
    const QLocale locale( name );
    const QString language = QLocale::languageToString( locale.language() );
    const QString country = QLocale::countryToString( locale.country() );
    return QString( "%1 (%2)" ).arg( language ).arg( country );
}

void pLocaleModel::populate()
{
    QSet<QString> names;
    QHash<QString, QSet<QString> > childNames;
    
    for ( int i = QLocale::C +1; i < QLocale::LastLanguage; i++ ) {
        const QLocale::Language language = QLocale::Language( i );
        
        if ( language == QLocale::C ) {
            continue;
        }
        
        foreach ( const QLocale::Country& country, QLocale::countriesForLanguage( language ) ) {
            const QLocale locale( language, country );
            const QString languageCode = locale.name().section( '_', 0, 0 );
            const QLocale localeParent( languageCode );
            const QString localeParentName = localeParent.name();
            
            names << localeParentName;
            
            if ( locale != localeParent ) {
                childNames[ localeParentName ] << locale.name();
            }
        }
    }
    
    mLocales = names.toList();
    qSort( mLocales.begin(), mLocales.end(), caseInsensitiveLessThanStringLocale );
    
    foreach ( const QString& name, childNames.keys() ) {
        QStringList locales = childNames[ name ].toList();
        qSort( locales.begin(), locales.end(), caseInsensitiveLessThanStringLocale );
        mChildrenLocales[ name ] = locales;
    }
}

void pLocaleModel::debug( const QModelIndex& root, int prof ) const
{
    static bool ok = false;
    
    if ( !ok ) {
        ok = true;
        
        qWarning() << mLocales << mLocales.count();
        foreach ( const QString& name, mLocales ) {
            qWarning() << qPrintable( QString( 0, QChar( '\t' ) ) ) << qPrintable( name );
            
            foreach ( const QString& subName, mChildrenLocales.value( name ) ) {
                qWarning() << qPrintable( QString( 1, QChar( '\t' ) ) ) << qPrintable( subName );
            }
        }
    }
    
    if ( root.isValid() ) {
        const QString indent = QString( prof, '\t' );
        qWarning() << qPrintable( indent ) << qPrintable( root.data().toString() ) << indexToLocale( root ) << root;
        
        prof++;
    }
    
    for ( int i = 0; i < rowCount( root ); i++ ) {
        const QModelIndex index = this->index( i, 0, root );
        Q_ASSERT( index.parent() == root );
        debug( index, prof );
    }
    
    if ( root.isValid() ) {
        prof--;
    }
    
    Q_ASSERT( prof < 2 );
}
