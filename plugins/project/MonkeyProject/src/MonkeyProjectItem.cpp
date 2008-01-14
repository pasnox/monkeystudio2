/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : MonkeyProjectItem.cpp
** Date      : 2008-01-14T00:54:06
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
**
****************************************************************************/
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>

#include "MonkeyProjectItem.h"
#include "ProjectPlugin.h"
#include "ProjectsModel.h"
#include "UIMonkeyProjectSettings.h"
#include "pCommand.h"
#include "pConsoleManager.h"
#include "PluginsManager.h"
#include "pMenuBar.h"
#include "pAction.h"
#include "MonkeyCore.h"

MonkeyProjectItem::MonkeyProjectItem (  ProjectItem::NodeType t, ProjectItem* i )
{
    init();
    setType( t );
    mBuilder = new GenericBuilder;
    if ( i )
        i->appendRow( this );
}

MonkeyProjectItem::~MonkeyProjectItem ()
{
    delete mBuilder;
    uninstallCommands ();
}

void MonkeyProjectItem::editSettings()
{
    new UIMonkeyProjectSettings (this);
}

void MonkeyProjectItem::close()
{
    // only project item can call this
    if ( !isProject() )
        return;
    
    // close subprojects first
    while ( ProjectItem* p = childrenProjects( false ).value( 0 ) )
        p->close();
    
    // tell we will close the project
    emit aboutToClose();
    
    // ask to save project if needed
    save( true );
    
    // tell project is closed
    emit closed();
    
    // remove it from model
    remove();
}

void MonkeyProjectItem::remove()
{
    model()->removeRow( row(), index().parent() );
}

void MonkeyProjectItem::save (bool)
{
    QSettings settings (getValue(), QSettings::IniFormat);
    settings.setValue ("projectName", text());
    settings.setValue ("targetsCount", targets.size());
    QStringList files;
    for (int i = 0; i < rowCount(); i++)
        files.append (child(i)->getFilePath());
    settings.setValue ("files", files);
    for ( int i = 0; i < targets.size(); i++)
    {
        settings.setValue (QString ("target%1text").arg(i), targets[i].text);
        settings.setValue (QString ("target%1command").arg(i), targets[i].command);
    }
    setModified (false);
}

bool MonkeyProjectItem::open()
{
    if (!isProject())
    {
        qWarning ("Trying to open something for not a project item");
        return false;
    }
    QSettings settings (getValue(), QSettings::IniFormat);
    if ( settings.status() == QSettings::AccessError)
    {
        QMessageBox::warning (QApplication::activeWindow(),tr("Error"),tr("Access denided for a file %1").arg (getFilePath()));
        return false;
    }
    else if ( settings.status() == QSettings::FormatError)
    {
        QMessageBox::warning (QApplication::activeWindow(),tr("Error"),tr("Wrong file %1").arg (getFilePath()));
        return false;
    }

    setText( settings.value ("projectName", "Project").toString());
    QStringList files = settings.value ("files").toStringList();
    qDebug () <<files;
    addExistingFiles(files,"");
    int targetsCount = settings.value ("targetsCount").toInt();
    QString text, command;
    for ( int i = 0; i < targetsCount; i++)
    {
        text = settings.value (QString ("target%1text").arg(i),"").toString();
        command = settings.value (QString ("target%1command").arg(i),"").toString();
        targets.append ( Target( text, command,NULL ) );
    }
    
    setModified (false);
    return true;
}

void MonkeyProjectItem::buildMenuTriggered ()
{
    foreach ( Target t, targets)
        if ( t.action == sender())
        {
                pCommand cmd;
                cmd.setText (t.action->text());
                cmd.setCommand (t.command);
                cmd.setWorkingDirectory (QFileInfo(getValue()).absolutePath());
                cmd.setSkipOnError (false);
                cmd.setTryAllParsers (true);
                MonkeyCore::consoleManager()->addCommand(cmd);
                return;
        }
};

void MonkeyProjectItem::uninstallCommands ()
{
    foreach ( Target t, targets)
        if (t.action)
            delete t.action;
}

void MonkeyProjectItem::installCommands ()
{
    QMenu* menu = MonkeyCore::menuBar()->menu( "mBuilder" );
    menu->setEnabled (true);    
    for (int i = 0; i < targets.size(); i++)
    {
            targets[i].action = (pAction*)MonkeyCore::menuBar()->action(QString("mBuilder/aAction%1").arg(i),targets[i].text);
            targets[i].action->setEnabled ( !targets[i].command.isEmpty() );
            connect ( targets[i].action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
    }
}

ProjectPlugin* MonkeyProjectItem::getParentPlugin ()
{
    return MonkeyCore::pluginsManager()->plugin<ProjectPlugin*>( PluginsManager::stAll, "MonkeyProject" );
}

void MonkeyProjectItem::addExistingFiles(const QStringList& files, const QString& /*scope*/, const QString&)
{
    if ( !isProject() )
        return;
    addExistingFiles (files, this);
}

void MonkeyProjectItem::addExistingFiles(const QStringList& files, ProjectItem* /*item*/, const QString&)
{
    if ( !isProject() )
        return;
    foreach (QString file, files)
    {
        addExistingFile (file,this);
    }
}

void MonkeyProjectItem::addExistingFile(const QString& file, const QString& /*scope*/, const QString&)
{
    addExistingFile (file, this);
}

void MonkeyProjectItem::addExistingFile(const QString& file, ProjectItem* /*item*/, const QString&)
{
    ProjectItem* it = new MonkeyProjectItem( ProjectItem::ValueType, this );
    it->setValue (file);
    it->setFilePath (file);
    it->setText (QFileInfo(file).fileName());
}

void MonkeyProjectItem::appendRow(ProjectItem* i)
{ insertRow( rowCount(), i ); }

void MonkeyProjectItem::insertRow(int i, ProjectItem* it)
{
    QStandardItem::insertRow(i, it);
    // set project modified
    project()->setModified( true );
}
