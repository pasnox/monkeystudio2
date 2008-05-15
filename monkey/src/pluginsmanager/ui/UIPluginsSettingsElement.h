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
	BasePlugin* plugin() const;

protected:
	BasePlugin* mPlugin;

protected slots:
};

#endif // UIPLUGINSSETTINGSELEMENT_H
