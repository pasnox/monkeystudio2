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
