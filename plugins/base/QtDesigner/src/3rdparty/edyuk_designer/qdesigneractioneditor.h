/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesigneractioneditor.h
** Date : mar aoû 29 17:13:25 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _QDESIGNER_ACTION_EDITOR_H_
#define _QDESIGNER_ACTION_EDITOR_H_

#include <QDockWidget>

class QtDesignerChild;
class QDesignerActionEditorInterface;
class QDesignerFormWindowInterface;

class QDesignerActionEditor : public QDockWidget
{
	Q_OBJECT
	
	public:
		QDesignerActionEditor(QtDesignerChild *p);
		virtual ~QDesignerActionEditor();
		
	private slots:
		void formChanged(QDesignerFormWindowInterface *w);
		
	private:
		QtDesignerChild *pParent;
		QDesignerFormWindowInterface *pForm;
		QDesignerActionEditorInterface *pInterface;
};

#endif // _QDESIGNER_ACTION_EDITOR_H_
