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
#include "MkSDesignerIntegration.h"

#include <QWidget>

MkSDesignerIntegration::MkSDesignerIntegration( QDesignerFormEditorInterface* core, QObject* parent )
    : qdesigner_internal::QDesignerIntegration( core, parent )
{
}

/*
    This fix the bug that resize the MkS mainwindow
    when editing geometry of toplevel form in the property editor.
*/
QWidget* MkSDesignerIntegration::containerWindow( QWidget* widget ) const
{
    // Find the parent window to apply a geometry to.
    while ( widget )
    {
        if ( widget->isWindow() )
        {
            break;
        }
        
        if ( !qstrcmp( widget->metaObject()->className(), "SharedTools::Internal::FormResizer" ) )
        {
            break;
        }
        
        widget = widget->parentWidget();
    }

    return widget;
}
