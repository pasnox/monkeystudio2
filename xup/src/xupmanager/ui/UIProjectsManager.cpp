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
	: QDockWidget( w ), mModel( new ProjectItemModel( this ) )
{
	// setup ui
	setupUi( this );
	// associate model
	tvProjects->setModel( mModel );
	tvProxiedProjects->setModel( mModel->filteredModel() );
	// hide headers
	tvProjects->header()->hide();
	tvProxiedProjects->header()->hide();
	// connection
	connect( tvProxiedProjects->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	
	connect( this, SIGNAL( projectOpen( ProjectItem* ) ), this, SLOT( on_projectOpen( ProjectItem* ) ) );
	connect( this, SIGNAL( projectAboutToClose( ProjectItem* ) ), this, SLOT( on_projectAboutToClose( ProjectItem* ) ) );
	connect( this, SIGNAL( projectModifiedChanged( ProjectItem*, bool ) ), this, SLOT( on_projectModifiedChanged( ProjectItem*, bool ) ) );
	connect( this, SIGNAL( currentProjectChanged( ProjectItem* ) ), this, SLOT( on_currentProjectChanged( ProjectItem* ) ) );
	connect( this, SIGNAL( projectDoubleClicked( ProjectItem* ) ), this, SLOT( on_projectDoubleClicked( ProjectItem* ) ) );
	connect( this, SIGNAL( fileDoubleClicked( ProjectItem*, const QString& ) ), this, SLOT( on_fileDoubleClicked( ProjectItem*, const QString& ) ) );
	
	// set current proejct to null
	setCurrentProject( 0 );
}

UIProjectsManager::~UIProjectsManager()
{}

void UIProjectsManager::initializeProject( ProjectItem* pi )
{
	// add project into model
	mModel->appendRow( pi );
	// connection
	connect( pi, SIGNAL( modifiedChanged( ProjectItem*, bool ) ), this, SLOT( projectModified( ProjectItem*, bool ) ) );
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
	// set current project
	setCurrentProject( pi );
	// tell proejct is open
	emit projectOpen( pi );
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
	tbClose->setEnabled( pi );
	tbSave->setEnabled( pi && pi->modified() );
	tbSaveAs->setEnabled( pi );
	tbSettings->setEnabled( pi );
	
	// change index and emit signal
	if ( currentProject() != pi )
	{
		tvProxiedProjects->setCurrentIndex( mModel->filteredModel()->mapFromSource( pi ? pi->index() : QModelIndex() ) );
		emit currentProjectChanged( const_cast<ProjectItem*>( pi ) );
	}
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

void UIProjectsManager::on_tbOpen_clicked()
{
	const QString s = QFileDialog::getOpenFileName( window(), tr( "Choose a project to open..." ), QString(), tr( "All Projects (*.xup *.pro);;XUP Project (*.xup);;Qt Project (*.pro)" ) );
	if ( !s.isNull() && !openProject( s ) )
		warning( tr( "An error occur while opening project : %1" ).arg( s ) );
}

void UIProjectsManager::on_tbClose_clicked()
{
	if ( ProjectItem* it = currentProject() )
	{
		if ( it->modified() && question( tr( "Project modified '%1', save it ?" ).arg( it->defaultValue() ) ) )
			if ( !saveProject( it, QString() ) )
				warning( tr( "An error occur while saving project" ) );
		emit projectAboutToClose( it );
		it->closeProject();
		if ( !mModel->topLevelProjects().count() )
			setCurrentProject( 0 );
	}
}

void UIProjectsManager::on_tbSave_clicked()
{
	if ( ProjectItem* it = currentProject() )
		if ( !saveProject( it, QString() ) )
			warning( tr( "An error occur while saving project" ) );
}

void UIProjectsManager::on_tbSaveAs_clicked()
{
	if ( ProjectItem* it = currentProject() )
	{
		const QString s = QFileDialog::getSaveFileName( window(), tr( "Save project..." ), QString(), tr( "XUP Project (*.xup);;Qt Project (*.pro)" ) );
		if ( !s.isNull() && !saveProject( it, s ) )
			warning( tr( "An error occur while saving project as : %1" ).arg( s ) );
	}
}

void UIProjectsManager::on_tbSettings_clicked()
{}

void UIProjectsManager::on_tbSources_clicked()
{
	if ( ProjectItem* pi = currentProject() )
		teLog->setPlainText( pi->toDomDocument().toString() );
}

void UIProjectsManager::projectModified( ProjectItem* it, bool b )
{
	if ( it->project() == currentProject() )
		tbSave->setEnabled( b );
	emit projectModifiedChanged( it, b );
}

void UIProjectsManager::currentChanged( const QModelIndex& c, const QModelIndex& )
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( c ) )
		setCurrentProject( fit->project() );
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
		else if ( it->value( "type" ) == "value" && XUPManager::fileVariables().contains( it->parent()->defaultValue() ) )
			emit fileDoubleClicked( it, it->filePath() );
	}
}




void UIProjectsManager::on_projectOpen( ProjectItem* it )
{
	teLog->append( tr( "Project Open: %1" ).arg( it->defaultValue() ) );
}

void UIProjectsManager::on_projectAboutToClose( ProjectItem* it )
{
	teLog->append( tr( "Project About To Close: %1" ).arg( it->defaultValue() ) );
}

void UIProjectsManager::on_projectModifiedChanged( ProjectItem* it, bool b )
{
	teLog->append( tr( "Project Modified Changed: %1, %2" ).arg( it->defaultValue() ).arg( b ? tr( "True" ) : tr( "False" ) ) );
}

void UIProjectsManager::on_currentProjectChanged( ProjectItem* it )
{
	teLog->append( tr( "Current Project Changed: %1" ).arg( it->defaultValue() ) );
}

void UIProjectsManager::on_projectDoubleClicked( ProjectItem* it )
{
	teLog->append( tr( "Project Double Clicked: %1" ).arg( it->defaultValue() ) );
}

void UIProjectsManager::on_fileDoubleClicked( ProjectItem* it, const QString& s )
{
	teLog->append( tr( "File Double Clicked: %1, %2" ).arg( it->defaultValue() ).arg( s ) );
}

