#ifndef SEARCHRESULTSDOCK_H
#define SEARCHRESULTSDOCK_H

#include <pDockWidget.h>

#include <QModelIndex>

class SearchThread;
class QHBoxLayout;
class SearchResultsModel;
class QTreeView;

class SearchResultsDock : public pDockWidget
{
	Q_OBJECT
	
public:
	SearchResultsDock( SearchThread* searchThread, QWidget* parent = 0 );

protected:
	SearchThread* mSearchThread;
	QHBoxLayout* mLayout;
	SearchResultsModel* mModel;
	QTreeView* mView;

protected slots:
	void view_activated( const QModelIndex& index );
};

#endif // SEARCHRESULTSDOCK_H
