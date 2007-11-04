/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : MonkeyProjectItem.cpp
** Date      : 2007-11-04T22:53:35
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
        qWarning ("<%s>:%i Trying to open something for not a project item",__func__,__LINE__);
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
        targets.append ( (Target){text, command,NULL});
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
                pConsoleManager::instance()->addCommand(cmd);
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
    QMenu* menu = pMenuBar::instance()->menu( "mBuilder" );
    menu->setEnabled (true);    
    for (int i = 0; i < targets.size(); i++)
    {
            targets[i].action = (pAction*)pMenuBar::instance()->action(QString("mBuilder/aAction%1").arg(i),targets[i].text);
            targets[i].action->setEnabled ( !targets[i].command.isEmpty() );
            connect ( targets[i].action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
    }
}

ProjectPlugin* MonkeyProjectItem::getParentPlugin ()
{
    return PluginsManager::instance()->plugin<ProjectPlugin*>( PluginsManager::stAll, "MonkeyProject" );
}

void MonkeyProjectItem::addExistingFiles(const QStringList& files, const QString& scope, const QString&)
{
    if ( !isProject() )
        return;
    addExistingFiles (files, this);
}

void MonkeyProjectItem::addExistingFiles(const QStringList& files, ProjectItem* item, const QString&)
{
    if ( !isProject() )
        return;
    foreach (QString file, files)
    {
        addExistingFile (file,this);
    }
}

void MonkeyProjectItem::addExistingFile(const QString& file, const QString& scope, const QString&)
{
    addExistingFile (file, this);
}

void MonkeyProjectItem::addExistingFile(const QString& file, ProjectItem* item, const QString&)
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
