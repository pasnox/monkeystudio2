/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UIProjectsManager
** FileName  : UIProjectsManager.cpp
** Date      : 2007-09-03T20:14:20
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UIProjectsManager.h"
#include "pMenuBar.h"

UIProjectsManager::UIProjectsManager( QWidget* w )
	: QDockWidget( w ), mProjects( new ProjectsModel( this ) )
{
	setupUi( this );
	// get menubar
	pMenuBar* mb = pMenuBar::instance();
	// set buttons action
	tbFiltered->setDefaultAction( mb->action( "mView/aFiltered" ) );
	tbNew->setDefaultAction( mb->action( "mProject/aNew" ) );
	tbOpen->setDefaultAction( mb->action( "mProject/aOpen" ) );
	tbOpen->setMenu( mb->menu( "mProject/mRecents" ) );
	tbSave->setDefaultAction( mb->action( "mProject/mSave/aCurrent" ) );
	tbSaveAll->setDefaultAction( mb->action( "mProject/mSave/aAll" ) );
	tbClose->setDefaultAction( mb->action( "mProject/mClose/aCurrent" ) );
	tbCloseAll->setDefaultAction( mb->action( "mProject/mClose/aAll" ) );
	tbSettings->setDefaultAction( mb->action( "mProject/aSettings" ) );
	// set projects model
	tvProjects->setModel( mProjects );
}

UIProjectsManager::~UIProjectsManager()
{
}

void UIProjectsManager::openProject( const QString& )
{
	qWarning( "open project" );
}

void UIProjectsManager::projectNew_triggered()
{
	qWarning( "new" );
}

void UIProjectsManager::projectOpen_triggered()
{
	qWarning( "open" );
}

void UIProjectsManager::projectSaveCurrent_triggered()
{
	qWarning( "savecurrent" );
}

void UIProjectsManager::projectSaveAll_triggered()
{
	qWarning( "saveall" );
}

void UIProjectsManager::projectCloseCurrent_triggered()
{
	qWarning( "closecurrent" );
}

void UIProjectsManager::projectCloseAll_triggered()
{
	qWarning( "closeall" );
}

void UIProjectsManager::projectSettings_triggered()
{
	qWarning( "settings" );
}

