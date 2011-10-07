/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef CommandsEDITORMODEL_H
#define CommandsEDITORMODEL_H

#include "MonkeyExport.h"
#include "xupmanager/core/XUPProjectItemHelper.h"

#include <QAbstractItemModel>

class pMenuBar;

class Q_MONKEY_EXPORT CommandsEditorModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    CommandsEditorModel( QObject* parent = 0 );
    virtual ~CommandsEditorModel();
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    QModelIndex index( const QString& menu ) const;
    QModelIndex index( const pCommand& command, const QString& menu ) const;
    
    QString menu( const QModelIndex& index ) const;
    pCommand command( const QModelIndex& index ) const;
    
    bool isEmpty() const;
    void clear();
    
    void setCommands( const MenuCommandListMap& commands, pMenuBar* menuBar );
    MenuCommandListMap commands() const;
    
    QModelIndex addCommand( const QModelIndex& menuIndex, const pCommand& command );
    void swapCommand( const QModelIndex& menuIndex, int fromCommand, int toCommand );

public slots:
    virtual void revert();
    virtual bool submit();

protected:
    pMenuBar* mMenuBar;
    QHash<QString, int> mMenusOrder;
    MenuCommandListMap mCommands;
    
    void addCommands( const QString& menu, const pCommand::List& commands );
};

#endif // CommandsEDITORMODEL_H
