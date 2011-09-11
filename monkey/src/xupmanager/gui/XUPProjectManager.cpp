#include "xupmanager/gui/XUPProjectManager.h"
#include "UIXUPFindFiles.h"
#include "xupmanager/core/XUPProjectModel.h"
#include "xupmanager/core/XUPProjectItemCache.h"
#include "xupmanager/core/XUPFilteredProjectModel.h"
#include "XUPOpenedProjectsModel.h"
#include "pMonkeyStudio.h"
#include "coremanager/MonkeyCore.h"
#include "pluginsmanager/PluginsManager.h"
#include "workspace/pWorkspace.h"
#include "pluginsmanager/XUPPlugin.h"
#include "templatesmanager/ui/UITemplatesWizard.h"
#include "recentsmanager/pRecentsManager.h"

#include <pDockWidgetTitleBar.h>
#include <pQueuedMessageToolBar.h>
#include <pMenuBar.h>
#include <pIconManager.h>

#include <QTextCodec>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileSystemWatcher>
#include <QDebug>

XUPProjectManager::XUPProjectManager( QWidget* parent )
	: pDockWidget( parent )
{
	setObjectName( metaObject()->className() );
	setupUi( this );
	setActionsManager( MonkeyCore::actionsManager() );
	
	titleBar()->addAction( action( atNew ), 0 );
	titleBar()->addAction( action( atClose ), 1 );
	titleBar()->addAction( action( atCloseAll ), 2 );
	titleBar()->addAction( action( atEdit ), 3 );
	titleBar()->addAction( action( atAddFiles ), 4 );
	titleBar()->addAction( action( atRemoveFiles ), 5 );
	titleBar()->addSeparator( 6 );
	
	mFilteredModel = new XUPFilteredProjectModel( tvFiltered );
	tvFiltered->setModel( mFilteredModel );
	
	mOpenedProjectsModel = new XUPOpenedProjectsModel( this );
	cbProjects->setModel( mOpenedProjectsModel );
	
	connect( tvFiltered->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( tvFiltered_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( tvFiltered, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SIGNAL( projectCustomContextMenuRequested( const QPoint& ) ) );
	
	pMonkeyStudio::showMacFocusRect( wCentral, false, true );
	pMonkeyStudio::setMacSmallSize( wCentral, true, true );
	setCurrentProject( 0, 0 );
}

XUPProjectManager::~XUPProjectManager()
{
}

void XUPProjectManager::on_cbProjects_currentIndexChanged( int id )
{
	XUPProjectModel* model = mOpenedProjectsModel->project( id );
	XUPProjectItem* project = model ? model->rootProject() : 0;
	setCurrentProject( project, currentProject() );
}

void XUPProjectManager::tvFiltered_currentChanged( const QModelIndex& current, const QModelIndex& previous )
{
	XUPItem* curItem = mFilteredModel->mapToSource( current );
	XUPItem* preItem = mFilteredModel->mapToSource( previous );
	XUPProjectItem* curProject = curItem ? curItem->project() : 0;
	XUPProjectItem* preProject = preItem ? preItem->project() : 0;
	
	if ( !previous.isValid() )
	{
		preProject = curProject->topLevelProject();
	}
	
	setCurrentProject( curProject, preProject );
}

void XUPProjectManager::on_tvFiltered_activated( const QModelIndex& index )
{
	XUPItem* item = mFilteredModel->mapToSource( index );
	
	if ( item ) {
		if ( item->type() == XUPItem::Project ) {
			emit projectDoubleClicked( item->project() );
		}
		else if ( item->type() == XUPItem::File ) {
			XUPProjectItem* project = item->project();
			const QString content = item->cacheValue( "content" );
			QString filePath = project->filePath( content );
			
			if ( !QFile::exists( filePath ) ) {
				const XUPProjectItem* rootIncludeProject = project->rootIncludeProject();
				const QString findFile = QFileInfo( content ).fileName();
				QFileInfoList files = project->findFile( findFile );
				
				if ( files.isEmpty() ) {
					files = rootIncludeProject->findFile( findFile );
				}
				
				switch ( files.count() ) {
					case 0:
						filePath.clear();
						break;
					case 1:
						filePath = files.at( 0 ).absoluteFilePath();
						break;
					default: {
						UIXUPFindFiles dlg( findFile, this );
						dlg.setFiles( files, rootIncludeProject->path() );
						filePath.clear();
						
						if ( dlg.exec() == QDialog::Accepted ) {
							filePath = dlg.selectedFile();
						}
						
						break;
					}
				}
			}
			
			if ( QFile::exists( filePath ) ) {
				emit fileDoubleClicked( project, filePath, project->codec() );
				emit fileDoubleClicked( filePath, project->codec() );
			}
			else {
				MonkeyCore::messageManager()->appendMessage( tr( "Can't find file to open '%1'" ).arg( QFileInfo( content ).fileName() ) );
			}
		}
	}
}

void XUPProjectManager::on_tvFiltered_customContextMenuRequested( const QPoint& pos )
{
	if ( currentProject() )
	{
		// get menubar
		pMenuBar* mb = MonkeyCore::menuBar();
		QMenu menu;
		
		// add menu commands
		menu.addActions( mb->menu( "mProject" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mBuilder" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mDebugger" )->actions() );
		menu.addSeparator();
		menu.addActions( mb->menu( "mInterpreter" )->actions() );
		
		// show menu
		menu.exec( tvFiltered->mapToGlobal( pos ) );
	}
}

QAction* XUPProjectManager::action( XUPProjectManager::ActionType type )
{
	if ( mActions.contains( type ) )
	{
		return mActions[ type ];
	}
	
	QAction* action = 0;
	switch ( type )
	{
		case XUPProjectManager::atNew:
			action = new QAction( pIconManager::icon( "new.png", ":/project" ), tr( "New project..." ), this );
			mActionsManager->setDefaultShortcut( action, tr( "Ctrl+Shift+N" ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( newProject() ) );
			break;
		case XUPProjectManager::atClose:
			action = new QAction( pIconManager::icon( "close.png", ":/project" ), tr( "Close current project" ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeProject() ) );
			break;
		case XUPProjectManager::atCloseAll:
			action = new QAction( pIconManager::icon( "closeall.png", ":/project" ), tr( "Close all projects" ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( closeAllProjects() ) );
			break;
		case XUPProjectManager::atEdit:
			action = new QAction( pIconManager::icon( "settings.png", ":/project" ), tr( "Edit current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( editProject() ) );
			break;
		case XUPProjectManager::atAddFiles:
			action = new QAction( pIconManager::icon( "add.png", ":/project" ), tr( "Add existing files to current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( addFiles() ) );
			break;
		case XUPProjectManager::atRemoveFiles:
			action = new QAction( pIconManager::icon( "remove.png", ":/project" ), tr( "Remove files from current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( removeFiles() ) );
			break;
	}
	
	if ( action )
	{
		action->setToolTip( action->text() );
		action->setStatusTip( action->text() );
		mActions[ type ] = action;
	}
	
	return action;
}

void XUPProjectManager::newProject()
{
	UITemplatesWizard wizard( this );
	wizard.setType( "Projects" );
	wizard.exec();
}

void XUPProjectManager::openProject( XUPProjectItem* project )
{
	XUPProjectModel* model = new XUPProjectModel( this );
	model->handleProject( project );
	
	mOpenedProjectsModel->addProject( model );
	setCurrentProject( model->rootProject(), currentProject() );
	emit projectOpened( currentProject() );
}

bool XUPProjectManager::openProject( const QString& fileName, const QString& codec )
{
	// check that project is not yet open
	foreach ( XUPProjectItem* project, topLevelProjects() )
	{
		if ( pMonkeyStudio::isSameFile( project->fileName(), fileName ) )
		{
			setCurrentProject( project, currentProject() );
			return true;
		}
	}
	
	QFileInfo fi( fileName );
	
	if ( fi.exists() && fi.isFile() )
	{
		XUPProjectModel* model = new XUPProjectModel( this );
		if ( model->open( fileName, codec ) )
		{
			MonkeyCore::recentsManager()->addRecentProject( fileName );
			mOpenedProjectsModel->addProject( model );
			setCurrentProject( model->rootProject(), currentProject() );
			emit projectOpened( currentProject() );
			return true;
		}
		else
		{
			MonkeyCore::recentsManager()->removeRecentProject( fileName );
			delete model;
		}
	}
	
	return false;
}

void XUPProjectManager::closeProject()
{
	XUPProjectModel* curModel = currentProjectModel();
	
	if ( curModel ) {
		XUPProjectItem* preProject = currentProject();
		const int curRow = mOpenedProjectsModel->row( curModel );
		int row = curRow;
		
		if ( row > 0 ) {
			row--;
		}
		else {
			row++;
		}
		
		row = qBound( 0, row, mOpenedProjectsModel->rowCount() -1 );
		
		if ( row != curRow ) {
			setCurrentProjectModel( mOpenedProjectsModel->project( row ) );
			XUPProjectItem* curProject = currentProject();
			setCurrentProject( curProject, preProject );
		}
		else {
			setCurrentProjectModel( 0 );
			setCurrentProject( 0, preProject );
		}
		
		emit projectAboutToClose( preProject );
		
		mOpenedProjectsModel->removeProject( curModel );
		curModel->close();
		curModel->deleteLater();
	}
}

void XUPProjectManager::closeAllProjects()
{
	while ( !mOpenedProjectsModel->projects().isEmpty() ) {
		closeProject();
	}
}

void XUPProjectManager::editProject()
{
	XUPProjectItem* project = currentProject();
	
	if ( !project ) {
		return;
	}
	
	// edit project and save it if needed
	if ( project->edit() ) {
		project->save();
		
		// update menu actions
		project->uninstallCommands();
		project->installCommands();
	}
}

void XUPProjectManager::addFiles()
{
	XUPProjectItem* project = currentProject();
	
	if ( !project ) {
		return;
	}
	
	// edit project and save it if needed
	if ( project->editProjectFiles() ) {
		project->save();
		
		// update menu actions
		project->uninstallCommands();
		project->installCommands();
	}
}

void XUPProjectManager::removeFiles()
{
	addFiles();
}

void XUPProjectManager::addFiles( const QStringList& files, XUPItem* scope )
{
	XUPProjectItem* project = scope->project();
	project->addFiles( files, scope );
	project->save();
}

XUPProjectModel* XUPProjectManager::currentProjectModel() const
{
	return mFilteredModel->sourceModel();
}

XUPProjectItem* XUPProjectManager::currentProject() const
{
	XUPProjectModel* curModel = currentProjectModel();
	XUPItem* curItem = currentItem();
	
	if ( curItem )
	{
		return curItem->project();
	}
	
	if ( !curItem && curModel )
	{
		return curModel->mRootProject;
	}
	
	return 0;
}

XUPItem* XUPProjectManager::currentItem() const
{
	const QModelIndex index = tvFiltered->currentIndex();
	return mFilteredModel->mapToSource( index );
}

XUPProjectItemList XUPProjectManager::topLevelProjects() const
{
	XUPProjectItemList projects;
	
	foreach ( const XUPProjectModel* model, mOpenedProjectsModel->projects() ) {
		projects << model->rootProject();
	}
	
	return projects;
}

void XUPProjectManager::setCurrentProjectModel( XUPProjectModel* model )
{
	mFilteredModel->setSourceModel( model );
	cbProjects->setCurrentIndex( mOpenedProjectsModel->row( model ) );
}

void XUPProjectManager::setCurrentProject( XUPProjectItem* curProject, XUPProjectItem* preProject )
{
	// update project actions
	action( atClose )->setEnabled( curProject );
	action( atCloseAll )->setEnabled( curProject );
	action( atEdit )->setEnabled( curProject );
	action( atAddFiles )->setEnabled( curProject );
	action( atRemoveFiles )->setEnabled( curProject );
	
	if ( !curProject )
	{
		setCurrentProjectModel( 0 );
	}
	else if ( curProject->model() != currentProjectModel() )
	{
		setCurrentProjectModel( curProject->model() );
	}
	
	// if new project != old update gui
	if ( curProject != preProject )
	{
		emit currentProjectChanged( curProject, preProject );
		emit currentProjectChanged( curProject );
	}
}
