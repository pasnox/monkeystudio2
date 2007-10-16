#ifndef UISETTINGSASTYLE_H
#define UISETTINGSASTYLE_H

#include <QDialog>
#include "ui_UISettingsAStyle.h"

enum AStylePredefinedStyle
{
	aspsAnsi = 0,
	aspsKr,
	aspsLinux,
	aspsGnu,
	aspsJava,
	aspsCustom
};

class UISettingsAStyle : public QWidget, public Ui::UISettingsAStyle
{
	Q_OBJECT
	
public:
	UISettingsAStyle( QWidget* = 0 );
	
private slots:
	void onStyleChange();
	void on_pbApply_clicked();
	
private:
	void loadSettings();
	void saveSettings();
	void setStyle( AStylePredefinedStyle );
	
};

#endif // UISETTINGSASTYLE_H
