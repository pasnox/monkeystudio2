/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignerwidgetbox.cpp
** Date : mar aoû 29 16:29:41 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qdesignerwidgetbox.h"

#include "QtDesignerChild.h"

#include <QDir>

#include <QDesignerComponents>

#include <QDesignerWidgetBoxInterface>
#include <QDesignerFormEditorInterface>

QDesignerWidgetBox::QDesignerWidgetBox(QtDesignerChild *p)
 : QDockWidget(0), pParent(p)
{
	setWindowTitle(tr("Widget Box"));
	setObjectName("x-designer/widgetbox");
	
	pInterface = QDesignerComponents::createWidgetBox(p->core(), this);
	
	pInterface->setFileName(":/trolltech/widgetbox/widgetbox.xml");
	pInterface->load();
	
	pInterface->setFileName(QDir::homePath() + "./designer/widgetbox.xml");
	pInterface->load();
	
	setWidget(pInterface);
	
	p->core()->setWidgetBox(pInterface);
}

QDesignerWidgetBox::~QDesignerWidgetBox()
{
	
}
