/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignersignalsloteditor.cpp
** Date : mar aoû 29 17:14:01 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qdesignersignalsloteditor.h"

#include "QtDesignerChild.h"

#include <QDesignerComponents>

#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>

QDesignerSignalSlotEditor::QDesignerSignalSlotEditor(QtDesignerChild *p)
 : QDockWidget(0), pParent(p)
{
	setObjectName("x-designer/signalsloteditor");
	setWindowTitle(tr("Signal and slots"));
	
	pInterface = QDesignerComponents::createSignalSlotEditor(p->core(), this);
	setWidget(pInterface);
}

QDesignerSignalSlotEditor::~QDesignerSignalSlotEditor()
{
	
}

