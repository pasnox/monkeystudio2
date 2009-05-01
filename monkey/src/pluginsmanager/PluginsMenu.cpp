#include "PluginsMenu.h"
#include "PluginsManager.h"
#include "UIPluginsSettingsAbout.h"

#include <QDesktopWidget>

PluginsMenu::PluginsMenu( PluginsManager* manager )
	: QMenu()
{
	Q_ASSERT( manager );
	
	mManager = manager;
}

PluginsMenu::~PluginsMenu()
{
}

void PluginsMenu::initPluginMenusActions( BasePlugin* plugin, BasePlugin::Type type )
{
	const TypeStringPair pair = qMakePair( type, plugin->captionVersionString() );
	QMenu* menu = mMenus[ pair ];
	
	if ( !menu )
	{
		QMenu* tmenu = mTypeMenus[ type ];
		
		if ( !tmenu )
		{
			tmenu = addMenu( BasePlugin::typeToString( type ) );
			mTypeMenus[ type ] = tmenu;
		}
		
		menu = tmenu->addMenu( plugin->pixmap(), plugin->infos().Caption );
		mMenus[ pair ] = menu;
		
		menu->addAction( plugin->stateAction() );
		plugin->stateAction()->disconnect( this );
		connect( plugin->stateAction(), SIGNAL( triggered( bool ) ), this, SLOT( actionEnable_triggered( bool ) ) );
		
		if ( plugin->haveSettingsWidget() )
		{
			QAction* action = menu->addAction( tr( "Configure..." ) );
			action->setData( QVariant::fromValue( plugin ) );
			connect( action, SIGNAL( triggered() ), this, SLOT( actionConfigure_triggered() ) );
		}
		
		menu->addSeparator();
		
		QAction* actionAbout = menu->addAction( tr( "About..." ) );
		actionAbout->setData( QVariant::fromValue( plugin ) );
		connect( actionAbout, SIGNAL( triggered() ), this, SLOT( actionAbout_triggered() ) );
	}
}

void PluginsMenu::addPlugin( BasePlugin* plugin )
{
	BasePlugin::Types type = plugin->infos().Type;
	
	initPluginMenusActions( plugin, BasePlugin::iAll );
	
	if ( type & BasePlugin::iBase )
	{
		initPluginMenusActions( plugin, BasePlugin::iBase );
	}
	else if ( type & BasePlugin::iChild )
	{
		initPluginMenusActions( plugin, BasePlugin::iChild );
	}
	else if ( type & BasePlugin::iCLITool )
	{
		initPluginMenusActions( plugin, BasePlugin::iCLITool );
	}
	else if ( type & BasePlugin::iBuilder )
	{
		initPluginMenusActions( plugin, BasePlugin::iBuilder );
	}
	else if ( type & BasePlugin::iCompiler )
	{
		initPluginMenusActions( plugin, BasePlugin::iCompiler );
	}
	else if ( type & BasePlugin::iDebugger )
	{
		initPluginMenusActions( plugin, BasePlugin::iDebugger );
	}
	else if ( type & BasePlugin::iInterpreter )
	{
		initPluginMenusActions( plugin, BasePlugin::iInterpreter );
	}
	else if ( type & BasePlugin::iXUP )
	{
		initPluginMenusActions( plugin, BasePlugin::iXUP );
	}
}

void PluginsMenu::actionEnable_triggered( bool checked )
{
	QAction* action = qobject_cast<QAction*>( sender() );
	BasePlugin* plugin = action->data().value<BasePlugin*>();
	
	action->setChecked( !checked );
	plugin->setEnabled( checked );
}

void PluginsMenu::actionConfigure_triggered()
{
	QAction* action = qobject_cast<QAction*>( sender() );
	BasePlugin* plugin = action->data().value<BasePlugin*>();
	QWidget* widget = plugin->settingsWidget();
	
#ifdef Q_OS_MAC
	widget->setParent( qApp->activeWindow(), Qt::Sheet );
#else
	widget->setParent( qApp->activeWindow(), Qt::Dialog );
#endif
	widget->setWindowModality( Qt::ApplicationModal );
	widget->setAttribute( Qt::WA_DeleteOnClose );
	widget->adjustSize();
	
	QRect rect = widget->frameGeometry();
	QRect drect = qApp->desktop()->availableGeometry( qApp->activeWindow() );
	rect.moveCenter( drect.center() );
	
	widget->move( rect.topLeft() );
	widget->show();
}

void PluginsMenu::actionAbout_triggered()
{
	QAction* action = qobject_cast<QAction*>( sender() );
	BasePlugin* plugin = action->data().value<BasePlugin*>();
	UIPluginsSettingsAbout about( plugin, qApp->activeWindow() );
	
	about.exec();
}
