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
#ifndef QMAKECONFIGURATIONEDITOR_H
#define QMAKECONFIGURATIONEDITOR_H

#include <xupmanager/gui/XUPPageEditor.h>
#include "UIQMakeEditor.h"

class Ui_QMakeConfigurationEditor;
class pGenericTableModel;

class QModelIndex;

class QMakeConfigurationEditor : public XUPPageEditor
{
    Q_OBJECT

public:
    QMakeConfigurationEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent = 0 );
    virtual ~QMakeConfigurationEditor();

    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

protected:
    UIQMakeEditor::ProjectValues& mPositiveValues;
    UIQMakeEditor::ProjectValues& mNegativeValues;
    Ui_QMakeConfigurationEditor* ui;
    XUPProjectItem* mProject;
    pGenericTableModel* mPositiveQtModulesModel;
    pGenericTableModel* mPositiveConfigurationModel;
    pGenericTableModel* mNegativeQtModulesModel;
    pGenericTableModel* mNegativeConfigurationModel;

protected slots:
    void showIndexHelp( const QModelIndex& index );
};

#endif // QMAKECONFIGURATIONEDITOR_H
