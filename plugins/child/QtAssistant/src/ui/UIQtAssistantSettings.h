#ifndef UIQTASSISTANTSETTINGS_H
#define UIQTASSISTANTSETTINGS_H

#include "ui_UIQtAssistantSettings.h"

class QtAssistant;

class UIQtAssistantSettings : public QWidget, protected Ui::UIQtAssistantSettings
{
	Q_OBJECT
	
public:
	UIQtAssistantSettings( QtAssistant* plugin, QWidget* parent = 0 );

protected:
	QtAssistant* mPlugin;

protected slots:
	void on_tbDocPath_clicked();
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // UIQTASSISTANTSETTINGS_H
