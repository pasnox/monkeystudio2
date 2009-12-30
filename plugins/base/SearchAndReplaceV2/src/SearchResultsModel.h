#ifndef SEARCHRESULTSMODEL_H
#define SEARCHRESULTSMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>

#include "SearchWidget.h"

class SearchThread;

class SearchResultsModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	struct Result
	{
		Result( const QString& _fileName = QString::null, const QString& _capture = QString::null, int _position = -1, int _line = -1, bool _checkable = false, Qt::CheckState _checkState = Qt::Unchecked )
		{
			fileName = _fileName;
			capture = _capture;
			position = _position;
			line = _line;
			checkable = _checkable;
			checkState = _checkState;
			row = -1;
		}
		
		bool operator==( const SearchResultsModel::Result& other ) const
		{
			return fileName == other.fileName &&
				capture == other.capture &&
				position == other.position &&
				line == other.line &&
				checkable == other.checkable/* &&
				checked == other.checked*/ &&
				row == other.row;
		}
		
		QString fileName;
		QString capture;
		int position;
		int line;
		bool checkable;
		Qt::CheckState checkState;
		int row; // internally used by the model for parent indexes, must not be modified.
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

protected:
	int mRowCount;
	QHash<QString, SearchResultsModel::Result*> mParents; // fileName, result
	QHash<int, SearchResultsModel::Result*> mParentsRows; // row, result
	QList<SearchResultsModel::ResultList> mResults;
	SearchThread* mSearchThread;

protected slots:
	void thread_reset();
	void thread_resultsAvailable( const QString& fileName, const SearchResultsModel::ResultList& results );
};

#endif // SEARCHRESULTSMODEL_H
