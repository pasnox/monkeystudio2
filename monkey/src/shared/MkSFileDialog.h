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
#ifndef MKSFILEDIALOG_H
#define MKSFILEDIALOG_H

#include "pFileDialog.h"

class XUPItem;

class Q_MONKEY_EXPORT MkSFileDialog : public pFileDialog
{
    Q_OBJECT
    
public:
    MkSFileDialog( QWidget* parent = 0, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString(), bool textCodecEnabled = true );
    
    void setRecentNameFilter( const QString& name );
    QString recentNameFilter() const;
    
    static pFileDialogResult getOpenFileName( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, QString* selectedFilter = 0, Options options = 0 );
    static pFileDialogResult getOpenFileNames( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, QString* selectedFilter = 0, Options options = 0 );
    
    static pFileDialogResult getSaveFileName( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, QString* selectedFilter = 0, Options options = 0 );
    static pFileDialogResult getExistingDirectory( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), bool enabledTextCodec = true, Options options = ShowDirsOnly );
    
    static pFileDialogResult getNewEditorFile( QWidget* parent = 0 );
};

#endif // MKSFILEDIALOG_H
