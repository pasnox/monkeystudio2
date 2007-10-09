/****************************************************************************
**
**      Created using kate
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>
** Project   : MonkeyProject project type 
** FileName  : MonkeyProjectItem.cpp
** Date      : 2007-09-28
** License   : GPL
** Comment   : MonkeyProject project class
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QMessageBox>
#include <QDebug>

#include "MonkeyProjectItem.h"
#include "ProjectPlugin.h"
#include "ProjectsModel.h"
#include "UIMonkeyProjectSettings.h"
#include "pCommand.h"
#include "pConsoleManager.h"
#include "PluginsManager.h"
#include "pMenuBar.h"

MonkeyProjectItem::MonkeyProjectItem (  ProjectItem::NodeType t, ProjectItem* i )
{
	init();
	setType( t );
	if ( i )
		i->appendRow( this );
}

MonkeyProjectItem::~MonkeyProjectItem ()
{
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
	settings.setValue ("projectPath", projectPath);
	settings.setValue ("targetsCount", targets.size());
	for ( int i = 0; i < targets.size(); i++)
	{
		settings.setValue (QString ("target%1text").arg(i), targets[i].text);
		settings.setValue (QString ("target%1command").arg(i), targets[i].command);
	}
	settings.sync();
	setModified (false);
}

void MonkeyProjectItem::buildMenuTriggered ()
{
	foreach ( Target t, targets)
		if ( t.action == sender())
		{
				pCommand cmd;
				cmd.setText (t.action->text());
				cmd.setCommand (t.command);
				cmd.setWorkingDirectory (projectPath);
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
        {
			t.action->setVisible (false);
            t.action = NULL;
        }
}

void MonkeyProjectItem::installCommands ()
{
	QMenu* menu = pMenuBar::instance()->menu( "mBuild" );
	menu->setEnabled (true);	
	for (int i = 0; i < targets.size(); i++)
	{
		if (targets[i].action)
			targets[i].action->setVisible (true);
		else
        {	targets[i].action = pMenuBar::instance()->action(QString("mBuild/aAction%1").arg(i),targets[i].text);  // !!! I can not see    way to delete action from menu, so, deleting it and creating again. hlamer
			connect ( targets[i].action, SIGNAL (triggered()), this, SLOT (buildMenuTriggered()));
		}
	targets[i].action->setEnabled ( !targets[i].command.isEmpty() );
	targets[i].action->setText (targets[i].text);
	}
}

ProjectPlugin* MonkeyProjectItem::getParentPlugin ()
{
	return PluginsManager::instance()->plugin<ProjectPlugin*>( "MonkeyProject" );
}

bool MonkeyProjectItem::open()
{
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
	projectPath = settings.value ("projectPath", canonicalPath()).toString();

	int targetsCount = settings.value ("targetsCount").toInt();
	QString text, command;
	for ( int i = 0; i < targetsCount; i++)
	{
		text = settings.value (QString ("target%1text").arg(i),"").toString();
		command = settings.value (QString ("target%1command").arg(i),"").toString();
		targets.append ( (Target){text, command,NULL});
	}

	return true;
}
