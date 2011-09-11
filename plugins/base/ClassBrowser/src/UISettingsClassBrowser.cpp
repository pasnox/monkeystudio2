#include "UISettingsClassBrowser.h"
#include "ui_UISettingsClassBrowser.h"
#include "ClassBrowser.h"

#include <QFileDialog>

UISettingsClassBrowser::UISettingsClassBrowser( ClassBrowser* plugin, QWidget* parent )
	: QWidget( parent ), mPlugin( plugin ), ui( new Ui_UISettingsClassBrowser )
{
	qCtagsSenseProperties properties = mPlugin->properties();
	
	ui->setupUi( this );
	
	ui->cbIntegrationMode->addItem( tr( "Dock" ), ClassBrowser::imDock );
	ui->cbIntegrationMode->addItem( tr( "Combo" ), ClassBrowser::imCombo );
	ui->cbIntegrationMode->addItem( tr( "Both" ), ClassBrowser::imBoth );
	ui->cbIntegrationMode->setCurrentIndex( ui->cbIntegrationMode->findData( plugin->integrationMode() ) );
	
	ui->gbUseDBFileName->setChecked( properties.UsePhysicalDatabase );
	ui->leDBFileName->setText( properties.DatabaseFileName );
	ui->pePaths->setValues( properties.SystemPaths );
	ui->sleSuffixes->setValues( properties.FilteredSuffixes );
}

UISettingsClassBrowser::~UISettingsClassBrowser()
{
	delete ui;
}

void UISettingsClassBrowser::on_tbDBFileName_clicked()
{
	const QString fn = QFileDialog::getSaveFileName( this, tr( "Select a filename to use for the temporary database" ), ui->leDBFileName->text() );
	
	if ( !fn.isNull() ) {
		ui->leDBFileName->setText( fn );
	}
}

void UISettingsClassBrowser::on_dbbButtons_clicked( QAbstractButton* button )
{
	Q_UNUSED( button );
	qCtagsSenseProperties properties;
	properties.SystemPaths = ui->pePaths->values();
	properties.FilteredSuffixes = ui->sleSuffixes->values();
	properties.UsePhysicalDatabase = ui->gbUseDBFileName->isChecked();
	properties.DatabaseFileName = ui->leDBFileName->text();
	
	mPlugin->setIntegrationMode( (ClassBrowser::IntegrationMode)ui->cbIntegrationMode->itemData( ui->cbIntegrationMode->currentIndex() ).toInt() );
	mPlugin->setProperties( properties );
}
