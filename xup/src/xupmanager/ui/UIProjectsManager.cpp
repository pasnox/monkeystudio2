#include "UIProjectsManager.h"
#include "QMakeProjectItem.h"
#include "ProjectItemModel.h"
#include "FilteredProjectItemModel.h"
#include "XUPManager.h"

#include <QFileDialog>
#include <QTextEdit>
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
	//connect( tvProxiedProjects->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( currentChanged( const QModelIndex&, const QModelIndex& ) ) );
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
	foreach ( ProjectItem* it, QList<ProjectItem*>() << pi << pi->children( true, true ) )
	{
		// expand needed items
		tvProjects->setExpanded( it->index(), QVariant( it->value( "expanded", "false" ) ).toBool() );
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
	if ( ProjectItem* it = mModel->itemFromIndex( tvProjects->currentIndex() ) )
		return it->project();
	return 0;
}

void UIProjectsManager::setCurrentProject( const ProjectItem* pi )
{
	if ( currentProject() != pi )
		tvProjects->setCurrentIndex( pi ? pi->index() : QModelIndex() );
	tbClose->setEnabled( pi );
	tbSave->setEnabled( pi && pi->modified() );
	tbSaveAs->setEnabled( pi );
	tbSettings->setEnabled( pi );
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
		if ( it->modified() && question( "Project modified, save it ?" ) )
			if ( !saveProject( it, QString() ) )
				warning( "An error occur while saving project" );
		it->closeProject();
		setCurrentProject( mModel->topLevelProjects().value( 0 ) );
	}
}

void UIProjectsManager::on_tbSave_clicked()
{
	if ( ProjectItem* it = currentProject() )
		if ( !saveProject( it, QString() ) )
			warning( "An error occur while saving project" );
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
	{
		QTextEdit* te = new QTextEdit;
		te->setPlainText( pi->toDomDocument().toString() );
		te->show();
	}
}

void UIProjectsManager::projectModified( ProjectItem* it, bool b )
{
	if ( it->project() == currentProject() )
		tbSave->setEnabled( b );
}

void UIProjectsManager::currentChanged( const QModelIndex& c, const QModelIndex& )
{
	/*
	if ( ProjectItem* it = mModel->filteredModel()->itemFromIndex( c ) )
		setCurrentProject( it->project() );
	*/
}

void UIProjectsManager::on_tvProxiedProjects_collapsed( const QModelIndex& i )
{
	/*
	if ( ProjectItem* it = mModel->filteredModel()->itemFromIndex( i ) )
		it->setValue( "expanded", "false" );
	*/
}

void UIProjectsManager::on_tvProxiedProjects_expanded( const QModelIndex& i )
{
	/*
	if ( ProjectItem* it = mModel->filteredModel()->itemFromIndex( i ) )
		it->setValue( "expanded", "true" );
	*/
}

void UIProjectsManager::on_tvProxiedProjects_doubleClicked( const QModelIndex& i )
{
	if ( ProjectItem* it = dynamic_cast<ProjectItem*>( mModel->filteredModel()->itemFromIndex( i ) ) )
	{
		const QString t = it->value( "type" );
		if ( t == "value" )
		{
			if ( XUPManager::fileVariables().contains( it->parent()->defaultValue() ) )
				warning( QString( "Double click file: %1" ).arg( it->filePath() ) );
		}
	}
}
