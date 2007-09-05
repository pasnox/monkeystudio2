#ifndef UIITEMSETTINGS_H
#define UIITEMSETTINGS_H
//
#include "ui_UIItemSettings.h"
//
#include <QPointer>
//
class QMakeProjectItem;
class QMakeProjectModel;
//
class UIItemSettings : public QDialog, public Ui::UIItemSettings
{
	Q_OBJECT
	//
public:
	static UIItemSettings* edit( QMakeProjectModel*, QMakeProjectItem*, QWidget* = 0 );
	//
protected:
	UIItemSettings( QMakeProjectModel*, QMakeProjectItem*, QWidget* = 0 );
	//
	static QPointer<UIItemSettings> mSelf;
	QMakeProjectItem* mItem;
	QMakeProjectModel* mProject;
	//
private slots:
	void accept();
	//
};
//
#endif // UIITEMSETTINGS_H
