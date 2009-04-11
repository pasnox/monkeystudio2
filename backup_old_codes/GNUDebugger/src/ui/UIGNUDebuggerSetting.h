/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
/*!
	\file UIGNUDebuggerSetting.h
	\date 14/08/08
	\author Xiantia
	\version 1.3.2
	\brief Main Ui for setting GNU debugger
*/

#ifndef UIGNUDEBUGGERSETTING_H
#define UIGNUDEBUGGERSETTING_H
#include "ui_UIGNUDebuggerSetting.h"

#include "../../src/kernel/gdbKernelDispatcher.h"

#include "UIManageParsingFile.h"

#include <QPointer>
#include <QProcess>

/*!
	\brief Main Ui for setting GNU debugger
	\details This Ui can select path of Gdb, path of gdbparsing.txt file and path of script file.
	You can manage gdbparsing.txt file and enable or not some AddOn

	This Ui find version of Gdb, for this it start Gdb if it can and show the current version
*/
class UIGNUDebuggerSetting : public QWidget, public Ui::UIGNUDebuggerSetting
{
	Q_OBJECT

public:

	UIGNUDebuggerSetting( QWidget* = 0 );

protected slots:

	void on_dbbButtons_clicked( QAbstractButton* button );

	void onReadyRead( );
	void started();
	void finished( int , QProcess::ExitStatus  );
	void error( QProcess::ProcessError );

	void bClickedPathGdb();
	void bClickedPathScript();
	void bClickedPathParsingFile();
	void bClickedManageFile();

	void loadSettings();
	void saveSettings();

private :

	QPointer<QProcess> Process;
	QStringList mPluginList;

	QVBoxLayout *vb;
};

#endif // UIGNUDEBUGGERSETTING_H
