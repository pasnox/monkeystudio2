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
	XUPProjectModel* currentProject() const;
	XUPProjectItem* currentProjectItem() const;
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
	bool openProject();
	void closeProject();
	void closeAllProjects();
	void editProject();
	void setCurrentProject( XUPProjectModel* project );

protected slots:
	void fileClosed( QObject* object );
	void on_cbProjects_currentIndexChanged( int id );
	void debugMenu_triggered( QAction* action );
	void on_pbFiltered_toggled( bool checked );
	void on_pbNative_toggled( bool checked );
	void on_tvFiltered_activated( const QModelIndex& index );
	void on_tvNative_activated( const QModelIndex& index );
};

#endif // XUPPROJECTMANAGER_H
