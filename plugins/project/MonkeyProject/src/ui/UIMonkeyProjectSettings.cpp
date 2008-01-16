/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : UIMonkeyProjectSettings.cpp
** Date      : 2008-01-14T00:54:04
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
#include <QFileDialog>

#include "UIMonkeyProjectSettings.h"
#include "pMenuBar.h"
#include <QDebug>
UIMonkeyProjectSettings::UIMonkeyProjectSettings(MonkeyProjectItem* pproject): QDialog()
{
    project = pproject;
    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    setWindowTitle( tr("Project Settings"));
    projectName->setText (project->text());
    targets = project->targets;
    foreach (MonkeyProjectItem::Target t, targets)
    {
        qDebug () <<"Adding action";
        actionsList->addItem (t.text);
    }
    connect (acceptBtn, SIGNAL (clicked()), this, SLOT (accept()));
    connect (actionsList, SIGNAL (currentRowChanged(int)), this, SLOT(selectedRowChanged(int)));
    connect (actionName, SIGNAL (textEdited(QString)), this, SLOT(actionNameEdited(QString)));
    connect (addActionBtn, SIGNAL (clicked()), this, SLOT (addAction()));
    connect (removeActionBtn, SIGNAL (clicked()), this, SLOT (removeAction()));
    connect (commandsEdit, SIGNAL (textChanged()), this, SLOT (actionCommandEdited ()));
    exec();
}

void UIMonkeyProjectSettings::accept()
{
    project->setText(projectName->text());
    project->targets = targets;
    project->uninstallCommands ();
    project->installCommands ();
    project->setModified (true);
    QDialog::accept();
}

void UIMonkeyProjectSettings::selectedRowChanged(int actionNum)
{
    actionName->setText(targets[actionNum].text);
    actionName->setEnabled (true);
    commandsEdit->setText (targets[actionNum].command);
    commandsEdit->setEnabled (true);
}

void UIMonkeyProjectSettings::actionNameEdited(QString name)
{
    if (actionsList->currentItem())
    {
        actionsList->currentItem()->setText(name);
        targets[actionsList->currentRow()].text = name;
    }
}

void UIMonkeyProjectSettings::addAction()
{
    targets.append ( MonkeyProjectItem::Target( "","",NULL ));
    actionsList->addItem ("");
    actionsList->setCurrentRow (actionsList->count()-1);
    actionName->setFocus(Qt::OtherFocusReason);
}

void UIMonkeyProjectSettings::removeAction()
{
    targets.removeAt (actionsList->currentRow());
    delete actionsList->takeItem (actionsList->currentRow());
}

void UIMonkeyProjectSettings::actionCommandEdited ()
{
    targets[actionsList->currentIndex().row()].command = commandsEdit->toPlainText();
}
