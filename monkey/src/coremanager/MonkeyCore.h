/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef MONKEYCORE_H
#define MONKEYCORE_H

#include <MonkeyExport.h>

#include <QObject>
#include <QHash>

class Settings;
class PluginsManager;
class UIMain;
class pMenuBar;
class pRecentsManager;
class pActionsManager;
//class pToolsManager;
class XUPProjectManager;
class pFileManager;
class pWorkspace;
class QStatusBar;
class pConsoleManager;
class StatusBar;
class pQueuedMessageToolBar;
class MkSShellInterpreter;
class pAbbreviationsManager;
class pMultiToolBar;
class TranslationManager;
class ProjectTypesIndex;

class Q_MONKEY_EXPORT MonkeyCore : public QObject
{
    Q_OBJECT
    
public:
    static void init();
    static Settings* settings();
    static PluginsManager* pluginsManager();
    static UIMain* mainWindow();
    static pMenuBar* menuBar();
    static pRecentsManager* recentsManager();
    static pActionsManager* actionsManager();
    static XUPProjectManager* projectsManager();
    static pFileManager* fileManager();
    static pWorkspace* workspace();
    static pConsoleManager* consoleManager();
    static pQueuedMessageToolBar* messageManager();
    static StatusBar* statusBar();
    static MkSShellInterpreter* interpreter();
    static pAbbreviationsManager* abbreviationsManager();
    static pMultiToolBar* multiToolBar();
    static TranslationManager* translationsManager();
    static ProjectTypesIndex* projectTypesIndex();
// variablemaanger
// templates manager

private:
    static QHash<const QMetaObject*, QObject*> mInstances;
};

#endif // MONKEYCORE_H
