/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UICommandsEditor
** FileName  : UICommandsEditor.h
** Date      : 2007-10-07T02:09:14
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UICOMMANDSEDITOR_H
#define UICOMMANDSEDITOR_H

#include "ui_UICommandsEditor.h"
#include "pCommand.h"

#include <QWidget>

class UICommandsEditor : public QWidget, public Ui::UICommandsEditor
{
	Q_OBJECT
	
protected:
	pCommandList mDefaults;
	pCommandList mCommands;

public:
	UICommandsEditor( const pCommandList&, const pCommandList&, QWidget* = 0 );
	~UICommandsEditor();

	void loadCommands();

protected slots:
	void on_lwCommands_itemSelectionChanged();

};

#endif // UICOMMANDSEDITOR_H
