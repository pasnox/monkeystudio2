/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIItemSettings.h
** Date      : 2007-11-04T22:53:37
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef UIITEMSETTINGS_H
#define UIITEMSETTINGS_H

#include "ui_UIItemSettings.h"
#include "QSingleton.h"

class ProjectItem;

class UIItemSettings : public QDialog, public Ui::UIItemSettings, public QSingleton<UIItemSettings>
{
	Q_OBJECT
	friend class QSingleton<UIItemSettings>;
	friend class QValueFileFolderEdit;
	
public:
	static bool edit( ProjectItem*, QWidget* = 0 );
	
protected:
	UIItemSettings( ProjectItem* );
	
	ProjectItem* mItem;
	
private slots:
	void accept();
	
};

#endif // UIITEMSETTINGS_H
