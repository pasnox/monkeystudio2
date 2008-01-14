#include "UIProjectsManager.h"
#include "QMakeProjectItem.h"
#include "ProjectItemModel.h"
#include "FilteredProjectItemModel.h"
#include "XUPManager.h"

#include <QFileDialog>
#include <QHeaderView>

using namespace XUPManager;

#include <QMessageBox>
void warning( const QString& s )
{ QMessageBox::warning( 0, "Warning...", s ); }

bool question( const QString& s )
{ return QMessageBox::question( 0, "Question...", s, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes; }

UIProjectsManager::UIProjectsManager( QWidget* w )
	: QDockWidget( w ), mModel( new ProjectItemModel( this ) ), aNew( 0 ), aOpen( 0 ), aSaveCurrent( 0 ), aSaveAll( 0 ),
		aCloseCurrent( 0 ), aCloseAll( 0 ), aAdd( 0 ), aRemove( 0 ), aSettings( 0 ), aSource( 0 )
{
	// setup ui
	setupUi( this );
	// temporary init
	initGui();
	// associate model
	tvProjects->setModel( mModel );
	tvProxiedProjects->setModel( mModel->filteredModel() );
	// hide headers
	tvProjects->header()->hide();
	tvProxiedProjects->header()->hide();
	// connection
	connect( tvProxiedProjects->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( this, SIGNAL( projectOpen( ProjectItem* ) ), this, SLOT( internal_projectOpen( ProjectItem* ) ) );
	connect( this, SIGNAL( projectAboutToClose( ProjectItem* ) ), this, SLOT( internal_projectAboutToClose( ProjectItem* ) ) );
	connect( this, SIGNAL( projectModifiedChanged( ProjectItem*, bool ) ), this, SLOT( internal_projectModifiedChanged( ProjectItem*, bool ) ) );
	connect( this, SIGNAL( currentProjectChanged( ProjectItem* ) ), this, SLOT( internal_currentProjectChanged( ProjectItem* ) ) );
	connect( this, SIGNAL( projectDoubleClicked( ProjectItem* ) ), this, SLOT( internal_projectDoubleClicked( ProjectItem* ) ) );
	connect( this, SIGNAL( fileDoubleClicked( ProjectItem*, const QString& ) ), this, SLOT( internal_fileDoubleClicked( ProjectItem*, const QString& ) ) );
	// set current proejct to null
	setCurrentProject( 0 );
}

UIProjectsManager::~UIProjectsManager()
{}

void UIProjectsManager::initGui()
{
	// create actions
	setActionNew( new QAction( QIcon(), tr( "New" ), this ) );
	setActionOpen( new QAction( QIcon(), tr( "Open" ), this ) );
	setActionSaveCurrent( new QAction( QIcon(), tr( "Save Current" ), this ) );
	setActionSaveAll( new QAction( QIcon(), tr( "Save All" ), this ) );
	setActionCloseCurrent( new QAction( QIcon(), tr( "Close Current" ), this ) );
	setActionCloseAll( new QAction( QIcon(), tr( "Close All" ), this ) );
	setActionAdd( new QAction( QIcon(), tr( "Add..." ), this ) );
	setActionRemove( new QAction( QIcon(), tr( "Remove..." ), this ) );
	setActionSettings( new QAction( QIcon(), tr( "Settings..." ), this ) );
	setActionSource( new QAction( QIcon(), tr( "Source..." ), this ) );
	// add actions to toolbar
	tbActions->addAction( actionNew() );
	tbActions->addAction( actionOpen() );
	tbActions->addAction( actionSaveCurrent() );
	tbActions->addAction( actionSaveAll() );
	tbActions->addAction( actionCloseCurrent() );
	tbActions->addAction( actionCloseAll() );
	tbActions->addAction( actionAdd() );
	tbActions->addAction( actionRemove() );
	tbActions->addAction( actionSettings() );
	tbActions->addAction( actionSource() );
}

void UIProjectsManager::initializeProject( ProjectItem* pi )
{
	// add project into model
	mModel->appendRow( pi );
	// tell proejct is open
	emit projectOpen( pi );
	// set current project
	setCurrentProject( pi );
	// connection
	connect( pi, SIGNAL( modifiedChanged( ProjectItem*, bool ) ), this, SIGNAL( projectModifiedChanged( ProjectItem*, bool ) ) );
	connect( pi, SIGNAL( aboutToClose( ProjectItem* ) ), this, SIGNAL( projectAboutToClose( ProjectItem* ) ) );
}

ProjectItem* UIProjectsManager::currentProject() const
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( tvProxiedProjects->currentIndex() ) )
		return fit->project();
	return 0;
}

void UIProjectsManager::setCurrentProject( const ProjectItem* pi )
{
	// update gui
	actionSaveCurrent()->setEnabled( pi && pi->modified() );
	actionSaveAll()->setEnabled( actionSaveCurrent()->isEnabled() );
	actionCloseCurrent()->setEnabled( pi );
	actionCloseAll()->setEnabled( pi );
	actionAdd()->setEnabled( pi );
	actionRemove()->setEnabled( currentValue() );
	actionSettings()->setEnabled( pi );
	actionSource()->setEnabled( pi );
	// change index
	if ( currentProject() != pi )
		tvProxiedProjects->setCurrentIndex( mModel->filteredModel()->mapFromSource( pi ? pi->index() : QModelIndex() ) );
}

ProjectItem* UIProjectsManager::currentValue() const
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( tvProxiedProjects->currentIndex() ) )
		return fit->item()->isType( "value" ) ? fit->item() : 0;
	return 0;
}

bool UIProjectsManager::openProject( const QString& s )
{
	ProjectItem* pi = 0;
	if ( QDir::match( "*.pro", s ) )
		pi = new QMakeProjectItem;
	else if ( QDir::match( "*.xup", s ) )
		pi = new ProjectItem;
	else
		return false;
	if ( pi->loadProject( s ) )
	{
		initializeProject( pi );
		return true;
	}
	delete pi;
	return false;
}

bool UIProjectsManager::saveProject( ProjectItem* pi, const QString& s )
{ return pi ? pi->saveProject( s ) : false; }

QAction* UIProjectsManager::actionNew() const
{ return aNew; }

void UIProjectsManager::setActionNew( QAction* a )
{
	if ( aNew != a )
	{
		if ( aNew )
			disconnect( aNew, SIGNAL( triggered() ), this, SLOT( actionNewTriggered() ) );
		aNew = a;
		if ( aNew )
			connect( aNew, SIGNAL( triggered() ), this, SLOT( actionNewTriggered() ) );
	}
}

QAction* UIProjectsManager::actionOpen() const
{ return aOpen; }

void UIProjectsManager::setActionOpen( QAction* a )
{
	if ( aOpen != a )
	{
		if ( aOpen )
			disconnect( aOpen, SIGNAL( triggered() ), this, SLOT( actionOpenTriggered() ) );
		aOpen = a;
		if ( aOpen )
			connect( aOpen, SIGNAL( triggered() ), this, SLOT( actionOpenTriggered() ) );
	}
}

QAction* UIProjectsManager::actionSaveCurrent() const
{ return aSaveCurrent; }

void UIProjectsManager::setActionSaveCurrent( QAction* a )
{
	if ( aSaveCurrent != a )
	{
		if ( aSaveCurrent )
			disconnect( aSaveCurrent, SIGNAL( triggered() ), this, SLOT( actionSaveCurrentTriggered() ) );
		aSaveCurrent = a;
		if ( aSaveCurrent )
			connect( aSaveCurrent, SIGNAL( triggered() ), this, SLOT( actionSaveCurrentTriggered() ) );
	}
}

QAction* UIProjectsManager::actionSaveAll() const
{ return aSaveAll; }

void UIProjectsManager::setActionSaveAll( QAction* a )
{
	if ( aSaveAll != a )
	{
		if ( aSaveAll )
			disconnect( aSaveAll, SIGNAL( triggered() ), this, SLOT( actionSaveAllTriggered() ) );
		aSaveAll = a;
		if ( aSaveAll )
			connect( aSaveAll, SIGNAL( triggered() ), this, SLOT( actionSaveAllTriggered() ) );
	}
}

QAction* UIProjectsManager::actionCloseCurrent() const
{ return aCloseCurrent; }

void UIProjectsManager::setActionCloseCurrent( QAction* a )
{
	if ( aCloseCurrent != a )
	{
		if ( aCloseCurrent )
			disconnect( aCloseCurrent, SIGNAL( triggered() ), this, SLOT( actionCloseCurrentTriggered() ) );
		aCloseCurrent = a;
		if ( aCloseCurrent )
			connect( aCloseCurrent, SIGNAL( triggered() ), this, SLOT( actionCloseCurrentTriggered() ) );
	}
}

QAction* UIProjectsManager::actionCloseAll() const
{ return aCloseAll; }

void UIProjectsManager::setActionCloseAll( QAction* a )
{
	if ( aCloseAll != a )
	{
		if ( aCloseAll )
			disconnect( aCloseAll, SIGNAL( triggered() ), this, SLOT( actionCloseAllTriggered() ) );
		aCloseAll = a;
		if ( aCloseAll )
			connect( aCloseAll, SIGNAL( triggered() ), this, SLOT( actionCloseAllTriggered() ) );
	}
}

QAction* UIProjectsManager::actionAdd() const
{ return aAdd; }

void UIProjectsManager::setActionAdd( QAction* a )
{
	if ( aAdd != a )
	{
		if ( aAdd )
			disconnect( aAdd, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );
		aAdd = a;
		if ( aAdd )
			connect( aAdd, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );
	}
}

QAction* UIProjectsManager::actionRemove() const
{ return aRemove; }

void UIProjectsManager::setActionRemove( QAction* a )
{
	if ( aRemove != a )
	{
		if ( aRemove )
			disconnect( aRemove, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );
		aRemove = a;
		if ( aRemove )
			connect( aRemove, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );
	}
}

QAction* UIProjectsManager::actionSettings() const
{ return aSettings; }

void UIProjectsManager::setActionSettings( QAction* a )
{
	if ( aSettings != a )
	{
		if ( aSettings )
			disconnect( aSettings, SIGNAL( triggered() ), this, SLOT( actionSettingsTriggered() ) );
		aSettings = a;
		if ( aSettings )
			connect( aSettings, SIGNAL( triggered() ), this, SLOT( actionSettingsTriggered() ) );
	}
}

QAction* UIProjectsManager::actionSource() const
{ return aSource; }

void UIProjectsManager::setActionSource( QAction* a )
{
	if ( aSource != a )
	{
		if ( aSource )
			disconnect( aSource, SIGNAL( triggered() ), this, SLOT( actionSourceTriggered() ) );
		aSource = a;
		if ( aSource )
			connect( aSource, SIGNAL( triggered() ), this, SLOT( actionSourceTriggered() ) );
	}
}

void UIProjectsManager::actionNewTriggered()
{ warning( tr( "Not Yet Implemented" ) ); }

void UIProjectsManager::actionOpenTriggered()
{
	const QString s = QFileDialog::getOpenFileName( window(), tr( "Choose a project to open..." ), QString(), tr( "All Projects (*.xup *.pro);;XUP Project (*.xup);;Qt Project (*.pro)" ) );
	if ( !s.isNull() && !openProject( s ) )
		warning( tr( "An error occur while opening project : %1" ).arg( s ) );
}

void UIProjectsManager::actionSaveCurrentTriggered()
{
	if ( ProjectItem* pi = currentProject() )
		if ( !saveProject( pi, QString() ) )
			warning( tr( "An error occur while saving project" ) );
}

void UIProjectsManager::actionSaveAllTriggered()
{
	foreach ( ProjectItem* pi, mModel->topLevelProjects() )
	{
		if ( !saveProject( pi, QString() ) )
			warning( tr( "An error occur while saving project: %1" ).arg( pi->defaultValue() ) );
		foreach ( ProjectItem* cpi, pi->children( true, false ) )
			if ( cpi->isProject() )
				if ( !saveProject( cpi, QString() ) )
					warning( tr( "An error occur while saving project: %1" ).arg( cpi->defaultValue() ) );
	}
}

void UIProjectsManager::actionCloseCurrentTriggered()
{
	if ( ProjectItem* pi = currentProject() )
		pi->closeProject();
}

void UIProjectsManager::actionCloseAllTriggered()
{
	foreach ( ProjectItem* pi, mModel->topLevelProjects() )
		pi->closeProject();
}

void UIProjectsManager::actionAddTriggered()
{ warning( tr( "Not Yet Implemented" ) ); }

void UIProjectsManager::actionRemoveTriggered()
{ warning( tr( "Not Yet Implemented" ) ); }

void UIProjectsManager::actionSettingsTriggered()
{ warning( tr( "Not Yet Implemented" ) ); }

void UIProjectsManager::actionSourceTriggered()
{
	if ( ProjectItem* pi = currentProject() )
		teLog->setPlainText( pi->toDomDocument().toString() );
}

void UIProjectsManager::currentChanged( const QModelIndex& c, const QModelIndex& o )
{
	// old
	FilteredProjectItem* ofit = mModel->filteredModel()->itemFromIndex( o );
	ProjectItem* op = ofit ? ofit->project() : 0;
	// current
	FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( c );
	ProjectItem* p = fit ? fit->project() : 0;
	// if new project != old update gui
	if ( op != p )
	{
		setCurrentProject( p );
		emit currentProjectChanged( p );
	}
}

void UIProjectsManager::on_tvProxiedProjects_collapsed( const QModelIndex& i )
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( i ) )
		fit->item()->setValue( "expanded", "false" );
}

void UIProjectsManager::on_tvProxiedProjects_expanded( const QModelIndex& i )
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( i ) )
		fit->item()->setValue( "expanded", "true" );
}

void UIProjectsManager::on_tvProxiedProjects_doubleClicked( const QModelIndex& i )
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( i ) )
	{
		ProjectItem* it = fit->item();
		if ( it->isProject() )
			emit projectDoubleClicked( it );
		else if ( it->isType( "value" ) && XUPManager::fileVariables().contains( it->parent()->defaultValue() ) )
			emit fileDoubleClicked( it, it->filePath() );
	}
}

void UIProjectsManager::internal_projectOpen( ProjectItem* pi )
{
	if ( pi )
	{
		teLog->append( tr( "Project Open: %1" ).arg( pi->defaultValue() ) );
		teLog->append( tr( "Restoring Session..." ) );
		// restore project
		foreach ( ProjectItem* it, QList<ProjectItem*>() << pi << pi->children( true, false ) )
		{
			// expand needed items
			if ( bool b = QVariant( it->value( "expanded", "false" ) ).toBool() )
				tvProxiedProjects->setExpanded( mModel->filteredModel()->mapFromSource( it->index() ), b );
			// if value and file based variable
			if ( it->value( "type" ) == "value" && fileVariables().contains( it->parent()->value( "name" ) ) )
			{
				/*
				// file configuration
				qWarning( "file name: %s", qPrintable( it->filePath() ) );
				qWarning( "file open: %s", qPrintable( it->value( "open", "false" ) ) );
				qWarning( "file cursor: %s", qPrintable( it->value( "cursor", "none" ) ) );
				qWarning( "file selection: %s", qPrintable( it->value( "selection", "none" ) ) );
				qWarning( "file folds: %s", qPrintable( it->value( "folds", "none" ) ) );
				qWarning( "file bookmarks: %s", qPrintable( it->value( "bookmarks", "none" ) ) );
				qWarning( "file breakpoint: %s", qPrintable( it->value( "breakpoints", "none" ) ) );
				*/
			}
		}
		teLog->append( tr( "Restoring Session Finished..." ) );
	}
}

void UIProjectsManager::internal_projectAboutToClose( ProjectItem* pi )
{
	if ( pi )
	{
		teLog->append( tr( "Project About To Close: %1" ).arg( pi->defaultValue() ) );
		// save project session
		teLog->append( tr( "Saving Session..." ) );
		// restore project
		foreach ( ProjectItem* it, QList<ProjectItem*>() << pi << pi->children( true, false ) )
		{
			// check files and set correct attribute on files item ( open, cursor position, ... )
			if ( it->value( "type" ) == "value" && fileVariables().contains( it->parent()->value( "name" ) ) )
			{
			}
		}
		teLog->append( tr( "Saving Session Finished..." ) );
		// save project if needed
		if ( pi->modified() && question( tr( "Project modified '%1', save it ?" ).arg( pi->defaultValue() ) ) )
			if ( !saveProject( pi, QString() ) )
				warning( tr( "An error occur while saving project" ) );
		// change current proejct to none if there is no longer project open
		if ( !mModel->topLevelProjects().count() )
			setCurrentProject( 0 );
	}
}

void UIProjectsManager::internal_projectModifiedChanged( ProjectItem* pi, bool b )
{
	if ( pi )
	{
		if ( pi->project() == currentProject() )
			actionSaveCurrent()->setEnabled( b );
		teLog->append( tr( "Project Modified Changed: %1, %2" ).arg( pi->defaultValue() ).arg( b ? tr( "True" ) : tr( "False" ) ) );
	}
}

void UIProjectsManager::internal_currentProjectChanged( ProjectItem* it )
{
	if ( it )
		teLog->append( tr( "Current Project Changed: %1" ).arg( it->defaultValue() ) );
}

void UIProjectsManager::internal_projectDoubleClicked( ProjectItem* it )
{
	if ( it )
		teLog->append( tr( "Project Double Clicked: %1" ).arg( it->defaultValue() ) );
}

void UIProjectsManager::internal_fileDoubleClicked( ProjectItem* it, const QString& s )
{
	if ( it )
		teLog->append( tr( "File Double Clicked: %1, %2" ).arg( it->defaultValue() ).arg( s ) );
}

