/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerActionEditor.h
** Date      : 2007-11-04T22:53:14
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QDESIGNERACTIONEDITOR_H
#define QDESIGNERACTIONEDITOR_H

#include <QDockWidget>

class QtDesignerChild;
class QDesignerActionEditorInterface;

class QDesignerActionEditor : public QDockWidget
{
	Q_OBJECT

public:
	QDesignerActionEditor( QtDesignerChild* );
	inline QDesignerActionEditorInterface* interface() const { return mInterface; }
	
private:
	QDesignerActionEditorInterface* mInterface;
};

#endif // QDESIGNERACTIONEDITOR_H
