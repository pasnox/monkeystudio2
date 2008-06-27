#ifndef UIMESSAGEBOXSETTINGS_H
#define UIMESSAGEBOXSETTINGS_H

#include <QWidget>

#include "ui_UIMessageBoxSettings.h"

class MessageBox;

class UIMessageBoxSettings : public QWidget, public Ui::UIMessageBoxSettings
{
	Q_OBJECT
	
public:
	enum Dock { BuildStep = 0, Output, Command };
	UIMessageBoxSettings( MessageBox* plugin, QWidget* parent = 0 );

protected:
	MessageBox* mPlugin;

protected slots:
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // UIMESSAGEBOXSETTINGS_H
