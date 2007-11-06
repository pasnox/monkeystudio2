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
