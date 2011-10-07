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
#ifndef XUPDYNAMICFOLDERITEM_H
#define XUPDYNAMICFOLDERITEM_H

#include "MonkeyExport.h"

#include <QObject>

#include "XUPItem.h"

class FileSystemModel;

class Q_MONKEY_EXPORT XUPDynamicFolderItem : public QObject, public XUPItem
{
    Q_OBJECT
    friend class XUPDynamicFolderChildItem;
    
public:
    XUPDynamicFolderItem( const QDomElement& node, XUPItem* parent );
    virtual ~XUPDynamicFolderItem();
    
    virtual int childCount() const;
    virtual bool hasChildren() const;
    virtual int childIndex( XUPItem* child ) const;
    virtual XUPItem* child( int i );
    virtual XUPItemList childrenList() const;
    virtual QString displayText() const;
    virtual QIcon displayIcon() const;
    
    void setRootPath( const QString& path, const QStringList& filters );

protected:
    FileSystemModel* mFSModel;
    QPersistentModelIndex mFSRootIndex;
    QHash<QPersistentModelIndex, XUPItem*> mFSItems; // fs persistent index, item

protected slots:
    void columnsAboutToBeInserted( const QModelIndex& parent, int start, int end );
    void columnsAboutToBeMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationColumn );
    void columnsAboutToBeRemoved( const QModelIndex& parent, int start, int end );
    void columnsInserted( const QModelIndex& parent, int start, int end );
    void columnsMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationColumn );
    void columnsRemoved( const QModelIndex& parent, int start, int end );
    void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    void headerDataChanged( Qt::Orientation orientation, int first, int last );
    void layoutAboutToBeChanged();
    void layoutChanged();
    void modelAboutToBeReset();
    void modelReset();
    void rowsAboutToBeInserted( const QModelIndex& parent, int start, int end );
    void rowsAboutToBeMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow );
    void rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end );
    void rowsInserted( const QModelIndex& parent, int start, int end );
    void rowsMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow );
    void rowsRemoved( const QModelIndex& parent, int start, int end );
    void rootPathChanged( const QString& newPath );
    void directoryLoaded( const QString& path );
};

#endif // XUPDYNAMICFOLDERITEM_H
