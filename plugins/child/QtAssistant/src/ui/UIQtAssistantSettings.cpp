#include "UIQtAssistantSettings.h"
#include "../QtAssistant.h"

#include <QLibraryInfo>
#include <QFileDialog>
#include <QWhatsThis>
#include <QPushButton>

UIQtAssistantSettings::UIQtAssistantSettings( QtAssistant* plugin, QWidget* parent )
	: QWidget( parent )
{
	mPlugin = plugin;
	setupUi( this );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QIcon( ":/help/icons/help/keyword.png" ) );
	dbbButtons->button( QDialogButtonBox::RestoreDefaults )->setIcon( QIcon( ":/file/icons/file/backup.png" ) );
	dbbButtons->button( QDialogButtonBox::Apply )->setIcon( QIcon( ":/file/icons/file/save.png" ) );
	// init gui
	const QString docPath = mPlugin->settingsValue( "DocPath", QLibraryInfo::location( QLibraryInfo::DocumentationPath ) ).toString();
	leDocPath->setText( docPath );
}

void UIQtAssistantSettings::on_tbDocPath_clicked()
{
	const QString path = QFileDialog::getExistingDirectory( window(), tr( "Locate your Qt 4 doc path" ), leDocPath->text() );
	if ( !path.isEmpty() )
		leDocPath->setText( path );
}

void UIQtAssistantSettings::on_dbbButtons_clicked( QAbstractButton* button )
{
	if ( button == dbbButtons->button( QDialogButtonBox::Help ) )
	{
		const QString help = tr( "If the Qt documentation path can't be found, locate the doc path of your Qt install to fix it." );
		QWhatsThis::showText( mapToGlobal( rect().center() ), help, this ) ;
	}
	else if ( button == dbbButtons->button( QDialogButtonBox::RestoreDefaults ) )
	{
		const QString docPath = QLibraryInfo::location( QLibraryInfo::DocumentationPath );
		leDocPath->setText( docPath );
	}
	else if ( button == dbbButtons->button( QDialogButtonBox::Apply ) )
	{
		mPlugin->setSettingsValue( "DocPath", leDocPath->text() );
	}
}
