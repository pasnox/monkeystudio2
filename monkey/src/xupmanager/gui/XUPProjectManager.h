#ifndef XUPPROJECTMANAGER_H
#define XUPPROJECTMANAGER_H

#include "MonkeyExport.h"
#include "ui_XUPProjectManager.h"
#include "XUPProjectItem.h"

#include <QMap>

class XUPProjectModel;
class XUPFilteredProjectModel;
class XUPItem;

class Q_MONKEY_EXPORT XUPProjectManager : public pDockWidget, public Ui::XUPProjectManager
{
	Q_OBJECT

public:
	enum ActionType { atNew = 0, atOpen, atClose, atCloseAll, atEdit, atAddFiles, atRemoveFiles };
	
	XUPProjectManager( QWidget* parent = 0 );
	virtual ~XUPProjectManager();
	
	QAction* action( XUPProjectManager::ActionType type );
	XUPProjectModel* currentProjectModel() const;
	XUPProjectItem* currentProject() const;
	XUPItem* currentItem() const;
	XUPProjectItemList topLevelProjects() const;

protected:
	QMap<XUPProjectManager::ActionType, QAction*> mActions;
	XUPFilteredProjectModel* mFilteredModel;
	QMenu* mDebugMenu;
	
	void addFilesToScope( XUPItem* scope, const QStringList& allFiles, const QString& op );

public slots:
	void addError( const QString& error );
	bool openProject( const QString& fileName, const QString& encoding );
	void newProject();
	bool openProject();
	void closeProject();
	void closeAllProjects();
	void editProject();
	void addFiles();
	void removeFiles();
	void setCurrentProject( XUPProjectItem* curProject, XUPProjectItem* preProject );

protected slots:
	void setCurrentProjectModel( XUPProjectModel* model );
	void on_cbProjects_currentIndexChanged( int id );
	void debugMenu_triggered( QAction* action );
	void on_pbFiltered_toggled( bool checked );
	void on_pbNative_toggled( bool checked );
	void tvFiltered_currentChanged( const QModelIndex& current, const QModelIndex& previous );
	void on_tvFiltered_doubleClicked( const QModelIndex& index );

signals:
	void projectCustomContextMenuRequested( const QPoint& pos );
	
	void projectOpened( XUPProjectItem* project );
	void projectAboutToClose( XUPProjectItem* project );
	
	void currentProjectChanged( XUPProjectItem* currentProject, XUPProjectItem* previousProject );
	void currentProjectChanged( XUPProjectItem* currentProject );
	
	void projectDoubleClicked( XUPProjectItem* project );
	void fileDoubleClicked( XUPProjectItem* project, const QString& fileName, const QString& encoding );
	void fileDoubleClicked( const QString& fileName, const QString& encoding );
};

#endif // XUPPROJECTMANAGER_H
