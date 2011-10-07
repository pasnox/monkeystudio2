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
#ifndef EXMDIAREA_H
#define EXMDIAREA_H

#include <QMdiArea>

class ExMdiArea : public QMdiArea
{
    Q_OBJECT

public:
    enum DocumentViewMode {
        Undefined = -1, // do not use
        MDI,
        NoTabs,
        TopTabs,
        BottomTabs,
        LeftTabs,
        RightTabs
    };
    
    ExMdiArea( QWidget* parent = 0 );
    
    ExMdiArea::DocumentViewMode documentViewMode() const;
    void setDocumentViewMode( ExMdiArea::DocumentViewMode mode );

protected:
    ExMdiArea::DocumentViewMode mMode;
    
    Qt::WindowFlags documentViewModeWindowFlags( ExMdiArea::DocumentViewMode mode ) const;
    
protected slots:
    void updateWindowFlags( QMdiSubWindow* window );
    void updateWindowsFlags();
};

#endif // EXMDIAREA_H
