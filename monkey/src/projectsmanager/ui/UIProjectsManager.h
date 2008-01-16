/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIProjectsManager.h
** Date      : 2008-01-14T00:37:03
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
**
****************************************************************************/
#ifndef UIPROJECTSMANAGER_H
#define UIPROJECTSMANAGER_H

#include "MonkeyExport.h"
#include "ui_UIProjectsManager.h"
#include "ProjectItem.h"

#include <QDockWidget>

class ProjectsProxy;
class ProjectsModel;

class Q_MONKEY_EXPORT UIProjectsManager : public QDockWidget, public Ui::UIProjectsManager
{
	Q_OBJECT
	friend class MonkeyCore;
	
public:
	ProjectItem* currentItem() const;
	ProjectItem* currentProject() const;
	void setCurrentProject( ProjectItem* );
	ProjectItemList rootProjects() const;

	inline ProjectsModel* model() const { return mProjects; }
	inline ProjectsProxy* proxy() const { return mProxy; }

protected:
	ProjectsModel* mProjects;
	ProjectsProxy* mProxy;

	UIProjectsManager( QWidget* = 0 );
	void initializeProject( ProjectItem* );

private slots:
	void cbProjects_activated( const QModelIndex& );
	void tvProjects_currentChanged( const QModelIndex&, const QModelIndex& );
	void on_tvProjects_doubleClicked( const QModelIndex& );
	void on_tvProjects_customContextMenuRequested( const QPoint& );

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
	void projectRemove_triggered();

signals:
	void aboutToClose( ProjectItem* );
	void closed( ProjectItem* );
	void modifiedChanged( ProjectItem*, bool );
	void currentChanged( ProjectItem* );
	void opened( ProjectItem* );

};

#endif // UIPROJECTSMANAGER_H
