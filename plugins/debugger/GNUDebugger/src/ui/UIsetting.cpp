#include "UIsetting.h"
//
#include <QCloseEvent>
//
UIsetting::UIsetting( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	show();
}
//
void UIsetting::closeEvent( QCloseEvent* e )
{
	e->accept();
}
