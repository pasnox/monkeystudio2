/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef XUPPROJECTMANAGER_H
#define XUPPROJECTMANAGER_H

#include "MonkeyExport.h"
#include "ui_XUPProjectManager.h"
#include "xupmanager/core/XUPProjectItem.h"

#include <QMap>

class XUPProjectModel;
class XUPFilteredProjectModel;
class XUPOpenedProjectsModel;
class XUPItem;

class Q_MONKEY_EXPORT XUPProjectManager : public pDockWidget, public Ui::XUPProjectManager
{
    Q_OBJECT
    friend class DebugDockWidget;

public:
    enum ActionType { atNew = 0, atClose, atCloseAll, atEdit, atAddFiles, atRemoveFiles, atOpen };
    
    XUPProjectManager( QWidget* parent = 0 );
    virtual ~XUPProjectManager();
    
    QAction* action( XUPProjectManager::ActionType type );
    XUPProjectModel* currentProjectModel() const;
    XUPProjectItem* currentProject() const;
    XUPItem* currentItem() const;
    XUPProjectItemList topLevelProjects() const;
    
    void addFiles( const QStringList& files, XUPItem* scope = NULL);

protected:
    QMap<XUPProjectManager::ActionType, QAction*> mActions;
    XUPFilteredProjectModel* mFilteredModel;
    XUPOpenedProjectsModel* mOpenedProjectsModel;
    
    // debug member to be used by AppDebug plugin
    void openProject( XUPProjectItem* project );

public slots:
    bool openProject( const QString& fileName, const QString& codec );
    void newProject();
    void closeProject();
    void closeAllProjects();
    void editProject();
    void addFiles();
    void removeFiles();
    void openProjectFile();
    void setCurrentProject( XUPProjectItem* curProject, XUPProjectItem* preProject );

protected slots:
    void setCurrentProjectModel( XUPProjectModel* model );
    void on_cbProjects_currentIndexChanged( int id );
    void tvFiltered_currentChanged( const QModelIndex& current, const QModelIndex& previous );
    void on_tvFiltered_activated( const QModelIndex& index );
    void on_tvFiltered_customContextMenuRequested( const QPoint& pos );

signals:
    void projectCustomContextMenuRequested( const QPoint& pos );
    
    void projectOpened( XUPProjectItem* project );
    void projectAboutToClose( XUPProjectItem* project );
    
    void currentProjectChanged( XUPProjectItem* currentProject, XUPProjectItem* previousProject );
    void currentProjectChanged( XUPProjectItem* currentProject );
    
    void projectDoubleClicked( XUPProjectItem* project );
    void fileDoubleClicked( XUPProjectItem* project, const QString& fileName, const QString& codec );
    void fileDoubleClicked( const QString& fileName, const QString& codec );
};

#endif // XUPPROJECTMANAGER_H
