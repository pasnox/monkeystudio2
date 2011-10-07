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
#ifndef COMMANDSEDITOR_H
#define COMMANDSEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include "xupmanager/core/XUPProjectItemHelper.h"

#include <QModelIndex>
#include <QItemSelection>

class Ui_CommandsEditor;
class CommandsEditorModel;

class Q_MONKEY_EXPORT CommandsEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    CommandsEditor( QWidget* parent = 0 );
    virtual ~CommandsEditor();
    
    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

protected:
    Ui_CommandsEditor* ui;
    CommandsEditorModel* mModel;
    XUPProjectItem* mProject;
    
    void setCommand( const QModelIndex& commandIndex );
    void getCommand( const QModelIndex& commandIndex );
    void updateState();

protected slots:
    void tvCommands_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
    void on_tbCommandAdd_clicked();
    void on_tbCommandUp_clicked();
    void on_tbCommandDown_clicked();
};

#endif // COMMANDSEDITOR_H
