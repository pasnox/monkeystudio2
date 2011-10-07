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
#ifndef UIQMAKEEDITOR_H
#define UIQMAKEEDITOR_H

#include <xupmanager/gui/UIXUPEditor.h>

#include <QHash>

class XUPItem;

class UIQMakeEditor : public UIXUPEditor
{
    Q_OBJECT
    
public:
    typedef QHash<QString, QStringList> ProjectValues;
    UIQMakeEditor( QWidget* parent = 0 );
    virtual ~UIQMakeEditor();
    
    virtual void setupProject( XUPProjectItem* project );
    
    static QStringList handledVariables();

protected:
    UIQMakeEditor::ProjectValues mPositiveValues;
    UIQMakeEditor::ProjectValues mNegativeValues;
    
    virtual void finalize();
    
    void initializeVariables( XUPProjectItem* project );
    XUPItem* uniqueVariable( XUPItem* scope, const QString& variableName, bool positive, bool create );
    void updateVariable( XUPItem* scope, const QString& variableName, bool positive, const QStringList& values );
};

#endif // UIQMAKEEDITOR_H
