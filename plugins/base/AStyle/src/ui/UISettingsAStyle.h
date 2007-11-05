/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UISettingsAStyle.h
** Date      : 2007-11-04T22:50:29
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
