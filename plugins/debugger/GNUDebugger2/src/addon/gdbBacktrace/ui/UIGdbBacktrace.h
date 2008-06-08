#ifndef UIGDBBACKTRACE_H
#define UIGDBBACKTRACE_H
//
#include "ui_UIGdbBacktrace.h"
//
#include <QPointer>
//
class UIGdbBacktrace : public QWidget, public Ui::UIGdbBacktrace
{
	Q_OBJECT
	//
public:
	static UIGdbBacktrace* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbBacktrace( QWidget* = 0 );
	static QPointer<UIGdbBacktrace> _self;
	//
};
//
#endif // UIGDBBACKTRACE_H
