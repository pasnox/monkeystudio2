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
//TODO make it standalone application

#include "RegExpEditor.h"
#include "UIRegExpEditor.h"

#include <coremanager/MonkeyCore.h>
#include <maininterface/UIMain.h>
#include <pMenuBar.h>

void RegExpEditor::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "Regular Expression Editor" );
    mPluginInfos.Description = tr( "This plugin allow you to test regular expression for a given buffer." );
    mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "1.0.0";
    mPluginInfos.FirstStartEnabled = false;
    mPluginInfos.Pixmap = pIconManager::pixmap( "regexp.png", ":/icons" );
}

bool RegExpEditor::install()
{
    // create action
    QAction* a = MonkeyCore::menuBar()->action( "mTools/aRegExpEditor", infos().Caption, infos().Pixmap, QString::null, infos().Description );
    // connections
    connect( a, SIGNAL( triggered() ), this, SLOT( action_triggered() ) );
    return true;
}

bool RegExpEditor::uninstall()
{
    // delete widget
    delete mEditor;
    // delete action
    delete MonkeyCore::menuBar()->action( "mTools/aRegExpEditor" );
    return true;
}

void RegExpEditor::action_triggered()
{
    if ( !mEditor )
        mEditor = new UIRegExpEditor( MonkeyCore::mainWindow() );
    mEditor->setVisible( !mEditor->isVisible() );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( BaseRegExpEditor, RegExpEditor )
#endif
