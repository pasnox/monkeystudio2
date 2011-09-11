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
