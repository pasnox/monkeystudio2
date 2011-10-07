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
#include "pGroupPath.h"

pGroupPath::pGroupPath()
{
    mNum = -1;
    mMaxNum = -1;
}

pGroupPath::pGroupPath( const QString& name )
{
    mStr = name;
    mNum = -1;
    mMaxNum = -1;
}

pGroupPath::pGroupPath( const QString& name, bool guessArraySize )
{
    mStr = name;
    mNum = 0;
    mMaxNum = guessArraySize ? 0 : -1;
}

pGroupPath::pGroupPath( const pGroupPath& other )
{
    mStr = other.mStr;
    mNum = other.mNum;
    mMaxNum = other.mMaxNum;
}

pGroupPath::~pGroupPath()
{
}

pGroupPath& pGroupPath::operator=( const pGroupPath& other )
{
    if( *this != other )
    {
        mStr = other.mStr;
        mNum = other.mNum;
        mMaxNum = other.mMaxNum;
    }

    return *this;
}

bool pGroupPath::operator==( const pGroupPath& other ) const
{
    return mStr == other.mStr && mNum == other.mNum && mMaxNum == other.mMaxNum;
}

bool pGroupPath::operator!=( const pGroupPath& other ) const
{
    return !operator==( other );
}

QString pGroupPath::name() const
{
    return mStr;
}

QString pGroupPath::toString() const
{
    QString result;
    result = mStr;
    
    if ( mNum > 0 )
    {
        result += QLatin1Char( '/' );
        result += QString::number( mNum );
    }
    
    return result;
}

bool pGroupPath::isArray() const
{
    return mNum != -1;
}

int pGroupPath::arraySizeGuess() const
{
    return mMaxNum;
}

void pGroupPath::setArrayIndex( int i )
{
    mNum = i +1;
    
    if ( mMaxNum != -1 && mNum > mMaxNum )
    {
        mMaxNum = mNum;
    }
}
