#ifndef UIGNUDEBUGGERSETTING_H
#define UIGNUDEBUGGERSETTING_H
#include "ui_UIGNUDebuggerSetting.h"

class UIGNUDebuggerSetting : public QWidget, public Ui::UIGNUDebuggerSetting
{
	Q_OBJECT

public:
	UIGNUDebuggerSetting( QWidget* = 0 );

protected slots:
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // UIGNUDEBUGGERSETTING_H
