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
#ifndef VARIABLESEDITOR_H
#define VARIABLESEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"
#include "xupmanager/core/XUPItemVariableEditorModel.h"

#include <QModelIndex>

class Ui_VariablesEditor;

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT VariablesEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    VariablesEditor( QWidget* parent = 0 );
    virtual ~VariablesEditor();
    
    bool isQuoteSpacedValuesVisible() const;
    bool isQuoteSpacedValuesChecked() const;
    
    virtual XUPItemVariableEditorModel::FilterMode filterMode() const;
    virtual bool isFriendlyDisplayText() const;
    virtual QStringList filteredVariables() const;
    
    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

public slots:
    void setQuoteSpacedValuesVisible( bool visible );
    void setQuoteSpacedValuesChecked( bool checked );

protected:
    Ui_VariablesEditor* ui;
    QAction* aValuesAddValue;
    QAction* aValuesAddFile;
    QAction* aValuesAddPath;
    QAction* aValuesEditValue;
    QAction* aValuesEditFile;
    QAction* aValuesEditPath;
    XUPProjectItem* mProject;
    XUPItemVariableEditorModel* mModel;
    
    QModelIndex currentVariable() const;
    QModelIndex currentValue() const;

protected slots:
    void lvVariables_selectionModel_selectionChanged();
    void on_tbVariablesAdd_clicked();
    void on_tbVariablesEdit_clicked();
    
    void lvValues_selectionModel_selectionChanged();
    void on_tbValuesAdd_clicked();
    void on_tbValuesAdd_triggered( QAction* action );
    void on_tbValuesEdit_clicked();
    void on_tbValuesEdit_triggered( QAction* action );
    void on_tbValuesClear_clicked();
};

#endif // VARIABLESEDITOR_H
