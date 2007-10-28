/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignerpropertyeditor.cpp
** Date : mar aoû 29 16:45:53 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qdesignerpropertyeditor.h"

#include "QtDesignerChild.h"

#include <QDesignerComponents>

#include <QDesignerPropertyEditorInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>
#include <QDesignerFormWindowManagerInterface>

QDesignerPropertyEditor::QDesignerPropertyEditor(QtDesignerChild *p)
 : QDockWidget(0), pParent(p), pForm(0)
{
	setWindowTitle(tr("Properties"));
	setObjectName("x-designer/propertyeditor");
	
	pInterface = QDesignerComponents::createPropertyEditor(p->core(), this);
	
	connect(p->core()->formWindowManager(),
			SIGNAL( activeFormWindowChanged(QDesignerFormWindowInterface*) ),
			this,
			SLOT  ( formChanged(QDesignerFormWindowInterface*) ) );
	
	setWidget(pInterface);
	
	p->core()->setPropertyEditor(pInterface);
}

QDesignerPropertyEditor::~QDesignerPropertyEditor()
{
	
}

void QDesignerPropertyEditor::selectionChanged()
{
	if ( !pForm )
		return;
	
	QDesignerFormWindowCursorInterface *c = pForm->cursor();
	
	if ( c->hasSelection() )
		pInterface->setObject(c->selectedWidget(0));
	else
		pInterface->setObject(pForm);
}

void QDesignerPropertyEditor::formChanged(QDesignerFormWindowInterface *w)
{
	if ( pForm )
	{
		disconnect(	pForm	, SIGNAL( selectionChanged() ),
					this	, SLOT  ( selectionChanged() ) );
		
	}
	
	pForm = w;
	pInterface->setObject(pForm);
	
	if ( pForm )
	{
		connect(pForm	, SIGNAL( selectionChanged() ),
				this	, SLOT  ( selectionChanged() ) );
		
	}
}
