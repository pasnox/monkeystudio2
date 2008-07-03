#include "UIGdbBacktrace.h"
//
#include <QCloseEvent>
//
QPointer<UIGdbBacktrace> UIGdbBacktrace::_self = 0L;
//
UIGdbBacktrace* UIGdbBacktrace::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbBacktrace( parent );
	return _self;
}
//
UIGdbBacktrace::UIGdbBacktrace( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
}
//
void UIGdbBacktrace::closeEvent( QCloseEvent* e )
{
	e->accept();
}
