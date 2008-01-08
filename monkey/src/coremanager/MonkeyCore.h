#ifndef MONKEYCORE_H
#define MONKEYCORE_H

#include "MonkeyExport.h"
#include <QObject>
#include <QHash>

class pSettings;
class PluginsManager;
class UIMain;
class pMenuBar;
class UIProjectsManager;
class pFileManager;
class pWorkspace;
class pSearch;
class QStatusBar;
class pConsoleManager;
class pRecentsManager;
class pToolsManager;

class Q_MONKEY_EXPORT MonkeyCore : public QObject
{
	Q_OBJECT
	
public:
	static void init();
	static pSettings* settings();
	static PluginsManager* pluginsManager();
	static UIMain* mainWindow();
	static pMenuBar* menuBar();
	static pRecentsManager* recentsManager();
	static pToolsManager* toolsManager();
	static UIProjectsManager* projectsManager();
	static pFileManager* fileManager();
	static pWorkspace* workspace();
	static pConsoleManager* consoleManager();
	static pSearch* searchDock();
	static QStatusBar* statusBar();
// variablemaanger

private:
	static QHash<const QMetaObject*, QObject*> mInstances;
};

#endif // MONKEYCORE_H
