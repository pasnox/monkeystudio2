#include "MessageBox.h"
#include "UIMain.h"
#include "pMenuBar.h"
#include "pDockToolBar.h"
#include "pDockMessageBox.h"

#include <QIcon>
#include <QTabWidget>

MessageBox::MessageBox()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Message Box" );
	mPluginInfos.Description = tr( "Plugin for catching internal command console" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "MessageBox";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

MessageBox::~MessageBox()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool MessageBox::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		UIMain::instance()->dockToolBar( Qt::BottomToolBarArea )->addDock( pDockMessageBox::instance(), infos().Caption, QIcon( ":/icons/console.png" ) );
		// connect signals so we will be able to save/restore state of dock settings
		connect( pMenuBar::instance()->action( "mView/aShowErrors", tr( "Show Errors" ), QIcon( ":/icons/taberror.png" ) ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showErrors() ) );
		connect( pMenuBar::instance()->action( "mView/aShowMessages", tr( "Show Messages" ), QIcon( ":/icons/tabmessage.png" ) ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showMessages() ) );
		connect( pDockMessageBox::instance(), SIGNAL( saveSettings() ), this, SLOT( saveSettings() ) );
		connect( pDockMessageBox::instance(), SIGNAL( restoreSettings() ), this, SLOT( restoreSettings() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		delete pMenuBar::instance()->action( "mView/aShowErrors" );
		delete pMenuBar::instance()->action( "mView/aShowMessages" );
		// it will remove itself from dock toolbar when deleted
		pDockMessageBox::instance()->deleteLater();
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

void MessageBox::saveSettings()
{
	// save current tab index
	setSettingsValue( "CurrentIndex", pDockMessageBox::instance()->twMessageBox->currentIndex() );
}

void MessageBox::restoreSettings()
{
	// restore tab index
	pDockMessageBox::instance()->twMessageBox->setCurrentIndex( settingsValue( "CurrentIndex", 0 ).toInt() );
}

Q_EXPORT_PLUGIN2( BaseMessageBox, MessageBox )
