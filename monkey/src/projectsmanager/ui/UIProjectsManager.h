/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UIProjectsManager
** FileName  : UIProjectsManager.h
** Date      : 2007-09-03T20:14:20
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIPROJECTSMANAGER_H
#define UIPROJECTSMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UIProjectsManager.h"
#include "ProjectsModel.h"

#include <QDockWidget>

class Q_MONKEY_EXPORT UIProjectsManager : public QDockWidget, public Ui::UIProjectsManager, public QSingleton<UIProjectsManager>
{
	Q_OBJECT
	friend class QSingleton<UIProjectsManager>;

private:
	ProjectsModel* mProjects;

	UIProjectsManager( QWidget* = 0 );
	~UIProjectsManager();

public slots:
	void openProject( const QString& );
	void projectNew_triggered();
	void projectOpen_triggered();
	void projectSaveCurrent_triggered();
	void projectSaveAll_triggered();
	void projectCloseCurrent_triggered();
	void projectCloseAll_triggered();
	void projectSettings_triggered();

};

#endif // UIPROJECTSMANAGER_H
