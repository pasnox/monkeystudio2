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
#ifndef XUPPROJECTMODEL_H
#define XUPPROJECTMODEL_H

#include "MonkeyExport.h"

#include <QAbstractItemModel>

class XUPProjectItem;
class XUPItem;

class Q_MONKEY_EXPORT XUPProjectModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class XUPFilteredProjectModel;
    friend class XUPProjectManager;
    friend class XUPItem;
    friend class XUPDynamicFolderItem;
    
public:
    enum CustomRole {
        TypeRole = Qt::UserRole
    };
    
    XUPProjectModel( QObject* parent = 0 );
    virtual ~XUPProjectModel();

    // QAbstractItemModel reimplementation
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    
    //
    QModelIndex indexFromItem( XUPItem* item ) const;
    XUPItem* itemFromIndex( const QModelIndex& index ) const;
    XUPProjectItem* rootProject() const;
    
    // error handler
    void showError( const QString& error );
    
    // XUP Project members
    virtual bool open( const QString& fileName, const QString& codec );
    virtual void close();

protected:
    XUPProjectItem* mRootProject;
    
    void handleProject( XUPProjectItem* project );
};

Q_DECLARE_METATYPE( XUPProjectModel* )

#endif // XUPPROJECTMODEL_H
