/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIMain.h
** Date      : 2007-11-04T22:45:21
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
