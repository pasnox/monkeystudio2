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
#include "QMakeVariablesEditor.h"
#include "UIQMakeEditor.h"

#include <xupmanager/core/XUPProjectItem.h>

QMakeVariablesEditor::QMakeVariablesEditor( QWidget* parent )
    : VariablesEditor( parent )
{
    /*setQuoteSpacedValuesChecked( false );
    setQuoteSpacedValuesVisible( false );*/
}

QMakeVariablesEditor::~QMakeVariablesEditor()
{
}


XUPItemVariableEditorModel::FilterMode QMakeVariablesEditor::filterMode() const
{
    return XUPItemVariableEditorModel::Out;
}

bool QMakeVariablesEditor::isFriendlyDisplayText() const
{
    return false;
}

QStringList QMakeVariablesEditor::filteredVariables() const
{
    QStringList variables = UIQMakeEditor::handledVariables();
    
    if ( mProject ) {
        variables << mProject->documentFilters().fileVariables();
    }
    
    return variables;
}
