#ifndef POPENEDFILEEXPLORER_H
#define POPENEDFILEEXPLORER_H

#include "ui_pOpenedFileExplorer.h"
#include "pOpenedFileModel.h"

class pWorkspace;
class pAbstractChild;

class pOpenedFileExplorer : public pDockWidget, public Ui::pOpenedFileExplorer
{
	Q_OBJECT

public:
	pOpenedFileExplorer( pWorkspace* workspace );

protected:
	pWorkspace* mWorkspace;
	pOpenedFileModel* mModel;
	QMenu* mSortMenu;

protected slots:
	void sortTriggered( QAction* action );
	void documentChanged( pAbstractChild* document );
	void currentDocumentChanged( pAbstractChild* document );
	void sortModeChanged( pOpenedFileModel::SortMode mode );
	void selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	void on_tvFiles_customContextMenuRequested( const QPoint& pos );
};

#endif // POPENEDFILEEXPLORER_H
