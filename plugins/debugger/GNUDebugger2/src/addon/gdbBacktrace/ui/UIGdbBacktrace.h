#ifndef UIGDBBACKTRACE_H
#define UIGDBBACKTRACE_H
//
#include "ui_UIGdbBacktrace.h"
//
#include <QPointer>
//

//! Ui for show backtrace in QTabWiget. It used by GdbBacktrace class. 
/**
\author xiantia
\version 1.3.2
*/

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
