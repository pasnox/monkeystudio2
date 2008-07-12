#ifndef UIGDBREGISTER_H
#define UIGDBREGISTER_H
//
#include "ui_UIGdbRegister.h"
//
#include <QPointer>
//

//! Ui for show registers values in QTabWiget. It used by GdbRegister class. 
/**
\author xiantia
\version 1.3.2
*/
class UIGdbRegister : public QWidget, public Ui::UIGdbRegister
{
	Q_OBJECT
	//
public:
	static UIGdbRegister* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	UIGdbRegister( QWidget* = 0 );
	static QPointer<UIGdbRegister> _self;
	//
};
//
#endif // UIGDBBACKTRACE_H
