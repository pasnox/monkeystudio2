#ifndef UISETTING_H
#define UISETTING_H
//
#include "ui_UIGNUDebuggerSetting.h"
//

class UIGNUDebuggerSetting : public QWidget, public Ui::UIGNUDebuggerSetting
{
	Q_OBJECT
	//
public:
	UIGNUDebuggerSetting( QWidget* = 0 );

	//
private slots:
	void on_pbApply_clicked();

protected:
	void closeEvent( QCloseEvent* );
	//
};
//
#endif // UISETTING_H
