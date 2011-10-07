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
#ifndef PVERSION_H
#define PVERSION_H

#include <MonkeyExport.h>

#include <QString>

class Q_MONKEY_EXPORT pVersion
{
public:
    pVersion( const QString& version );
    pVersion( const char* version );
    
    const QString& toString() const;

    bool operator==( const pVersion& other ) const;
    bool operator!=( const pVersion& other ) const;
    bool operator<( const pVersion& other ) const;
    bool operator>( const pVersion& other ) const;
    bool operator<=( const pVersion& other ) const;
    bool operator>=( const pVersion& other ) const;

protected:
    QString mVersion;
    int major;
    int minor;
    int patch;
    int build;
    QString extra;
    
    void init();
};

#endif // PVERSION_H
