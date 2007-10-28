/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignerwidgetbox.h
** Date : mar aoû 29 16:29:32 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _QDESIGNER_WIDGET_BOX_H_
#define _QDESIGNER_WIDGET_BOX_H_

#include <QDockWidget>

class QtDesignerChild;
class QDesignerWidgetBoxInterface;

class QDesignerWidgetBox : public QDockWidget
{
	public:
		QDesignerWidgetBox(QtDesignerChild *p);
		virtual ~QDesignerWidgetBox();
		
	private:
		QtDesignerChild *pParent;
		QDesignerWidgetBoxInterface *pInterface;
};

#endif // _QDESIGNER_WIDGET_BOX_H_
