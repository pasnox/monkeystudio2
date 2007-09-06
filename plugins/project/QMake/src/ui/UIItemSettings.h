#ifndef UIITEMSETTINGS_H
#define UIITEMSETTINGS_H

#include "ui_UIItemSettings.h"

#include <QPointer>

class ProjectItem;
class ProjectsModel;

class UIItemSettings : public QDialog, public Ui::UIItemSettings
{
	Q_OBJECT
	
public:
	static UIItemSettings* edit( ProjectsModel*, ProjectItem*, QWidget* = 0 );
	
protected:
	UIItemSettings( ProjectsModel*, ProjectItem*, QWidget* = 0 );
	
	static QPointer<UIItemSettings> mSelf;
	ProjectItem* mItem;
	ProjectsModel* mProject;
	
private slots:
	void accept();
	
};

#endif // UIITEMSETTINGS_H
