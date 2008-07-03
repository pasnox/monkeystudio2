#include "UIGdbCli.h"
//
#include <QCloseEvent>
//
QPointer<UIGdbCli> UIGdbCli::_self = 0L;
//
UIGdbCli* UIGdbCli::self( QWidget* parent )
{
	if ( !_self )
		_self = new UIGdbCli( parent );
	return _self;
}
//
UIGdbCli::UIGdbCli( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
}
//
void UIGdbCli::closeEvent( QCloseEvent* e )
{
	e->accept();
}
