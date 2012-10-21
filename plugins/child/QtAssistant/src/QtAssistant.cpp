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
#include "QtAssistant.h"
#include "QtAssistantDock.h"
#include "QtAssistantChild.h"
#include "MkSQtDocInstaller.h"
#include "3rdparty/preferencesdialog.h"

#include <maininterface/UIMain.h>
#include <workspace/pWorkspace.h>
#include <pDockToolBar.h>

#include <QHelpEngine>

QWidget* QtAssistant::settingsWidget() const
{
    MkSQtDocInstaller::collectionFileDirectory( true );
    QHelpEngine* engine = new QHelpEngine( MkSQtDocInstaller::defaultHelpCollectionFileName() );
    
    QWidget* widget = new PreferencesDialog( engine, QApplication::activeWindow() );
    engine->setParent( widget );
    
    return widget;
}

pAbstractChild* QtAssistant::createDocument( const QString& fileName )
{
    Q_UNUSED( fileName );
    return 0;
}

void QtAssistant::fillPluginInfos()
{
    mPluginInfos.Caption = tr( "Qt Assistant" );
    mPluginInfos.Description = tr( "Qt Assistant Integration" );
    mPluginInfos.Author = "Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>";
    mPluginInfos.Type = BasePlugin::iBase | BasePlugin::iChild;
    mPluginInfos.Name = PLUGIN_NAME;
    mPluginInfos.Version = "0.5.0";
    mPluginInfos.FirstStartEnabled = true;
    mPluginInfos.HaveSettingsWidget = true;
    mPluginInfos.Pixmap = pIconManager::pixmap( "QtAssistant.png", ":/assistant-icons" );
}

bool QtAssistant::install()
{
    mDock = new QtAssistantDock;
    connect( mDock, SIGNAL( helpShown() ), this, SLOT( helpShown() ) );
    MonkeyCore::mainWindow()->dockToolBar( Qt::RightToolBarArea )->addDock( mDock, infos().Caption, pIconManager::icon( "QtAssistant.png", ":/assistant-icons" ) );
    return true;
}

bool QtAssistant::uninstall()
{
    delete mDock;
    return true;
}

void QtAssistant::helpShown()
{
    QtAssistantChild* child = mDock->child();
    pWorkspace* workspace = MonkeyCore::workspace();
    
    if ( !workspace->documents().contains( child ) )
    {
        workspace->handleDocument( child );
        emit child->fileOpened();
        child->showMaximized();
    }
    
    workspace->setCurrentDocument( child );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( ChildQtAssistant, QtAssistant )
#endif
