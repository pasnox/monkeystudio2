#include "UIPluginsSettingsElement.h"
#include "UIPluginsSettingsAbout.h"
#include "../BasePlugin.h"
#include "../../coremanager/MonkeyCore.h"
#include "../../settingsmanager/Settings.h"

UIPluginsSettingsElement::UIPluginsSettingsElement( BasePlugin* plugin, QWidget* p )
	: QWidget( p )
{
	Q_ASSERT( plugin );
	mPlugin = plugin;
	mSettingsWidget = mPlugin->settingsWidget();
	BasePlugin::PluginInfos infos = mPlugin->infos();
	
	// setup dialog
	setupUi( this );
	cbEnabled->setChecked( mPlugin->isEnabled() );
	if ( !mPlugin->pixmap().isNull() )
		lIcon->setPixmap( mPlugin->pixmap().scaledToWidth( lIcon->maximumWidth(), Qt::SmoothTransformation ) );
	lTitle->setText( infos.Caption );
	lDescription->setText( infos.Description );
	pbSettings->setVisible( mSettingsWidget );
	cbNeverEnable->setChecked( mPlugin->neverEnable() );
}

UIPluginsSettingsElement::~UIPluginsSettingsElement()
{ delete mSettingsWidget; }

BasePlugin* UIPluginsSettingsElement::plugin() const
{ return mPlugin; }

void UIPluginsSettingsElement::on_cbEnabled_toggled( bool checked )
{
	if ( checked )
		cbNeverEnable->setChecked( false );
	mPlugin->setEnabled( checked );
	MonkeyCore::settings()->setValue( QString( "Plugins/%1" ).arg( mPlugin->infos().Name ), checked );
}

void UIPluginsSettingsElement::on_pbSettings_clicked()
{
	// compute widget infos
	if ( !mSettingsWidget->property( "mInit" ).toBool() )
	{
		mSettingsWidget->setProperty( "mInit", true );
		mSettingsWidget->resize( mSettingsWidget->sizeHint() );
		mSettingsWidget->setParent( window(), Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint );
		mSettingsWidget->setWindowModality( Qt::ApplicationModal );
		mSettingsWidget->setAttribute( Qt::WA_DeleteOnClose, false );
		mSettingsWidget->setWindowIcon( mPlugin->pixmap() );
		mSettingsWidget->setWindowTitle( tr( "Settings %1" ).arg( mPlugin->infos().Caption ) );
	}
	// show widget
	QRect geometry = mSettingsWidget->geometry();
	geometry.moveCenter( window()->geometry().center() );
	if ( geometry.left() < 0 )
		geometry.moveLeft( -geometry.left() );
	if ( geometry.top() < 0 )
		geometry.moveTop( -geometry.top() );
	mSettingsWidget->setGeometry( geometry );
	mSettingsWidget->show();
}

void UIPluginsSettingsElement::on_pbAbout_clicked()
{
	UIPluginsSettingsAbout psa( mPlugin, window() );
	psa.resize( psa.sizeHint() );
	psa.exec();
}

void UIPluginsSettingsElement::on_cbNeverEnable_toggled( bool checked )
{
	if( checked )
		cbEnabled->setChecked( false );
	MonkeyCore::settings()->setValue( QString( "Plugins/%1/NeverEnable" ).arg( mPlugin->infos().Name ), checked );
}
