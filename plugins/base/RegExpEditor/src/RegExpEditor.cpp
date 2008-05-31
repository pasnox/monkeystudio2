#include "RegExpEditor.h"
#include "UIRegExpEditor.h"

#include <coremanager.h>
#include <maininterface.h>

RegExpEditor::RegExpEditor()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Regular Expression Editor" );
	mPluginInfos.Description = tr( "This plugin allow you to test regular expression for a given buffer." );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
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
		QAction* a = MonkeyCore::menuBar()->action( "mTools/aRegExpEditor", infos().Caption, pixmap(), QString::null, mPluginInfos.Description );
		// connections
		connect( a, SIGNAL( triggered() ), this, SLOT( action_triggered() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// delete widget
		delete mEditor;
		// delete action
		delete MonkeyCore::menuBar()->action( "mTools/aRegExpEditor" );
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

QPixmap RegExpEditor::pixmap() const
{ return QPixmap( ":/edit/icons/edit/search.png" ); }

void RegExpEditor::action_triggered()
{
	if ( !mEditor )
		mEditor = new UIRegExpEditor( MonkeyCore::mainWindow() );
	mEditor->setVisible( !mEditor->isVisible() );
}

Q_EXPORT_PLUGIN2( BaseRegExpEditor, RegExpEditor )
