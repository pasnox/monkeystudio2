/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pFileManager.h
** Date      : 2008-01-14T00:37:20
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
#ifndef PFILEMANAGER_H
#define PFILEMANAGER_H

#include <fresh.h>

#include <QObject>
#include <QPoint>

class pAbstractChild;
class ProjectItem;

class Q_MONKEY_EXPORT pFileManager : public QObject
{
	Q_OBJECT
	friend class MonkeyCore;
	friend class pWorkspace;

public:
	ProjectItem* currentProject() const;
	QString currentProjectFile() const;
	QString currentProjectPath() const;
	pAbstractChild* currentChild() const;
	QString currentChildFile() const;
	QString currentChildPath() const;
	ProjectItem* currentItem() const;
	QString currentItemFile() const;
	QString currentItemPath() const;

protected:
	pFileManager( QObject* = 0 );

public slots:
	pAbstractChild* openFile( const QString& );
	void closeFile( const QString& );
	void goToLine( const QString&, const QPoint&, bool );
	void openProject( const QString& );
	void closeProject( const QString& );

signals:
	// files
	void fileOpened( const QString& );
	void fileClosed( const QString& );
	void currentFileChanged( pAbstractChild*, const QString& );
	// projects
	void aboutToClose( ProjectItem* );
	void closed( ProjectItem* );
	void modifiedChanged( ProjectItem*, bool );
	void currentChanged( ProjectItem* );
	void opened( ProjectItem* );
};

#endif // PFILEMANAGER_H
