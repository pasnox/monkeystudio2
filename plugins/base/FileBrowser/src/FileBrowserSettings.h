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
/*!
    \file FileBrowserSettings.h
    \date 2008-01-14T00:40:08
    \author Filipe AZEVEDO, Andrei KOPATS
    \brief Settings widget of FileBrowser plugin
*/
#ifndef FILEBROWSERSETTINGS_H
#define FILEBROWSERSETTINGS_H

#include <QWidget>

class FileBrowser;
class pStringListEditor;

/*!
    Settigs widget of FileBrowser plugin
    
    Allows to edit filters for filtering out unneeded files from filesystem 
    view
*/
class FileBrowserSettings : public QWidget
{
    Q_OBJECT
    
public:
    FileBrowserSettings( FileBrowser* plugin, QWidget* parent = 0 );

protected:
    FileBrowser* mPlugin;
    pStringListEditor* mEditor;

protected slots:
    void applySettings();
};

#endif // FILEBROWSERSETTINGS_H
