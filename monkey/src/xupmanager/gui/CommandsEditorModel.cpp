#include "CommandsEditorModel.h"
#include "consolemanager/pCommand.h"

#include <pMenuBar.h>

#include <QApplication>
#include <QStyle>
#include <QDebug>

#define CommandsEditorModelColumnCount 1

CommandsEditorModel::CommandsEditorModel( QObject* parent )
    : QAbstractItemModel( parent )
{
    mMenuBar = 0;
}

CommandsEditorModel::~CommandsEditorModel()
{
}

int CommandsEditorModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent );
    return CommandsEditorModelColumnCount;
}

QVariant CommandsEditorModel::data( const QModelIndex& index, int role ) const
{
    const bool isMenu = index.parent() == QModelIndex();
    const QString menu = this->menu( index );
    const pCommand command = this->command( index );
    const bool toBeDeleted = command.userData() == Qt::Checked;
    
    switch ( role ) {
        case Qt::CheckStateRole:
            return isMenu ? QVariant() : ( command.userData().isNull() ? Qt::Unchecked : command.userData().toInt() );
        case Qt::DecorationRole: {
            const QIcon icon = isMenu ? ( mMenuBar && !menu.isEmpty() ? mMenuBar->menu( menu )->icon() : QIcon() ) : QIcon();
            return icon.isNull() ? QVariant() : icon;
        }
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return isMenu ? ( mMenuBar && !menu.isEmpty() ? mMenuBar->menu( menu )->title() : QVariant() ) : command.text();
        case Qt::ForegroundRole:
            return !isMenu && toBeDeleted ? QApplication::palette().brush( QPalette::Disabled, QPalette::WindowText ) : QVariant();
        case Qt::FontRole: {
            QFont font;
            font.setBold( isMenu );
            font.setStrikeOut( !isMenu && toBeDeleted );
            return font;
        }
        case Qt::EditRole:
            return QVariant::fromValue( command );
        case Qt::SizeHintRole: {
            const int height = QApplication::style()->pixelMetric( QStyle::PM_IndicatorHeight );
            return QSize( -1, height );
        }
    }
    
    return QVariant();
}

QModelIndex CommandsEditorModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( row < 0 || column < 0 || column >= CommandsEditorModelColumnCount ) {
        return QModelIndex();
    }
    
    if ( parent == QModelIndex() ) {
        return row < mCommands.count() ? createIndex( row, column, -1 ) : QModelIndex();
    }
    
    const QString menu = this->menu( parent );
    
    if ( menu.isEmpty() || row >= mCommands.value( menu ).count() ) {
        return QModelIndex();
    }
    
    return createIndex( row, column, mMenusOrder[ menu ] );
}

QModelIndex CommandsEditorModel::parent( const QModelIndex& index ) const
{
    return index.internalId() == -1 ? QModelIndex() : this->index( mMenusOrder.key( index.internalId() ) );
}

int CommandsEditorModel::rowCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? mCommands.count() : mCommands.value( menu( parent ) ).count();
}

Qt::ItemFlags CommandsEditorModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    
    if ( index.isValid() && index.parent().isValid() ) {
        flags |= Qt::ItemIsUserCheckable;
    }
    
    return flags;
}

bool CommandsEditorModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if ( !index.isValid() || index.parent() == QModelIndex() ) {
        return false;
    }
    
    const QString menu = this->menu( index.parent() );
    
    if ( menu.isEmpty() ) {
        return false;
    }
    
    switch ( role ) {
        case Qt::CheckStateRole:
            mCommands[ menu ][ index.row() ].setUserData( value );
            break;
        case Qt::EditRole:
            mCommands[ menu ][ index.row() ] = value.value<pCommand>();
            break;
        default:
            return false;
    }
    
    emit dataChanged( index, index );
    return true;
}

QModelIndex CommandsEditorModel::index( const QString& menu ) const
{
    if ( !mCommands.contains( menu ) ) {
        return QModelIndex();
    }
    
    return index( mMenusOrder[ menu ], 0 );
}

QModelIndex CommandsEditorModel::index( const pCommand& command, const QString& menu ) const
{
    const int row = mCommands.value( menu ).indexOf( command );
    return row != -1 ? index( row, 0, index( menu ) ) : QModelIndex();
}

QString CommandsEditorModel::menu( const QModelIndex& index ) const
{
    return index.parent() == QModelIndex() ? mMenusOrder.key( index.row() ) : QString::null;
}

pCommand CommandsEditorModel::command( const QModelIndex& index ) const
{
    const QString menu = this->menu( index.parent() );
    const int count = mCommands.value( menu ).count();
    return menu.isEmpty() || index.row() >= count ? pCommand() : mCommands[ menu ][ index.row() ];
}

bool CommandsEditorModel::isEmpty() const
{
    foreach ( const QString& menu, mCommands.keys() ) {
        if ( !mCommands[ menu ].isEmpty() ) {
            return false;
        }
    }
    
    return true;
}

void CommandsEditorModel::clear()
{
    if ( isEmpty() ) {
        return;
    }
    
    emit layoutAboutToBeChanged();
    
    const QModelIndexList oldIndexes = persistentIndexList();
    QModelIndexList newIndexes;
    
    foreach ( const QModelIndex& index, oldIndexes ) {
        if ( index.parent() != QModelIndex() ) {
            newIndexes << QModelIndex();
        }
        else {
            newIndexes << index;
        }
    }
    
    foreach ( const QString& menu, mCommands.keys() ) {
        mCommands[ menu ].clear();
    }
    
    changePersistentIndexList( oldIndexes, newIndexes );
    
    emit layoutChanged();
}
    
void CommandsEditorModel::setCommands( const MenuCommandListMap& commands, pMenuBar* menuBar )
{
    clear();
    
    mMenuBar = menuBar;
    
    if ( mCommands.isEmpty() ) {
        const QStringList menus = mMenuBar ? mMenuBar->rootMenusPath() : QStringList();
        const int count = menus.count();
        
        if ( count > 0 ) {
            beginInsertRows( QModelIndex(), 0, count -1 );
            int i = 0;
            foreach ( const QString& menu, menus ) {
                mCommands[ menu ] = pCommand::List();
                mMenusOrder[ menu ] = i;
                i++;
            }
            endInsertRows();
        }
    }
    else {
        emit dataChanged( index( 0, 0 ), index( rowCount() -1, columnCount() -1 ) );
    }
    
    foreach ( const QString& menu, commands.keys() ) {
        addCommands( menu, commands[ menu ] );
    }
}

MenuCommandListMap CommandsEditorModel::commands() const
{
    return mCommands;
}

QModelIndex CommandsEditorModel::addCommand( const QModelIndex& menuIndex, const pCommand& command )
{
    if ( !menuIndex.isValid() ) {
        return QModelIndex();
    }
    
    const int count = rowCount( menuIndex );
    
    beginInsertRows( menuIndex, count, count );
    mCommands[ menu( menuIndex ) ] << command;
    mCommands[ menu( menuIndex ) ].last().setUserData( Qt::Unchecked );
    endInsertRows();
    
    return index( count, 0, menuIndex );
}

void CommandsEditorModel::swapCommand( const QModelIndex& menuIndex, int fromCommand, int toCommand )
{
    const QString menu = this->menu( menuIndex );
    
    if ( menu.isEmpty() || fromCommand < 0 || toCommand < 0 || fromCommand == toCommand ) {
        return;
    }
    
    pCommand::List& commands = mCommands[ menu ];
    const int count = commands.count();
    
    if ( fromCommand >= count || toCommand >= count ) {
        return;
    }
    
    emit layoutAboutToBeChanged();
    
    const QModelIndex from = index( fromCommand, 0, menuIndex );
    const QModelIndex to = index( toCommand, 0, menuIndex );
    const QModelIndexList oldIndexes = persistentIndexList();
    QModelIndexList newIndexes = persistentIndexList();
    const int fromIndex = oldIndexes.indexOf( from );
    const int toIndex = oldIndexes.indexOf( to );
    
    if ( fromIndex != -1 ) {
        newIndexes[ fromIndex ] = to;
    }
    
    if ( toIndex != -1 ) {
        newIndexes[ toIndex ] = from;
    }
    
    commands.swap( fromCommand, toCommand );
    
    changePersistentIndexList( oldIndexes, newIndexes );
    
    emit layoutChanged();
}

void CommandsEditorModel::revert()
{
    QAbstractItemModel::revert();
}

bool CommandsEditorModel::submit()
{
    const bool senderIsItemSelectionModel = sender() ? sender()->inherits( "QItemSelectionModel" ) : false;
    
    if ( senderIsItemSelectionModel ) {
        return false;
    }
    
    emit layoutAboutToBeChanged();
    
    foreach ( const QString& menu, mCommands.keys() ) {
        pCommand::List& commands = mCommands[ menu ];
        
        for ( int i = commands.count() -1; i >= 0; i-- ) {
            pCommand& command = commands[ i ];
            
            if ( command.userData().toInt() == Qt::Checked ) {
                commands.removeAt( i );
            }
        }
    }
    
    emit layoutChanged();
    
    return true;
}

void CommandsEditorModel::addCommands( const QString& menu, const pCommand::List& commands )
{
    if ( commands.isEmpty() ) {
        return;
    }
    
    const int count = commands.count();
    
    beginInsertRows( index( menu ), 0, count -1 );
    mCommands[ menu ] = commands;
    endInsertRows();
}
