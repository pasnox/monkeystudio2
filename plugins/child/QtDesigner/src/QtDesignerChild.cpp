#include "QtDesignerChild.h"
#include "UIMain.h"
#include "pWorkspace.h"
#include "pDockToolBarManager.h"
#include "pDockToolBar.h"
#include "pMonkeyStudio.h"
#include "pMenuBar.h"

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
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QBuffer>
#include <QFormBuilder>

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
	
	// preview action
	aPreview = new QAction( this );
	aPreview->setText( tr( "Preview Form" ) );
	
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
	fwm->actionUndo()->setShortcut( pMenuBar::instance()->action( "mEdit/aUndo" )->shortcut() );
	fwm->actionRedo()->setIcon( QIcon( ":/icons/redo.png" ) );
	fwm->actionRedo()->setShortcut( pMenuBar::instance()->action( "mEdit/aRedo" )->shortcut() );
	fwm->actionDelete()->setIcon( QIcon( ":/icons/delete.png" ) );
	fwm->actionSelectAll()->setIcon( QIcon( ":/icons/selectall.png" ) );
	fwm->actionDelete()->setShortcut( tr( "Del" ) );
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
	// preview
	mToolBar->addSeparator();
	aPreview->setIcon( QIcon( ":/icons/preview.png" ) );
	aPreview->setShortcut( tr( "Ctrl+R" ));
	mToolBar->addAction( aPreview );
	
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
	connect( aPreview, SIGNAL( triggered() ), this, SLOT( previewCurrentForm() ) );
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

bool QtDesignerChild::eventFilter( QObject* o, QEvent* e )
{
	if ( o && e->type() == QEvent::Close )
		if ( QDesignerFormWindowInterface* w = qobject_cast<QDesignerFormWindowInterface*>( o ) )
			if ( w->property( "askSave" ).toBool() && w->isDirty() )
				if ( question( tr( "Form Modified..." ), tr( "The form has been modified, do you want to save it ?" ), window() ) )
					saveFile( w->fileName() );
	return pAbstractChild::eventFilter( o, e );
}

void QtDesignerChild::subWindowActivated( QMdiSubWindow* w )
{
	// update current form
	if ( w && mCore->formWindowManager()->activeFormWindow() != w->widget() )
		mCore->formWindowManager()->setActiveFormWindow( qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) );
	
	// update preview actino state
	aPreview->setEnabled( w );
}

void QtDesignerChild::activeFormWindowChanged( QDesignerFormWindowInterface* w )
{
	// update window state
	setModified( w );
	
	// emit signals
	emit currentFileChanged( w ? w->fileName() : QString() );
}

void QtDesignerChild::geometryChanged()
{
	// update widget property
	if ( QDesignerFormWindowInterface* w = qobject_cast<QDesignerFormWindowInterface*>( sender() ) )
	{
		// don t take care of first geometry changed
		if ( w->property( "firstGeometryChanged" ).toBool() )
		{
			w->setProperty( "firstGeometryChanged", false );
			return;
		}
		
		// update property
		QDesignerPropertySheetExtension* s = qt_extension<QDesignerPropertySheetExtension*>( mCore->extensionManager(), w );

		mCore->propertyEditor()->setPropertyValue( "geometry", s->property( s->indexOf( "geometry" ) ) );

		// set modified state
		w->setDirty( true );
		setModified( w );
		
		// emit modified state
		emit modifiedChanged( true );
	}
}

void QtDesignerChild::formChanged()
{
	if ( QDesignerFormWindowInterface* w = qobject_cast<QDesignerFormWindowInterface*>( sender() ) )
	{
		// update window state
		setModified( w );
		
		// emit modified state
		emit modifiedChanged( w->isDirty() );
	}
}

void QtDesignerChild::editWidgets()
{
	// set edit mode for all forms
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
			i->editWidgets();
}

void QtDesignerChild::previewCurrentForm()
{
	QDesignerFormWindowInterface* w = mArea->activeSubWindow() ? qobject_cast<QDesignerFormWindowInterface*>( mArea->activeSubWindow()->widget() ) : 0;
	if ( w )
	{
		// get buffer
		QBuffer b;
		b.setData( qPrintable( w->contents() ) );
		
		// create form
		QWidget* f = QFormBuilder().load( &b );
		
		// set flags
#ifdef Q_WS_WIN
		Qt::WindowFlags wf = ( f->windowType() == Qt::Window ) ? Qt::Window | Qt::WindowMaximizeButtonHint : Qt::Dialog;
#else
		Qt::WindowFlags wf = Qt::Dialog;
#endif
		
		// set parent
		f->setParent( mArea->activeSubWindow(), wf );
		
		// macdialgo have no close button
		f->setWindowModality( Qt::ApplicationModal );
		
		// set position
		f->setAttribute( Qt::WA_DeleteOnClose );
		f->move( w->mapToGlobal( QPoint( 10, 10) ) );
		
		// track escape key
		f->installEventFilter( this );
		
		// show form
		f->show();
	}
}

void QtDesignerChild::setModified( QDesignerFormWindowInterface* w )
{
	// update designer title
	setWindowTitle( tr( "Qt Designer" ).append( w ? QFileInfo( w->fileName() ).fileName().append( "]" ).prepend( " [" ) : QString::null ) );
	
	// update designer state
	setWindowModified( w && mCore->formWindowManager()->activeFormWindow() == w ? w->isDirty() : false );
	
	// update form mdi state
	if ( w )
		w->parentWidget()->setWindowModified( w->isDirty() );
}

void QtDesignerChild::openFile( const QString& s, const QPoint&, QTextCodec* )
{
	// create form
	QDesignerFormWindowInterface* w = createForm();
	
	// set property to tell ask to save in filter event
	w->setProperty( "askSave", true );
	
	// track close event 
	w->installEventFilter( this );
	
	// assign file name
	w->setFileName( s );
	
	// set default size
	QSize mainContainerSize( 400, 300 );
	
	// set contents
	if ( QFile::exists( s ) )
	{
		// set content
		QFile f( s );
		w->setContents( &f );
		
		// get original size
		mainContainerSize = w->mainContainer()->geometry().size();
		
		// set temporary property
		w->setProperty( "firstGeometryChanged", true );
		
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

	// track form changement
	connect( w, SIGNAL( geometryChanged() ), this, SLOT( geometryChanged() ) );
	connect( w, SIGNAL( changed() ), this, SLOT( formChanged() ) );
	
	// set window title
	mw->setWindowTitle( QFileInfo( s ).fileName() +"[*]" );
	
	// set modified state
	setModified( w );
	
	// emit not modified state
	emit modifiedChanged( w->isDirty() );
	
	// emit file opened
	emit fileOpened( s );
}

void QtDesignerChild::closeFile( const QString& s )
{
	// find file
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
	{
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
		{
			if ( isSameFile( i->fileName(), s ) )
			{
				// set property to tell don t ask to save in filter event
				i->setProperty( "askSave", false );
				
				// close form
				w->close();
				
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
			
			// set property to tell don t ask to save in filter event
			i->setProperty( "askSave", false );
			
			// close form
			w->close();
			
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
{ return false; }

bool QtDesignerChild::isRedoAvailable() const
{ return false; }

bool QtDesignerChild::isPasteAvailable() const
{ return false; }

bool QtDesignerChild::isCopyAvailable() const
{ return false; }

bool QtDesignerChild::isModified( const QString& s ) const
{
	// find file
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
				// cancel if not modified
				if ( !i->isDirty() )
					return;
				
				// write file
				QFile f( i->fileName() );
				if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) )
				{
					f.resize( 0 );
					f.write( qPrintable( i->contents() ) );
					f.close();
					i->setDirty( false );
					setModified( i );
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
			// check next if not modified
			if ( !i->isDirty() )
				continue;
			
			// save file
			QFile f( i->fileName() );
			if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) )
			{
				f.resize( 0 );
				f.write( qPrintable( i->contents() ) );
				f.close();
				i->setDirty( false );
				setModified( i );
				if ( mArea->currentSubWindow() == w )
					emit modifiedChanged( false );
			}
			else
				warning( tr( "Save Form..." ), tr( "An error occurs when saving :\n%1" ).arg( i->fileName() ) );
		}
	}
}

void printForm( QMdiSubWindow* w, bool b )
{
	// get printer
	QPrinter p;

	// if quick print
	if ( b )
	{
		// check if default printer is set
		if ( p.printerName().isEmpty() )
		{
			warning( QObject::tr( "Quick Print..." ), QObject::tr( "There is no default printer, please set one before trying quick print" ), w->window() );
			return;
		}
		
		// print and return
		QPainter pr( &p );
		pr.drawPixmap( 0, 0, QPixmap::grabWidget( w ) );
	}
	else
	{
		// printer dialog
		QPrintDialog d( &p );

		// if ok
		if ( d.exec() )
		{
			// print and return
			QPainter pr( &p );
			pr.drawPixmap( 0, 0, QPixmap::grabWidget( w ) );
		}
	}
}

void QtDesignerChild::printFile( const QString& s )
{
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
			if ( isSameFile( i->fileName(), s ) )
				printForm( w, false );
}

void QtDesignerChild::quickPrintFile( const QString& s )
{
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
			if ( isSameFile( i->fileName(), s ) )
				printForm( w, true );
}

void QtDesignerChild::undo() {}

void QtDesignerChild::redo() {}

void QtDesignerChild::cut() {}

void QtDesignerChild::copy() {}

void QtDesignerChild::paste() {}

void QtDesignerChild::searchReplace() {}

void QtDesignerChild::goTo() {}

void QtDesignerChild::goTo( const QString& s, const QPoint&, bool )
{ showFile( s ); }

void QtDesignerChild::backupCurrentFile( const QString& s )
{
	if ( QDesignerFormWindowInterface* w = qobject_cast<QDesignerFormWindowInterface*>( mArea->currentSubWindow()->widget() ) )
	{
		QFile f( s );
		if ( f.open( QIODevice::WriteOnly | QIODevice::Text ) )
		{
			f.resize( 0 );
			f.write( qPrintable( w->contents() ) );
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
{ return mArea->subWindowList().count(); }
