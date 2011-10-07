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
#include "pVersion.h"

#include <QStringList>

pVersion::pVersion( const QString& version )
{
    mVersion = version.trimmed();
    init();
}

pVersion::pVersion( const char* version )
{
    mVersion = QString::fromAscii( version ).trimmed();
    init();
}

void pVersion::init()
{
    const QStringList parts = mVersion.split( "." );
    const int index = parts.value( 3 ).indexOf( QRegExp( "[A-Z|a-z]" ) );

    major = parts.value( 0 ).toInt();
    minor = parts.value( 1 ).toInt();
    patch = parts.value( 2 ).toInt();

    if ( index != -1 ) {
        build = parts.value( 3 ).mid( 0, index ).toInt();
        extra = parts.value( 3 ).mid( index );
    }
    else {
        build = parts.value( 3 ).toInt();
    }
}

const QString& pVersion::toString() const
{
    return mVersion;
}

bool pVersion::operator==( const pVersion& other ) const
{
    return major == other.major
        && minor == other.minor
        && patch == other.patch
        && build == other.build
        && extra == other.extra;
}

bool pVersion::operator!=( const pVersion& other ) const
{
    return !operator==( other );
}

bool pVersion::operator<( const pVersion& other ) const
{
    if ( *this == other ) {
        return false;
    }

    if ( major != other.major ) {
        return major < other.major;
    }

    if ( minor != other.minor ) {
        return minor < other.minor;
    }

    if ( patch != other.patch ) {
        return patch < other.patch;
    }

    if ( build != other.build ) {
        return build < other.build;
    }

    if ( extra.isEmpty() && !other.extra.isEmpty() ) {
        return false;
    }
    
    if ( !extra.isEmpty() && other.extra.isEmpty() ) {
        return true;
    }
    
    return extra < other.extra; // not the best but afaik ;)
}

bool pVersion::operator>( const pVersion& other ) const
{
    if ( *this == other ) {
        return false;
    }

    if ( major != other.major ) {
        return major > other.major;
    }

    if ( minor != other.minor ) {
        return minor > other.minor;
    }

    if ( patch != other.patch ) {
        return patch > other.patch;
    }

    if ( build != other.build ) {
        return build > other.build;
    }
    
    if ( extra.isEmpty() && !other.extra.isEmpty() ) {
        return true;
    }
    
    if ( !extra.isEmpty() && other.extra.isEmpty() ) {
        return false;
    }

    return extra > other.extra; // not the best but afaik ;)
}

bool pVersion::operator<=( const pVersion& other ) const
{
    return operator<( other ) || operator==( other );
}

bool pVersion::operator>=( const pVersion& other ) const
{
    return operator>( other ) || operator==( other );
}
