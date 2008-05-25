#include "UIGNUDebuggerSetting.h"

#include <coremanager.h>
#include <settingsmanager.h>

#include <QPushButton>

UIGNUDebuggerSetting::UIGNUDebuggerSetting( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
}

void UIGNUDebuggerSetting::on_dbbButtons_clicked( QAbstractButton* button )
{
	if ( button == dbbButtons->button( QDialogButtonBox::Apply ) )
	{
		// hjere your code for apply/save
	}
}
