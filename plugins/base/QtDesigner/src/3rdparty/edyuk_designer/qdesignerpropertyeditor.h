/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignerpropertyeditor.h
** Date : mar aoû 29 16:29:53 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _QDESIGNER_PROPERTY_EDITOR_H_
#define _QDESIGNER_PROPERTY_EDITOR_H_

#include <QDockWidget>

class QString;
class QVariant;

class QtDesignerChild;
class QDesignerPropertyEditorInterface;
class QDesignerFormWindowInterface;

class QDesignerPropertyEditor : public QDockWidget
{
	Q_OBJECT
	
	public:
		QDesignerPropertyEditor(QtDesignerChild *p);
		virtual ~QDesignerPropertyEditor();
		
	private slots:
		void selectionChanged();
		void formChanged(QDesignerFormWindowInterface *w);
		
	private:
		QtDesignerChild *pParent;
		QDesignerFormWindowInterface *pForm;
		QDesignerPropertyEditorInterface *pInterface;
};

#endif // _QDESIGNER_PROPERTY_EDITOR_H_
