#ifndef POPENEDFILEMODEL_H
#define POPENEDFILEMODEL_H

#include <QAbstractItemModel>
#include <QIcon>

class pWorkspace;
class pAbstractChild;

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
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	
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

protected slots:
	void documentOpened( pAbstractChild* document );
	void documentModifiedChanged( pAbstractChild* document, bool modified );
	void documentClosed( pAbstractChild* document );
};

#endif // POPENEDFILEMODEL_H
