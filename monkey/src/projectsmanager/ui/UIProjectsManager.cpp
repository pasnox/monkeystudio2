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
#include "ProjectsProxy.h"
#include "ProjectsModel.h"
#include "ProjectItem.h"
#include "pFileManager.h"

#include <QHeaderView>

using namespace pMonkeyStudio;

UIProjectsManager::UIProjectsManager( QWidget* w )
	: QDockWidget( w ), mProjects( new ProjectsModel( this ) ), mProxy( new ProjectsProxy( mProjects ) )
{
	setupUi( this );
	
	// set icons size
	tbButtons->setIconSize( QSize( 16, 16 ) );
	
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
	tbButtons->addSeparator();
	tbButtons->addAction( mb->action( "mProject/aAddExistingFiles" ) );
	
	// set projects properties
	tvProjects->header()->hide();
	
	// set proxy properties
	// set types to filter
	mProxy->setFilterRoles( QList<int>() << ProjectsModel::ValueType );
	// filter are negate
	mProxy->setNegateFilter( false );
	// apply filtering
	mProxy->setFiltering( true );
	// set view proxy
	tvProjects->setModel( mProxy );
	
	// set filter filtering state
	mb->action( "mView/aFiltered" )->setChecked( mProxy->isFiltering() );
	
	// connections
	connect( mb->action( "mView/aFiltered" ), SIGNAL( triggered( bool ) ), mProxy, SLOT( setFiltering( bool ) ) );
	connect( mProxy, SIGNAL( filteringChanged( bool ) ), mb->action( "mView/aFiltered" ), SLOT( setChecked( bool ) ) );
	connect( tvProjects->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( tvProjects_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
}

UIProjectsManager::~UIProjectsManager()
{}

ProjectItem* UIProjectsManager::currentProject() const
{
	// get current item
	ProjectItem* it = mProjects->itemFromIndex( mProxy->mapToSource( tvProjects->currentIndex() ) );
	// return project item
	return it ? it->project() : 0;
}

void UIProjectsManager::initializeProject( ProjectItem* it )
{
	// clear selected item
	tvProjects->selectionModel()->clear();
	// append project item
	mProjects->appendRow( it );
	// refresh project
	mProxy->refresh( it );
	// set current project
	tvProjects->setCurrentIndex( mProxy->mapFromSource( it->index() ) );
}

void UIProjectsManager::tvProjects_currentChanged( const QModelIndex& /*c*/, const QModelIndex& /*p*/ )
{
	// get menubar
	pMenuBar* mb = pMenuBar::instance();
	// get pluginsmanager
	PluginsManager* pm = PluginsManager::instance();
	
	// if current project
	if ( ProjectItem* it = currentProject() )
	{
		// set compiler, debugger and interpreter
		pm->setCurrentCompiler( it->compiler() );
		pm->setCurrentDebugger( it->debugger() );
		pm->setCurrentInterpreter( it->interpreter() );
		// desactive project action
		mb->action( "mProject/mSave/aCurrent" )->setEnabled( true );
		mb->action( "mProject/mSave/aAll" )->setEnabled( true );
		mb->action( "mProject/mClose/aCurrent" )->setEnabled( true );
		mb->action( "mProject/mClose/aAll" )->setEnabled( true );
		mb->action( "mProject/aSettings" )->setEnabled( true );
		mb->action( "mProject/aAddExistingFiles" )->setEnabled( true );
	}
	else
	{
		// set compiler, debugger and interpreter to null
		pm->setCurrentCompiler( O );
		pm->setCurrentDebugger( O );
		pm->setCurrentInterpreter( 0 );
		// desactive project action
		mb->action( "mProject/mSave/aCurrent" )->setEnabled( false );
		mb->action( "mProject/mSave/aAll" )->setEnabled( false );
		mb->action( "mProject/mClose/aCurrent" )->setEnabled( false );
		mb->action( "mProject/mClose/aAll" )->setEnabled( false );
		mb->action( "mProject/aSettings" )->setEnabled( false );
		mb->action( "mProject/aAddExistingFiles" )->setEnabled( false );
	}
}

void UIProjectsManager::on_tvProjects_doubleClicked( const QModelIndex& i )
{
	// get item
	ProjectItem* it = mProjects->itemFromIndex( mProxy->mapToSource( i ) );
	// if item and file exists, open it
	if ( it && QFile::exists( it->getFilePath() ) )
		pFileManager::instance()->openFile( it->getFilePath() );
}

bool UIProjectsManager::openProject( const QString& s )
{
	if ( ProjectItem* it = PluginsManager::instance()->projectItem( s ) )
	{
		if ( it->open() )
		{
			initializeProject( it );
			return true;
		}
		else
			delete it;
	}
	else
		warning( tr( "Open Project..." ), tr( "There is no plugin that can manage this project :\n[%1]" ).arg( s ) );
	return false;
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
	QString mFilters = availableProjectsFilters();
	// prepend a all in one filter
	if ( !mFilters.isEmpty() )
	{
		QString s;
		foreach ( QStringList l, availableProjectsSuffixes().values() )
			s.append( l.join( " " ).append( " " ) );
		mFilters.prepend( QString( "All Supported Projects (%1);;" ).arg( s.trimmed() ) );
	}
	// open open file dialog
	QStringList l = getOpenFileNames( tr( "Choose the project(s) to open" ), mPath, mFilters, window() );
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
	ProjectItem* p = currentProject();
	if ( p )
		p->save( false );
}

void UIProjectsManager::projectSaveAll_triggered()
{
	qWarning( "saveall" );
}

void UIProjectsManager::projectCloseCurrent_triggered()
{
	ProjectItem* p = currentProject();
	if ( p )
		p->close();
}

void UIProjectsManager::projectCloseAll_triggered()
{
	while ( ProjectItem* p = currentProject() )
		p->close();
}

void UIProjectsManager::projectSettings_triggered()
{
	if ( ProjectItem* it = currentProject() )
		it->editSettings();
}

void UIProjectsManager::projectAddExistingFiles_triggered()
{
	if ( ProjectItem* it = currentProject() )
	{
		QStringList l = getOpenFileNames( tr( "Choose file(s) to add to your project" ), it->canonicalPath() );
		if ( !l.isEmpty() )
			it->addExistingFiles( l, it, "=" );
	}
}

