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
#ifndef UIPLUGINSSETTINGSELEMENT_H
#define UIPLUGINSSETTINGSELEMENT_H

#include <MonkeyExport.h>

#include "ui_UIPluginsSettingsElement.h"

class BasePlugin;

class Q_MONKEY_EXPORT UIPluginsSettingsElement : public QWidget, public Ui::UIPluginsSettingsElement
{
    Q_OBJECT
    
public:
    UIPluginsSettingsElement( BasePlugin* plugin, QWidget* parent = 0 );
    
    BasePlugin* plugin() const;

protected:
    BasePlugin* mPlugin;

protected slots:
    void on_cbEnabled_toggled( bool checked );
    void on_pbSettings_clicked();
    void on_pbAbout_clicked();
    void on_cbNeverEnable_toggled( bool checked );
};

#endif // UIPLUGINSSETTINGSELEMENT_H
