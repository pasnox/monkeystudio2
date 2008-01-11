/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pMainWindow.h
 * LICENSE      : GPL
 * COMMENTARY   : This class allow to track shortcut and write them
 ********************************************************************************************************/
#ifndef PMAINWINDOW_H
#define PMAINWINDOW_H

#include "MonkeyExport.h"

#include <QMainWindow>

class pMenuBar;
class pDockToolBarManager;
class pDockToolBar;
class pSettings;

class Q_MONKEY_EXPORT pMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	pMainWindow( QWidget* = 0, Qt::WindowFlags = 0 );

	pMenuBar* menuBar();
	pDockToolBarManager* dockToolBarManager();
	pDockToolBar* dockToolBar( Qt::ToolBarArea );

	void setSettings( pSettings* );
	pSettings* settings();

protected:
	pSettings* mSettings;

	void hideEvent( QHideEvent* );

public slots:
	virtual void saveState();
	virtual void restoreState();

};

#endif // PMAINWINDOW_H
