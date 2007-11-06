/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UIBuilderSettings
** FileName  : UIBuilderSettings.h
** Date      : 2007-10-07T02:09:14
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIBUILDERSETTINGS_H
#define UIBUILDERSETTINGS_H

#include "MonkeyExport.h"
#include "ui_UIBuilderSettings.h"
#include "pCommand.h"

#include <QWidget>

class BuilderPlugin;

class Q_MONKEY_EXPORT UIBuilderSettings : public QWidget, public Ui::UIBuilderSettings
{
	Q_OBJECT
	
protected:
	pCommand mDefault;
	pCommand mReset;
	pCommand mCommand;
	BuilderPlugin* mPlugin;

public:
	UIBuilderSettings( BuilderPlugin*, QWidget* = 0 );

	void updateCommand();
	void restoreDefault();
	void reset();
	void save();

protected slots:
	void on_tbBuildCommandCommand_clicked();
	void on_tbBuildCommandWorkingDirectory_clicked();
	void on_dbbButtons_clicked( QAbstractButton* );

};

#endif // UIBUILDERSETTINGS_H
