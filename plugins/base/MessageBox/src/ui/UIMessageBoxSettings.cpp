#include "UIMessageBoxSettings.h"
#include "../MessageBox.h"

#include <QWhatsThis>
#include <QPushButton>

UIMessageBoxSettings::UIMessageBoxSettings( MessageBox* plugin, QWidget* parent )
	: QWidget( parent )
{
	mPlugin = plugin;
	setupUi( this );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QIcon( ":/help/icons/help/keyword.png" ) );
	dbbButtons->button( QDialogButtonBox::RestoreDefaults )->setIcon( QIcon( ":/file/icons/file/backup.png" ) );
	dbbButtons->button( QDialogButtonBox::Apply )->setIcon( QIcon( ":/file/icons/file/save.png" ) );
	// fill combobox
	cbActivateDock->addItem( tr( "Build Step" ), UIMessageBoxSettings::BuildStep );
	cbActivateDock->addItem( tr( "Output" ), UIMessageBoxSettings::Output );
	cbActivateDock->addItem( tr( "Commands" ), UIMessageBoxSettings::Command );
	// restore settings
	gbActivateDock->setChecked( mPlugin->settingsValue( "ActivateDock", true ).toBool() );
	UIMessageBoxSettings::Dock dock = (UIMessageBoxSettings::Dock)mPlugin->settingsValue( "ActivatedDock", UIMessageBoxSettings::Output ).toInt();
	cbActivateDock->setCurrentIndex( cbActivateDock->findData( dock ) );
}

void UIMessageBoxSettings::on_dbbButtons_clicked( QAbstractButton* button )
{
	if ( button == dbbButtons->button( QDialogButtonBox::Help ) )
	{
		const QString help = tr( "You can activate a special Message Box dock when console is started, for this check the box and choose witch dock to activate." );
		QWhatsThis::showText( mapToGlobal( rect().center() ), help, this ) ;
	}
	else if ( button == dbbButtons->button( QDialogButtonBox::RestoreDefaults ) )
	{
		gbActivateDock->setChecked( true );
		cbActivateDock->setCurrentIndex( cbActivateDock->findData( UIMessageBoxSettings::Output ) );
	}
	else if ( button == dbbButtons->button( QDialogButtonBox::Apply ) )
	{
		mPlugin->setSettingsValue( "ActivateDock", gbActivateDock->isChecked() );
		mPlugin->setSettingsValue( "ActivatedDock", cbActivateDock->itemData( cbActivateDock->currentIndex() ).toInt() );
	}
}

