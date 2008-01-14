/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pFileManager.cpp
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
#include "pFileManager.h"
#include "pWorkspace.h"
#include "UIProjectsManager.h"
#include "pAbstractChild.h"
#include "MonkeyCore.h"

pFileManager::pFileManager( QObject* o )
	: QObject( o )
{
	// files
	connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString& ) ), this, SIGNAL( fileOpened( const QString& ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( fileClosed( const QString& ) ), this, SIGNAL( fileClosed( const QString& ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ), this, SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ) );
	// projects
	connect( MonkeyCore::projectsManager(), SIGNAL( aboutToClose( ProjectItem* ) ), this, SIGNAL( aboutToClose( ProjectItem* ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( closed( ProjectItem* ) ), this, SIGNAL( closed( ProjectItem* ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( modifiedChanged( ProjectItem*, bool ) ), this, SIGNAL( modifiedChanged( ProjectItem*, bool ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( currentChanged( ProjectItem* ) ), this, SIGNAL( currentChanged( ProjectItem* ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( opened( ProjectItem* ) ), this, SIGNAL( opened( ProjectItem* ) ) );
}

ProjectItem* pFileManager::currentProject() const
{ return MonkeyCore::projectsManager()->currentProject(); }

QString pFileManager::currentProjectFile() const
{ return currentProject() ? currentProject()->canonicalFilePath() : QString(); }

QString pFileManager::currentProjectPath() const
{ return currentProject() ? currentProject()->canonicalPath() : QString(); }

pAbstractChild* pFileManager::currentChild() const
{ return MonkeyCore::workspace()->currentChild(); }

QString pFileManager::currentChildFile() const
{ return currentChild() ? currentChild()->currentFile() : QString(); }

QString pFileManager::currentChildPath() const
{ return QFileInfo( currentChildFile() ).path(); }

ProjectItem* pFileManager::currentItem() const
{ return MonkeyCore::projectsManager()->currentItem(); }

QString pFileManager::currentItemFile() const
{ return currentItem() ? currentItem()->getFilePath() : QString(); }

QString pFileManager::currentItemPath() const
{ return QFileInfo( currentItemFile() ).path(); }

pAbstractChild* pFileManager::openFile( const QString& s )
{ return MonkeyCore::workspace()->openFile( s ); }

void pFileManager::closeFile( const QString& s )
{ MonkeyCore::workspace()->closeFile( s ); }

void pFileManager::goToLine( const QString& s, const QPoint& p, bool b )
{ MonkeyCore::workspace()->goToLine( s, p, b ); }

void pFileManager::openProject( const QString& s )
{ MonkeyCore::projectsManager()->openProject( s ); }

void pFileManager::closeProject( const QString& s )
{ MonkeyCore::projectsManager()->closeProject( s ); }
