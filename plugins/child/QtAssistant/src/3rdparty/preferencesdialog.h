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
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui/QDialog>
#include "ui_preferencesdialog.h"

QT_BEGIN_NAMESPACE

class FontPanel;
class QHelpEngineCore;

enum {
    ShowHomePage = 0,
    ShowBlankPage = 1,
    ShowLastPages = 2
};

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QHelpEngineCore *helpEngine, QWidget *parent = 0);
    ~PreferencesDialog();

    void showDialog();

private slots:
    void updateAttributes(QListWidgetItem *item);
    void updateFilterMap();
    void addFilter();
    void removeFilter();
    void addDocumentationLocal();
    void removeDocumentation();
    void applyChanges();
    void appFontSettingToggled(bool on);
    void appFontSettingChanged(int index);
    void browserFontSettingToggled(bool on);
    void browserFontSettingChanged(int index);
    
    void setBlankPage();
    void setCurrentPage();
    void setDefaultPage();

signals:
    void updateBrowserFont();
    void updateApplicationFont();

private:
    void updateFilterPage();
    void updateFontSettingsPage();
    void updateOptionsPage();

    Ui::PreferencesDialogClass m_ui;
    QHelpEngineCore *m_helpEngine;
    bool m_hideFiltersTab;
    bool m_hideDocsTab;
    QMap<QString, QStringList> m_filterMapBackup;
    QMap<QString, QStringList> m_filterMap;
    QStringList m_removedFilters;
    QStringList m_docsBackup;
    QStringList m_regDocs;
    QStringList m_unregDocs;
    QList<int> m_TabsToClose;
    FontPanel *m_appFontPanel;
    FontPanel *m_browserFontPanel;
    bool m_appFontChanged;
    bool m_browserFontChanged;
};

QT_END_NAMESPACE

#endif // SETTINGSDIALOG_H
