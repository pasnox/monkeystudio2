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
#ifndef QMAKE_H
#define QMAKE_H

#include <pluginsmanager/CLIToolPlugin.h>
#include <pluginsmanager/XUPPlugin.h>
#include <xupmanager/core/ProjectTypesIndex.h>

#include <QPointer>

class QtVersionManager;

class QMake : virtual public BasePlugin, public CLIToolPlugin, public XUPPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin CLIToolPlugin XUPPlugin )

public:
    // BasePlugin
    virtual QWidget* settingsWidget() const;
    // CLIToolPlugin
    virtual QStringList availableParsers() const;
    virtual pCommand defaultCommand() const;
    
    static QtVersionManager* versionManager();

protected:
    DocumentFilterMap mFilters;
    static QPointer<QtVersionManager> mQtVersionManager;
    // BasePlugin
    virtual void fillPluginInfos();
    virtual bool install();
    virtual bool uninstall();
};

#endif // QMAKE_H
