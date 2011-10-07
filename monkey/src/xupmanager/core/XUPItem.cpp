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
#include "XUPItem.h"
#include "xupmanager/core/XUPDynamicFolderItem.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/XUPProjectModel.h"
#include "pIconManager.h"

#include <QDebug>

XUPItem::XUPItem( const QDomElement& node, XUPItem* parent )
{
    mModel = 0;
    mDomElement = node;
    mParentItem = parent;
    
    // FIX old format projects
    if ( !mDomElement.attribute( "content" ).isEmpty() ) {
        setContent( mDomElement.attribute( "content" ) );
        mDomElement.removeAttribute( "content" );
    }
}

XUPItem::~XUPItem()
{
    qDeleteAll( mChildItems );
    mChildItems.clear();
}

bool XUPItem::sameTypeLess( const XUPItem& other ) const
{   
    switch ( other.type() ) {
        case XUPItem::Variable: {
            const XUPProjectItem* project = this->project();
            const DocumentFilterMap& filters = project->documentFilters();
            const QString left = attribute( "name" );
            const QString right = other.attribute( "name" );
            const int leftWeight = filters.value( left ).weight;
            const int rightWeight = filters.value( right ).weight;
            
            return leftWeight != rightWeight
                ? leftWeight < rightWeight
                : left.toLower() < right.toLower()
                ;
        }
        case XUPItem::Comment:
            return row() < other.row();
        case XUPItem::EmptyLine:
            return attribute( "count" ).toInt() < other.attribute( "count" ).toInt();
        case XUPItem::Project:
        case XUPItem::Value:
        case XUPItem::Function:
        case XUPItem::Scope:
        case XUPItem::Folder:
        case XUPItem::File:
        case XUPItem::Path:
        default:
            break;
    }
    
    return displayText().toLower() < other.displayText().toLower();
}

bool XUPItem::operator<( const XUPItem& other ) const
{
    if ( type() == other.type() ) {
        return sameTypeLess( other );
    }
    else {
        switch ( type() ) {
            case XUPItem::Project:
                return false;
            default:
                return other.type() != XUPItem::Folder;
        }
    }

    return displayText().toLower() < other.displayText().toLower();
}

QDomElement XUPItem::node() const
{
    return mDomElement;
}

XUPProjectItem* XUPItem::project() const
{
    if ( type() == XUPItem::Project ) {
        return static_cast<XUPProjectItem*>( const_cast<XUPItem*>( this ) );
    }
    else {
        return mParentItem ? mParentItem->project() : 0;
    }
}

XUPItem* XUPItem::parent() const
{
    return mParentItem;
}

void XUPItem::setParent( XUPItem* parentItem )
{
    mParentItem = parentItem;
}

XUPItem* XUPItem::child( int i )
{
    if ( mChildItems.contains( i ) ) {
        return mChildItems[ i ];
    }

    if ( i >= 0 && i < mDomElement.childNodes().count() ) {
        QDomElement childElement = mDomElement.childNodes().item( i ).toElement();
        XUPItem* childItem = childElement.tagName().toLower() == "dynamicfolder"
            ? new XUPDynamicFolderItem( childElement, const_cast<XUPItem*>( this ) )
            : new XUPItem( childElement, const_cast<XUPItem*>( this ) );
        mChildItems[ i ] = childItem;
        return childItem;
    }
    
    return 0;
}

XUPItemList XUPItem::childrenList() const
{
    // create all child if needed before returning list
    for ( int i = 0; i < childCount(); i++ ) {
        const_cast<XUPItem*>( this )->child( i );
    }
    
    // return children
    return mChildItems.values();
}

int XUPItem::childIndex( XUPItem* child ) const
{
    return mChildItems.key( child, -1 );
}

void XUPItem::addChild( XUPItem* item )
{
    int row = childCount();
    XUPProjectModel* m = model();
    
    // inform begin insert
    if ( m ) {
        m->beginInsertRows( index(), row, row );
    }
    
    mChildItems[ row ] = item;
    item->setParent( this );
    
    // inform end insert
    if ( m ) {
        m->endInsertRows();
    }
}

int XUPItem::row() const
{
    if ( mParentItem ) {
        return mParentItem->childIndex( const_cast<XUPItem*>( this ) );
    }
    else {
        return 0;
    }
}

int XUPItem::childCount() const
{
    int count = mDomElement.childNodes().count();
    
    // include/sub project are not node children, so have to take them in account
    if ( !mChildItems.isEmpty() ) {
        count = qMax( count, mChildItems.keys().last() +1 );
    }
    
    switch ( type() ) {
        case XUPItem::Comment:
        case XUPItem::EmptyLine:
        case XUPItem::Path:
        case XUPItem::File:
        case XUPItem::Value:
            return 0;
        default:
            return count;
    }
}

bool XUPItem::hasChildren() const
{
    return childCount() > 0;
}

void XUPItem::removeChild( XUPItem* item )
{
    int id = childIndex( item );
    
    if ( id != -1 ) {
        // inform model of remove
        XUPProjectModel* m = model();
        
        if ( m ) {
            // begin remove
            m->beginRemoveRows( index(), id, id );
            
            // remove
            bool isDirectChild = item->mDomElement.parentNode() == mDomElement;
            
            if ( isDirectChild ) {
                foreach ( const int& key, mChildItems.keys() ) {
                    if ( key == id ) {
                        QDomNode node = item->mDomElement;
                        mDomElement.removeChild( node );
                        mChildItems.remove( key );
                        delete item;
                    }
                    else if ( key > id ) {
                        mChildItems[ key -1 ] = mChildItems[ key ];
                        mChildItems.remove( key );
                    }
                }
            }
            else {
                delete mChildItems.take( id );
            }
            
            // end remove
            m->endRemoveRows();
        }
        else {
            delete mChildItems.take( id );
        }
    }
}

QDomElement XUPItem::addChildElement( XUPItem::Type type, int& row, bool emitSignals )
{
    // calculate row if needed
    if ( row == -1 ) {
        row = mDomElement.childNodes().count();
    }
    
    QString stringType;
    
    switch ( type ) {
        case XUPItem::Project:
            stringType = "project";
            break;
        case XUPItem::Comment:
            stringType = "comment";
            break;
        case XUPItem::EmptyLine:
            stringType = "emptyline";
            break;
        case XUPItem::Variable:
            stringType = "variable";
            break;
        case XUPItem::Value:
            stringType = "value";
            break;
        case XUPItem::Function:
            stringType = "function";
            break;
        case XUPItem::Scope:
            stringType = "scope";
            break;
        case XUPItem::Folder:
            stringType = "folder";
            break;
        case XUPItem::File:
            stringType = "file";
            break;
        case XUPItem::Path:
            stringType = "path";
            break;
        case XUPItem::DynamicFolder:
            stringType = "dynamicfolder";
            break;
        case XUPItem::Unknow:
            break;
    }
    
    // inform model of add
    XUPProjectModel* m = model();
    
    const int count = mDomElement.childNodes().count();
    
    if ( !stringType.isEmpty() && row <= count ) {
        // begin insert
        if ( m && emitSignals ) {
            m->beginInsertRows( index(), row, row );
        }
        
        // re inde existing items
        QList<int> rows = mChildItems.keys();
        qSort( rows.begin(), rows.end(), qGreater<int>() );
        
        foreach ( const int& key, rows ) {
            if ( key >= row ) {
                mChildItems[ key +1 ] = mChildItems.take( key );
            }
        }
        
        // add new one
        //mChildItems.remove( row );
        QDomElement element = mDomElement.ownerDocument().createElement( stringType );
        
        if ( count == 0 ) {
            mDomElement.appendChild( element );
        }
        else {
            if ( row == 0 ) {
                const QDomElement childElement = mDomElement.childNodes().at( 1 ).toElement();
                mDomElement.insertBefore( element, childElement );
            }
            else {
                const QDomElement childElement = mDomElement.childNodes().at( row -1 ).toElement();
                mDomElement.insertAfter( element, childElement );
            }
        }
        
        // end insert
        if ( m && emitSignals ) {
            m->endInsertRows();
        }
        
        return element;
    }
    
    return QDomElement();
}

XUPItem* XUPItem::addChild( XUPItem::Type type, int row )
{
    const QDomElement element = addChildElement( type, row );
    return element.isNull() ? 0 : child( row );
}

XUPProjectModel* XUPItem::model() const
{
    return mParentItem ? mParentItem->model() : mModel;
}

QModelIndex XUPItem::index() const
{
    XUPProjectModel* m = model();
    return m ? m->indexFromItem( const_cast<XUPItem*>( this ) ) : QModelIndex();
}

XUPItem::Type XUPItem::type() const
{
    const QString mType = mDomElement.nodeName().toLower();
    
    if ( mType == "project" )
        return XUPItem::Project;
    else if ( mType == "comment" )
        return XUPItem::Comment;
    else if ( mType == "emptyline" )
        return XUPItem::EmptyLine;
    else if ( mType == "variable" )
        return XUPItem::Variable;
    else if ( mType == "value" )
        return XUPItem::Value;
    else if ( mType == "function" )
        return XUPItem::Function;
    else if ( mType == "scope" )
        return XUPItem::Scope;
    else if ( mType == "folder" )
        return XUPItem::Folder;
    else if ( mType == "file" )
        return XUPItem::File;
    else if ( mType == "path" )
        return XUPItem::Path;
    else if ( mType == "dynamicfolder" )
        return XUPItem::DynamicFolder;
    return XUPItem::Unknow;
}

QString XUPItem::displayText() const
{
    switch ( type() ) {
        case XUPItem::Project:
            return attribute( "name" );
        case XUPItem::Comment:
            return  attribute( "value" );
        case XUPItem::EmptyLine:
            return QObject::tr( "%1 empty line(s)" ).arg( attribute( "count" ) );
        case XUPItem::Variable:
            return project()->documentFilters().variableDisplayText( attribute( "name" ) );
        case XUPItem::Value:
            return content();
        case XUPItem::Function:
            return QString( "%1( %2 )" ).arg( attribute( "name" ) ).arg( attribute( "parameters" ) );
        case XUPItem::Scope:
            return attribute( "name" );
        case XUPItem::Folder:
            return QFileInfo( attribute( "name" ) ).fileName().remove( "'" ).remove( '"' );
        case XUPItem::File:
            return QFileInfo( content() ).fileName().remove( "'" ).remove( '"' );
        case XUPItem::Path:
            return content();
        case XUPItem::DynamicFolder:
            return QObject::tr( "Dynamic Folder" );
        case XUPItem::Unknow:
            break;
    }
    
    return "#Unknow";
}

QIcon XUPItem::displayIcon() const
{
    if ( type() == XUPItem::Variable ) {
        return pIconManager::icon( project()->documentFilters().variableDisplayIcon( attribute( "name" ) ) );
    }
    
    const QString iconFileName = QString( "%1.png" ).arg( mDomElement.nodeName() );
    QString iconFilePath = pIconManager::filePath( iconFileName, project()->documentFilters().iconsPath() );
    
    if ( iconFilePath.isEmpty() ) {
        iconFilePath = pIconManager::filePath( iconFileName, project()->documentFilters().defaultIconsPath() );
    }
    
    return pIconManager::icon( iconFilePath );
}

QString XUPItem::content() const
{
    return mDomElement.text();
}

void XUPItem::setContent( const QString& content )
{
    QDomText textNode = mDomElement.firstChild().toText();
    
    if ( textNode.isNull() ) {
        textNode = mDomElement.ownerDocument().createTextNode( content );
        mDomElement.appendChild( textNode );
    }
    else {
        textNode.setData( content );
    }
    
    emitDataChanged();
}

QString XUPItem::attribute( const QString& name, const QString& defaultValue ) const
{
    return name == "content" ? content() : mDomElement.attribute( name, defaultValue );
}

void XUPItem::setAttribute( const QString& name, const QString& value )
{
    if ( mDomElement.attribute( name ) == value ) {
        return;
    }
    
    if ( name == "content" ) {
        setContent( value );
    }
    else {
        mDomElement.setAttribute( name, value );
    }
    
    mCacheValues.remove( name );
    emitDataChanged();
}

QString XUPItem::cacheValue( const QString& key, const QString& defaultValue ) const
{
    return mCacheValues.value( key, attribute( key, defaultValue ) );
}

void XUPItem::setCacheValue( const QString& key, const QString& value )
{
    mCacheValues[ key ] = value;
}

void XUPItem::clearCacheValue( const QString& key )
{
    mCacheValues.remove( key );
}

QString XUPItem::xmlContent() const
{
    QString content;
    QTextStream stream( &content );
    stream << mDomElement;
    return content;
}

void XUPItem::emitDataChanged()
{
    // update model if needed
    XUPProjectModel* model = this->model();
    
    if ( model ) {
        const QModelIndex index = this->index();
        emit model->dataChanged( index, index );
    }
}
