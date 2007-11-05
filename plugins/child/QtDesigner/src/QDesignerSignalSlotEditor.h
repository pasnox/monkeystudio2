/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerSignalSlotEditor.h
** Date      : 2007-11-04T22:53:18
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QDESIGNERSIGNALSLOTEDITOR_H
#define QDESIGNERSIGNALSLOTEDITOR_H

#include <QDockWidget>

class QtDesignerChild;

class QDesignerSignalSlotEditor : public QDockWidget
{
public:
	QDesignerSignalSlotEditor( QtDesignerChild* );
	inline QWidget* interface() const { return mInterface; }
	
private:
	QWidget* mInterface;
};

#endif // QDESIGNERSIGNALSLOTEDITOR_H
