/****************************************************************************
**
** 	Created using Edyuk IDE 0.5.1.0
**
** File : qdesignersignalsloteditor.h
** Date : mar aoû 29 17:13:44 2006
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _QDESIGNER_SIGNAL_SLOT_EDITOR_H_
#define _QDESIGNER_SIGNAL_SLOT_EDITOR_H_

#include <QDockWidget>

class QtDesignerChild;
class QDesignerSignalSlotEditorInterface;
class QDesignerFormWindowInterface;

class QDesignerSignalSlotEditor : public QDockWidget
{
	public:
		QDesignerSignalSlotEditor(QtDesignerChild *p);
		virtual ~QDesignerSignalSlotEditor();
		
	private:
		QtDesignerChild *pParent;
		QWidget *pInterface;
};

#endif // _QDESIGNER_SIGNAL_SLOT_EDITOR_H_
