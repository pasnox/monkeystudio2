#ifndef XUPPROJECTMODEL_H
#define XUPPROJECTMODEL_H

#include "MonkeyExport.h"

#include <QAbstractItemModel>

class XUPProjectItem;
class XUPItem;

class Q_MONKEY_EXPORT XUPProjectModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class XUPFilteredProjectModel;
	friend class XUPProjectManager;
	friend class XUPItem;
	friend class XUPDynamicFolderItem;
	
public:
	enum CustomRole {
		TypeRole = Qt::UserRole
	};
	
	XUPProjectModel( QObject* parent = 0 );
	virtual ~XUPProjectModel();

	// QAbstractItemModel reimplementation
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	
	//
	QModelIndex indexFromItem( XUPItem* item ) const;
	XUPItem* itemFromIndex( const QModelIndex& index ) const;
	XUPProjectItem* rootProject() const;
	
	// error handler
	void showError( const QString& error );
	
	// XUP Project members
	virtual bool open( const QString& fileName, const QString& codec );
	virtual void close();

protected:
	XUPProjectItem* mRootProject;
	
	void handleProject( XUPProjectItem* project );
};

Q_DECLARE_METATYPE( XUPProjectModel* )

#endif // XUPPROJECTMODEL_H
