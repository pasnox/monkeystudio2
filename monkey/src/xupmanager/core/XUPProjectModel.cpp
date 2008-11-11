#include "XUPProjectModel.h"
#include "XUPProjectItem.h"

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
	if ( !index.isValid() )
	{
		return QVariant();
	}

	switch ( role )
	{
		case Qt::DecorationRole:
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
		{
			XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );

			QDomNode node = item->node();
			QStringList attributes;
			QDomNamedNodeMap attributeMap = node.attributes();
			
			if ( role == Qt::DecorationRole )
			{
				return item->displayIcon();
			}
			else if ( role == Qt::DisplayRole )
			{
				return item->displayText();
			}
			else if ( role == Qt::ToolTipRole )
			{
				for ( int i = 0; i < attributeMap.count(); i++ )
				{
					QDomNode attribute = attributeMap.item( i );
					attributes << attribute.nodeName() +"=\"" +attribute.nodeValue() +"\"";
				}
				
				return attributes.join( "\n" );
			}
		}
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

void XUPProjectModel::setLastError( const QString& error )
{
	mLastError = error;
}

QString XUPProjectModel::lastError() const
{
	return mLastError;
}

bool XUPProjectModel::open( const QString& fileName, const QString& encoding )
{
	XUPProjectItem* tmpProject = XUPProjectItem::projectInfos()->newProjectItem( fileName );
	if ( !tmpProject )
	{
		setLastError( tr( "No project handler for this project file" ) );
		return false;
	}
	
	if ( tmpProject->open( fileName, encoding ) )
	{
		setLastError( QString::null );
		mRootProject = tmpProject;
		mRootProject->mModel = this;
		return true;
	}
	
	setLastError( tr( "Can't open this project file" ) );
	delete tmpProject;
	return false;
}

void XUPProjectModel::close()
{
	if ( mRootProject )
	{
		setLastError( QString::null );
		delete mRootProject;
		mRootProject = 0;
	}
}
