#ifndef UIITEMSETTINGS_H
#define UIITEMSETTINGS_H

#include "ui_UIItemSettings.h"

class ProjectItem;
class ProjectsModel;

class UIItemSettings : public QDialog, public Ui::UIItemSettings
{
	Q_OBJECT
	
public:
	static bool edit( ProjectsModel*, ProjectItem*, QWidget* = 0 );
	
protected:
	UIItemSettings( ProjectsModel*, ProjectItem*, QWidget* = 0 );
	
	ProjectItem* mItem;
	ProjectsModel* mProject;
	
private slots:
	void accept();
	
};

#endif // UIITEMSETTINGS_H
