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
#ifndef UIPROJECTHEADERS_H
#define UIPROJECTHEADERS_H

#include "ui_UIProjectHeaders.h"

class ProjectHeaders;

class UIProjectHeaders : public QDialog, public Ui::UIProjectHeaders
{
    Q_OBJECT
    
public:
    UIProjectHeaders( QWidget* parent, ProjectHeaders* plugin );

protected:
    ProjectHeaders* mPlugin;
    
    void setTemplatesHeaderRegExp( const QString& language, const QString& regexp );
    QString templatesHeaderRegExp( const QString& language ) const;
    QString defaultTemplatesHeaderRegExp( const QString& language ) const;

    void setTemplatesHeader( const QString& language, const QString& license );
    QString templatesHeader( const QString& language ) const;
    QString defaultTemplatesHeader( const QString& language ) const;

protected slots:
    void restoreSettings();
    void saveSettings();
    void on_tbDirectory_clicked();
    void on_cbLanguages_highlighted( int );
    void on_cbLanguages_currentIndexChanged( int );
    virtual void reject();
    virtual void accept();

};

#endif // UIPROJECTHEADERS_H
