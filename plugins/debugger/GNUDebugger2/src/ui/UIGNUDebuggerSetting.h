#ifndef UIGNUDEBUGGERSETTING_H
#define UIGNUDEBUGGERSETTING_H
#include "ui_UIGNUDebuggerSetting.h"

#include "../../src/kernel/gdbKernelDispatcher.1.3.h"

#include "UIManageParsingFile.h"

#include <QPointer>
#include <QProcess>

//! Main Ui for setting GNU debugger
/**
\author xiantia.

This Ui can select path of Gdb, path of gdbparsing.txt file and path of script file.
You can manage gdbparsing.txt file and enable or not some AddOn

This Ui find version of Gdb, for this it start Gdb if it can and show the current version
*/

class UIGNUDebuggerSetting : public QWidget, public Ui::UIGNUDebuggerSetting
{
	Q_OBJECT

public:

	UIGNUDebuggerSetting( QWidget* = 0 );

//	void setKernel(QList<QPointer< class GdbCore> >);

protected slots:

	//! apply button clicked
	void on_dbbButtons_clicked( QAbstractButton* button );

	//! Data from Gdb (it is not GdbProcess)
	/**
	Get the current version

	\code
		GNU gdb 6.7.50.20071127
		Copyright (C) 2007 Free Software Foundation, Inc.
		License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
		This is free software: you are free to change and redistribute it.
		There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
		and "show warranty" for details.
		This GDB was configured as "i686-pc-mingw32".
	\endcode
	
	The current version is "GNU gdb 6.7.50.20071127"

	*/
	void onReadyRead( );
	//! Process Gdb correctly started 
	void started();
	//! Process Gdb correctly finished
	void finished( int , QProcess::ExitStatus  );
	//! Process crash
	void error( QProcess::ProcessError );

	//! Button select path of Gdb clicked
	void bClickedPathGdb();
	//! Button select path of script clicked
	void bClickedPathScript();
	//! Button select path of gdbparsing.txt clicked
	void bClickedPathParsingFile();
	//! Button manage parse file clicked
	void bClickedManageFile();

	//! Load current setting
	void loadSettings();
	//! Save current setting
	void saveSettings();

private :

	
	QPointer<QProcess> Process;
	QStringList mPluginList;

	QVBoxLayout *vb;
};

#endif // UIGNUDEBUGGERSETTING_H
