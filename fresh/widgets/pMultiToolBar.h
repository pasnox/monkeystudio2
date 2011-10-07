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
#ifndef PMULTITOOLBAR_H
#define PMULTITOOLBAR_H

#include "MonkeyExport.h"

#include <QStackedWidget>
#include <QMap>

class QToolBar;

class Q_MONKEY_EXPORT pMultiToolBar : public QStackedWidget
{
    Q_OBJECT
    
public:
    pMultiToolBar( QWidget* parent = 0 );
    virtual ~pMultiToolBar();
    
    QString currentContext() const;
    void setCurrentContext( const QString& context );
    
    void removeContext( const QString& context, bool del = false );
    QStringList contexts() const;
    QToolBar* toolBar( const QString& context ) const;
    QToolBar* currentToolBar() const;

protected:
    mutable QMap<int, QToolBar*> mToolBars;
    
    void updateMaps();

protected slots:
    void _q_currentChanged( int index );
    void _q_widgetRemoved( int index );

signals:
    void currentContextChanged( const QString& context );
    void contextRemoved( const QString& context );
    void notifyChanges();
};

#endif // PMULTITOOLBAR_H
