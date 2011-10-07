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
#ifndef FILESEDITOR_H
#define FILESEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include <QModelIndex>

class Ui_FilesEditor;

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT FilesEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    FilesEditor( QWidget* parent = 0 );
    virtual ~FilesEditor();
    
    bool isQuoteSpacedValuesVisible() const;
    bool isQuoteSpacedValuesChecked() const;
    bool isDeleteRemovedFilesVisible() const;
    bool isDeleteRemovedFilesChecked() const;
    
    virtual QStringList filteredFileVariables() const;
    
    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

public slots:
    void setQuoteSpacedValuesVisible( bool visible );
    void setQuoteSpacedValuesChecked( bool checked );
    void setDeleteRemovedFilesVisible( bool visible );
    void setDeleteRemovedFilesChecked( bool checked );

protected:
    Ui_FilesEditor* ui;
    XUPProjectItem* mProject;
    XUPItemVariableEditorModel* mModel;
    
    QModelIndex currentVariable() const;
    QModelIndex currentValue() const;

protected slots:
    void tvVariables_selectionModel_selectionChanged();
    void on_tbAdd_clicked();
    void on_tbEdit_clicked();
};

#endif // FILESEDITOR_H
