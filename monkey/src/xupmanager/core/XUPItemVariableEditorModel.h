#ifndef XUPITEMVARIABLEEDITORMODEL_H
#define XUPITEMVARIABLEEDITORMODEL_H

#include "MonkeyExport.h"

#include <QAbstractItemModel>
#include <QHash>
#include <QStringList>

class XUPItem;
class DocumentFilterMap;

struct Q_MONKEY_EXPORT XUPItemVariableEditorModelItem {
    typedef QList<XUPItemVariableEditorModelItem> List;
    
    XUPItemVariableEditorModelItem( const QString& string = QString::null, bool toBeDeleted = false, XUPItem* item = 0 ) {
        this->string = string;
        this->toBeDeleted = toBeDeleted;
        this->item = item;
    }
    
    bool operator<( const XUPItemVariableEditorModelItem& other ) const {
        return string < other.string;
    }
    
    bool operator==( const XUPItemVariableEditorModelItem& other ) const {
        return string == other.string
            && toBeDeleted == other.toBeDeleted
            && item == other.item
            && children == other.children
            ;
    }
    
    QString string;
    bool toBeDeleted;
    XUPItem* item;
    XUPItemVariableEditorModelItem::List children;
};

class Q_MONKEY_EXPORT XUPItemVariableEditorModel : public QAbstractItemModel {
    Q_OBJECT

public:
    enum FilterMode {
        Out = 0,
        In
    };
    
    XUPItemVariableEditorModel( QObject* parent = 0 );
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    
    XUPItemVariableEditorModelItem* item( const QModelIndex& index ) const;
    QModelIndex childIndex( const QModelIndex& index, const QString& string ) const;
    
    void clear();
    void setRootItem( const XUPItemVariableEditorModelItem& item );
    void setRootItem( XUPItem* item );
    
    QModelIndex addVariable( const QString& variable );
    QModelIndex addValue( const QModelIndex& variable, const QString& value );
    
    QStringList fileVariables() const;
    QStringList pathVariables() const;
    QStringList knownVariables() const;
    
    void setFilterMode( XUPItemVariableEditorModel::FilterMode mode );
    XUPItemVariableEditorModel::FilterMode mode() const;
    
    void setFilteredVariables( const QStringList& filters );
    QStringList filteredVariables() const;
    
    void setFriendlyDisplayText( bool friendly );
    bool friendlyDisplayText() const;
    
    bool quoteValues() const;
    bool deleteRemovedFiles() const;

public slots:
    virtual void revert();
    virtual bool submit();
    
    void setQuoteValues( bool quote );
    void setDeleteRemovedFiles( bool del );

protected:
    XUPItemVariableEditorModel::FilterMode mMode;
    XUPItem* mRootItem;
    const DocumentFilterMap* mDocumentFilterMap;
    mutable XUPItemVariableEditorModelItem mRoot;
    QHash<XUPItemVariableEditorModelItem*, XUPItemVariableEditorModelItem*> mParentMapping;
    QStringList mFilteredVariables;
    bool mQuoteValues;
    bool mFriendlyDisplayText;
    bool mDeleteRemovedFiles;
    
    QString normalizedValue( const QString& value ) const;
    void buildParentMapping( XUPItemVariableEditorModelItem& item );
    void setRootItemInternal( const XUPItemVariableEditorModelItem& item, bool clearModel );
    void revert( XUPItem* item );
};

#endif // XUPITEMVARIABLEEDITORMODEL_H
