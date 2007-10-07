#include "Gcc.h"
#include "GccParser.h"
#include "pConsoleManager.h"

Gcc::Gcc ()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Gcc" );
	mPluginInfos.Description = tr( "Plugin for execute gcc in console and parse it's output" );
	mPluginInfos.Author = "Kopats Andrei aka hlamer <hlamer@tut.by>";
	mPluginInfos.Type = BasePlugin::iCLITool;
	mPluginInfos.Name = "Gcc";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
}

bool Gcc::setEnabled( bool b)
{
	if ( b == mPluginInfos.Enabled)
		return true;
	mPluginInfos.Enabled = b;
 	if (b)
 		pConsoleManager::instance()->addParser (new GccParser());
 	else
 		pConsoleManager::instance()->removeParser ("Gcc");
	return true;
}

QString Gcc::name()
{
    return QString ("Gcc");
}

QString Gcc::getCommand ()
{
    //here possible will be some code, determining needed version of compiler ( gcc-4.1, etc )
    return QString ("gcc"); 
}

QString Gcc::getSwitches (QString oldSwitches)
{
    //here possible will be code, showing to user config dialog
    return oldSwitches;
}

pCommandParser* Gcc::getParser()
{
    return new GccParser();
}

Q_EXPORT_PLUGIN2( CLIToolGcc, Gcc )

