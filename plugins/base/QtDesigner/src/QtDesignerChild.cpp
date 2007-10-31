#include "QtDesignerChild.h"
#include "UIMain.h"
#include "pWorkspace.h"
#include "pDockToolBarManager.h"
#include "pDockToolBar.h"
#include "pMonkeyStudio.h"

#include "QDesignerWidgetBox.h"
#include "QDesignerActionEditor.h"
#include "QDesignerPropertyEditor.h"
#include "QDesignerObjectInspector.h"
#include "QDesignerSignalSlotEditor.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPluginLoader>

#include <QDesignerComponents>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>
#include <QDesignerFormEditorPluginInterface>
#include <QDesignerObjectInspectorInterface>
#include <QDesignerPropertyEditorInterface>
#include <QDesignerActionEditorInterface>
#include <QDesignerFormWindowCursorInterface>
#include <QDesignerPropertySheetExtension>
#include <QExtensionManager>

#include "pluginmanager_p.h"
#include "qdesigner_integration_p.h"

using namespace pMonkeyStudio;

QtDesignerChild::QtDesignerChild()
	: pAbstractChild()
{
	// set icon
	setWindowIcon( QIcon( ":/icons/designer" ) );
	
	// set title
	setWindowTitle( tr( "Qt Designer" ) );
	
	// set frame style
	setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
	
	// create layout
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );
	
	// create toolbar
	vl->addWidget( mToolBar = new QToolBar );
	mToolBar->setIconSize( QSize( 16, 16 ) );
	
	// create mdiarea
	vl->addWidget( mArea = new QMdiArea );
	mArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	mArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	
	// set auto destroy
	setAttribute( Qt::WA_DeleteOnClose );
	
	// init designer core
	QDesignerComponents::initializeResources();
	mCore = QDesignerComponents::createFormEditor( this );
	(void) QDesignerComponents::createTaskMenu( mCore, this );
	
	// action group for modes
	QActionGroup* aModes = new QActionGroup( this );
	aModes->setExclusive( true );
	
	// create edit widget mode action
	QAction* aEditWidgets = new QAction( tr( "Edit Widgets" ), this );
	aEditWidgets->setIcon( QIcon( mCore->resourceLocation().append( "/widgettool.png" ) ) );
	aEditWidgets->setCheckable( true );
	aEditWidgets->setChecked( true );
	
	// add edit widget action to group
	aModes->addAction( aEditWidgets );
	
	// initialize designer plugins
	foreach ( QObject* o, QPluginLoader::staticInstances() << mCore->pluginManager()->instances() )
	{
		if (  QDesignerFormEditorPluginInterface* fep = qobject_cast<QDesignerFormEditorPluginInterface*>( o ) )
		{
			// initialize plugin if needed
			if ( !fep->isInitialized() )
				fep->initialize( mCore );
			
			// set action chackable
			fep->action()->setCheckable( true );
			
			// add action mode to group
			aModes->addAction( fep->action() );
		}
	}
	
	// add actions to toolbar
	QDesignerFormWindowManagerInterface* fwm = mCore->formWindowManager();
	// edit
	fwm->actionUndo()->setIcon( QIcon( ":/icons/undo.png" ) );
	fwm->actionRedo()->setIcon( QIcon( ":/icons/redo.png" ) );
	fwm->actionDelete()->setIcon( QIcon( ":/icons/delete.png" ) );
	fwm->actionSelectAll()->setIcon( QIcon( ":/icons/selectall.png" ) );
	mToolBar->addAction( fwm->actionUndo() );
	mToolBar->addAction( fwm->actionRedo() );
	mToolBar->addAction( fwm->actionCut() );
	mToolBar->addAction( fwm->actionCopy() );
	mToolBar->addAction( fwm->actionPaste() );
	mToolBar->addAction( fwm->actionLower() );
	mToolBar->addAction( fwm->actionRaise() );
	mToolBar->addAction( fwm->actionDelete() );
	mToolBar->addAction( fwm->actionSelectAll() );
	mToolBar->addSeparator();
	// tools
	mToolBar->addActions( aModes->actions() );
	mToolBar->addSeparator();
	// form
	mToolBar->addAction( fwm->actionHorizontalLayout() );
	mToolBar->addAction( fwm->actionVerticalLayout() );
	mToolBar->addAction( fwm->actionSplitHorizontal() );
	mToolBar->addAction( fwm->actionSplitVertical() );
	mToolBar->addAction( fwm->actionGridLayout() );
	mToolBar->addAction( fwm->actionBreakLayout() );
	mToolBar->addAction( fwm->actionAdjustSize() );
	
	// initialize plugins
	QDesignerComponents::initializePlugins( mCore );
	
	pWidgetBox = new QDesignerWidgetBox( this );
	pWidgetBox->setVisible( false );
	UIMain::instance()->dockToolBar( Qt::LeftToolBarArea )->addDock( pWidgetBox, tr( "Widget Box" ), QIcon( ":/icons/widget.png" ) );
	
	pObjectInspector = new QDesignerObjectInspector( this );
	pObjectInspector->setVisible( false );
	UIMain::instance()->dockToolBar( Qt::RightToolBarArea )->addDock( pObjectInspector, tr( "Object Inspector" ), QIcon( ":/icons/inspector.png" ) );
	
	pPropertyEditor = new QDesignerPropertyEditor( this );
	pPropertyEditor->setVisible( false );
	UIMain::instance()->dockToolBar( Qt::RightToolBarArea )->addDock( pPropertyEditor, tr( "Property Editor" ), QIcon( ":/icons/property.png" ) );
	
	pActionEditor = new QDesignerActionEditor( this );
	pActionEditor->setVisible( false );
	UIMain::instance()->dockToolBar( Qt::BottomToolBarArea )->addDock( pActionEditor, tr( "Action Editor" ), QIcon( ":/icons/action.png" ) );
	
	pSignalSlotEditor = new QDesignerSignalSlotEditor( this );
	pSignalSlotEditor->setVisible( false );
	UIMain::instance()->dockToolBar( Qt::TopToolBarArea )->addDock( pSignalSlotEditor, tr( "Signal/Slot Editor" ), QIcon( ":/icons/signal.png" ) );
	
	// perform integration
	new qdesigner_internal::QDesignerIntegration( mCore, this );
	mCore->setTopLevel( UIMain::instance() );
	
	// connection
	connect( mArea, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( subWindowActivated( QMdiSubWindow* ) ) );
	connect( mCore->formWindowManager(), SIGNAL( activeFormWindowChanged( QDesignerFormWindowInterface* ) ), this, SLOT( activeFormWindowChanged( QDesignerFormWindowInterface* ) ) );
	connect( aEditWidgets, SIGNAL( triggered() ), this, SLOT( editWidgets() ) );
}

QtDesignerChild::~QtDesignerChild()
{
	// close all files
	closeFiles();
	// delete designer docks
	delete pWidgetBox;
	delete pActionEditor;
	delete pPropertyEditor;
	delete pObjectInspector;
	delete pSignalSlotEditor;
}

QDesignerFormEditorInterface* QtDesignerChild::core()
{ return mCore; }

QDesignerFormWindowInterface* QtDesignerChild::createForm()
{
	// create form instance
	QDesignerFormWindowInterface* w = mCore->formWindowManager()->createFormWindow();
	// set form default features
	w->setFeatures( QDesignerFormWindowInterface::DefaultFeature );
	// return form instance
	return w;
}

void QtDesignerChild::subWindowActivated( QMdiSubWindow* w )
{ mCore->formWindowManager()->setActiveFormWindow( w ? qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) : 0 ); }

void QtDesignerChild::activeFormWindowChanged( QDesignerFormWindowInterface* w )
{
	// disconnect form selection changed
	if ( w )
	{
		disconnect( w, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
		disconnect( w, SIGNAL( changed() ), this, SLOT( formChanged() ) );
		disconnect( w, SIGNAL( geometryChanged() ), this, SLOT( geometryChanged() ) );
	}
	
	// inspector
	pObjectInspector->interface()->setFormWindow( w );
	
	// set new object for property editor
	pPropertyEditor->interface()->setObject( w );
	
	// set new object for action editor
	pActionEditor->interface()->setFormWindow( w );
	
	// connect form selection changed
	if ( w )
	{
		connect( w, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
		connect( w, SIGNAL( changed() ), this, SLOT( formChanged() ) );
		connect( w, SIGNAL( geometryChanged() ), this, SLOT( geometryChanged() ) );
	}
	
	// update window state
	//setWindowTitle( tr( "Qt Designer" ).append( w ? QString( " [%1]" ).arg( QFileInfo( w->fileName() ).fileName() ) : QString::null ) );
	//setWindowModified( w ? w->isDirty() : false );
	
	// emit signals
	emit currentFileChanged( w ? w->fileName() : QString() );
}

void QtDesignerChild::selectionChanged()
{
	// set current object for property editor
	if ( QDesignerFormWindowInterface* w = mCore->formWindowManager()->activeFormWindow() )
		pPropertyEditor->interface()->setObject( w->cursor()->hasSelection() ? w->cursor()->selectedWidget( 0 ) : w->mainContainer() );
}

void QtDesignerChild::geometryChanged()
{
	return;
	// update widget property
	if ( QDesignerFormWindowInterface* w = qobject_cast<QDesignerFormWindowInterface*>( sender() ) )
		w->cursor()->setWidgetProperty( w->mainContainer(), "geometry", w->mainContainer()->geometry() );
}

void QtDesignerChild::formChanged()
{
	if ( QDesignerFormWindowInterface* w = qobject_cast<QDesignerFormWindowInterface*>( sender() ) )
	{
		// update window state
		//setWindowModified( w->isDirty() );
		
		// hm create updateChildStateRequested ?!
		if ( mArea->currentSubWindow() == w->parentWidget() )
		{
			emit modifiedChanged( w->isDirty() );
			emit undoAvailableChanged( isUndoAvailable() );
			emit redoAvailableChanged( isRedoAvailable() );
			emit pasteAvailableChanged( isPasteAvailable() );
			emit copyAvailableChanged( isCopyAvailable() );
		}
	}
}

void QtDesignerChild::editWidgets()
{
	// set edit mode for all forms
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
			i->editWidgets();
}

void QtDesignerChild::openFile( const QString& s, const QPoint&, QTextCodec* )
{
	// create form
	QDesignerFormWindowInterface* w = createForm();
	
	// assign file name
	w->setFileName( s );
	
	// add filename to list
	mFiles.append( s );
	
	QSize mainContainerSize( 400, 300 );
	
	// set contents
	if ( QFile::exists( s ) )
	{
		// set content
		QFile f( s );
		w->setContents( &f );
		mainContainerSize = w->mainContainer()->geometry().size();
		
		// set clean
		w->setDirty( false );
	}
	else
	{
		w->setMainContainer( new QWidget() );
		w->setDirty( true );
	}
	
	// add to mdi area
	mArea->addSubWindow( w, Qt::CustomizeWindowHint | Qt::SubWindow | Qt::WindowShadeButtonHint | Qt::WindowSystemMenuHint );
	
	// resize to original container size
	QMdiSubWindow* mw = qobject_cast<QMdiSubWindow*>( w->parentWidget() );
	const QSize decorationSize = mw->size() -mw->contentsRect().size();
    mw->resize( mainContainerSize +decorationSize );
	
	// set it visible
	w->setVisible( true );
	
	// set window title
	w->setWindowTitle( QFileInfo( s ).fileName() +"[*]" );
	
	// emit not modified state
	//modifiedChanged( w->isDirty() );
	
	// emit file opened
	emit fileOpened( s );
}

void QtDesignerChild::closeFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
	{
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
		{
			if ( isSameFile( i->fileName(), s ) )
			{
				// close form
				w->close();
				// remove from files list
				mFiles.removeAll( s );
				// emit file closed
				emit fileClosed( s );
			}
		}
	}
}

void QtDesignerChild::closeFiles()
{
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
	{
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
		{
			// get filename
			const QString s = i->fileName();
			// close form
			w->close();
			// remove from files list
			mFiles.removeAll( s );
			// emit file closed
			emit fileClosed( s );
		}
	}
}

QStringList QtDesignerChild::files() const
{
	QStringList l;
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
			l << i->fileName();
	return l;
}

QPoint QtDesignerChild::cursorPosition() const
{ return QPoint( -1, -1 ); }

void QtDesignerChild::showFile( const QString& s )
{
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
	{
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
		{
			if ( isSameFile( i->fileName(), s ) )
			{
				mArea->setActiveSubWindow( w );
				break;
			}
		}
	}
}

QString QtDesignerChild::currentFile() const
{
	if ( mArea->currentSubWindow() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( mArea->currentSubWindow()->widget() ) )
			return i->fileName();
	return QString();
}

QString QtDesignerChild::currentFileName() const
{ return QFileInfo( currentFile() ).fileName(); }

bool QtDesignerChild::isModified() const
{
	if ( mArea->currentSubWindow() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( mArea->currentSubWindow()->widget() ) )
			return i->isDirty();
	return false;
}

bool QtDesignerChild::isUndoAvailable() const
{ return mCore->formWindowManager()->actionUndo()->isEnabled(); }

bool QtDesignerChild::isRedoAvailable() const
{ return mCore->formWindowManager()->actionRedo()->isEnabled(); }

bool QtDesignerChild::isPasteAvailable() const
{ return mCore->formWindowManager()->actionPaste()->isEnabled(); }

bool QtDesignerChild::isCopyAvailable() const
{ return mCore->formWindowManager()->actionCopy()->isEnabled(); }

bool QtDesignerChild::isModified( const QString& s ) const
{
	if ( !mFiles.contains( s ) )
		return false;
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
			if ( isSameFile( i->fileName(), s ) )
				return i->isDirty();
	return false;
}

void QtDesignerChild::saveFile( const QString& s )
{
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
	{
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
		{
			if ( isSameFile( i->fileName(), s ) )
			{
				if ( !i->isDirty() )
					return;
				QFile f( i->fileName() );
				if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) )
				{
					f.resize( 0 );
					f.write( qPrintable( i->contents() ) );
					f.close();
					i->setDirty( false );
					emit modifiedChanged( false );
				}
				else
					warning( tr( "Save Form..." ), tr( "An error occurs when saving :\n%1" ).arg( s ) );
				return;
			}
		}
	}
}

void QtDesignerChild::saveFiles()
{
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
	{
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
		{
			if ( !i->isDirty() )
				continue;
			QFile f( i->fileName() );
			if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) )
			{
				f.resize( 0 );
				f.write( qPrintable( i->contents() ) );
				f.close();
				i->setDirty( false );
				if ( mArea->currentSubWindow() == w )
					emit modifiedChanged( false );
			}
			else
				warning( tr( "Save Form..." ), tr( "An error occurs when saving :\n%1" ).arg( i->fileName() ) );
		}
	}
}

void QtDesignerChild::printFile( const QString& )
{}

void QtDesignerChild::quickPrintFile( const QString& )
{}

void QtDesignerChild::undo()
{ mCore->formWindowManager()->actionUndo()->trigger(); }

void QtDesignerChild::redo()
{ mCore->formWindowManager()->actionRedo()->trigger(); }

void QtDesignerChild::cut()
{ mCore->formWindowManager()->actionCut()->trigger(); }

void QtDesignerChild::copy()
{ mCore->formWindowManager()->actionCopy()->trigger(); }

void QtDesignerChild::paste()
{ mCore->formWindowManager()->actionPaste()->trigger(); }

void QtDesignerChild::searchReplace()
{}

void QtDesignerChild::goTo()
{}

void QtDesignerChild::backupCurrentFile( const QString& s )
{
	if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( mArea->currentSubWindow()->widget() ) )
	{
		QFile f( s );
		if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) )
		{
			f.resize( 0 );
			f.write( qPrintable( i->contents() ) );
			f.close();
		}
		else
			warning( tr( "Backup Form..." ), tr( "An error occurs when backuping :\n%1" ).arg( s ) );
	}
}

bool QtDesignerChild::isSearchReplaceAvailable() const
{ return false; }

bool QtDesignerChild::isGoToAvailable() const
{ return false; }

bool QtDesignerChild::isPrintAvailable() const
{ return false; }
