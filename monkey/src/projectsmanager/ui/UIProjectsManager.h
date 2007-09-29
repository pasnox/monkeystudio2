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

#include <QDockWidget>

class ProjectItem;
class ProjectsProxy;
class ProjectsModel;

class Q_MONKEY_EXPORT UIProjectsManager : public QDockWidget, public Ui::UIProjectsManager, public QSingleton<UIProjectsManager>
{
	Q_OBJECT
	friend class QSingleton<UIProjectsManager>;
	
public:
	ProjectItem* currentProject() const;

	inline ProjectsModel* model() const { return mProjects; }
	inline ProjectsProxy* proxy() const { return mProxy; }

private:
	ProjectsModel* mProjects;
	ProjectsProxy* mProxy;

	UIProjectsManager( QWidget* = 0 );
	~UIProjectsManager();

	void initializeProject( ProjectItem* );

private slots:
	void cbProjects_activated( const QModelIndex& );
	void tvProjects_currentChanged( const QModelIndex&, const QModelIndex& );
	void on_tvProjects_doubleClicked( const QModelIndex& );

public slots:
	bool openProject( const QString& );
	bool closeProject( const QString& );
	void projectNew_triggered();
	void projectOpen_triggered();
	void projectSaveCurrent_triggered();
	void projectSaveAll_triggered();
	void projectCloseCurrent_triggered();
	void projectCloseAll_triggered();
	void projectSettings_triggered();
	void projectAddExistingFiles_triggered();

};

#endif // UIPROJECTSMANAGER_H
