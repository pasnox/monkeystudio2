#include "GccParser.h"
#include "Parser.h"

GccParser::GccParser()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "GccParser" );
	mPluginInfos.Description = tr( "Plugin for parsing output of GNU Compiler Collection" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "0.1.0";
	mPluginInfos.Enabled = false;
}

GccParser::~GccParser()
{
	setEnabled (false);
}

bool GccParser::setEnabled( bool b)
{
	mPluginInfos.Enabled = b;
 	if ( b )
		pConsoleManager::instance()->addParser( new Parser(this) );
 	else
		pConsoleManager::instance()->removeParser( PLUGIN_NAME );
	return true;
}

Q_EXPORT_PLUGIN2( BaseGccParser, GccParser )
