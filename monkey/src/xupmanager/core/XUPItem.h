#ifndef XUPITEM_H
#define XUPITEM_H

#include "MonkeyExport.h"

#include <QDomElement>
#include <QMap>
#include <QIcon>
#include <QVariant>
#include <QModelIndex>

class XUPProjectItem;
class XUPProjectModel;

typedef QList<class XUPItem*> XUPItemList;

class Q_MONKEY_EXPORT XUPItem
{
    friend class XUPProjectModel;
    friend class XUPProjectItem;
    friend class XUPDynamicFolderItem;
    
public:
    // possible types for a node
    enum Type {
        Unknow = -1,
        Project, // a project node
        Comment, // a comment node
        EmptyLine, // a empty line node
        Variable, // a variabel node
        Value, // a value node
        Function, // a function node
        Scope, // a scope node
        //
        DynamicFolder, // a dynamic folder node
        Folder, // a folder node
        File, // a value that is a file node
        Path // a value that is a path node
    };
    
    // dtor
    virtual ~XUPItem();
    
    // the < operator for sorting items in tree
    virtual bool sameTypeLess( const XUPItem& other ) const;
    virtual bool operator<( const XUPItem& other ) const;
    
    // project item
    XUPProjectItem* project() const;
    // return the i child item
    virtual XUPItem* child( int i );
    // return children list
    virtual XUPItemList childrenList() const;
    // index of a child
    virtual int childIndex( XUPItem* child ) const;
    // set a child item for row i
    void addChild( XUPItem* item );
    // return the parent item
    XUPItem* parent() const;
    // return the item row. If item hasn't parent -1 will be return
    int row() const;
    // return child count
    virtual int childCount() const;
    virtual bool hasChildren() const;
    // remove a child and inform the model if possible
    void removeChild( XUPItem* item );
    // create a new child of type at given row, if row is -1 the item is append to the end
    XUPItem* addChild( XUPItem::Type type, int row = -1 );
    // return the model associated with the item or null if item is not yet in a model
    XUPProjectModel* model() const;
    // return the QModelIndex of the item if it's in a model, else an invalid qmodelindex
    QModelIndex index() const;
    
    // the type enum of this item
    virtual XUPItem::Type type() const;

    // return the content of attribute name or defaultValue if null/invalid
    virtual QString attribute( const QString& name, const QString& defaultValue = QString::null ) const;
    // set the attribute value for name
    void setAttribute( const QString& name, const QString& value );
    
    // view text, the text to shown in the item view
    virtual QString displayText() const;
    // view icon, the icon to shown in the item view
    virtual QIcon displayIcon() const;
    
    virtual QString content() const;
    void setContent( const QString& content );
    
    // return the stored cache value for key or defaultValue
    QString cacheValue( const QString& key, const QString& defaultValue = QString::null ) const;
    // set the cache value for key
    void setCacheValue( const QString& key, const QString& value );
    // clear cache data represented by key
    void clearCacheValue( const QString& key );
    // mostly for debugging purpose
    QString xmlContent() const;
    // emit dataChanged signal to model
    void emitDataChanged();

protected:
    XUPProjectModel* mModel;
    QDomElement mDomElement;
    mutable QMap<int, XUPItem*> mChildItems;
    XUPItem* mParentItem;
    QMap<QString, QString> mCacheValues;
    
    // developer must not be able to create/instanciate items itself, it must be done by the model
    XUPItem( const QDomElement& node, XUPItem* parent = 0 );
    // add a new domnode child, this can be use to insert a new node, the item will be created on demand
    virtual QDomElement addChildElement( XUPItem::Type type, int& row, bool emitSignals = true );
    // set the parent item. Call automaticaly from parent's addChild
    void setParent( XUPItem* parentItem );

    // return the node element associate with this item
    QDomElement node() const;
};

Q_DECLARE_METATYPE( XUPItem* );

#endif // XUPITEM_H
