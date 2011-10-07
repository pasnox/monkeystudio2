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
#ifndef PSTYLESACTIONGROUP_H
#define PSTYLESACTIONGROUP_H

#include "MonkeyExport.h"

#include <QActionGroup>
#include <QMap>

class Q_MONKEY_EXPORT pStylesActionGroup : public QActionGroup
{
    Q_OBJECT

public:
    pStylesActionGroup( const QString& textFormat = QLatin1String( "%1" ), QObject* parent = 0 );
    
    static QStringList availableStyles();
    
    static QString systemStyle();
    QAction* systemAction() const;
    
    static QString applicationStyle();
    QAction* applicationAction() const;
    
    bool isCheckable() const;
    QString currentStyle() const;

public slots:
    void refreshActions();
    
    void setCheckable( bool checkable );
    void setCurrentStyle( const QString& style );

protected:
    bool mCheckable;
    QString mTextFormat;
    QMap<QString, QAction*> mActions;

protected slots:
    void actionTriggered( QAction* action );

signals:
    void styleSelected( const QString& style );
};

#endif // PSTYLESACTIONGROUP_H
