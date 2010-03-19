#ifndef QCTAGSSENSEPROPERTIES_H
#define QCTAGSSENSEPROPERTIES_H

#include "qCtagsSenseExport.h"

struct QCTAGSSENSE_EXPORT qCtagsSenseProperties
{
	qCtagsSenseProperties();
	virtual ~qCtagsSenseProperties();
	
	/*** For dynamic memory management (can be removed if it isn't necessary) ***/
	qCtagsSenseProperties( const qCtagsSenseProperties& other );
	qCtagsSenseProperties& operator=( const qCtagsSenseProperties& other );
	bool operator==( const qCtagsSenseProperties& other ) const;
	bool operator!=( const qCtagsSenseProperties& other ) const;
	/*** End of dynamic memory management ***/
};

#endif // QCTAGSSENSEPROPERTIES_H
