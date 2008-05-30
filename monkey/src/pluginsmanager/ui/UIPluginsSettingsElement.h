#ifndef UIPLUGINSSETTINGSELEMENT_H
#define UIPLUGINSSETTINGSELEMENT_H

#include <fresh.h>

#include "ui_UIPluginsSettingsElement.h"

class BasePlugin;

class Q_MONKEY_EXPORT UIPluginsSettingsElement : public QWidget, public Ui::UIPluginsSettingsElement
{
	Q_OBJECT
	
public:
	UIPluginsSettingsElement( BasePlugin* plugin, QWidget* parent = 0 );
	~UIPluginsSettingsElement();
	
	BasePlugin* plugin() const;

protected:
	BasePlugin* mPlugin;
	QWidget* mSettingsWidget;

protected slots:
	void on_cbEnabled_toggled( bool checked );
	void on_pbSettings_clicked();
	void on_pbAbout_clicked();
	void on_cbNeverEnable_toggled( bool checked );
};

#endif // UIPLUGINSSETTINGSELEMENT_H
