#ifndef UIITEMSETTINGS_H
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
