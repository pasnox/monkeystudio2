#include "QtDesignerManager.h"
#include "QDesignerWidgetBox.h"
#include "QDesignerActionEditor.h"
#include "QDesignerPropertyEditor.h"
#include "QDesignerObjectInspector.h"
#include "QDesignerSignalSlotEditor.h"
#include "QDesignerResourcesEditor.h"

#include <MonkeyCore.h>
#include <UIMain.h>
#include <pWorkspace.h>

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
#include <QFormBuilder>

#include "pluginmanager_p.h"
#include "qdesigner_integration_p.h"

QtDesignerManager::QtDesignerManager( QObject* parent )
	: QObject( parent )
{
	// init designer core
	QDesignerComponents::initializeResources();
	mCore = QDesignerComponents::createFormEditor( MonkeyCore::workspace() );
	
	// initialize plugins
	QDesignerComponents::initializePlugins( mCore );
	
	// init task menus
	(void) QDesignerComponents::createTaskMenu( mCore, MonkeyCore::workspace() );
	
	// init actions
	QDesignerFormWindowManagerInterface* fwm = mCore->formWindowManager();
	
	// create edit widget mode action
	aEditWidgets = new QAction( tr( "Edit Widgets" ), this );
	aEditWidgets->setIcon( QIcon( mCore->resourceLocation().append( "/widgettool.png" ) ) );
	aEditWidgets->setCheckable( true );
	aEditWidgets->setChecked( true );
	
	// preview action
	aPreview = new QAction( this );
	aPreview->setIcon( QIcon( ":/icons/preview.png" ) );
	aPreview->setShortcut( tr( "Ctrl+R" ));
	aPreview->setText( tr( "Preview Form" ) );
	
	// action group for modes
	aModes = new QActionGroup( MonkeyCore::workspace() );
	aModes->setExclusive( true );
	aModes->addAction( aEditWidgets );
	
	// simplify gridlayout
	fwm->actionSimplifyLayout()->setIcon( fwm->actionGridLayout()->icon() );
	
	// edit actions
	fwm->actionUndo()->setIcon( QIcon( ":/icons/undo.png" ) );
	fwm->actionUndo()->setShortcut( MonkeyCore::menuBar()->action( "mEdit/aUndo" )->shortcut() );
	fwm->actionRedo()->setIcon( QIcon( ":/icons/redo.png" ) );
	fwm->actionRedo()->setShortcut( MonkeyCore::menuBar()->action( "mEdit/aRedo" )->shortcut() );
	fwm->actionDelete()->setIcon( QIcon( ":/icons/delete.png" ) );
	fwm->actionSelectAll()->setIcon( QIcon( ":/icons/selectall.png" ) );
	fwm->actionDelete()->setShortcut( tr( "Del" ) );
	
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
	
	// create designer docks
	pWidgetBox = new QDesignerWidgetBox( mCore );
	pWidgetBox->setVisible( false );
	MonkeyCore::mainWindow()->dockToolBar( Qt::LeftToolBarArea )->addDock( pWidgetBox, pWidgetBox->windowTitle(), pWidgetBox->windowIcon() );
	
	pObjectInspector = new QDesignerObjectInspector( mCore );
	pObjectInspector->setVisible( false );
	MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( pObjectInspector, pObjectInspector->windowTitle(), pObjectInspector->windowIcon() );
	
	pPropertyEditor = new QDesignerPropertyEditor( mCore );
	pPropertyEditor->setVisible( false );
	MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( pPropertyEditor, pPropertyEditor->windowTitle(), pPropertyEditor->windowIcon() );
	
	pActionEditor = new QDesignerActionEditor( mCore );
	pActionEditor->setVisible( false );
	MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( pActionEditor, pActionEditor->windowTitle(), pActionEditor->windowIcon() );
	
	pSignalSlotEditor = new QDesignerSignalSlotEditor( mCore );
	pSignalSlotEditor->setVisible( false );
	MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( pSignalSlotEditor, pSignalSlotEditor->windowTitle(), pSignalSlotEditor->windowIcon() );
	
	pResourcesEditor = new QDesignerResourcesEditor( mCore );
	pResourcesEditor->setVisible( false );
	MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( pResourcesEditor, pResourcesEditor->windowTitle(), pResourcesEditor->windowIcon() );
	
	// perform integration
	new qdesigner_internal::QDesignerIntegration( mCore, MonkeyCore::workspace() );
	mCore->setTopLevel( MonkeyCore::workspace() );
	
	// connections
	connect( aEditWidgets, SIGNAL( triggered() ), this, SLOT( editWidgets() ) );
	connect( aPreview, SIGNAL( triggered() ), this, SLOT( previewCurrentForm() ) );
}

QtDesignerManager::~QtDesignerManager()
{
	delete pWidgetBox;
	delete pActionEditor;
	delete pPropertyEditor;
	delete pObjectInspector;
	delete pSignalSlotEditor;
	delete pResourcesEditor;
}

QDesignerFormEditorInterface* QtDesignerManager::core()
{
	return mCore;
}

QDesignerFormWindowInterface* QtDesignerManager::createNewForm( QWidget* parent )
{
	QDesignerFormWindowInterface* form = mCore->formWindowManager()->createFormWindow( parent );
	form->setFeatures( QDesignerFormWindowInterface::DefaultFeature );
	return form;
}

void QtDesignerManager::addFormWindow( QDesignerFormWindowInterface* form )
{
	mCore->formWindowManager()->addFormWindow( form );
}

void QtDesignerManager::setActiveFormWindow( QDesignerFormWindowInterface* form )
{
	// update active form
	if ( form && mCore->formWindowManager()->activeFormWindow() != form )
	{
		mCore->formWindowManager()->setActiveFormWindow( form );
	}
	
	// update preview actino state
	aPreview->setEnabled( form );
}

void QtDesignerManager::editWidgets()
{
	// set edit mode for all forms
	QDesignerFormWindowManagerInterface* fwm = mCore->formWindowManager();
	
	for ( int i = 0; i < fwm->formWindowCount(); i++ )
	{
		fwm->formWindow( i )->editWidgets();
	}
}

void QtDesignerManager::previewCurrentForm()
{
//finnir filter event + copier preview qtcreator
	QDesignerFormWindowInterface* form = mCore->formWindowManager()->activeFormWindow();
	
	if ( form )
	{
		// get buffer
		QBuffer b;
		b.setData( form->contents().toUtf8() );
		
		if ( !b.open( QIODevice::ReadOnly ) )
		{
			return;
		}
		
		// create form
		QWidget* f = QFormBuilder().load( &b );
		
		// delete on close
		f->setAttribute( Qt::WA_DeleteOnClose );
		
		// set flags
		/*
		Qt::WindowFlags wf = Qt::Window | Qt::WindowSystemMenuHint;
		if ( f->windowType() == Qt::Window )
			wf |= Qt::WindowMaximizeButtonHint;
		*/
#ifdef Q_WS_WIN
		Qt::WindowFlags wf = ( f->windowType() == Qt::Window ) ? Qt::Window | Qt::WindowMaximizeButtonHint : Qt::Dialog;
#else
		Qt::WindowFlags wf = Qt::Dialog;
#endif
		
		// set parent
		f->setParent( MonkeyCore::mainWindow(), wf );
		
		// macdialgo have no close button
		f->setWindowModality( Qt::ApplicationModal );
		
		// set position
		f->move( form->mapToGlobal( QPoint( 10, 10) ) );
		
		// track escape key
		f->installEventFilter( this );
		
		// show form
		f->show();
	}
}
