#include "XUPProjectManager.h"
#include "UIXUPFindFiles.h"
#include "pIconManager.h"
#include "XUPProjectModel.h"
#include "XUPFilteredProjectModel.h"

#include <QFileDialog>
#include <QTextCodec>

#include <QDebug>
#include <QMenu>
#include <QInputDialog>

XUPProjectManager::XUPProjectManager( QWidget* parent )
	: pDockWidget( parent )
{
	setupUi( this );
	
	// register generic xup project format
	XUPProjectItem* pItem = new XUPProjectItem();
	pItem->registerProjectType();
	
	tbActions->addAction( action( atNew ) );
	tbActions->addAction( action( atOpen ) );
	tbActions->addAction( action( atClose ) );
	tbActions->addAction( action( atCloseAll ) );
	tbActions->addAction( action( atEdit ) );
	tbActions->addAction( action( atAddFiles ) );
	tbActions->addAction( action( atRemoveFiles ) );
	tbActions->addAction( aDebug );
	
	mFilteredModel = new XUPFilteredProjectModel( tvFiltered );
	tvFiltered->setModel( mFilteredModel );
	
	mDebugMenu = new QMenu( this );
	aDebug->setMenu( mDebugMenu );
	
	mDebugMenu->addAction( "interpretValue" );
	mDebugMenu->addAction( "interpretVariable" );
	mDebugMenu->addAction( "project" );
	mDebugMenu->addAction( "topLevelProject" );
	mDebugMenu->addAction( "rootIncludeProject" );
	mDebugMenu->addAction( "editAttribute" );
	mDebugMenu->addAction( "removeItem" );
	mDebugMenu->addAction( "addItem" );
	mDebugMenu->addAction( "debugFilteredModel" );
	mDebugMenu->addAction( "projectSettingsDebug" );
	
	connect( mDebugMenu, SIGNAL( triggered( QAction* ) ), this, SLOT( debugMenu_triggered( QAction* ) ) );
}

XUPProjectManager::~XUPProjectManager()
{
	delete XUPProjectItem::projectInfos();
}

void XUPProjectManager::fileClosed( QObject* object )
{
	QWidget* widget = qobject_cast<QWidget*>( object ); // fuck bug on casting to QPlainTextEdit that return 0
	if ( widget )
		mOpenedFiles.remove( widget->windowFilePath() );
}

void XUPProjectManager::on_cbProjects_currentIndexChanged( int id )
{
	XUPProjectModel* project = cbProjects->itemData( id ).value<XUPProjectModel*>();
	setCurrentProject( project );
}

void XUPProjectManager::debugMenu_triggered( QAction* action )
{
	const QModelIndex index = tvNative->currentIndex(); // tvFiltered->currentIndex();
	if ( !index.isValid() )
		return;
	XUPItem* item = static_cast<XUPItem*>( index.internalPointer() ); // mFilteredModel->mapToSource( index );
	
	pteLog->appendPlainText( "------------------" );
	
	QString attribute;
	
	switch ( item->type() )
	{
		case XUPItem::Project:
			attribute = "name";
			break;
		case XUPItem::Comment:
			attribute = "value";
			break;
		case XUPItem::EmptyLine:
			attribute = "count";
			break;
		case XUPItem::Variable:
			attribute = "name";
			break;
		case XUPItem::Value:
			attribute = "content";
			break;
		case XUPItem::Function:
			attribute = "parameters";
			break;
		case XUPItem::Scope:
			attribute = "name";
			break;
		case XUPItem::DynamicFolder:
			attribute = "name";
			break;
		case XUPItem::Folder:
			attribute = "name";
			break;
		case XUPItem::File:
			attribute = "content";
			break;
		case XUPItem::Path:
			attribute = "content";
			break;
		case XUPItem::Unknow:
			break;
	}
	
	if ( action->text() == "interpretValue" )
	{
		if ( item->type() == XUPItem::Value )
		{
			pteLog->appendPlainText( item->attribute( attribute ).prepend( "Interpret value '" ).append( "'" ) );
			pteLog->appendPlainText( item->project()->rootIncludeProject()->interpretValue( item, attribute ) );
		}
	}
	else if ( action->text() == "interpretVariable" )
	{
		if ( item->type() == XUPItem::Variable )
		{
			pteLog->appendPlainText( item->attribute( attribute ).prepend( "Interpret variable '" ).append( "'" ) );
			pteLog->appendPlainText( item->project()->rootIncludeProject()->interpretVariable( item->attribute( attribute ), item, "#Null" ) );
		}
	}
	else if ( action->text() == "project" )
	{
		pteLog->appendPlainText( item->project()->displayText().prepend( "Project: " ) );
	}
	else if ( action->text() == "topLevelProject" )
	{
		pteLog->appendPlainText( item->project()->topLevelProject()->displayText().prepend( "Top level project: " ) );
	}
	else if ( action->text() == "rootIncludeProject" )
	{
		pteLog->appendPlainText( item->project()->rootIncludeProject()->displayText().prepend( "Root include project: " ) );
	}
	else if ( action->text() == "editAttribute" )
	{
		bool ok;
		QString value = QInputDialog::getText( this, "title", "label", QLineEdit::Normal, item->attribute( attribute ), &ok );
		if ( ok && !value.isEmpty() )
		{
			item->setAttribute( attribute, value );
		}
	}
	else if ( action->text() == "removeItem" )
	{
		XUPItem* parentItem = item->parent();
		if ( parentItem )
		{
			parentItem->removeChild( item );
		}
	}
	else if ( action->text() == "addItem" )
	{
		int row = qrand() % item->childCount();
		XUPItem* child = item->addChild( XUPItem::File, row );
		if ( child )
		{
			child->setAttribute( "content", QString( "my new item %1" ).arg( qrand() % 256 ) );
		}
	}
	else if ( action->text() == "debugFilteredModel" )
	{
		mFilteredModel->debug( item->project() );
	}
	else if ( action->text() == "projectSettingsDebug" )
	{
		XUPProjectItem* project = item->project();
		
		project->setProjectSettingsValue( "EDITOR", "SimpleQMakeEditor" );
		project->setProjectSettingsValue( "DEBUG_BINARY", "myappd" );
		project->setProjectSettingsValue( "RELEASE_BINARY", "myapp" );
		project->setProjectSettingsValue( "TEST", "merde" );
		
		project->addProjectSettingsValue( "TEST", "youpi" );
		
		QStringList values;
		values << project->projectSettingsValue( "EDITOR" );
		values << project->projectSettingsValue( "DEBUG_BINARY" );
		values << project->projectSettingsValue( "RELEASE_BINARY" );
		values << project->projectSettingsValue( "TEST" );
		pteLog->appendPlainText( values.join( " " ) );
		
		project->setProjectSettingsValue( "TEST", "okimichel" );
		pteLog->appendPlainText( project->projectSettingsValue( "TEST" ) );
	}
}

void XUPProjectManager::on_pbFiltered_toggled( bool checked )
{
	pbNative->setChecked( !checked );
	
	if ( checked )
	{
		swPages->setCurrentIndex( 0 );
	}
}

void XUPProjectManager::on_pbNative_toggled( bool checked )
{
	pbFiltered->setChecked( !checked );
	
	if ( checked )
	{
		swPages->setCurrentIndex( 1 );
	}
}

void XUPProjectManager::on_tvFiltered_activated( const QModelIndex& index )
{
	XUPItem* item = mFilteredModel->mapToSource( index );
	if ( item )
	{
		if ( item->type() == XUPItem::File )
		{
			XUPProjectItem* pItem = item->project()->rootIncludeProject();
			QString fn = pItem->filePath( pItem->interpretValue( item, "content" ) );
			
			if ( !QFile::exists( fn ) )
			{
				QString findFile = item->attribute( "content" ).remove( '"' );
				QFileInfoList files = pItem->findFile( findFile );
				switch ( files.count() )
				{
					case 0:
						fn.clear();
						break;
					case 1:
						fn = files.at( 0 ).absoluteFilePath();
						break;
					default:
					{
						UIXUPFindFiles dlg( findFile, this );
						dlg.setFiles( files, pItem->path() );
						fn.clear();
						if ( dlg.exec() )
						{
							fn = dlg.selectedFile();
						}
						break;
					}
				}
			}
			
			if ( QFile::exists( fn ) )
			{
				openFile( fn );
			}
		}
		
		// simply for testing XUPItem::index() member
		QModelIndex index = item->index();
		tvNative->setCurrentIndex( index );
		tvNative->setExpanded( index, true );
	}
}

void XUPProjectManager::on_tvNative_activated( const QModelIndex& index )
{
	QModelIndex proxyIndex = mFilteredModel->mapFromSource( static_cast<XUPItem*>( index.internalPointer() ) );
	on_tvFiltered_activated( proxyIndex );
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
			connect( action, SIGNAL( triggered() ), this, SLOT( newProject() ) );
			break;
		case XUPProjectManager::atOpen:
			action = new QAction( pIconManager::icon( "open.png", ":/project" ), tr( "Open project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( openProject() ) );
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
			connect( action, SIGNAL( triggered() ), this, SLOT( editProject() ) );
			break;
		case XUPProjectManager::atRemoveFiles:
			action = new QAction( pIconManager::icon( "remove.png", ":/project" ), tr( "Remove files from current project..." ), this );
			connect( action, SIGNAL( triggered() ), this, SLOT( editProject() ) );
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

void XUPProjectManager::addError( const QString& error )
{
	pteLog->appendPlainText( error );
}

bool XUPProjectManager::openFile( const QString& fileName, const QString& encoding )
{
	// check already open file
	QPlainTextEdit* pte = mOpenedFiles.value( fileName );
	if ( pte )
	{
		if ( pte->isMinimized() )
			pte->showNormal();
		pte->activateWindow();
		return true;
	}
	
	// get QFile
	QFile file( fileName );
	
	// check existence
	if ( !file.exists() )
	{
		addError( tr( "file not exists: %1" ).arg( fileName ) );
		return false;
	}
	
	// check is file
	if ( !QFileInfo( fileName ).isFile() )
	{
		addError( tr( "file is not a file: %1" ).arg( fileName ) );
		return false;
	}
	
	// try open it for reading
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		addError( tr( "Can't open file for reading: %1" ).arg( fileName ) );
		return false;
	}
	
	// decode content
	QTextCodec* codec = QTextCodec::codecForName( encoding.toUtf8() );
	QString buffer = codec->toUnicode( file.readAll() );
	
	pte = new QPlainTextEdit( this );
	pte->setWindowFlags( Qt::Window );
	pte->setAttribute( Qt::WA_DeleteOnClose );
	pte->setWindowFilePath( fileName );
	pte->setPlainText( buffer );
	connect( pte, SIGNAL( modificationChanged( bool )), pte, SLOT( setWindowModified( bool ) ) );
	connect( pte, SIGNAL( destroyed( QObject* )), this, SLOT( fileClosed( QObject* ) ) );
	
	mOpenedFiles[ fileName ] = pte;
	
	pte->show();
	
	return true;
}

bool XUPProjectManager::openProject( const QString& fileName, const QString& encoding )
{
	QFileInfo fi( fileName );
	
	if ( fi.exists() && fi.isFile() )
	{
		XUPProjectModel* project = new XUPProjectModel( this );
		if ( project->open( fileName ) )
		{
			int id = cbProjects->count();
			cbProjects->addItem( project->headerData( 0, Qt::Horizontal, Qt::DisplayRole ).toString(), QVariant::fromValue<XUPProjectModel*>( project ) );
			cbProjects->setItemIcon( id, project->headerData( 0, Qt::Horizontal, Qt::DecorationRole ).value<QIcon>() );
			cbProjects->setCurrentIndex( id );
			return true;
		}
		else
		{
			pteLog->appendPlainText( project->lastError() );
			delete project;
		}
	}
	
	return false;
}

bool XUPProjectManager::openProject()
{
	const QString fn = QFileDialog::getOpenFileName( this, tr( "Choose a project to open" ), QLatin1String( "." ), XUPProjectItem::projectInfos()->projectsFilter() );
	return openProject( fn );
	
}

void XUPProjectManager::XUPProjectManager::closeProject()
{
	XUPProjectModel* project = currentProject();
	if ( project )
	{
		if ( !project->save() )
		{
			pteLog->appendPlainText( project->lastError() );
		}
		project->close();
		int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( project ) );
		cbProjects->removeItem( id );
		delete project;
	}
}

void XUPProjectManager::closeAllProjects()
{
	while ( cbProjects->count() > 0 )
	{
		closeProject();
	}
}

void XUPProjectManager::editProject()
{
#warning uncomment XUPProjectManager::editProject
/*
	XUPProjectItem* project = currentProjectItem();
	
	if ( project )
	{
		UISimpleQMakeEditor editor( project, this );
		if ( editor.exec() == QDialog::Accepted )
		{
			
		}
	}
*/
}

XUPProjectModel* XUPProjectManager::currentProject() const
{
	return qobject_cast<XUPProjectModel*>( tvNative->model() );
}

XUPProjectItem* XUPProjectManager::currentProjectItem() const
{
	XUPProjectModel* model = currentProject();
	if ( !model )
	{
		return 0;
	}
	
	XUPProjectItem* project = 0;
	XUPItem* item = mFilteredModel->mapToSource( tvFiltered->currentIndex() );
	qWarning() << item;
	if ( item )
	{
		project = item->project();
	}
	else
	{
		project = model->mRootProject;
	}
	
	return project;
}

XUPItem* XUPProjectManager::currentItem() const
{
	const QModelIndex index = tvFiltered->currentIndex();
	return mFilteredModel->mapToSource( index );
}

XUPProjectItemList XUPProjectManager::topLevelProjects() const
{
	XUPProjectItemList projects;
	
	for ( int i = 0; i < cbProjects->count(); i++ )
	{
		XUPProjectItem* project = cbProjects->itemData( i ).value<XUPProjectModel*>()->mRootProject;
		projects << project;
	}
	
	return projects;
}

void XUPProjectManager::setCurrentProject( XUPProjectModel* project )
{
	mFilteredModel->setSourceModel( project );
	tvNative->setModel( project );
	
	int id = cbProjects->findData( QVariant::fromValue<XUPProjectModel*>( project ) );
	if ( cbProjects->currentIndex() != id )
	{
		cbProjects->setCurrentIndex( id );
	}
}
