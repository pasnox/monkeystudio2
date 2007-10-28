/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qtdesignerperspective.cpp
** Date : lun aug 28 12:02:22 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qdesignerperspective.h"

#include "qdesignerclient.h"

#include "qdesignerwidgetbox.h"
#include "qdesigneractioneditor.h"
#include "qdesignerpropertyeditor.h"
#include "qdesignerobjectinspector.h"
#include "qdesignersignalsloteditor.h"

#include "qmdiworkspace.h"
#include "qmdimainwindow.h"

#include <QIcon>
#include <QAction>
#include <QTimerEvent>
#include <QPluginLoader>

#include <QDesignerComponents>

#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormEditorPluginInterface>
#include <QDesignerFormWindowManagerInterface>

#include "pluginmanager_p.h"
#include "qdesigner_integration_p.h"

QDesignerPerspective::QDesignerPerspective(QProjectModel *m, QCodeModel *c)
 : EdyukPerspective(0)
// : qmdiPerspective(0)
{
	/*
	qmdiWorkspace *w = new qmdiWorkspace(0, 0);
	w->hide();
	setPrivateWorkspace(w);
	
	connect(w	, SIGNAL( indexChanged(QWidget*) ),
			this, SLOT  ( formChanged(QWidget*) ) );
	*/
	
	setProjectManager(m);
	setClassBrowser(c);
	
	QDesignerComponents::initializeResources();
	
	pDesigner = QDesignerComponents::createFormEditor(this);
	
	(void)QDesignerComponents::createTaskMenu(pDesigner, this);
	
	aModes = new QActionGroup(this);
	aModes->setExclusive(true);
	
	aEditWidgets = new QAction(tr("Edit Widgets"), this);
	aEditWidgets->setCheckable(true);
	aEditWidgets->setIcon(QIcon(pDesigner->resourceLocation() + "/widgettool.png"));
	
	connect(aEditWidgets, SIGNAL( triggered() ),
			this		, SLOT  ( editWidgets() ) );
	
	aEditWidgets->setChecked(true);
	
	aModes->addAction(aEditWidgets);
	menus["&Mode"]->addAction(aEditWidgets);
	toolbars["Mode"]->addAction(aEditWidgets);
	
	QList<QObject*> plugins = QPluginLoader::staticInstances();
	plugins += pDesigner->pluginManager()->instances();
	
	foreach (QObject *plugin, plugins)
	{
		QDesignerFormEditorPluginInterface *fep;
		
		if ( (fep = qobject_cast<QDesignerFormEditorPluginInterface*>(plugin)) )
		{
			if ( !fep->isInitialized() )
				fep->initialize(pDesigner);
			
			fep->action()->setCheckable(true);
			
			aModes->addAction(fep->action());
			menus["&Mode"]->addAction(fep->action());
			toolbars["Mode"]->addAction(fep->action());
		}
	}
	
	aModes->setEnabled(false);
	
	QDesignerComponents::initializePlugins(pDesigner);
	
	pWidgetBox = new QDesignerWidgetBox(this);
	addDockWidget(pWidgetBox, "Widget box", Qt::LeftDockWidgetArea);
	
	pObjectInspector = new QDesignerObjectInspector(this);
	addDockWidget(pObjectInspector, "Object inspector", Qt::RightDockWidgetArea);
	
	pPropertyEditor = new QDesignerPropertyEditor(this);
	addDockWidget(pPropertyEditor, "Property Editor", Qt::RightDockWidgetArea);
	
	pActionEditor = new QDesignerActionEditor(this);
	addDockWidget(pActionEditor, "Action editor", Qt::BottomDockWidgetArea);
	
	pSignalSlotEditor = new QDesignerSignalSlotEditor(this);
	addDockWidget(pSignalSlotEditor, "Signals/Slots editor", Qt::TopDockWidgetArea);
	
	//qdesigner_internal::QDesignerIntegration *pIntegration =
	new qdesigner_internal::QDesignerIntegration(pDesigner, this);
}


QDesignerPerspective::~QDesignerPerspective()
{
	
}

QIcon QDesignerPerspective::icon() const
{
	return QIcon(":/designer.png");
}

QString QDesignerPerspective::name() const
{
	return tr("Integrated Qt4 Designer");
}

void QDesignerPerspective::retranslate()
{
	pWidgetBox->retranslate();
	pPropertyEditor->retranslate();
	pObjectInspector->retranslate();
	pActionEditor->retranslate();
	pSignalSlotEditor->retranslate();
}

void QDesignerPerspective::setMainWindow(qmdiMainWindow *w)
{
	if ( !w )
		return;
	
	pDesigner->setTopLevel(w);
	
	connect(w->commonWorkspace(), SIGNAL( indexChanged(QWidget*) ),
			this				, SLOT  ( formChanged(QWidget*) ) );
	
	
	qmdiPerspective::setMainWindow(w);
}

qmdiClient* QDesignerPerspective::open(const QString& filename)
{
	if ( filename.endsWith(".ui") )
	{
		QDesignerClient *c = new QDesignerClient(createForm(), filename);
		c->setPerspective(this);
		
		return c;
	}
	
	return 0;
}

bool QDesignerPerspective::canOpen(const QString& filename) const
{
	return filename.endsWith(".ui");
}

bool QDesignerPerspective::needsPrivateWorkspace(qmdiClient *c) const
{
	return false;
	//return dynamic_cast<QDesignerClient*>(c);
}

QDesignerFormEditorInterface* QDesignerPerspective::handler()
{
	return pDesigner;
}

QDesignerFormWindowInterface* QDesignerPerspective::createForm()
{
	QDesignerFormWindowManagerInterface* m = pDesigner->formWindowManager();
	QDesignerFormWindowInterface* w = m->createFormWindow();
	
	w->setFeatures(QDesignerFormWindowInterface::DefaultFeature);
	return w;
}


void QDesignerPerspective::formChanged(QWidget *w)
{
	QDesignerFormWindowInterface *i = 0;
	QDesignerClient *c = qobject_cast<QDesignerClient*>(w);
	
	if ( c )
		i = c->interface();
	
	aModes->setEnabled(i);
	
	pDesigner->formWindowManager()->setActiveFormWindow(i);
}

void QDesignerPerspective::editWidgets()
{
	if ( !mainWindow() || !mainWindow()->commonWorkspace() )
		return;
	
	foreach ( QWidget *w, mainWindow()->commonWorkspace()->windowList() )
	{
		QDesignerClient *c = qobject_cast<QDesignerClient*>(w);
		
		if ( !c || !c->interface() )
			continue;
		
		c->interface()->editWidgets();
	}
}

qmdiClient* QDesignerPerspective::createEmptyClient(qmdiClientFactory *f)
{
	Q_UNUSED(f)
	
	/*
		TODO : propose a choice between QDialog, QWidget, QMainWindow, ...
	*/
	
	static int count = 0;
	
	QDesignerFormWindowInterface* w = createForm();
	w->setMainContainer(new QWidget());
	
	QDesignerClient *dc = new QDesignerClient(w, "");
	dc->setPerspective(this);
	dc->setTitle( tr("untitled form %1").arg(++count) );
	w->setDirty(true);
	
	return dc;
}
