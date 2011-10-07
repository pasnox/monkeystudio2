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
#ifndef PFILEDIALOG_H
#define PFILEDIALOG_H

#include "MonkeyExport.h"

#include <QFileDialog>
#include <QMap>
#include <QVariant>

class QGridLayout;
class QLabel;
class QComboBox;
class QCheckBox;

typedef QMap<QString, QVariant> pFileDialogResult;

class Q_MONKEY_EXPORT pFileDialog : public QFileDialog
{
    Q_OBJECT
    
public:
    pFileDialog( QWidget* parent = 0, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString(), bool textCodecEnabled = true, bool openReadOnlyEnabled = false );
    
    QString textCodec() const;
    void setTextCodec( const QString& codec );
    
    bool textCodecEnabled() const;
    void setTextCodecEnabled( bool enabled );
    
    bool openReadOnly() const;
    void setOpenReadOnly( bool readOnly );
    
    bool openReadOnlyEnabled() const;
    void setOpenReadOnlyEnabled( bool enabled );
    
    static pFileDialogResult getOpenFileName( QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, bool enabledOpenReadOnly = true, QString* selectedFilter = 0, Options options = 0 );
    static pFileDialogResult getOpenFileNames( QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, bool enabledOpenReadOnly = true, QString* selectedFilter = 0, Options options = 0 );
    static pFileDialogResult getSaveFileName( QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, QString* selectedFilter = 0, Options options = 0 );

protected:
    QGridLayout* glDialog;
    bool mTextCodecEnabled;
    QLabel* lCodec;
    QComboBox* cbCodec;
    bool mOpenReadOnlyEnabled;
    QCheckBox* cbOpenReadOnly;
    
    QDir::Filters filterForMode() const;
    static void setDialog( pFileDialog* dlg, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, QString* selectedFilter, Options options );
    static void setOpenFileNameDialog( pFileDialog* dlg, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, QString* selectedFilter, Options options );
    static void setOpenFileNamesDialog( pFileDialog* dlg, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, QString* selectedFilter, Options options );
    static void setSaveFileNameDialog( pFileDialog* dlg, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, QString* selectedFilter, Options options );
};

#endif // PFILEDIALOG_H
