//TODO make it standalone application

#include "RegExpEditor.h"
#include "UIRegExpEditor.h"

#include <MonkeyCore.h>
#include <UIMain.h>

BasePlugin::PluginInfos RegExpEditor::infos() const
{
	PluginInfos pluginInfos;
	pluginInfos.Caption = tr( "Regular Expression Editor" );
	pluginInfos.Description = tr( "This plugin allow you to test regular expression for a given buffer." );
	pluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	pluginInfos.Type = BasePlugin::iBase;
	pluginInfos.Name = PLUGIN_NAME;
	pluginInfos.Version = "1.0.0";
	pluginInfos.FirstStartEnabled = false;
	pluginInfos.Pixmap = pIconManager::pixmap( "regexp.png", ":/icons" );
	
	return pluginInfos;
}

RegExpEditor::~RegExpEditor()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool RegExpEditor::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create action
		QAction* a = MonkeyCore::menuBar()->action( "mTools/aRegExpEditor", infos().Caption, infos().Pixmap, QString::null, infos().Description );
		// connections
		connect( a, SIGNAL( triggered() ), this, SLOT( action_triggered() ) );
		// set plugin enabled
		stateAction()->setChecked( true );
	}
	else if ( !b && isEnabled() )
	{
		// delete widget
		delete mEditor;
		// delete action
		delete MonkeyCore::menuBar()->action( "mTools/aRegExpEditor" );
		// set plugin disabled
		stateAction()->setChecked( false );
	}
	// return default value
	return true;
}

void RegExpEditor::action_triggered()
{
	if ( !mEditor )
		mEditor = new UIRegExpEditor( MonkeyCore::mainWindow() );
	mEditor->setVisible( !mEditor->isVisible() );
}

Q_EXPORT_PLUGIN2( BaseRegExpEditor, RegExpEditor )
