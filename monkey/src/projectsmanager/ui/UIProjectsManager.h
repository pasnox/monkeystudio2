/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIProjectsManager.h
** Date      : 2007-11-04T22:45:50
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
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
#include "ProjectItem.h"

#include <QDockWidget>

class ProjectsProxy;
class ProjectsModel;

class Q_MONKEY_EXPORT UIProjectsManager : public QDockWidget, public Ui::UIProjectsManager, public QSingleton<UIProjectsManager>
{
	Q_OBJECT
	friend class QSingleton<UIProjectsManager>;
	
public:
	ProjectItem* currentItem() const;
	ProjectItem* currentProject() const;
	void setCurrentProject( ProjectItem* );
	ProjectItemList rootProjects() const;

	inline ProjectsModel* model() const { return mProjects; }
	inline ProjectsProxy* proxy() const { return mProxy; }

private:
	ProjectsModel* mProjects;
	ProjectsProxy* mProxy;

	UIProjectsManager( QWidget* = 0 );

	void initializeProject( ProjectItem* );

private slots:
	void cbProjects_activated( const QModelIndex& );
	void tvProjects_currentChanged( const QModelIndex&, const QModelIndex& );
	void on_tvProjects_doubleClicked( const QModelIndex& );

	void internal_aboutToClose();
	void internal_closed();
	void internal_modifiedChanged( bool );

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

signals:
	void aboutToClose( ProjectItem* );
	void closed( ProjectItem* );
	void modifiedChanged( ProjectItem*, bool );
	void currentChanged( ProjectItem* );
	void opened( ProjectItem* );

};

#endif // UIPROJECTSMANAGER_H
