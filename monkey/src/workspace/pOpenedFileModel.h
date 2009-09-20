#ifndef POPENEDFILEMODEL_H
#define POPENEDFILEMODEL_H

#include <QAbstractItemModel>
#include <QIcon>

#include "pAbstractChild.h"

class pWorkspace;

class pOpenedFileModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	enum SortMode
	{
		OpeningOrder,
		FileName,
		URL,
		Suffixes,
		Custom
	};
	
	pOpenedFileModel( pWorkspace* workspace );
	virtual ~pOpenedFileModel();
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual QStringList mimeTypes() const;
	virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;
	virtual bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent );
	virtual Qt::DropActions supportedDropActions() const;
	
	pAbstractChild* document( const QModelIndex& index ) const;
	QModelIndex index( pAbstractChild* document ) const;
	
	pOpenedFileModel::SortMode sortMode() const;
	void setSortMode( pOpenedFileModel::SortMode mode );

protected:
	pWorkspace* mWorkspace;
	pOpenedFileModel::SortMode mSortMode;
	QList<pAbstractChild*> mDocuments;
	QIcon mTransparentIcon;
	QIcon mModifiedIcon;
	
	void sortDocuments();
	void insertDocument( pAbstractChild* document, int index );

protected slots:
	void documentOpened( pAbstractChild* document );
	void documentModifiedChanged( pAbstractChild* document, bool modified );
	void documentClosed( pAbstractChild* document );

signals:
	void documentMoved( pAbstractChild* document );
	void documentMoved( const QModelIndex& index );
	void sortModeChanged( pOpenedFileModel::SortMode mode );
};

#endif // POPENEDFILEMODEL_H
