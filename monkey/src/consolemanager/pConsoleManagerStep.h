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
#ifndef PCONSOLEMANAGERSTEP_H
#define PCONSOLEMANAGERSTEP_H

#include <MonkeyExport.h>

#include <QString>
#include <QPoint>
#include <QList>
#include <QVariant>

class Q_MONKEY_EXPORT pConsoleManagerStep
{
public:
    enum Type
    {
        Invalid = -1,
        Error,
        Message,
        Warning,
        Action,
        Finish,
        Unknown,
        Good,
        Bad
    };
    
    enum CustomRoles
    {
        TypeRole = Qt::UserRole,
        FileNameRole,
        PositionRole
    };
    
    typedef QMap<int, QVariant> Data;
    
    pConsoleManagerStep( pConsoleManagerStep::Data data = pConsoleManagerStep::Data() );
    
    bool operator==( const pConsoleManagerStep& other ) const;
    
    pConsoleManagerStep::Type type() const;
    
    QVariant roleValue( int role ) const;
    void setRoleValue( int role, const QVariant& value );

protected:
    Data mData;
    
    static QVariant defaultRoleValue( int role, pConsoleManagerStep::Type type );
};

typedef QList<pConsoleManagerStep> pConsoleManagerStepList;

Q_DECLARE_METATYPE( pConsoleManagerStep );
Q_DECLARE_METATYPE( pConsoleManagerStepList );

#endif // PCONSOLEMANAGERSTEP_H
