/********************************************************************************************************
 * PROGRAM      : monkey2
 * DATE - TIME  : 
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : UIMain.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef UIMAIN_H
#define UIMAIN_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "pMainWindow.h"

class QActionGroup;
class pFileManager;
class pWorkspace;
class PluginsManager;
class UIProjectsManager;
class pConsoleManager;

class Q_MONKEY_EXPORT UIMain : public pMainWindow, public QSingleton<UIMain>
{
	Q_OBJECT
	friend class QSingleton<UIMain>;

public:
	QMenu* createPopupMenu();
	PluginsManager* pluginsManager();
	pFileManager* fileManager();
	pWorkspace* workspace();
	UIProjectsManager* projectsManager();
	pConsoleManager* consoleManager();

protected:
	void closeEvent( QCloseEvent* );

protected slots:
	void menu_aboutToShow();

private:
	UIMain( QWidget* = 0 );
	void initMenuBar();
	void initToolBar();
	void initConnections();
	void initGui();

	QActionGroup* agStyles;

};

#endif // UIMAIN_H
