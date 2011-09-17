#include "pCheckComboBox.h"
#include "pGenericTableModel.h"

#include <QEvent>
#include <QHelpEvent>
#include <QListView>
#include <QStandardItemModel>
#include <QStylePainter>
#include <QToolTip>
#include <QWhatsThis>
#include <QDebug>

// pCheckComboBoxDelegate

pCheckComboBoxDelegate::pCheckComboBoxDelegate( QObject* parent, QComboBox* cmb )
    : QStyledItemDelegate( parent ), mCombo( cmb )
{
}

bool pCheckComboBoxDelegate::isSeparator( const QModelIndex& index )
{
    return index.data( Qt::AccessibleDescriptionRole ).toString() == QLatin1String( "separator" );
}

void pCheckComboBoxDelegate::setSeparator( QAbstractItemModel* model, const QModelIndex& index, bool set )
{
    model->setData( index, set ? QLatin1String( "separator" ) : QVariant(), Qt::AccessibleDescriptionRole );
    
    if ( QStandardItemModel* m = qobject_cast<QStandardItemModel*>( model ) ) {
        if ( QStandardItem* item = m->itemFromIndex( index ) ) {
            if ( set ) {
                item->setFlags( item->flags() & ~( Qt::ItemIsSelectable | Qt::ItemIsEnabled ) );
            }
            else {
                item->setFlags( item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            }
        }
    }
    
    if ( pGenericTableModel* m = qobject_cast<pGenericTableModel*>( model ) ) {
        Qt::ItemFlags flags = m->data( index, pGenericTableModel::ItemFlagsRole ).value<Qt::ItemFlags>();
        
        if ( set ) {
            flags = flags & ~( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        }
        else {
            flags = flags | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        
        m->setData( index, QVariant::fromValue( flags ), pGenericTableModel::ItemFlagsRole );
    }
}

void pCheckComboBoxDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    if ( isSeparator( index ) ) {
        QRect rect = option.rect;
        
        if ( const QStyleOptionViewItemV3* v3 = qstyleoption_cast<const QStyleOptionViewItemV3*>( &option ) ) {
            if ( const QAbstractItemView* view = qobject_cast<const QAbstractItemView*>( v3->widget ) ) {
                rect.setWidth( view->viewport()->width() );
            }
        }
        
        QStyleOption opt;
        opt.rect = rect;
        mCombo->style()->drawPrimitive( QStyle::PE_IndicatorToolBarSeparator, &opt, painter, mCombo );
    }
    else {
        QStyledItemDelegate::paint( painter, option, index );
    }
}

QSize pCheckComboBoxDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    if ( isSeparator( index ) ) {
        int pm = mCombo->style()->pixelMetric( QStyle::PM_DefaultFrameWidth, 0, mCombo );
        return QSize( pm, pm );
    }
    
    return QStyledItemDelegate::sizeHint( option, index );
}

// pCheckComboBox

pCheckComboBox::pCheckComboBox( QWidget* parent )
    : QComboBox( parent )
{
    mModel = new pGenericTableModel( this );
    mModel->setColumnCount( 1 );
    
    mView = new QListView( this );
    mView->setUniformItemSizes( true );
    mView->setAlternatingRowColors( false );
    
    mDelegate = new pCheckComboBoxDelegate( this, this );
    
    setView( mView );
    setModel( mModel );
    setItemDelegate( mDelegate );
    
    // some styles force alternating...
    mView->setStyleSheet( QString(
        "QListView {"
            "background-color: %1;"
            "alternate-background-color: %1;"
        "}"
        ).arg( palette().color( QPalette::Base ).name() )
    );
}

pCheckComboBox::~pCheckComboBox()
{
}

void pCheckComboBox::showPopup()
{
    const Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    
    for ( int i = 0; i < model()->rowCount( rootModelIndex() ); i++ ) {
        const QModelIndex index = modelIndex( i );
        
        if ( isSeparator( index.row() ) ) {
            continue;
        }
        
        model()->setData( index, QSize( 0, 21 ), Qt::SizeHintRole );
        
        if ( QStandardItemModel* m = qobject_cast<QStandardItemModel*>( model() ) ) {
            if ( QStandardItem* item = m->itemFromIndex( index ) ) {
                item->setFlags( flags );
            }
        }
        
        if ( model()->inherits( "pGenericTableModel" ) ) {
            model()->setData( index, QVariant::fromValue( flags ),  pGenericTableModel::ItemFlagsRole );
        }
        
        if ( index.data( Qt::CheckStateRole ).isNull() ) {
            model()->setData( index, Qt::Unchecked, Qt::CheckStateRole );
        }
        
    }
    
    QComboBox::showPopup();
}

void pCheckComboBox::insertSeparator( int index )
{
    const int itemCount = count();
    
    index = qBound( 0, index, itemCount );
    
    if ( index >= maxCount() ) {
        return;
    }
    
    insertItem( index, QIcon(), QString::null );
    pCheckComboBoxDelegate::setSeparator( model(), modelIndex( index ), true );
}

void pCheckComboBox::addSeparator()
{
    insertSeparator( maxCount() );
}

bool pCheckComboBox::isSeparator( int index )
{
    return mDelegate->isSeparator( modelIndex( index ) );
}

void pCheckComboBox::setSeparator( int index, bool set )
{
    mDelegate->setSeparator( model(), modelIndex( index ), set );
}

QStringList pCheckComboBox::checkedStringList() const
{
    QStringList strings;
    
    if ( model() ) {
        for ( int i = 0; i < model()->rowCount( rootModelIndex() ); i++ ) {
            const QModelIndex index = model()->index( i, modelColumn(), rootModelIndex() );
            const QVariant value = index.data( Qt::CheckStateRole );
            const Qt::CheckState state = value.isNull() ? Qt::Unchecked : Qt::CheckState( value.toInt() );
            
            if ( state == Qt::Checked ) {
                strings << index.data().toString();
            }
        }
    }
    
    return strings;
}

void pCheckComboBox::retranslateUi()
{
    // do your custom retranslate here
}

QModelIndex pCheckComboBox::modelIndex( int index ) const
{
    return model()->index( index, modelColumn(), rootModelIndex() );
}

QString pCheckComboBox::text() const
{
    return checkedStringList().join( ", " );
}

bool pCheckComboBox::event( QEvent* event )
{
    switch ( event->type() ) {
        case QEvent::ToolTip:
            QToolTip::showText( static_cast<QHelpEvent*>( event )->globalPos(), text(), this );
            return true;
        case QEvent::WhatsThis:
            QWhatsThis::showText( static_cast<QHelpEvent*>( event )->globalPos(), text(), this );
            return true;
        default:
            return QComboBox::event( event );
    }
}

void pCheckComboBox::changeEvent( QEvent* event )
{
    QComboBox::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}

void pCheckComboBox::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );
    
    QStylePainter painter( this );
    QStyleOptionComboBox option;
    
    initStyleOption( &option );
    option.currentIcon = QIcon();
    option.currentText = text();
    
    painter.setPen( palette().color( QPalette::Text ) );
    painter.drawComplexControl( QStyle::CC_ComboBox, option );
    painter.drawControl( QStyle::CE_ComboBoxLabel, option );
}
