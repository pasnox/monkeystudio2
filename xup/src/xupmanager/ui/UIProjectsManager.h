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

protected slots:
	void projectModified( ProjectItem*, bool );
	void currentChanged( const QModelIndex&, const QModelIndex& );
	void on_tvProjects_collapsed( const QModelIndex& );
	void on_tvProjects_expanded( const QModelIndex& );

signals:
	void projectOpen( ProjectItem* );

};

#endif // UIPROJECTSMANAGER_H
