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
	// sort project
	it->redoLayout();
	// sort proxy
	mProxy->sort( 0, Qt::AscendingOrder );
	// set current project
	tvProjects->setCurrentIndex( mProxy->mapFromSource( it->index() ) );
}

void UIProjectsManager::tvProjects_currentChanged( const QModelIndex& c, const QModelIndex& p )
{
	// get menubar
	pMenuBar* mb = pMenuBar::instance();
	// get pluginsmanager
	PluginsManager* pm = PluginsManager::instance();
	// if valid
	if ( c.isValid() )
	{
		// get item
		ProjectItem* it = mProjects->itemFromIndex( mProxy->mapToSource( c ) );
		// looking plugin that can manage this project
		ProjectPlugin* pp = pm->plugin<ProjectPlugin*>( it->pluginName() );
		//
		if ( pp && pp->isEnabled() )
		{
			// desactive compiler, debugger and interpreter
			pm->setCurrentCompiler( pp->compiler( currentProject() ) );
			pm->setCurrentDebugger( pp->debugger( currentProject() ) );
			pm->setCurrentInterpreter( pp->interpreter( currentProject() ) );
			// desactive menu entries
			mb->menu( "mBuild" )->setEnabled( !pm->currentCompiler().isEmpty() );
			mb->menu( "mDebugger" )->setEnabled( !pm->currentDebugger().isEmpty() );
			mb->menu( "mInterpreter" )->setEnabled( !pm->currentInterpreter().isEmpty() );
			// desactive project action
			mb->action( "mProject/mSave/aCurrent" )->setEnabled( true );
			mb->action( "mProject/mSave/aAll" )->setEnabled( true );
			mb->action( "mProject/mClose/aCurrent" )->setEnabled( true );
			mb->action( "mProject/mClose/aAll" )->setEnabled( true );
			mb->action( "mProject/aSettings" )->setEnabled( true );
		}
	}
	else
	{
		// desactive compiler, debugger and interpreter
		pm->setCurrentCompiler( QString::null );
		pm->setCurrentDebugger( QString::null );
		pm->setCurrentInterpreter( QString::null );
		// desactive menu entries
		mb->menu( "mBuild" )->setEnabled( false );
		mb->menu( "mDebugger" )->setEnabled( false );
		mb->menu( "mInterpreter" )->setEnabled( false );
		// desactive project action
		mb->action( "mProject/mSave/aCurrent" )->setEnabled( false );
		mb->action( "mProject/mSave/aAll" )->setEnabled( false );
		mb->action( "mProject/mClose/aCurrent" )->setEnabled( false );
		mb->action( "mProject/mClose/aAll" )->setEnabled( false );
		mb->action( "mProject/aSettings" )->setEnabled( false );
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
	if ( ProjectPlugin* pp = PluginsManager::instance()->projectPluginForFileName( s ) )
	{
		if ( ProjectItem* it = pp->openProject( s ) )
		{
			initializeProject( it );
			return true;
		}
		else
			pMonkeyStudio::warning( tr( "Open Project" ), tr( "An error occur while opening this project:\n[%1]" ).arg( s ) );
	}
	else
		pMonkeyStudio::warning( tr( "Open Project..." ), tr( "There is no plugin that can manage this kind of project.\n[%1]" ).arg( s ) );
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
		PluginsManager::instance()->plugin<ProjectPlugin*>( it->pluginName() )->editSettings( it->project() );
}

