/****************************************************************************
**
**         Created using Monkey Studio v1.7.0
** Author    : Kopats Andrei aka hlamer <hlamer at tut by>
** Project   : SciShortcutsManager
** FileName  : SciShortcutsManager.h
** Date      : 2007-10-27T10:52:20
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QSCISHORTCUTSMANAGER
#define QSCISHORTCUTSMANAGER

#include <QApplication>

#include "QSingleton.h"
#include "MonkeyExport.h"
#include "pAction.h"

/*QScintilla using set of shorctuts. Class clearing this shortcuts, and, if need
to do QSintilla's action, sending to it message. It's need for replacing 
QScintilla's own shourcuts with Monkey Studio actions. Because actions can be 
configured, and can be used in the macroses.
Message will be sended to current editor.
*/
class pEditor;


struct SciAction
{
	SciAction (QString _name, QString _text, QIcon _icon, QString _defaultShortcut, QString _toolTip, int _messageCode):
		name (_name), text (_text), icon (_icon), defaultShortcut (_defaultShortcut), toolTip (_toolTip), messageCode (_messageCode){};
    QString name;
    QString text;
    QIcon icon;
    QString defaultShortcut;
    QString toolTip;
    int messageCode;
};


class Q_MONKEY_EXPORT qSciShortcutsManager: public QObject, public QSingleton<qSciShortcutsManager>
{
    Q_OBJECT
    friend class QSingleton<qSciShortcutsManager>;
protected:
	qSciShortcutsManager (QObject* parent = QApplication::instance());

protected:
    QList<SciAction> sactions;

protected slots:    
    void keyBoardShortcutPressed ();
};

#endif //QSCISHORTCUTSMANAGER
