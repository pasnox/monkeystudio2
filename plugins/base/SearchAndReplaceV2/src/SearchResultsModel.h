#ifndef SEARCHRESULTSMODEL_H
#define SEARCHRESULTSMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>

#include "SearchWidget.h"

class SearchThread;

class SearchResultsModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class SearchWidget;
	
public:
	struct Result
	{
		Result( const QString& _fileName = QString::null, const QString& _capture = QString::null, const QPoint& _position = QPoint(), int _offset = -1, bool _checkable = false, Qt::CheckState _checkState = Qt::Unchecked )
		{
			fileName = _fileName;
			capture = _capture;
			position = _position;
			offset = _offset;
			checkable = _checkable;
			checkState = _checkState;
		}
		
		bool operator==( const SearchResultsModel::Result& other ) const
		{
			return fileName == other.fileName &&
				capture == other.capture &&
				position == other.position &&
				offset == other.offset &&
				checkable == other.checkable/* &&
				checked == other.checked*/;
		}
		
		QString fileName;
		QString capture;
		QPoint position;
		int offset;
		bool checkable;
		Qt::CheckState checkState;
	};
	
	typedef QList<SearchResultsModel::Result*> ResultList;
	
	SearchResultsModel( SearchThread* searchThread, QObject* parent = 0 );
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
	
	const QList<SearchResultsModel::ResultList>& results() const;

protected:
	int mRowCount;
	mutable QHash<QString, SearchResultsModel::Result*> mParents; // fileName, result
	mutable QList<SearchResultsModel::Result*> mParentsList; // ordered parents
	mutable QList<SearchResultsModel::ResultList> mResults; // parents children
	SearchThread* mSearchThread;

protected slots:
	void thread_reset();
	void thread_resultsAvailable( const QString& fileName, const SearchResultsModel::ResultList& results );
	void thread_resultsHandled( const QString& fileName, const SearchResultsModel::ResultList& results );
};

#endif // SEARCHRESULTSMODEL_H
