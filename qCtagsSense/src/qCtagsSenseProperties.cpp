#include "qCtagsSenseProperties.h"

qCtagsSenseProperties::qCtagsSenseProperties()
{
}

qCtagsSenseProperties::~qCtagsSenseProperties()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
qCtagsSenseProperties::qCtagsSenseProperties( const qCtagsSenseProperties& other )
{
	// copy datas there
}

qCtagsSenseProperties& qCtagsSenseProperties::operator=( const qCtagsSenseProperties& other )
{
	// In order to support the operation "Obj = Obj;"
	if( *this != other )
	{
		// Copy datas there
	}

	return *this;
}

bool qCtagsSenseProperties::operator==( const qCtagsSenseProperties& other ) const
{
	// Comparaison of datas here

	return false;	// Change it
}

bool qCtagsSenseProperties::operator!=( const qCtagsSenseProperties& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/
