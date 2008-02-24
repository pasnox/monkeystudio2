#ifndef UIPROJECTSMANAGER_H
#define UIPROJECTSMANAGER_H

#include "ui_UIProjectsManager.h"

#include <QHash>

class ProjectItemModel;
class ProjectItem;
class QAction;

class UIProjectsManager : public QDockWidget, public Ui::UIProjectsManager
{
	Q_OBJECT
	
public:
	enum Actions
	{
		New = 0, // new project
		Open, // open project
		SaveCurrent, // save current project
		SaveAll, // save all projects
		CloseCurrent, // close current proejct
		CloseAll, // close all projects
		Add, // add file to current project
		Remove, // remove items/files from current project
		Settings, // open settings dialog for current project
		Source, // show xup source of current project
		Filtered, // view filtered tree
		UserAction // enum for adding custom action to project manager, ie : setAction( UIProjectsManager::UserAction +1, ... );
	};
	
	UIProjectsManager( QWidget* = 0 );
	~UIProjectsManager();

	void initGui();

	void registerItem( ProjectItem* item );
	void unRegisterItem( ProjectItem* item );
	QStringList projectsFilters() const;

	ProjectItem* currentProject() const;
	void setCurrentProject( const ProjectItem* );

	ProjectItem* currentValue() const;
	ProjectItem* currentItem() const;

	bool openProject( const QString& );
	bool saveProject( ProjectItem*, const QString& );

	QAction* action( UIProjectsManager::Actions ) const;
	void setAction( UIProjectsManager::Actions, QAction* );

protected:
	ProjectItemModel* mModel;
	QHash<QString, ProjectItem*> mRegisteredItems;
	QHash<UIProjectsManager::Actions, QAction*> mActions;

	void initializeProject( ProjectItem* );

protected slots:
	void actionNewTriggered();
	void actionOpenTriggered();
	void actionSaveCurrentTriggered();
	void actionSaveAllTriggered();
	void actionCloseCurrentTriggered();
	void actionCloseAllTriggered();
	void actionAddTriggered();
	void actionRemoveTriggered();
	void actionSettingsTriggered();
	void actionSourceTriggered();
	void actionFilteredTriggered();
	void currentChanged( const QModelIndex&, const QModelIndex& );
	void on_tvProxiedProjects_collapsed( const QModelIndex& );
	void on_tvProxiedProjects_expanded( const QModelIndex& );
	void on_tvProxiedProjects_doubleClicked( const QModelIndex& );
	void internal_projectOpen( ProjectItem* );
	void internal_projectAboutToClose( ProjectItem* );
	void internal_projectClosed( ProjectItem* );
	void internal_projectModifiedChanged( ProjectItem*, bool );
	void internal_currentProjectChanged( ProjectItem* );
	void internal_projectDoubleClicked( ProjectItem* );
	void internal_fileDoubleClicked( ProjectItem*, const QString& );

signals:
	void projectOpen( ProjectItem* );
	void projectAboutToClose( ProjectItem* );
	void projectClosed( ProjectItem* );
	void projectModifiedChanged( ProjectItem*, bool );
	void currentProjectChanged( ProjectItem* );
	void projectDoubleClicked( ProjectItem* );
	void fileDoubleClicked( ProjectItem*, const QString& );
};

#endif // UIPROJECTSMANAGER_H
