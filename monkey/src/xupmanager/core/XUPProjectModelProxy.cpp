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
#include "XUPProjectModelProxy.h"
#include "xupmanager/core/XUPProjectModel.h"
#include "XUPItem.h"

XUPProjectModelProxy::XUPProjectModelProxy( QObject* parent, bool showDisabled )
    : QSortFilterProxyModel( parent )
{
    mShowDisabled = showDisabled;
    mSourceModel = 0;
}

void XUPProjectModelProxy::setSourceModel( QAbstractItemModel* sourceModel )
{
    mSourceModel = qobject_cast<XUPProjectModel*>( sourceModel );
    QSortFilterProxyModel::setSourceModel( mSourceModel );
}

Qt::ItemFlags XUPProjectModelProxy::flags( const QModelIndex& index ) const
{
    if ( !mSourceModel || !index.isValid() )
    {
        return 0;
    }
    
    QModelIndex idx = mapToSource( index );
    XUPItem* item = mSourceModel->itemFromIndex( idx );
    
    bool enabled = false;
    
    if ( item->type() == XUPItem::Project || item->type() == XUPItem::Scope )
    {
        enabled = true;
    }
    else if ( item->type() == XUPItem::Function && item->attribute( "name" ).toLower() != "include" )
    {
        enabled = true;
    }
    
    return enabled ? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::ItemFlags();
}

bool XUPProjectModelProxy::filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const
{
    const QModelIndex sourceIndex = mSourceModel->index( sourceRow, 0, sourceParent );
    XUPItem* item = mSourceModel->itemFromIndex( sourceIndex );
    
    bool isEnabled = false;
    
    if ( item->type() == XUPItem::Project || item->type() == XUPItem::Scope )
    {
        isEnabled = true;
    }
    else if ( item->type() == XUPItem::Function && item->attribute( "name" ).toLower() != "include" )
    {
        isEnabled = true;
    }
    
    return isEnabled ? true : mShowDisabled;
}

bool XUPProjectModelProxy::isShowDisabled() const
{
    return mShowDisabled;
}

void XUPProjectModelProxy::setShowDisabled( bool showDisabled )
{
    if ( mShowDisabled != showDisabled )
    {
        mShowDisabled = showDisabled;
        reset();
    }
}
