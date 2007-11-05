/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerPropertyEditor.h
** Date      : 2007-11-04T22:53:17
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QDESIGNERPROPERTYEDITOR_H
#define QDESIGNERPROPERTYEDITOR_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerFormWindowInterface;
class QDesignerPropertyEditorInterface;

class QDesignerPropertyEditor : public QDockWidget
{
	Q_OBJECT
	
public:
	QDesignerPropertyEditor( QtDesignerChild* );
	inline QDesignerPropertyEditorInterface* interface() const { return mInterface; }
	
private:
	QDesignerPropertyEditorInterface* mInterface;
};

#endif // QDESIGNERPROPERTYEDITOR_H
