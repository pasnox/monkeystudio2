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
#ifndef UIXUPEDITOR_H
#define UIXUPEDITOR_H

#include "MonkeyExport.h"

#include <QDialog>

class Ui_UIXUPEditor;
class XUPPageEditor;
class XUPProjectItem;

class Q_MONKEY_EXPORT UIXUPEditor : public QDialog
{
    Q_OBJECT
    
public:
    UIXUPEditor( QWidget* parent = 0 );
    virtual ~UIXUPEditor();
    
    int insertPage( int index, XUPPageEditor* page );
    int addPage( XUPPageEditor* page );
    void addPages( const QList<XUPPageEditor*>& pages );
    XUPPageEditor* page( int index ) const;
    void removePage( int index );
    
    QList<XUPPageEditor*> pages() const;
    void setCurrentPage( int index );
    int currentPage() const;
    
    virtual void setupProject( XUPProjectItem* project );
    virtual bool showProjectFilesPage();

protected:
    Ui_UIXUPEditor* ui;
    XUPProjectItem* mProject;
    QList<XUPPageEditor*> mPages;
    
    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

protected slots:
    void on_lwPages_currentRowChanged( int row );
    void accept();
};

#endif // UIXUPEDITOR_H
