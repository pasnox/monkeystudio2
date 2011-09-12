#include "xupmanager/core/XUPFilteredProjectModel.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/XUPDynamicFolderItem.h"
#include "xupmanager/core/XUPProjectItemHelper.h"

#include <QDebug>

void XUPFilteredProjectModel::debug( XUPItem* root, int mode )
{
    if ( mode == 0 )
    {
        static int prof = 0;
        QString prep = QString().fill( ' ', prof );
        qWarning("%s", root->displayText().prepend( prep ).toLocal8Bit().constData() );
        XUPItemMappingIterator it = mItemsMapping.constFind( root );
        Q_ASSERT( it != mItemsMapping.constEnd() );
        Q_ASSERT( it.value() );
        foreach ( XUPItem* item, it.value()->mMappedChildren )
        {
            prof += 4;
            debug( item );
            prof -= 4;
        }
    }
    else
    {
        foreach ( XUPItem* item, mItemsMapping.keys() )
        {
            qWarning() << "Mapped" << item->displayText();
            foreach ( XUPItem* it, mItemsMapping.constFind( item ).value()->mMappedChildren )
            {
                qWarning() << "\tChild" << it->displayText();
            }
        }
    }
}

bool xupItemLessThan( const XUPItem* left, const XUPItem* right )
{
    return left->operator<( *right );
}

void qSortItems( XUPItemList& items )
{
    qSort( items.begin(), items.end(), xupItemLessThan );
}

XUPFilteredProjectModel::XUPFilteredProjectModel( QObject* parent, XUPProjectModel* sourceModel )
    : QAbstractItemModel( parent )
{
    mSourceModel = 0;
    setSourceModel( sourceModel );
}

XUPFilteredProjectModel::~XUPFilteredProjectModel()
{
}

QModelIndex XUPFilteredProjectModel::index( int row, int column, const QModelIndex& parentProxy ) const
{
    XUPItem* item = mapToSource( parentProxy );
    XUPItemMappingIterator it = mItemsMapping.constFind( item );
    
    // try dinamic mapping
    /*{
        qWarning() << "hereeee";
        XUPItem* parentItem = mSourceModel->itemFromIndex( proxyIndex.parent() );
        
        if ( parentItem ) {
            createMapping( parentItem, parentItem->parent() );
            
            XUPItem* item = mSourceModel->itemFromIndex( proxyIndex );
            
            if ( item ) {
                createMapping( item, parentItem );
                return item;
            }
        }
    }*/
    
    if ( it == mItemsMapping.constEnd() )
    {
        if ( row == 0 && column == 0 && mSourceModel )
        {
            it = mItemsMapping.constFind( mSourceModel->mRootProject );
            
            if ( it != mItemsMapping.constEnd() )
            {
                return createIndex( row, column, *it );
            }
        }
    }
    else
    {
        XUPItem* item = it.value()->mMappedChildren.value( row );
        if ( item )
        {
            it = mItemsMapping.constFind( item );
            if ( it != mItemsMapping.constEnd() )
            {
                return createIndex( row, column, *it );
            }
        }
    }
    
    return QModelIndex();
}

QModelIndex XUPFilteredProjectModel::parent( const QModelIndex& proxyIndex ) const
{
    XUPItem* item = mapToSource( proxyIndex );
    XUPItemMappingIterator iterator = mItemsMapping.constFind( item );

    if ( iterator != mItemsMapping.constEnd() )
    {
        XUPItem* parentItem = iterator.value()->mParent;
        XUPItemMappingIterator parentItemIterator = mItemsMapping.constFind( parentItem );
        
        if ( parentItemIterator != mItemsMapping.constEnd() )
        {
            XUPItem* parentItemParent = parentItemIterator.value()->mParent;
            XUPItemMappingIterator parentItemParentIterator = mItemsMapping.constFind( parentItemParent );
            
            if ( parentItemParentIterator != mItemsMapping.constEnd() ) {
                return createIndex( parentItemParentIterator.value()->mMappedChildren.indexOf( parentItem ), 0, *parentItemIterator );
            }
            else {
                return createIndex( 0, 0, *parentItemIterator );
            }
        }
    }

    return QModelIndex();
}

int XUPFilteredProjectModel::rowCount( const QModelIndex& proxyParent ) const
{
    if ( mSourceModel )
    {
        XUPItem* item = mapToSource( proxyParent );
        XUPItemMappingIterator it = mItemsMapping.constFind( item );
        
        if ( it != mItemsMapping.constEnd() ) {
            switch ( item->type() ) {
                case XUPItem::DynamicFolder:
                case XUPItem::Folder:
                    return item->childCount();
                default:
                    return it.value()->mMappedChildren.count();
            }
        }
        
        return 1;
    }
    
    return 0;
}

int XUPFilteredProjectModel::columnCount( const QModelIndex& proxyParent ) const
{
    Q_UNUSED( proxyParent );
    return mSourceModel ? 1 : 0;
}

QVariant XUPFilteredProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    return mSourceModel ? mSourceModel->headerData( section, orientation, role ) : QVariant();
}

QVariant XUPFilteredProjectModel::data( const QModelIndex& proxyIndex, int role ) const
{
    if ( !proxyIndex.isValid() )
    {
        return QVariant();
    }
    
    XUPItem* item = mapToSource( proxyIndex );
    
    Q_ASSERT( item );
    
    return item->index().data( role );
}

Qt::ItemFlags XUPFilteredProjectModel::flags( const QModelIndex& proxyIndex ) const
{
    if ( !proxyIndex.isValid() )
    {
        return 0;
    }
    
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool XUPFilteredProjectModel::hasChildren( const QModelIndex& proxyParent ) const
{
    if ( mSourceModel )
    {
        XUPItem* item = mapToSource( proxyParent );
        XUPItemMappingIterator it = mItemsMapping.constFind( item );
        
        if ( it != mItemsMapping.constEnd() ) {
            switch ( item->type() ) {
                case XUPItem::DynamicFolder:
                case XUPItem::Folder:
                    const_cast<XUPFilteredProjectModel*>( this )->populateDynamicFolder( item );
                    return item->hasChildren();
                default:
                    return !it.value()->mMappedChildren.isEmpty();
            }
        }
        
        return 1;
    }
    
    return 0;
}

// MAPPING

XUPItemMappingIterator XUPFilteredProjectModel::indexToIterator( const QModelIndex& proxyIndex ) const
{
    Q_ASSERT( proxyIndex.isValid() );
    const void* p = proxyIndex.internalPointer();
    Q_ASSERT( p );
    XUPItemMappingIterator it = static_cast<const Mapping*>( p )->mIterator;
    Q_ASSERT( it != mItemsMapping.constEnd() );
    Q_ASSERT( it.value() );
    return it;
}

XUPItem* XUPFilteredProjectModel::mapToSource( const QModelIndex& proxyIndex ) const
{
    if ( proxyIndex.isValid() ) {
        XUPItemMappingIterator it = indexToIterator( proxyIndex );
        if ( it != mItemsMapping.constEnd() ) {
            return it.key();
        }
    }
    return 0;
}

QModelIndex XUPFilteredProjectModel::mapFromSource( XUPItem* sourceItem ) const
{
    XUPItemMappingIterator iterator = mItemsMapping.constFind( sourceItem );
    
    if ( iterator != mItemsMapping.constEnd() ) {
        XUPItem* parentItem = iterator.value()->mParent;
        XUPItemMappingIterator parentItemIterator = mItemsMapping.constFind( parentItem );
        
        if ( parentItemIterator != mItemsMapping.constEnd() ) {
            return createIndex( parentItemIterator.value()->mMappedChildren.indexOf( sourceItem ), 0, *iterator );
        }
        else {
            return createIndex( 0, 0, *iterator );
        }
    }
        
    return QModelIndex();
}

XUPItemMappingIterator XUPFilteredProjectModel::createMapping( XUPItem* item, XUPItem* parent ) const
{
    XUPItemMappingIterator it = mItemsMapping.constFind( item );
    if ( it != mItemsMapping.constEnd() ) // was mapped already
        return it;

    Mapping* m = new Mapping;
    it = XUPItemMappingIterator( mItemsMapping.insert( item, m ) );
    m->mParent = parent;
    m->mIterator = it;

    if ( item != mSourceModel->mRootProject )
    {
        Q_ASSERT( parent );
        XUPItemMappingIterator parentIt = createMapping( parent, parent->parent() );
        Q_ASSERT( parentIt != mItemsMapping.constEnd() );
        parentIt.value()->mMappedChildren << item;
    }
    else
    {
        m->mParent = 0;
    }

    Q_ASSERT( it != mItemsMapping.constEnd() );
    Q_ASSERT( it.value() );

    return it;
}

void XUPFilteredProjectModel::removeMapping( XUPItem* item )
{   
    if ( Mapping* m = mItemsMapping.take( item ) )
    {
        for ( int i = 0; i < m->mMappedChildren.size(); ++i )
        {
            removeMapping( m->mMappedChildren.at( i ) );
        }
        
        if ( item != mSourceModel->mRootProject )
        {
            XUPItemMappingIterator parentIt = mItemsMapping.constFind( m->mParent );

            if ( parentIt != mItemsMapping.constEnd() )
            {
                parentIt.value()->mMappedChildren.removeAll( item );
            }
        }
        
        delete m;
    }
}

void XUPFilteredProjectModel::clearMapping()
{
    qDeleteAll( mItemsMapping );
    mItemsMapping.clear();
}

void XUPFilteredProjectModel::setSourceModel( XUPProjectModel* model )
{
    if ( mSourceModel )
    {
        disconnect( mSourceModel, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( internal_rowsInserted( const QModelIndex&, int, int ) ) );
        disconnect( mSourceModel, SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( internal_rowsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
        disconnect( mSourceModel, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( internal_dataChanged( const QModelIndex&, const QModelIndex& ) ) );
    }
    
    mSourceModel = 0;
    
    clearMapping();
    reset();
    
    if ( model )
    {
        mSourceModel = model;
        
        connect( mSourceModel, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( internal_rowsInserted( const QModelIndex&, int, int ) ) );
        connect( mSourceModel, SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( internal_rowsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
        connect( mSourceModel, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( internal_dataChanged( const QModelIndex&, const QModelIndex& ) ) );
        
        // header
        beginInsertColumns( QModelIndex(), 0, 0 );
        endInsertColumns();
        
        // populate existing indexes
        if ( mSourceModel->hasChildren() ) {
            internal_rowsInserted( QModelIndex(), 0, mSourceModel->rowCount() -1 );
        }
    }
}

XUPProjectModel* XUPFilteredProjectModel::sourceModel() const
{
    return mSourceModel;
}

XUPItemList XUPFilteredProjectModel::getFilteredVariables( XUPItem* root )
{
    XUPItemList variables;
    XUPProjectItem* project = root->project();
    const QStringList filteredVariables = project->documentFilters().filteredVariables();
    
    for ( int i = 0; i < root->childCount(); i++ )
    {
        XUPItem* child = root->child( i );
        
        switch ( child->type() )
        {
            case XUPItem::Project:
                populateProject( child->project() );
                break;
            case XUPItem::Comment:
                break;
            case XUPItem::EmptyLine:
                break;
            case XUPItem::Variable:
            case XUPItem::DynamicFolder:
                if ( filteredVariables.contains( child->attribute( "name" ) ) )
                {
                    variables << child;
                }
                variables << getFilteredVariables( child );
                break;
            case XUPItem::Value:
                break;
            case XUPItem::Function:
                variables << getFilteredVariables( child );
                break;
            case XUPItem::Scope:
                variables << getFilteredVariables( child );
                break;
            default:
                break;
        }
    }
    
    return variables;
}

XUPItemList XUPFilteredProjectModel::getValues( XUPItem* root )
{
    XUPItemList values;
    for ( int i = 0; i < root->childCount(); i++ )
    {
        XUPItem* child = root->child( i );
        switch ( child->type() )
        {
            case XUPItem::Value:
            case XUPItem::File:
            case XUPItem::Path:
                values << child;
                break;
            case XUPItem::Folder:
            case XUPItem::DynamicFolder:
                values << getValues( child );
            default:
                break;
        }
    }
    return values;
}

void XUPFilteredProjectModel::populateDynamicFolder( XUPItem* folder )
{
    if ( !folder ) {
        return;
    }
    
    foreach ( XUPItem* child, folder->childrenList() ) {
        switch ( child->type() ) {
            case XUPItem::File:
            case XUPItem::Folder:
                createMapping( child, folder );
                continue;
            default:
                break;
        }
    }
    
    XUPItemMappingIterator parentIterator = createMapping( folder, folder->parent() );
    XUPItemList& childrenItems = parentIterator.value()->mMappedChildren;
    qSortItems( childrenItems );
}

void XUPFilteredProjectModel::populateVariable( XUPItem* variable )
{
    XUPProjectItem* project = variable->project();
    XUPItemMappingIterator projectIterator = mItemsMapping.constFind( project );
    
    if ( projectIterator == mItemsMapping.constEnd() )
    {
        return;
    }
    
    XUPItemList tmpValuesItem = getValues( variable );
    XUPItem* tmp = projectIterator.value()->findVariable( variable->attribute( "name" ) );
    
    if ( tmp )
    {
        variable = tmp;
    }
    
    XUPItemMappingIterator variableIterator = createMapping( variable, project );

    foreach ( XUPItem* value, tmpValuesItem )
    {
        if ( !value->content().isEmpty() && !variableIterator.value()->findValue( value ) )
        {
            createMapping( value, variable );
        }
    }
    
    XUPItemList& variableValues = variableIterator.value()->mMappedChildren;
    qSortItems( variableValues );
}

void XUPFilteredProjectModel::populateProject( XUPProjectItem* project )
{
    XUPItemMappingIterator projectIterator = createMapping( project, project->parentProject() );
    
    XUPItemList variables = getFilteredVariables( project );
    
    foreach ( XUPItem* variable, variables )
    {
        populateVariable( variable );
    }
    
    XUPItemList& projectVariables = projectIterator.value()->mMappedChildren;
    qSortItems( projectVariables );
    
    populateDynamicFolder( XUPProjectItemHelper::projectDynamicFolderItem( project, false ) );
}

void XUPFilteredProjectModel::internal_rowsInserted( const QModelIndex& parent, int start, int end )
{
    emit layoutAboutToBeChanged();
    
    XUPProjectItem* project = mSourceModel->mRootProject;
    const QStringList filteredVariables = project->documentFilters().filteredVariables();
    bool hasDynamicFolderItems = false;
    
    for ( int i = start; i < end +1; i++ ) {
        QModelIndex childIndex = mSourceModel->index( i, 0, parent );
        XUPItem* item = static_cast<XUPItem*>( childIndex.internalPointer() );
        XUPItem* parentItem = item->parent();
        
        // special case for dynamic folder
        if ( parentItem && ( parentItem->type() == XUPItem::DynamicFolder || parentItem->type() == XUPItem::Folder ) ) {
            switch ( item->type() ) {
                case XUPItem::DynamicFolder:
                case XUPItem::File:
                case XUPItem::Folder: {
                    hasDynamicFolderItems = true;
                    createMapping( item, parentItem );
                    continue;
                }
                default:
                    break;
            }
        }
        
        // normal case
        switch ( item->type() ) {
            case XUPItem::Project: {
                populateProject( item->project() );
                break;
            }
            case XUPItem::Variable: {
                if ( filteredVariables.contains( item->attribute( "name" ) ) ) {
                    populateVariable( item );
                }
                break;
            }
            case XUPItem::Value:
            case XUPItem::File:
            case XUPItem::Path: {
                if ( parentItem->type() == XUPItem::Variable
                    && filteredVariables.contains( parentItem->attribute( "name" ) ) ) {
                    populateVariable( parentItem );
                }
                
                break;
            }
            case XUPItem::Scope:
            case XUPItem::Function:
            default:
                break;
        }
    }
    
    if ( hasDynamicFolderItems ) {
        XUPItem* parentItem = static_cast<XUPItem*>( parent.internalPointer() );
        XUPItemMappingIterator parentIterator = createMapping( parentItem, parentItem->parent() );
        XUPItemList& childrenItems = parentIterator.value()->mMappedChildren;
        qSortItems( childrenItems );
    }
    
    emit layoutChanged();
}

void XUPFilteredProjectModel::recursiveRemoveItems( XUPItem* item )
{
    XUPItemMappingIterator itemIt = mItemsMapping.constFind( item );
    
    if ( itemIt == mItemsMapping.constEnd() )
    {
        for ( int i = item->childCount() -1; i > -1; i-- )
        {
            recursiveRemoveItems( item->child( i ) );
        }
    }
    else
    {
        XUPItem* parentItem = itemIt.value()->mParent;
        XUPItemMappingIterator parentIt = mItemsMapping.constFind( parentItem );
    
        if ( parentIt != mItemsMapping.constEnd() )
        {
            QModelIndex parentProxy = mapFromSource( parentItem );
            QModelIndex indexProxy = mapFromSource( item );
            int indexRow = indexProxy.row();
            
            beginRemoveRows( parentProxy, indexRow, indexRow );
            removeMapping( item );
            endRemoveRows();
        }
    }
}

void XUPFilteredProjectModel::internal_rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end )
{
    for ( int i = start; i < end +1; i++ )
    {
        XUPItem* item = static_cast<XUPItem*>( mSourceModel->index( start, 0, parent ).internalPointer() );
        recursiveRemoveItems( item );
    }
}

void XUPFilteredProjectModel::internal_dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    Q_UNUSED( bottomRight );
    
    emit layoutAboutToBeChanged();
    
    XUPProjectItem* project = static_cast<XUPItem*>( topLeft.internalPointer() )->project();
    populateProject( project );
    
    emit layoutChanged();
}
