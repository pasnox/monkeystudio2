/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UICLIToolSettings.h
** Date      : 2007-11-04T22:45:24
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UICLITOOLSETTINGS_H
#define UICLITOOLSETTINGS_H

#include "MonkeyExport.h"
#include "ui_UICLIToolSettings.h"
#include "pCommand.h"

#include <QWidget>

class BasePlugin;

class Q_MONKEY_EXPORT UICLIToolSettings : public QWidget, public Ui::UICLIToolSettings
{
	Q_OBJECT
	
protected:
	pCommandList mDefaults;
	pCommandList mReset;
	pCommandList mCommands;
	BasePlugin* mPlugin;
	bool mUpdating;

public:
	UICLIToolSettings( BasePlugin*, const pCommandList&, const pCommandList&, QWidget* = 0 );

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

#endif // UICLITOOLSETTINGS_H
