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

class CompilerPlugin;

class UICommandsEditor : public QWidget, public Ui::UICommandsEditor
{
	Q_OBJECT
	
protected:
	pCommandList mDefaults;
	pCommandList mReset;
	pCommandList mCommands;
	CompilerPlugin* mPlugin;
	bool mUpdating;

public:
	UICommandsEditor( CompilerPlugin*, const pCommandList&, const pCommandList&, QWidget* = 0 );

	void updateCommands();
	void restoreDefaults();
	void reset();
	void save();

protected slots:
	void on_lwCommands_itemSelectionChanged();
	void on_lwCommands_currentItemChanged( QListWidgetItem*, QListWidgetItem* );
	void on_pbCommandAdd_clicked();
	void on_pbCommandRemove_clicked();
	void on_pbCommandUp_clicked();
	void on_pbCommandDown_clicked();
	void on_tbCommandCommand_clicked();
	void on_tbCommandWorkingDirectory_clicked();
	void on_dbbButtons_clicked( QAbstractButton* );

};

#endif // UICOMMANDSEDITOR_H
