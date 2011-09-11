#include "xupmanager/core/XUPProjectModel.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "coremanager/MonkeyCore.h"

#include <pQueuedMessageToolBar.h>

#include <QDebug>

XUPProjectModel::XUPProjectModel( QObject* parent )
	: QAbstractItemModel( parent )
{
	mRootProject = 0;
}

XUPProjectModel::~XUPProjectModel()
{
	close();
}

QModelIndex XUPProjectModel::indexFromItem( XUPItem* item ) const
{
	if ( !item )
	{
		return QModelIndex();
	}
	
	int column = 0;
	int row = item->parent() ? item->parent()->childIndex( item ) : 0;
	
	return createIndex( row, column, item );
}

XUPItem* XUPProjectModel::itemFromIndex( const QModelIndex& index ) const
{
	if ( index.isValid() )
	{
		return static_cast<XUPItem*>( index.internalPointer() );
	}
	
	return 0;
}

XUPProjectItem* XUPProjectModel::rootProject() const
{
	return mRootProject;
}

QModelIndex XUPProjectModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( !hasIndex( row, column, parent ) )
	{
		return QModelIndex();
	}
	
	if ( !parent.isValid() )
	{
		if ( row == 0 && mRootProject )
		{
			return createIndex( row, column, static_cast<XUPItem*>( mRootProject ) );
		}
	}
	else
	{
		XUPItem* parentItem = static_cast<XUPItem*>( parent.internalPointer() );
		XUPItem* childItem = parentItem->child( row );
		
		if ( childItem )
		{
			return createIndex( row, column, childItem );
		}
	}
	
	return QModelIndex();
}

QModelIndex XUPProjectModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return QModelIndex();
	}

	XUPItem* childItem = static_cast<XUPItem*>( index.internalPointer() );
	XUPItem* parentItem = childItem->XUPItem::parent();

	if ( !parentItem || childItem == mRootProject )
	{
		return QModelIndex();
	}

	return createIndex( parentItem->row(), 0, parentItem );
}

int XUPProjectModel::rowCount( const QModelIndex& parent ) const
{
	if ( parent.column() > 0 )
	{
		return 0;
	}

	if ( !parent.isValid() )
	{
		return mRootProject ? 1 : 0;
	}

	XUPItem* parentItem = static_cast<XUPItem*>( parent.internalPointer() );
	return parentItem->childCount();
}

int XUPProjectModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return mRootProject ? 1 : 0;
}

QVariant XUPProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( orientation == Qt::Horizontal && section == 0 )
	{
		if ( mRootProject )
		{
			if ( role == Qt::DecorationRole )
			{
				return mRootProject->displayIcon();
			}
			else if ( role == Qt::DisplayRole )
			{
				return mRootProject->displayText();
			}
		}
	}
	
	return QVariant();
}

QVariant XUPProjectModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() ) {
		return QVariant();
	}
	
	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );

	switch ( role ) {
		case Qt::DecorationRole:
			return item->displayIcon();
		case Qt::DisplayRole:
			return item->displayText();
		case XUPProjectModel::TypeRole:
			return item->type();
		case Qt::ToolTipRole:
		{
			const QDomNode node = item->node();
			const QDomNamedNodeMap attributeMap = node.attributes();
			QStringList attributes;
			
			if ( item->type() == XUPItem::Project ) {
				attributes << QString( "Project: %1" ).arg( item->project()->fileName() );
			}
			
			for ( int i = 0; i < attributeMap.count(); i++ ) {
				const QDomNode attribute = attributeMap.item( i );
				const QString name = attribute.nodeName();
				const QString value = attribute.nodeValue();
				
				attributes << QString( "%1=\"%2\"" ).arg( name ).arg( value );
			}
			
			switch ( item->type() ) {
				case XUPItem::Value:
					attributes << QString( "Value=\"%1\"" ).arg( item->content() );
					break;
				case XUPItem::File:
					attributes << QString( "File=\"%1\"" ).arg( item->content() );
					break;
				case XUPItem::Path:
					attributes << QString( "Path=\"%1\"" ).arg( item->content() );
					break;
				default:
					break;
			}
			
			return attributes.join( "\n" );
		}
		case Qt::SizeHintRole:
			return QSize( -1, 18 );
		default:
			break;
	}
	
	return QVariant();
}

Qt::ItemFlags XUPProjectModel::flags( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return 0;
	}
	
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool XUPProjectModel::hasChildren( const QModelIndex& parent ) const
{
	if ( parent.column() > 0 )
	{
		return 0;
	}

	if ( !parent.isValid() )
	{
		return mRootProject ? 1 : 0;
	}

	XUPItem* parentItem = static_cast<XUPItem*>( parent.internalPointer() );
	return parentItem->hasChildren();
}

void XUPProjectModel::showError( const QString& error )
{
	if ( !error.trimmed().isEmpty() ) {
		MonkeyCore::messageManager()->appendMessage( error );
	}
}

void XUPProjectModel::handleProject( XUPProjectItem* project )
{
	mRootProject = project;
	mRootProject->mModel = this;
}

bool XUPProjectModel::open( const QString& fileName, const QString& codec )
{
	XUPProjectItem* tmpProject = MonkeyCore::projectTypesIndex()->newProjectItem( fileName );
	
	if ( !tmpProject )
	{
		showError( tr( "No project handler for this project file '%1'" ).arg( fileName ) );
		return false;
	}
	
	if ( tmpProject->open( fileName, codec ) )
	{
		handleProject( tmpProject );
		return true;
	}
	
	showError( tr( "Can't open this project file '%1'" ).arg( fileName ) );
	delete tmpProject;
	return false;
}

void XUPProjectModel::close()
{
	if ( mRootProject )
	{
		delete mRootProject;
		mRootProject = 0;
	}
}
