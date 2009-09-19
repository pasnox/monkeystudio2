#ifndef POPENEDFILEEXPLORER_H
#define POPENEDFILEEXPLORER_H

#include "ui_pOpenedFileExplorer.h"

class pWorkspace;
class pOpenedFileModel;
class pAbstractChild;

class pOpenedFileExplorer : public pDockWidget, public Ui::pOpenedFileExplorer
{
	Q_OBJECT

public:
	pOpenedFileExplorer( pWorkspace* workspace );
	virtual ~pOpenedFileExplorer();

protected:
	pWorkspace* mWorkspace;
	pOpenedFileModel* mModel;
	QMenu* mSortMenu;

protected slots:
	void sortTriggered( QAction* action );
	void documentChanged( pAbstractChild* document );
	void currentDocumentChanged( pAbstractChild* document );
	void selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
};

#endif // POPENEDFILEEXPLORER_H
