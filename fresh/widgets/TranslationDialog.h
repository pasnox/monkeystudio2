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
#ifndef TRANSLATIONDIALOG_H
#define TRANSLATIONDIALOG_H

#include <QDialog>
#include <QHash>

class TranslationManager;
class QTreeWidgetItem;

namespace Ui {
    class TranslationDialog;
};

class TranslationDialog : public QDialog
{
    Q_OBJECT

public:
    TranslationDialog( TranslationManager* translationManager, QWidget* parent = 0 );
    virtual ~TranslationDialog();
    
    QString selectedLocale() const;
    
    static QString getLocale( TranslationManager* translationManager, QWidget* parent = 0 );

protected:
    Ui::TranslationDialog* ui;
    TranslationManager* mTranslationManager;
    QHash<QString, QTreeWidgetItem*> mRootItems;
    
    QTreeWidgetItem* newItem( const QLocale& locale );
    QTreeWidgetItem* rootItem( const QLocale& locale );

protected slots:
    void on_tbLocate_clicked();
    void on_tbReload_clicked();
};

#endif // TRANSLATIONDIALOG_H
