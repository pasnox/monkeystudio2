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
#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "pRecentsManager.h"

#include <QHeaderView>

UIProjectsManager::UIProjectsManager( QWidget* w )
	: QDockWidget( w ), mProjects( new ProjectsModel( this ) )
{
	setupUi( this );
	// set icons size
	tbButtons->setIconSize( QSize( 10, 10 ) );
	// set layout margin & spacing
	tbButtons->layout()->setMargin( 0 );
	tbButtons->layout()->setSpacing( 0 );
	// get menubar
	pMenuBar* mb = pMenuBar::instance();
	// set buttons action
	tbButtons->addAction( mb->action( "mView/aFiltered" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aNew" ) );
	tbButtons->addAction( mb->action( "mProject/aOpen" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/mSave/aCurrent" ) );
	tbButtons->addAction( mb->action( "mProject/mSave/aAll" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/mClose/aCurrent" ) );
	tbButtons->addAction( mb->action( "mProject/mClose/aAll" ) );
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aSettings" ) );
	// set projects properties
	tvProjects->header()->hide();
	tvProjects->setModel( mProjects );
}

UIProjectsManager::~UIProjectsManager()
{
}

bool UIProjectsManager::openProject( const QString& s )
{
	// looking plugins that can open this suffixe
	ProjectPlugin* pp = PluginsManager::instance()->projectPluginForFileName( s );
	// project root item
	QStandardItem* it = 0;
	// try opening project
	if ( pp )
	{
		// parse project
		it = pp->openProject( s );
		if ( it )
			mProjects->appendRow( it );
		else
			pMonkeyStudio::warning( tr( "Open Project" ), tr( "An error occur while opening this project:\n[%1]" ).arg( s ) );
	}
	else
		pMonkeyStudio::warning( tr( "Open Project..." ), tr( "There is no plugin that can manage this kind of project.\n[%1]" ).arg( s ) );
	// return it
	return it;
}

void UIProjectsManager::projectNew_triggered()
{
	qWarning( "new" );
}

void UIProjectsManager::projectOpen_triggered()
{
	// get last file open path
	const QString mPath = pRecentsManager::instance()->recentProjectOpenPath();

	// get available filters
	QString mFilters = pMonkeyStudio::availableProjectsFilters();

	// prepend a all in one filter
	if ( !mFilters.isEmpty() )
	{
		QString s;
		foreach ( QStringList l, pMonkeyStudio::availableProjectsSuffixes().values() )
			s.append( l.join( " " ).append( " " ) );
		mFilters.prepend( QString( "All Supported Projects (%1);;" ).arg( s.trimmed() ) );
	}

	// open open file dialog
	QStringList l = pMonkeyStudio::getOpenFileNames( tr( "Choose the project(s) to open" ), mPath, mFilters, window() );

	// for each entry, open file
	foreach ( QString s, l )
	{
		if ( openProject( s ) )
			// append file to recents
			pRecentsManager::instance()->addRecentProject( s );
		else
			// remove it from recents files
			pRecentsManager::instance()->removeRecentProject( s );
	}

	// store file open path
	if ( !l.isEmpty() )
		pRecentsManager::instance()->setRecentProjectOpenPath( QFileInfo( l.at( 0 ) ).canonicalPath() );
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

