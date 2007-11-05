/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIPluginsSettings.h
** Date      : 2007-11-04T22:45:26
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef UIPLUGINSSETTINGS_H
#define UIPLUGINSSETTINGS_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UIPluginsSettings.h"

#include <QApplication>

class PluginsManager;

class Q_MONKEY_EXPORT UIPluginsSettings : public QDialog, public Ui::UIPluginsSettings, public QSingleton<UIPluginsSettings>
{
	Q_OBJECT
	friend class QSingleton<UIPluginsSettings>;
	
public:
	void clearInfos();
	void updateList();
	
private:
	UIPluginsSettings( QWidget* = QApplication::activeWindow() );
	PluginsManager* mPluginsManager;
	
private slots:
	void on_cbType_currentIndexChanged( int );
	void on_lwNames_itemSelectionChanged();
	void on_cbEnableUponStart_clicked( bool );
	void on_cbEnabled_clicked( bool );
	virtual void accept();
	
};

#endif // UIPLUGINSSETTINGS_H
