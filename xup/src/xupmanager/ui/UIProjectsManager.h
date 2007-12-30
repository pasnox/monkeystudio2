#ifndef UIPROJECTSMANAGER_H
#define UIPROJECTSMANAGER_H

#include "ui_UIProjectsManager.h"

class ProjectItemModel;
class ProjectItem;

class UIProjectsManager : public QDockWidget, public Ui::UIProjectsManager
{
	Q_OBJECT
	
public:
	UIProjectsManager( QWidget* = 0 );
	~UIProjectsManager();

	ProjectItem* currentProject() const;
	void setCurrentProject( const ProjectItem* );

	bool openProject( const QString& );
	bool saveProject( ProjectItem*, const QString& );

protected:
	ProjectItemModel* mModel;

	void initializeProject( ProjectItem* );

public slots:
	void on_tbOpen_clicked();
	void on_tbClose_clicked();
	void on_tbSave_clicked();
	void on_tbSaveAs_clicked();
	void on_tbSettings_clicked();
	void on_tbSources_clicked();

protected slots:
	void projectModified( ProjectItem*, bool );
	void currentChanged( const QModelIndex&, const QModelIndex& );
	void on_tvProxiedProjects_collapsed( const QModelIndex& );
	void on_tvProxiedProjects_expanded( const QModelIndex& );
	void on_tvProxiedProjects_doubleClicked( const QModelIndex& );

	void on_projectOpen( ProjectItem* );
	void on_projectAboutToClose( ProjectItem* );
	void on_projectModifiedChanged( ProjectItem*, bool );
	void on_currentProjectChanged( ProjectItem* );
	void on_projectDoubleClicked( ProjectItem* );
	void on_fileDoubleClicked( ProjectItem*, const QString& );

signals:
	void projectOpen( ProjectItem* );
	void projectAboutToClose( ProjectItem* );
	void projectModifiedChanged( ProjectItem*, bool );
	void currentProjectChanged( ProjectItem* );
	void projectDoubleClicked( ProjectItem* );
	void fileDoubleClicked( ProjectItem*, const QString& );

};

#endif // UIPROJECTSMANAGER_H
