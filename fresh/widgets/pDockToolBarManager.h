/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pDockToolBarManager.h
 * LICENSE      : GPL
 * COMMENTARY   : This class manage pDockToolBar
 ********************************************************************************************************/
#ifndef PDOCKTOOLBARMANAGER_H
#define PDOCKTOOLBARMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QHash>
#include <QBoxLayout>

class QMainWindow;
class pDockToolBar;
class QDockWidget;

class Q_MONKEY_EXPORT pDockToolBarManager : public QObject, public QSingleton<pDockToolBarManager>
{
	Q_OBJECT
	friend class QSingleton<pDockToolBarManager>;

public:
	QMainWindow* mainWindow() const;
	pDockToolBar* bar( Qt::ToolBarArea );

	static Qt::ToolBarArea dockWidgetAreaToToolBarArea( Qt::DockWidgetArea );
	static Qt::DockWidgetArea toolBarAreaToDockWidgetArea( Qt::ToolBarArea );
	static QBoxLayout::Direction toolBarAreaToBoxLayoutDirection( Qt::ToolBarArea );

private:
	QMainWindow* mMain;
	QHash<Qt::ToolBarArea, pDockToolBar*> mBars;

	pDockToolBarManager( QMainWindow* );

public slots:
	void dockWidgetAreaChanged( QDockWidget*, pDockToolBar* );
	virtual void restoreState( pDockToolBar* = 0 );
	virtual void saveState( pDockToolBar* = 0 );

};

#endif // PDOCKTOOLBARMANAGER_H
