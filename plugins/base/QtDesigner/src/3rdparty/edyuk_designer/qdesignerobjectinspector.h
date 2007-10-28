/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignerobjectinspector.h
** Date : mar aoû 29 16:30:12 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _QDESIGNER_OBJECT_INSPECTOR_H_
#define _QDESIGNER_OBJECT_INSPECTOR_H_

#include <QDockWidget>

class QtDesignerChild;
class QDesignerObjectInspectorInterface;
class QDesignerFormWindowInterface;

class QDesignerObjectInspector : public QDockWidget
{
	Q_OBJECT
	
	public:
		QDesignerObjectInspector(QtDesignerChild *p);
		virtual ~QDesignerObjectInspector();
		
	private slots:
		void formChanged(QDesignerFormWindowInterface *w);
		
	private:
		QtDesignerChild *pParent;
		QDesignerFormWindowInterface *pForm;
		QDesignerObjectInspectorInterface *pInterface;
};

#endif // _QDESIGNER_OBJECT_INSPECTOR_H_
