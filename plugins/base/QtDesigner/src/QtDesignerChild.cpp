#include "QtDesignerChild.h"
#include "UIMain.h"
#include "pDockToolBar.h"
#include "pDockToolBarManager.h"

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

#include "pluginmanager_p.h"
#include "qdesigner_integration_p.h"

#include "qdesignerwidgetbox.h"
#include "qdesigneractioneditor.h"
#include "qdesignerpropertyeditor.h"
#include "qdesignerobjectinspector.h"
#include "qdesignersignalsloteditor.h"

QtDesignerChild::QtDesignerChild()
	: pAbstractChild()
{
	// set up container child
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
	UIMain::instance()->dockToolBar( Qt::LeftToolBarArea )->addDock( pWidgetBox, tr( "Widget Box" ), QIcon( ":/icons/widget.png" ) );
	
	pObjectInspector = new QDesignerObjectInspector( this );
	UIMain::instance()->dockToolBar( Qt::RightToolBarArea )->addDock( pObjectInspector, tr( "Object Inspector" ), QIcon( ":/icons/inspector.png" ) );
	
	pPropertyEditor = new QDesignerPropertyEditor( this );
	UIMain::instance()->dockToolBar( Qt::RightToolBarArea )->addDock( pPropertyEditor, tr( "Property Editor" ), QIcon( ":/icons/property.png" ) );
	
	pActionEditor = new QDesignerActionEditor( this );
	UIMain::instance()->dockToolBar( Qt::BottomToolBarArea )->addDock( pActionEditor, tr( "Action Editor" ), QIcon( ":/icons/action.png" ) );
	
	pSignalSlotEditor = new QDesignerSignalSlotEditor( this );
	UIMain::instance()->dockToolBar( Qt::TopToolBarArea )->addDock( pSignalSlotEditor, tr( "Signal/Slot Editor" ), QIcon( ":/icons/signal.png" ) );
	
	// perform integration
	new qdesigner_internal::QDesignerIntegration( mCore, this );
	mCore->setTopLevel( UIMain::instance() );
	
	// connection
	connect( mArea, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( formActivated( QMdiSubWindow* ) ) );
	connect( aEditWidgets, SIGNAL( triggered() ), this, SLOT( editWidgets() ) );
	
	// dummy open file test
	openFile( "/home/pasnox/Documents/Development/Qt4/monkeyrepos/v2/trunk/plugins/base/Ctags2Api/src/ui/UICtags2Api.ui" );
}

QtDesignerChild::~QtDesignerChild()
{
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

void QtDesignerChild::formActivated( QMdiSubWindow* w )
{
	// get form
	QDesignerFormWindowInterface* i = w ? qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) : 0;
	
	// set it current for designer
	mCore->formWindowManager()->setActiveFormWindow( i );
}

void QtDesignerChild::editWidgets()
{
	// set edit mode for all forms
	foreach ( QMdiSubWindow* w, mArea->subWindowList() )
		if ( QDesignerFormWindowInterface* i = qobject_cast<QDesignerFormWindowInterface*>( w->widget() ) )
			i->editWidgets();
}
/*
QDesignerFormWindowInterface* QtDesignerChild::newForm(qmdiClientFactory *f)
{
	Q_UNUSED(f)
	
	
	// TODO : propose a choice between QDialog, QWidget, QMainWindow, ...
	
	
	static int count = 0;
	
	QDesignerFormWindowInterface* w = createForm();
	w->setMainContainer(new QWidget());
	
	QDesignerClient *dc = new QDesignerClient(w, "");
	dc->setPerspective(this);
	dc->setTitle( tr("untitled form %1").arg(++count) );
	w->setDirty(true);
	
	return dc;
}
*/
void QtDesignerChild::openFile( const QString& s, const QPoint&, QTextCodec* )
{
	// create form
	QDesignerFormWindowInterface* w = createForm();
	
	// assign file content
	w->setFileName( s );
	QFile f( s );
	w->setContents( &f );
	
	// set clean
	w->setDirty( false );
	
	// add to mdi area
	mArea->addSubWindow( w );
}












QStringList QtDesignerChild::files() const
{
	return QStringList();
	/*
	QStringList l;
	foreach ( HelpWindow* w, browsersList() )
		l << w->source().toString();
	return l;
	*/
}

QPoint QtDesignerChild::cursorPosition() const
{ return QPoint( -1, -1 ); }

void QtDesignerChild::showFile( const QString& )
{
	/*
	foreach ( HelpWindow* w, browsersList() )
	{
		if ( w->source().toString() == s )
		{
			mMain->browsers()->findChild<QTabWidget*>( "tab" )->setCurrentWidget( w );
			return;
		}
	}
	*/
}

QString QtDesignerChild::currentFile() const
{
	return QString();
	//return mMain->browsers()->currentBrowser()->source().toString();
}

QString QtDesignerChild::currentFileName() const
{ return QFileInfo( currentFile() ).fileName(); }

bool QtDesignerChild::isModified() const
{ return false; }

bool QtDesignerChild::isUndoAvailable() const
{ return false; }

bool QtDesignerChild::isRedoAvailable() const
{ return false; }

bool QtDesignerChild::isPasteAvailable() const
{ return false; }

bool QtDesignerChild::isCopyAvailable() const
{ return false; }

bool QtDesignerChild::isModified( const QString& ) const
{ return false; }

void QtDesignerChild::saveFile( const QString& )
{}

void QtDesignerChild::saveFiles()
{}

void QtDesignerChild::printFile( const QString& )
{}

void QtDesignerChild::quickPrintFile( const QString& )
{}

void QtDesignerChild::undo()
{}

void QtDesignerChild::redo()
{}

void QtDesignerChild::cut()
{}

void QtDesignerChild::copy()
{}

void QtDesignerChild::paste()
{}

void QtDesignerChild::searchReplace()
{}

void QtDesignerChild::goTo()
{}

bool QtDesignerChild::isSearchReplaceAvailable() const
{ return false; }

bool QtDesignerChild::isGoToAvailable() const
{ return false; }

bool QtDesignerChild::isPrintAvailable() const
{ return false; }
