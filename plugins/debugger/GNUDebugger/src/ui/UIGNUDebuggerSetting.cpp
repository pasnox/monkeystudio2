#include "UIGNUDebuggerSetting.h"
//
#include <QCloseEvent>
//
#include <coremanager.h>
#include <settingsmanager.h>

UIGNUDebuggerSetting::UIGNUDebuggerSetting( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );

}
//
void UIGNUDebuggerSetting::closeEvent( QCloseEvent* e )
{
	e->accept();
}

void UIGNUDebuggerSetting::on_pbApply_clicked()
{
}