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
#ifndef XUPFILTEREDPROJECTMODEL_H
#define XUPFILTEREDPROJECTMODEL_H

#include "MonkeyExport.h"
#include "xupmanager/core/XUPProjectModel.h"
#include "XUPItem.h"

#include <QMap>

typedef QMap<XUPItem*, class Mapping*> XUPItemMapping;
typedef XUPItemMapping::const_iterator XUPItemMappingIterator;

struct Q_MONKEY_EXPORT Mapping
{
    Mapping()
    {
        mParent = 0;
    }
    
    XUPItem* mParent;
    XUPItemList mMappedChildren;
    XUPItemMappingIterator mIterator;
    
    XUPItem* findVariable( const QString& name ) const
    {
        foreach ( XUPItem* item, mMappedChildren )
        {
            if ( ( item->type() == XUPItem::Variable ) &&
                item->attribute( "name" ) == name )
            {
                return item;
            }
        }
        return 0;
    }
    
    XUPItem* findValue( XUPItem* item ) const
    {
        foreach ( XUPItem* child, mMappedChildren )
        {
            switch ( child->type() )
            {
                case XUPItem::Value:
                case XUPItem::File:
                case XUPItem::Path:
                    if ( child->content() == item->content() )
                    {
                        return child;
                    }
                    break;
                case XUPItem::Folder:
                    if ( child->attribute( "name" ) == item->attribute( "name" ) )
                    {
                        return child;
                    }
                    break;
                default:
                    break;
            }
        }
        return 0;
    }
};

class Q_MONKEY_EXPORT XUPFilteredProjectModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    XUPFilteredProjectModel( QObject* parent = 0, XUPProjectModel* sourceModel = 0 );
    virtual ~XUPFilteredProjectModel();

    // QAbstractItemModel reimplementation
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    
    XUPItemMappingIterator indexToIterator( const QModelIndex& proxyIndex ) const;
    XUPItem* mapToSource( const QModelIndex& proxyIndex ) const;
    QModelIndex mapFromSource( XUPItem* sourceItem ) const;
    
    void setSourceModel( XUPProjectModel* model );
    XUPProjectModel* sourceModel() const;
    
    XUPItemList getFilteredVariables( XUPItem* root );
    XUPItemList getValues( XUPItem* root );
    
    void populateDynamicFolder( XUPItem* folder );
    void populateVariable( XUPItem* variable );
    void populateProject( XUPProjectItem* item );
    
    void debug( XUPItem* root, int mode = 0 );

protected:
    XUPProjectModel* mSourceModel;
    mutable XUPItemMapping mItemsMapping;
    
    XUPItemMappingIterator createMapping( XUPItem* item, XUPItem* parent = 0 ) const;
    void removeMapping( XUPItem* item );
    void clearMapping();
    void recursiveRemoveItems( XUPItem* item );

protected slots:
    void internal_rowsInserted( const QModelIndex& parent, int start, int end );
    void internal_rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end );
    void internal_dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
};

#endif // XUPFILTEREDPROJECTMODEL_H
