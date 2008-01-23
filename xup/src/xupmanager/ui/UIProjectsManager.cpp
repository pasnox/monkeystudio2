#include "UIProjectsManager.h"
#include "ProjectItem.h"
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
	: QDockWidget( w ), mModel( new ProjectItemModel( this ) )
{
	// setup widget
	setupUi( this );
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
{
	// delete actions
	qDeleteAll( mActions );
	mActions.clear();
}

void UIProjectsManager::initGui()
{
	// create actions
	setAction( UIProjectsManager::New, new QAction( QIcon( ":/actions/new.png" ), tr( "New" ), this ) );
	setAction( UIProjectsManager::Open, new QAction( QIcon( ":/actions/open.png" ), tr( "Open" ), this ) );
	setAction( UIProjectsManager::SaveCurrent, new QAction( QIcon( ":/actions/save.png" ), tr( "Save Current" ), this ) );
	setAction( UIProjectsManager::SaveAll, new QAction( QIcon( ":/actions/saveall.png" ), tr( "Save All" ), this ) );
	setAction( UIProjectsManager::CloseCurrent, new QAction( QIcon( ":/actions/close.png" ), tr( "Close Current" ), this ) );
	setAction( UIProjectsManager::CloseAll, new QAction( QIcon( ":/actions/closeall.png" ), tr( "Close All" ), this ) );
	setAction( UIProjectsManager::Add, new QAction( QIcon( ":/actions/add.png" ), tr( "Add..." ), this ) );
	setAction( UIProjectsManager::Remove, new QAction( QIcon( ":/actions/remove.png" ), tr( "Remove..." ), this ) );
	setAction( UIProjectsManager::Settings, new QAction( QIcon( ":/actions/settings.png" ), tr( "Settings..." ), this ) );
	setAction( UIProjectsManager::Source, new QAction( QIcon( ":/actions/source.png" ), tr( "Source..." ), this ) );
	setAction( UIProjectsManager::Filtered, new QAction( QIcon( ":/actions/filtered.png" ), tr( "Filtered" ), this ) );
	// add actions to toolbar
	tbActions->addAction( action( UIProjectsManager::New ) );
	tbActions->addAction( action( UIProjectsManager::Open ) );
	tbActions->addAction( action( UIProjectsManager::SaveCurrent ) );
	tbActions->addAction( action( UIProjectsManager::SaveAll ) );
	tbActions->addAction( action( UIProjectsManager::CloseCurrent ) );
	tbActions->addAction( action( UIProjectsManager::CloseAll ) );
	tbActions->addAction( action( UIProjectsManager::Add ) );
	tbActions->addAction( action( UIProjectsManager::Remove ) );
	tbActions->addAction( action( UIProjectsManager::Settings ) );
	tbActions->addAction( action( UIProjectsManager::Source ) );
	tbActions->addAction( action( UIProjectsManager::Filtered ) );
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
	action( UIProjectsManager::SaveCurrent )->setEnabled( pi && pi->modified() );
	action( UIProjectsManager::SaveAll )->setEnabled( action( UIProjectsManager::SaveCurrent )->isEnabled() );
	action( UIProjectsManager::CloseCurrent )->setEnabled( pi );
	action( UIProjectsManager::CloseAll )->setEnabled( pi );
	action( UIProjectsManager::Add )->setEnabled( pi );
	action( UIProjectsManager::Remove )->setEnabled( currentValue() );
	action( UIProjectsManager::Settings )->setEnabled( pi );
	action( UIProjectsManager::Source )->setEnabled( pi );
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
	/*
	if ( QDir::match( "*.pro", s ) )
		pi = new QMakeProjectItem;
	else*/ if ( QDir::match( "*.xup", s ) )
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

QAction* UIProjectsManager::action( UIProjectsManager::Actions at ) const
{ return mActions.value( at ); }

void UIProjectsManager::setAction( UIProjectsManager::Actions at, QAction* a )
{
	// get old action if available
	QAction* oa = action( at );
	// check actions are differents
	if ( oa == a )
		return;
	// disconnect old action, connect new one
	switch ( at )
	{
		case UIProjectsManager::New:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionNewTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionNewTriggered() ) );
			break;
		case UIProjectsManager::Open:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionOpenTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionOpenTriggered() ) );
			break;
		case UIProjectsManager::SaveCurrent:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSaveCurrentTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSaveCurrentTriggered() ) );
			break;
		case UIProjectsManager::SaveAll:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSaveAllTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSaveAllTriggered() ) );
			break;
		case UIProjectsManager::CloseCurrent:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionCloseCurrentTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionCloseCurrentTriggered() ) );
			break;
		case UIProjectsManager::CloseAll:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionCloseAllTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionCloseAllTriggered() ) );
			break;
		case UIProjectsManager::Add:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );
			break;
		case UIProjectsManager::Remove:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );
			break;
		case UIProjectsManager::Settings:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSettingsTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSettingsTriggered() ) );
			break;
		case UIProjectsManager::Source:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSourceTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSourceTriggered() ) );
			break;
		case UIProjectsManager::Filtered:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionFilteredTriggered() ) );
			if ( a )
			{
				a->setCheckable( true );
				a->setChecked( swViews->currentWidget() == tvProxiedProjects );
				connect( a, SIGNAL( triggered() ), this, SLOT( actionFilteredTriggered() ) );
			}
			break;
		default:
			break;
	}
	// delete old action
	delete mActions.take( at );
	// set new action
	if ( a )
		mActions[at] = a;
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
		teLog->setPlainText( pi->domDocument().toString() );
}

void UIProjectsManager::actionFilteredTriggered()
{
	bool b = qobject_cast<QAction*>( sender() )->isChecked();
	swViews->setCurrentWidget( b ? tvProxiedProjects : tvProjects );
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
		else if ( it->isType( "value" ) && it->fileVariables().contains( it->parent()->defaultValue() ) )
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
			if ( it->value( "type" ) == "value" && it->fileVariables().contains( it->parent()->value( "name" ) ) )
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
			if ( it->value( "type" ) == "value" && pi->fileVariables().contains( it->parent()->value( "name" ) ) )
			{
				// to do
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
			action( UIProjectsManager::SaveCurrent )->setEnabled( b );
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

