#ifndef SEARCHRESULTSDOCK_H
#define SEARCHRESULTSDOCK_H

#include <pDockWidget.h>

class SearchThread;
class QHBoxLayout;
class SearchResultsModel;
class QTreeView;

class SearchResultsDock : public pDockWidget
{
	Q_OBJECT
	
public:
	SearchResultsDock( SearchThread* searchThread, QWidget* parent = 0 );
	virtual ~SearchResultsDock();

protected:
	QHBoxLayout* mLayout;
	SearchResultsModel* mModel;
	QTreeView* mView;
};

#endif // SEARCHRESULTSDOCK_H
