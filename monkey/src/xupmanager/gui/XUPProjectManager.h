#ifndef XUPPROJECTMANAGER_H
#define XUPPROJECTMANAGER_H

#include "MonkeyExport.h"
#include "ui_XUPProjectManager.h"
#include "XUPProjectItem.h"

#include <QMap>

class XUPProjectModel;
class XUPFilteredProjectModel;
class XUPItem;
class QPlainTextEdit;

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
	QMap<QString, QPlainTextEdit*> mOpenedFiles;
	QMenu* mDebugMenu;

public slots:
	void addError( const QString& error );
	bool openFile( const QString& fileName, const QString& encoding = "UTF-8" );
	bool openProject( const QString& fileName, const QString& encoding = "UTF-8" );
	void newProject();
	bool openProject();
	void closeProject();
	void closeAllProjects();
	void editProject();
	void addFiles();
	void removeFiles();
	void setCurrentProjectModel( XUPProjectModel* model );
	void setCurrentProject( XUPProjectItem* project );

protected slots:
	void fileClosed( QObject* object );
	void on_cbProjects_currentIndexChanged( int id );
	void debugMenu_triggered( QAction* action );
	void on_pbFiltered_toggled( bool checked );
	void on_pbNative_toggled( bool checked );
	void tvFiltered_currentChanged( const QModelIndex& current, const QModelIndex& previous );
	void on_tvFiltered_activated( const QModelIndex& index );

signals:
	void projectCustomContextMenuRequested( const QPoint& pos );
	
	void currentProjectChanged( XUPProjectItem* currentProject, XUPProjectItem* previousProject );
	void currentProjectChanged( XUPProjectItem* currentProject );
	
	/*
	void projectOpen( XUPItem* project );
	void projectAboutToClose( XUPItem* project );
	void projectClosed( XUPItem* project );
	void projectModifiedChanged( XUPItem* project, bool modified );
	void currentProjectChanged( XUPItem* currentProject, XUPItem* previousProject );
	void currentProjectChanged( XUPItem* currentProject );
	void projectDoubleClicked( XUPItem* project );
	void fileDoubleClicked( XUPItem* project, const QString& fileName );
	void fileDoubleClicked( const QString& fileName );
	void projectCustomContextMenuRequested( const QPoint& pos );
	void projectInstallCommandRequested( const pCommand& cmd, const QString& mnu );
	void projectUninstallCommandRequested( const pCommand& cmd, const QString& mnu );
	*/
};

#endif // XUPPROJECTMANAGER_H
