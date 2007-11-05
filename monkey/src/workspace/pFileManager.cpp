/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pFileManager.cpp
** Date      : 2007-11-04T22:46:57
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pFileManager.h"
#include "pWorkspace.h"
#include "UIProjectsManager.h"
#include "pAbstractChild.h"

pFileManager::pFileManager( QObject* o )
	: QObject( o )
{
	// files
	connect( pWorkspace::instance(), SIGNAL( fileOpened( const QString& ) ), this, SIGNAL( fileOpened( const QString& ) ) );
	connect( pWorkspace::instance(), SIGNAL( fileClosed( const QString& ) ), this, SIGNAL( fileClosed( const QString& ) ) );
	connect( pWorkspace::instance(), SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ), this, SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ) );
	// projects
	connect( UIProjectsManager::instance(), SIGNAL( aboutToClose( ProjectItem* ) ), this, SIGNAL( aboutToClose( ProjectItem* ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( closed( ProjectItem* ) ), this, SIGNAL( closed( ProjectItem* ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( modifiedChanged( ProjectItem*, bool ) ), this, SIGNAL( modifiedChanged( ProjectItem*, bool ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( currentChanged( ProjectItem* ) ), this, SIGNAL( currentChanged( ProjectItem* ) ) );
	connect( UIProjectsManager::instance(), SIGNAL( opened( ProjectItem* ) ), this, SIGNAL( opened( ProjectItem* ) ) );
}

ProjectItem* pFileManager::currentProject() const
{ return UIProjectsManager::instance()->currentProject(); }

QString pFileManager::currentProjectFile() const
{ return currentProject() ? currentProject()->canonicalFilePath() : QString(); }

QString pFileManager::currentProjectPath() const
{ return currentProject() ? currentProject()->canonicalPath() : QString(); }

pAbstractChild* pFileManager::currentChild() const
{ return pWorkspace::instance()->currentChild(); }

QString pFileManager::currentChildFile() const
{ return currentChild() ? currentChild()->currentFile() : QString(); }

QString pFileManager::currentChildPath() const
{ return QFileInfo( currentChildFile() ).path(); }

ProjectItem* pFileManager::currentItem() const
{ return UIProjectsManager::instance()->currentItem(); }

QString pFileManager::currentItemFile() const
{ return currentItem() ? currentItem()->getFilePath() : QString(); }

QString pFileManager::currentItemPath() const
{ return QFileInfo( currentItemFile() ).path(); }

pAbstractChild* pFileManager::openFile( const QString& s, const QPoint& p )
{ return pWorkspace::instance()->openFile( s, p ); }

void pFileManager::closeFile( const QString& s )
{ pWorkspace::instance()->closeFile( s ); }

void pFileManager::goToLine( const QString& s, const QPoint& p, bool b )
{ pWorkspace::instance()->goToLine( s, p, b ); }

void pFileManager::openProject( const QString& s )
{ UIProjectsManager::instance()->openProject( s ); }

void pFileManager::closeProject( const QString& s )
{ UIProjectsManager::instance()->closeProject( s ); }
