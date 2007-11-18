/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UICompilerSettings
** FileName  : UICompilerSettings.h
** Date      : 2007-10-07T02:09:14
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UICOMPILERSETTINGS_H
#define UICOMPILERSETTINGS_H

#include "MonkeyExport.h"
#include "ui_UICompilerSettings.h"
#include "pCommand.h"

#include <QWidget>

class CompilerPlugin;

class Q_MONKEY_EXPORT UICompilerSettings : public QWidget, public Ui::UICompilerSettings
{
	Q_OBJECT
	
protected:
	pCommand mDefault;
	pCommand mReset;
	pCommand mCommand;
	CompilerPlugin* mPlugin;

public:
	UICompilerSettings( CompilerPlugin*, QWidget* = 0 );

	void updateCommand();
	void restoreDefault();
	void reset();
	void save();

protected slots:
	void on_tbBuildCommandCommand_clicked();
	void on_tbBuildCommandWorkingDirectory_clicked();
	void on_dbbButtons_clicked( QAbstractButton* );

};

#endif // UICOMPILERSETTINGS_H
