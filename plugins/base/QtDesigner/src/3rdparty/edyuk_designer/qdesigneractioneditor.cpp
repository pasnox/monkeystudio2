/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesigneractioneditor.cpp
** Date : mar aoû 29 17:13:34 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qdesigneractioneditor.h"

#include "QtDesignerChild.h"

#include <QDesignerComponents>

#include <QDesignerActionEditorInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>

QDesignerActionEditor::QDesignerActionEditor(QtDesignerChild *p)
 : QDockWidget(0), pParent(p), pForm(0)
{
	setWindowTitle(tr("Actions"));
	setObjectName("x-designer/actioneditor");
	
	pInterface = QDesignerComponents::createActionEditor(p->core(), this);
	
	connect(p->core()->formWindowManager(),
			SIGNAL( activeFormWindowChanged(QDesignerFormWindowInterface*) ),
			this,
			SLOT  ( formChanged(QDesignerFormWindowInterface*) ) );
	
	
	setWidget(pInterface);
	
	p->core()->setActionEditor(pInterface);
}

QDesignerActionEditor::~QDesignerActionEditor()
{
	
}

void QDesignerActionEditor::formChanged(QDesignerFormWindowInterface *w)
{
	pForm = w;
	pInterface->setFormWindow(pForm);
}
