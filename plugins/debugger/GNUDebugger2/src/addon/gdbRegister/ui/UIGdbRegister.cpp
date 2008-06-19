#include "UIGdbRegister.h"
//
#include <QCloseEvent>
//
QPointer<UIGdbRegister> UIGdbRegister::_self = 0L;
//
UIGdbRegister* UIGdbRegister::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbRegister( parent );
	return _self;
}
//
UIGdbRegister::UIGdbRegister( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
}
//
void UIGdbRegister::closeEvent( QCloseEvent* e )
{
	e->accept();
}
