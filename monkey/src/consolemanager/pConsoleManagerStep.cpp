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
#include "pConsoleManagerStep.h"

#include <pIconManager.h>

pConsoleManagerStep::pConsoleManagerStep( pConsoleManagerStep::Data data )
{
    mData = data;
}

bool pConsoleManagerStep::operator==( const pConsoleManagerStep& other ) const
{
    return mData == other.mData;
}

QVariant pConsoleManagerStep::defaultRoleValue( int role, pConsoleManagerStep::Type type )
{
    switch ( role ) {
        case Qt::DecorationRole: {
            switch ( type ) {
                case pConsoleManagerStep::Invalid:
                case pConsoleManagerStep::Error:
                    return pIconManager::icon( "error.png", ":/steps/icons/steps/" );
                case pConsoleManagerStep::Message:
                    return pIconManager::icon( "message.png", ":/steps/icons/steps/" );
                case pConsoleManagerStep::Warning:
                    return pIconManager::icon( "warning.png", ":/steps/icons/steps/" );
                case pConsoleManagerStep::Action:
                    return pIconManager::icon( "action.png", ":/steps/icons/steps/" );
                case pConsoleManagerStep::Finish:
                    return pIconManager::icon( "finish.png", ":/steps/icons/steps/" );
                case pConsoleManagerStep::Unknown:
                    return pIconManager::icon( "unknown.png", ":/steps/icons/steps/" );
                case pConsoleManagerStep::Good:
                    return pIconManager::icon( "good.png", ":/steps/icons/steps/" );
                case pConsoleManagerStep::Bad:
                    return pIconManager::icon( "bad.png", ":/steps/icons/steps/" );
            }
            
            break;
        }
        case Qt::BackgroundRole: {
            switch ( type ) {
                case pConsoleManagerStep::Error:
                    return QColor( 255, 0, 0, 20 );
                case pConsoleManagerStep::Message:
                    return QColor( 0, 0, 126, 20 );
                case pConsoleManagerStep::Warning:
                    return QColor( 0, 255, 0, 20 );
                case pConsoleManagerStep::Action:
                    return QColor( 0, 0, 255, 20 );
                case pConsoleManagerStep::Finish:
                    return QColor( 65, 65, 65, 20 );
                case pConsoleManagerStep::Good:
                    return QColor( 0, 255, 0, 90 );
                case pConsoleManagerStep::Bad:
                    return QColor( 255, 0, 0, 90 );
                case pConsoleManagerStep::Unknown:
                case pConsoleManagerStep::Invalid:
                default:
                    return QColor( 125, 125, 125, 20 );
            }
            
            break;
        }
        case Qt::SizeHintRole:
            return QSize( 0, 18 );
        default:
            break;
    }
    
    return QVariant();
}

pConsoleManagerStep::Type pConsoleManagerStep::type() const
{
    return pConsoleManagerStep::Type( mData.value( pConsoleManagerStep::TypeRole, pConsoleManagerStep::Invalid ).toInt() );
}

QVariant pConsoleManagerStep::roleValue( int role ) const
{
    if ( mData.contains( role ) ) {
        return mData[ role ];
    }
    
    return defaultRoleValue( role, type() );
}

void pConsoleManagerStep::setRoleValue( int role, const QVariant& value )
{
    mData[ role ] = value;
}
