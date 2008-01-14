#ifndef UIPROJECTSMANAGER_H
#define UIPROJECTSMANAGER_H

#include "ui_UIProjectsManager.h"

class ProjectItemModel;
class ProjectItem;
class QAction;

class UIProjectsManager : public QDockWidget, public Ui::UIProjectsManager
{
	Q_OBJECT
	
public:
	UIProjectsManager( QWidget* = 0 );
	~UIProjectsManager();

	void initGui();

	ProjectItem* currentProject() const;
	void setCurrentProject( const ProjectItem* );

	ProjectItem* currentValue() const;

	bool openProject( const QString& );
	bool saveProject( ProjectItem*, const QString& );

	QAction* actionNew() const;
	void setActionNew( QAction* );
	QAction* actionOpen() const;
	void setActionOpen( QAction* );
	QAction* actionSaveCurrent() const;
	void setActionSaveCurrent( QAction* );
	QAction* actionSaveAll() const;
	void setActionSaveAll( QAction* );
	QAction* actionCloseCurrent() const;
	void setActionCloseCurrent( QAction* );
	QAction* actionCloseAll() const;
	void setActionCloseAll( QAction* );
	QAction* actionAdd() const;
	void setActionAdd( QAction* );
	QAction* actionRemove() const;
	void setActionRemove( QAction* );
	QAction* actionSettings() const;
	void setActionSettings( QAction* );
	QAction* actionSource() const;
	void setActionSource( QAction* );

protected:
	void initializeProject( ProjectItem* );

	ProjectItemModel* mModel;
	QAction* aNew;
	QAction* aOpen;
	QAction* aSaveCurrent;
	QAction* aSaveAll;
	QAction* aCloseCurrent;
	QAction* aCloseAll;
	QAction* aAdd;
	QAction* aRemove;
	QAction* aSettings;
	QAction* aSource;

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
	void currentChanged( const QModelIndex&, const QModelIndex& );
	void on_tvProxiedProjects_collapsed( const QModelIndex& );
	void on_tvProxiedProjects_expanded( const QModelIndex& );
	void on_tvProxiedProjects_doubleClicked( const QModelIndex& );

	void internal_projectOpen( ProjectItem* );
	void internal_projectAboutToClose( ProjectItem* );
	void internal_projectModifiedChanged( ProjectItem*, bool );
	void internal_currentProjectChanged( ProjectItem* );
	void internal_projectDoubleClicked( ProjectItem* );
	void internal_fileDoubleClicked( ProjectItem*, const QString& );

signals:
	void projectOpen( ProjectItem* );
	void projectAboutToClose( ProjectItem* );
	void projectModifiedChanged( ProjectItem*, bool );
	void currentProjectChanged( ProjectItem* );
	void projectDoubleClicked( ProjectItem* );
	void fileDoubleClicked( ProjectItem*, const QString& );
};

#endif // UIPROJECTSMANAGER_H
