/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIMonkeyProjectSettings.cpp
** Date      : 2007-11-04T22:53:32
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
    targets.append ( (MonkeyProjectItem::Target){"","",NULL});
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
