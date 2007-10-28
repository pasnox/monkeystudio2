/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignerobjectinspector.cpp
** Date : mar aoû 29 16:30:22 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qdesignerobjectinspector.h"

#include "QtDesignerChild.h"

#include <QDesignerComponents>

#include <QDesignerObjectInspectorInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>

QDesignerObjectInspector::QDesignerObjectInspector(QtDesignerChild *p)
 : QDockWidget(0), pParent(p), pForm(0)
{
	setWindowTitle(tr("Object Inspector"));
	setObjectName("x-designer/objectinspector");
	
	pInterface = QDesignerComponents::createObjectInspector(p->core(), this);
	
	connect(p->core()->formWindowManager(),
			SIGNAL( activeFormWindowChanged(QDesignerFormWindowInterface*) ),
			this,
			SLOT  ( formChanged(QDesignerFormWindowInterface*) ) );
	
	
	setWidget(pInterface);
	
	p->core()->setObjectInspector(pInterface);
}

QDesignerObjectInspector::~QDesignerObjectInspector()
{
	
}

void QDesignerObjectInspector::formChanged(QDesignerFormWindowInterface *w)
{
	pForm = w;
	pInterface->setFormWindow(pForm);
}
