/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : MessageBox.cpp
** Date      : 2007-11-04T22:50:38
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
		connect( pMenuBar::instance()->action( "mView/aShowBuild", tr( "Show Build" ), QIcon( ":/icons/tabbuild.png" ) ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showBuild() ) );
		connect( pMenuBar::instance()->action( "mView/aShowOutput", tr( "Show Output" ), QIcon( ":/icons/taboutput.png" ) ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showOutput() ) );
		connect( pMenuBar::instance()->action( "mView/aShowLog", tr( "Show Log" ), QIcon( ":/icons/tablog.png" ) ), SIGNAL( triggered() ), pDockMessageBox::instance(), SLOT( showLog() ) );
		connect( pDockMessageBox::instance(), SIGNAL( saveSettings() ), this, SLOT( saveSettings() ) );
		connect( pDockMessageBox::instance(), SIGNAL( restoreSettings() ), this, SLOT( restoreSettings() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		delete pMenuBar::instance()->action( "mView/aShowBuild" );
		delete pMenuBar::instance()->action( "mView/aShowOutput" );
		delete pMenuBar::instance()->action( "mView/aShowLog" );
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
