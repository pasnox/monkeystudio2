#include "pAStyle.h"
#include "UISettingsAStyle.h"
#include "pMenuBar.h"
#include "pAbstractChild.h"
#include "pWorkspace.h"
#include "pEditor.h"
#include "astyle.h"
#include "pFormatterSettings.h"

pAStyle::pAStyle()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "AStyle Formatter" );
	mPluginInfos.Description = tr( "Uses AStyle to reformat your sources. Useful when copying code from the net or if you just want to reformat your sources based on a specific style" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

pAStyle::~pAStyle()
{
	if ( isEnabled() )
		setEnabled( false );
}

QWidget* pAStyle::settingsWidget()
{ return new UISettingsAStyle; }

bool pAStyle::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create action
		QAction* a = pMenuBar::instance()->action( "mEdit/aAStyle",  tr( "AStyle Formatter" ), QIcon( ":/icons/astyle.png" ), tr( "Ctrl+Alt+A" ), mPluginInfos.Description );
		connect( a, SIGNAL( triggered() ), this, SLOT( applyFormatter() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// delete action
		delete pMenuBar::instance()->action( "mEdit/aAStyle" );
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

void pAStyle::applyFormatter()
{
	if ( pAbstractChild* c = pWorkspace::instance()->currentChild() )
	{
		if ( pEditor* e = c->currentEditor() )
		{
			// vars
			QString s1 = e->text();
			QString s2;
			astyle::ASFormatter f;
			
			// load settings
			pFormatterSettings s;
			s.applyTo( f );
			
			// eol
			QString eolChars;
			switch ( e->eolMode() )
			{
			case pEditor::EolWindows:
				eolChars = "\r\n";
				break;
			case pEditor::EolUnix:
				eolChars = "\n";
				break;
			case pEditor::EolMac:
				eolChars = "\r";
				break;
			}
			
			// add eol
			if ( s1.length() && !s1.endsWith( '\r' ) && !s1.endsWith( '\n' ) )
				s1 += eolChars;
			
			// iterate lines
			istringstream iter( s1.toStdString() );
			f.init( iter );
			while ( f.hasMoreLines() )
			{
				s2.append( f.nextLine().c_str() );
				if ( f.hasMoreLines() )
					s2.append( eolChars );
			}
			
			// update text and restore cursor position
			int l, i;
			e->getCursorPosition(  &l, &i );
			e->clear();
			e->insert( s2 );
			e->setCursorPosition( l, i );
		}
	}
}

Q_EXPORT_PLUGIN2( BasepAStyle, pAStyle )
