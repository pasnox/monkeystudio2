#include "UIXUPManager.h"
#include "../ProjectItemModel.h"
#include "../FilteredProjectItemModel.h"
#include "../XUPItem.h"
#include "../XUPProjectItem.h"
#include "../AddFilesDialog.h"

#include <coremanager.h>
#include <pluginsmanager.h>
#include <templatesmanager.h>
#include <recentsmanager.h>
#include <monkey.h>

#include <QHeaderView>
#include <QInputDialog>

UIXUPManager::UIXUPManager( QWidget* w )
	: QDockWidget( w ), mModel( new ProjectItemModel( this ) )
{
	// register base item
	//registerItem( new XUPProjectItem ); // FIXME commented during dividing of classes
	// setup widget
	setupUi( this );
	// associate model
	tvProjects->setModel( mModel );
	tvProxiedProjects->setModel( mModel->filteredModel() );
	// hide headers
	tvProjects->header()->hide();
	tvProxiedProjects->header()->hide();
	// hide log
	teLog->hide();
	// connection
	connect( tvProxiedProjects->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( this, SIGNAL( projectOpen( XUPProjectItem* ) ), this, SLOT( internal_projectOpen( XUPProjectItem* ) ) );
	connect( this, SIGNAL( projectAboutToClose( XUPProjectItem* ) ), this, SLOT( internal_projectAboutToClose( XUPProjectItem* ) ) );
	connect( this, SIGNAL( projectClosed( XUPProjectItem* ) ), this, SLOT( internal_projectClosed( XUPProjectItem* ) ) );
	connect( this, SIGNAL( projectModifiedChanged( XUPProjectItem*, bool ) ), this, SLOT( internal_projectModifiedChanged( XUPProjectItem*, bool ) ) );
	connect( this, SIGNAL( currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ), this, SLOT( internal_currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ) );
	connect( this, SIGNAL( projectDoubleClicked( XUPProjectItem* ) ), this, SLOT( internal_projectDoubleClicked( XUPProjectItem* ) ) );
	connect( this, SIGNAL( fileDoubleClicked( XUPItem*, const QString& ) ), this, SLOT( internal_fileDoubleClicked( XUPItem*, const QString& ) ) );
	connect( tvProxiedProjects, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SIGNAL( projectCustomContextMenuRequested( const QPoint& ) ) );
}

UIXUPManager::~UIXUPManager()
{
	// delete actions
	qDeleteAll( mActions );
	mActions.clear();
	// delete registered items
	qDeleteAll( mRegisteredItems );
	mRegisteredItems.clear();
}

void UIXUPManager::initGui()
{
	// create actions
	setAction( UIXUPManager::Source, new QAction( QIcon( ":/actions/source.png" ), tr( "Source..." ), this ) );
	setAction( UIXUPManager::Filtered, new QAction( QIcon( ":/actions/filtered.png" ), tr( "Filtered" ), this ) );
	action( UIXUPManager::Filtered )->setChecked( false ); // FIXME deactivated while have problems with filtered model
	// add actions to toolbar
	tbActions->addAction( action( UIXUPManager::New ) );
	tbActions->addAction( action( UIXUPManager::Open ) );
	tbActions->addAction( action( UIXUPManager::SaveCurrent ) );
	tbActions->addAction( action( UIXUPManager::SaveAll ) );
	tbActions->addAction( action( UIXUPManager::CloseCurrent ) );
	tbActions->addAction( action( UIXUPManager::CloseAll ) );
	tbActions->addAction( action( UIXUPManager::Add ) );
	tbActions->addAction( action( UIXUPManager::Remove ) );
	tbActions->addAction( action( UIXUPManager::Settings ) );
	tbActions->addAction( action( UIXUPManager::Source ) );
	tbActions->addAction( action( UIXUPManager::Filtered ) );
	// set current proejct to null
	setCurrentProject( 0 );
}

ProjectItemModel* UIXUPManager::model() const
{ return mModel; }

void UIXUPManager::registerItem( XUPProjectItem* it )
{
	const QString s = it->metaObject()->className();
	if ( mRegisteredItems.keys().contains( s ) )
		delete mRegisteredItems[s];
	mRegisteredItems[s] = it;
}

void UIXUPManager::unRegisterItem( XUPProjectItem* it )
{
	const QString s = it->metaObject()->className();
	if ( mRegisteredItems.keys().contains( s ) )
		delete mRegisteredItems.take( s );
}

QStringList UIXUPManager::projectsFilters() const
{
	QStringList l, e;
	foreach ( XUPProjectItem* rpi, mRegisteredItems )
	{
		foreach ( QString label, rpi->suffixes().keys() )
		{
			e << rpi->suffixes().value( label );
			l << QString( "%1 (%2)" ).arg( label ).arg( rpi->suffixes().value( label ).join( " " ) );
		}
	}
	qSort( l );
	if ( l.count() > 1 )
		l.prepend( QString( tr( "All Project Files (%1)" ) ).arg( e.join( " " ) ) );
	return l;
}

void UIXUPManager::initializeProject( XUPProjectItem* pi )
{
	// add project into model
	mModel->appendRow( pi );
	// connection
	connect( pi, SIGNAL( modifiedChanged( XUPProjectItem*, bool ) ), this, SIGNAL( projectModifiedChanged( XUPProjectItem*, bool ) ) );
	connect( pi, SIGNAL( aboutToClose( XUPProjectItem* ) ), this, SIGNAL( projectAboutToClose( XUPProjectItem* ) ) );
	connect( pi, SIGNAL( closed( XUPProjectItem* ) ), this, SIGNAL( projectClosed( XUPProjectItem* ) ) );
	connect( pi, SIGNAL( installCommandRequested( const pCommand&, const QString& ) ), this, SIGNAL( projectInstallCommandRequested( const pCommand&, const QString& ) ) );
	connect( pi, SIGNAL( uninstallCommandRequested( const pCommand&, const QString& ) ), this, SIGNAL( projectUninstallCommandRequested( const pCommand&, const QString& ) ) );
	// tell proejct is open
	emit projectOpen( pi );
	// set current project
	setCurrentProject( pi );
}

QList<XUPProjectItem*> UIXUPManager::topLevelProjects() const
{ return mModel->topLevelProjects(); }

XUPProjectItem* UIXUPManager::currentProject() const
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( tvProxiedProjects->currentIndex() ) )
		return fit->project();
	return 0;
}

void UIXUPManager::setCurrentProject( const XUPProjectItem* pi )
{
	// update gui
	action( UIXUPManager::SaveCurrent )->setEnabled( pi && pi->modified() );
	action( UIXUPManager::SaveAll )->setEnabled( mModel->topLevelProjects().count() );
	action( UIXUPManager::CloseCurrent )->setEnabled( pi );
	action( UIXUPManager::CloseAll )->setEnabled( pi );
	action( UIXUPManager::Add )->setEnabled( pi );
	action( UIXUPManager::Remove )->setEnabled( currentValue() );
	action( UIXUPManager::Settings )->setEnabled( pi );
	action( UIXUPManager::Source )->setEnabled( pi );
	// change index
	if ( currentProject() != pi )
		tvProxiedProjects->setCurrentIndex( mModel->filteredModel()->mapFromSource( pi ? pi->index() : QModelIndex() ) );
}

XUPItem* UIXUPManager::currentValue() const
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( tvProxiedProjects->currentIndex() ) )
		return fit->item()->isType( "value" ) ? fit->item() : 0;
	return 0;
}

XUPItem* UIXUPManager::currentItem() const
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( tvProxiedProjects->currentIndex() ) )
		return fit->item()->isType( "folder" ) ? fit->project() : fit->item();
	return 0;
}

QAction* UIXUPManager::action( UIXUPManager::Actions at ) const
{ return mActions.value( at ); }

void UIXUPManager::setAction( UIXUPManager::Actions at, QAction* a )
{
	// get old action if available
	QAction* oa = action( at );
	// check actions are differents
	if ( oa == a )
		return;
	// disconnect old action, connect new one
	switch ( at )
	{
		case UIXUPManager::New:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionNewTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionNewTriggered() ) );
			break;
		case UIXUPManager::Open:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionOpenTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionOpenTriggered() ) );
			break;
		case UIXUPManager::SaveCurrent:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSaveCurrentTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSaveCurrentTriggered() ) );
			break;
		case UIXUPManager::SaveAll:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSaveAllTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSaveAllTriggered() ) );
			break;
		case UIXUPManager::CloseCurrent:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionCloseCurrentTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionCloseCurrentTriggered() ) );
			break;
		case UIXUPManager::CloseAll:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionCloseAllTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionCloseAllTriggered() ) );
			break;
		case UIXUPManager::Add:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionAddTriggered() ) );
			break;
		case UIXUPManager::Remove:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionRemoveTriggered() ) );
			break;
		case UIXUPManager::Settings:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSettingsTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSettingsTriggered() ) );
			break;
		case UIXUPManager::Source:
			if ( oa )
				disconnect( oa, SIGNAL( triggered() ), this, SLOT( actionSourceTriggered() ) );
			if ( a )
				connect( a, SIGNAL( triggered() ), this, SLOT( actionSourceTriggered() ) );
			break;
		case UIXUPManager::Filtered:
			if ( oa )
				disconnect( oa, SIGNAL( toggled( bool ) ), this, SLOT( actionFilteredToggled( bool ) ) );
			if ( a )
			{
				a->setCheckable( true );
				a->setChecked( swViews->currentWidget() == tvProxiedProjects );
				connect( a, SIGNAL( toggled( bool ) ), this, SLOT( actionFilteredToggled( bool ) ) );
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

void UIXUPManager::addFiles( XUPItem* scope, QWidget* parent )
{
	if ( !scope )
		return;
	// show dialog
	AddFilesDialog d( mModel->scopedModel(), scope, parent );
	if ( d.exec() )
	{
		QStringList files;
		foreach ( const QString& path, d.selectedFilesFolders() )
		{
			QFileInfo pi( path );
			if ( pi.isDir() )
			{
				QDir dir( path );
				foreach ( const QFileInfo& fi, pMonkeyStudio::getFiles( dir, QString::null, d.isRecursive() ) )
					files << fi.absoluteFilePath();
			}
			else
				files << path;
		}
		if ( !files.isEmpty() )
		{
			// import files if needed
			if ( d.importFiles() )
			{
				const QString projectPath = d.currentItem()->projectPath();
				const QString importPath = d.importPath();
				const QString importRootPath = d.directory().absolutePath();
				for ( int i = 0; i < files.count(); i++ )
				{
					if ( !files.at( i ).startsWith( projectPath ) )
					{
						QString fn = QString( files.at( i ) ).remove( importRootPath ).replace( "\\", "/" );
						fn = QDir::cleanPath( QString( "%1/%2/%3" ).arg( projectPath ).arg( importPath ).arg( fn ) );
						if ( d.directory().mkpath( QFileInfo( fn ).absolutePath() ) && QFile::copy( files.at( i ), fn ) )
							files[ i ] = fn;
					}
				}
			}
			// add files
			d.currentItem()->addFiles( files, d.currentItem(), d.currentOperator() );
		}
	}
}

void UIXUPManager::removeFiles( XUPItem* item, QWidget* parent )
{
	if ( !item || !item->isType( "value" ) )
		return;
	if ( pMonkeyStudio::question( tr( "Remove Value..." ), tr( "Are you sur you want to remove this value ?" ), parent ) )
	{
		// if file based
		if ( item->project()->fileVariables().contains( item->parent()->defaultValue() ) )
		{
			QString fp = item->filePath();
			if ( QFile::exists( fp ) && pMonkeyStudio::question( tr( "Delete associations..." ), tr( "Do you want to delete the associate file ?" ), parent ) )
				if ( !QFile::remove( fp ) )
					pMonkeyStudio::warning( tr( "Error..." ), tr( "Can't delete file: %1" ).arg( fp ), parent );
		}
		// get parent
		XUPItem* parent = item->parent();
		// remove item
		item->remove();
		// delete parent is empty
		if ( !parent->hasChildren() )
			parent->remove();
	}
}

bool UIXUPManager::openProject( const QString& s )
{
	// check that project is not yet open
	foreach ( XUPProjectItem* it, topLevelProjects() )
	{
		if ( pMonkeyStudio::isSameFile( it->projectFilePath(), s ) )
		{
			setCurrentProject( it );
			return true;
		}
	}

	// get project item
	XUPProjectItem* pi = 0;
	foreach ( XUPProjectItem* rpi, mRegisteredItems )
	{
		foreach ( QStringList suffixes, rpi->suffixes().values() )
		{
			foreach ( QString suffix, suffixes )
			{
				if ( QDir::match( suffix, s ) )
				{
					pi = rpi->clone( false );
					break;
				}
			}
		}
	}

	// if no suitable item cancel
	if ( !pi )
		return false;

	// load project
	if ( pi->loadProject( s ) )
	{
		initializeProject( pi );
		return true;
	}

	// can load project
	delete pi;
	return false;
}

bool UIXUPManager::saveProject( XUPProjectItem* pi, const QString& s )
{ return pi ? pi->saveProject( s ) : false; }

void UIXUPManager::actionNewTriggered()
{
	UITemplatesWizard* d = UITemplatesWizard::instance( this );
	d->setType( "Projects" );
	d->exec();
}

void UIXUPManager::actionOpenTriggered()
{
	// request user a project file
	const QString s = QFileDialog::getOpenFileName( window(), tr( "Choose a project to open..." ), QString::null, projectsFilters().join( ";;" ) );
	// if got file
	if ( !s.isNull() )
	{
		// try open project
		if ( openProject( s ) )
		{
			// append file to recents project
			MonkeyCore::recentsManager()->addRecentProject( s );
		}
		else
		{
			MonkeyCore::recentsManager()->removeRecentProject( s );
			// inform user about error
			pMonkeyStudio::warning( tr( "Error..." ), tr( "An error occur while opening project : %1" ).arg( s ) );
		}
	}
}

void UIXUPManager::actionSaveCurrentTriggered()
{
	if ( XUPProjectItem* pi = currentProject() )
		if ( pi->modified() && !saveProject( pi, QString() ) )
			pMonkeyStudio::warning( tr( "Error..." ), tr( "An error occur while saving project" ) );
}

void UIXUPManager::actionSaveAllTriggered()
{
	foreach ( XUPProjectItem* pi, mModel->topLevelProjects() )
	{
		if ( pi->modified() && !saveProject( pi, QString() ) )
			pMonkeyStudio::warning( tr( "Error..." ), tr( "An error occur while saving project: %1" ).arg( pi->defaultValue() ) );
		foreach ( XUPItem* cpi, pi->children( true, false ) )
		{
			XUPProjectItem* cpip = dynamic_cast<XUPProjectItem*> (cpi);
			if (cpip)
				if ( cpip->modified() && !saveProject( cpip, QString() ) )
					pMonkeyStudio::warning( tr( "Error..." ), tr( "An error occur while saving project: %1" ).arg( cpip->defaultValue() ) );
		}
	}
}

void UIXUPManager::actionCloseCurrentTriggered()
{
	mYesToAll = false;
	mNoToAll = false;
	if ( XUPProjectItem* pi = currentProject() )
		pi->closeProject();
}

void UIXUPManager::actionCloseAllTriggered()
{
	mYesToAll = false;
	mNoToAll = false;
	foreach ( XUPProjectItem* pi, mModel->topLevelProjects() )
		pi->closeProject();
}

void UIXUPManager::actionAddTriggered()
{
	if ( XUPItem* pi = currentItem() )
		addFiles( pi, window() );
}

void UIXUPManager::actionRemoveTriggered()
{
	if ( XUPItem* it = currentValue() )
		removeFiles( it, window() );
}

void UIXUPManager::actionSettingsTriggered()
{
	if ( XUPProjectItem* pi = currentProject() )
	{
		// get plugin name that can manage this project
		if ( pi->projectSettingsValue( "EDITOR" ).isEmpty() || !MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll, pi->projectSettingsValue( "EDITOR" ) ).value( 0 ) )
		{
			// get xup plugins
			QHash<QString, XUPPlugin*> xpl;
			foreach ( XUPPlugin* xp, MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll ) )
				xpl[ xp->infos().Caption ] = xp;
		
			bool ok;
			const QString caption = QInputDialog::getItem( window(), tr( "Choose an editor plugin..." ), tr( "Your project is not yet editable, please select a correct project settings plugin" ), xpl.keys(), 0, false, &ok );
			if ( ok && !caption.isEmpty() )
				pi->setProjectSettingsValue( "EDITOR", xpl[ caption ]->infos().Name );
		}
		
		// edit project settings
		if ( pi->projectSettingsValue( "EDITOR" ).isEmpty() )
			pMonkeyStudio::warning( tr( "Warning..." ), tr( "The project can't be edited because there is no associate project settings plugin." ) );
		else if ( XUPPlugin* xp = MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll, pi->projectSettingsValue( "EDITOR" ) ).value( 0 ) )
		{
			// get current filtered project in scoped view
			XUPProjectItem* curFilteredProject = mModel->scopedModel()->filteredProject();
			// set current filtered proejct to current one
			mModel->scopedModel()->setFilteredProject( pi );
			// edit project
			xp->editProject( pi );
			// restore filtered proejct
			mModel->scopedModel()->setFilteredProject( curFilteredProject );
		}
	}
}

void UIXUPManager::actionSourceTriggered()
{
	if ( XUPItem* pi = currentProject() )
		teLog->setPlainText( pi->domDocument().toString() );
}

void UIXUPManager::actionFilteredToggled( bool toggled )
{ swViews->setCurrentWidget( toggled ? tvProxiedProjects : tvProjects ); }

void UIXUPManager::currentChanged( const QModelIndex& c, const QModelIndex& o )
{
	// old
	FilteredProjectItem* ofit = mModel->filteredModel()->itemFromIndex( o );
	XUPProjectItem* op;
	if (ofit)
		op = dynamic_cast<XUPProjectItem*> (ofit->project());
	else
		op = NULL;
	// current
	FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( c );
	XUPProjectItem* p;
	if (fit)
		p = dynamic_cast<XUPProjectItem*> (fit->project());
	else
		p = NULL;
	// update some actions
	action( UIXUPManager::Remove )->setEnabled( currentValue() );
	// if new project != old update gui
	if ( op != p )
	{
		setCurrentProject( p );
		emit currentProjectChanged( p, op );
		emit currentProjectChanged( p );
	}
}

void UIXUPManager::on_tvProxiedProjects_collapsed( const QModelIndex& i )
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( i ) )
		fit->item()->setValue( "expanded", "false" );
}

void UIXUPManager::on_tvProxiedProjects_expanded( const QModelIndex& i )
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( i ) )
		fit->item()->setValue( "expanded", "true" );
}

void UIXUPManager::on_tvProxiedProjects_doubleClicked( const QModelIndex& i )
{
	if ( FilteredProjectItem* fit = mModel->filteredModel()->itemFromIndex( i ) )
	{
		XUPItem* it = fit->item();
		if ( it->isProject() )
			emit projectDoubleClicked( dynamic_cast<XUPProjectItem*> (it) );
		else if ( it->isType( "value" ) && it->project()->fileVariables().contains( it->parent()->defaultValue() ) )
		{
			const QString fp = it->filePath();
			emit fileDoubleClicked( it, fp );
			emit fileDoubleClicked( fp );
		}
	}
}

void UIXUPManager::internal_projectOpen( XUPProjectItem* pi )
{
	if ( pi )
	{
		teLog->append( tr( "Project Open: %1" ).arg( pi->defaultValue() ) );
		teLog->append( tr( "Restoring Session..." ) );
		// restore project
		foreach ( XUPItem* it, QList<XUPItem*>() << pi << pi->children( true, false ) )
		{
			// expand needed items
			if ( bool b = QVariant( it->value( "expanded", "false" ) ).toBool() )
				tvProxiedProjects->setExpanded( mModel->filteredModel()->mapFromSource( it->index() ), b );
			// if value and file based variable
			if ( it->value( "type" ) == "value" && it->project()->fileVariables().contains( it->parent()->value( "name" ) ) )
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

void UIXUPManager::internal_projectAboutToClose( XUPProjectItem* pi )
{
	if ( pi )
	{
		teLog->append( tr( "Project About To Close: %1" ).arg( pi->defaultValue() ) );
		// save project session
		teLog->append( tr( "Saving Session..." ) );
		foreach ( XUPItem* it, QList<XUPItem*>() << pi << pi->children( true, false ) )
		{
			// check files and set correct attribute on files item ( open, cursor position, ... )
			if ( it->value( "type" ) == "value" && pi->fileVariables().contains( it->parent()->value( "name" ) ) )
			{
				// to do
			}
		}
		teLog->append( tr( "Saving Session Finished..." ) );
		// save project if needed
		if ( pi->modified() )
		{
			// prepare dialog infos
			const QString title = tr( "Project Modified..." );
			const QString text = tr( "Project modified '%1', save it ?" ).arg( pi->defaultValue() );
			QMessageBox::StandardButton button = QMessageBox::NoButton;
			QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;
			if ( !mYesToAll && !mNoToAll )
			{
				buttons |= QMessageBox::YesToAll | QMessageBox::NoToAll;
				// query user
				button = QMessageBox::question( QApplication::activeWindow(), title, text, buttons, QMessageBox::Yes );
			}
			// update xxToAll state
			if ( button == QMessageBox::YesToAll )
				mYesToAll = true;
			else if ( button == QMessageBox::NoToAll )
				mNoToAll = true;
			// check if need save
			bool save = button == QMessageBox::Yes || mYesToAll;
			// try save else show error
			if ( save && !saveProject( pi, QString() ) )
				pMonkeyStudio::warning( tr( "Error..." ), tr( "An error occur while saving project" ) );
		}
	}
}

void UIXUPManager::internal_projectClosed( XUPProjectItem* pi )
{
	if ( pi )
	{
		teLog->append( tr( "Project Closed: %1" ).arg( pi->defaultValue() ) );
		// remove project from model
		pi->remove();
		// set current project to none if there is no longer opened project
		if ( !mModel->topLevelProjects().count() )
			setCurrentProject( 0 );
		else if ( !currentProject() )
			setCurrentProject( mModel->topLevelProjects().value( 0 ) );
	}
}

void UIXUPManager::internal_projectModifiedChanged( XUPProjectItem* pi, bool b )
{
	if ( pi )
	{
		if ( pi->project() == currentProject() )
			action( UIXUPManager::SaveCurrent )->setEnabled( b );
		action( UIXUPManager::SaveAll )->setEnabled( mModel->topLevelProjects().count() );
		teLog->append( tr( "Project Modified Changed: %1, %2" ).arg( pi->defaultValue() ).arg( b ? tr( "True" ) : tr( "False" ) ) );
	}
}

void UIXUPManager::internal_currentProjectChanged( XUPProjectItem* it, XUPProjectItem* )
{
	if ( it )
		teLog->append( tr( "Current Project Changed: %1" ).arg( it->defaultValue() ) );
}

void UIXUPManager::internal_projectDoubleClicked( XUPProjectItem* it )
{
	if ( it )
		teLog->append( tr( "Project Double Clicked: %1" ).arg( it->defaultValue() ) );
}

void UIXUPManager::internal_fileDoubleClicked( XUPItem* it, const QString& s )
{
	if ( it )
		teLog->append( tr( "File Double Clicked: %1, %2" ).arg( it->defaultValue() ).arg( s ) );
}
