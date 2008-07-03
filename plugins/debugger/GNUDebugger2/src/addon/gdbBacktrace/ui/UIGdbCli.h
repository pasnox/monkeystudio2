#ifndef UIGDCLI_H
#define UIGDCLI_H
//
#include "ui_UIGdbCli.h"
//
#include <QPointer>
//
class UIGdbCli : public QWidget, public Ui::UIGdbCli
{
	Q_OBJECT
	//
public:
	static UIGdbCli* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbCli( QWidget* = 0 );
	static QPointer<UIGdbCli> _self;
	//
};
//
#endif // UIGDBBACKTRACE_H
