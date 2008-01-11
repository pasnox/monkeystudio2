#ifndef SETTINGS_H
#define SETTINGS_H

#include "pSettings.h"

class Q_MONKEY_EXPORT Settings : public pSettings
{
	Q_OBJECT
	
public:
	Settings( QObject* = 0 );
	virtual void setDefaultSettings();
};

#endif // SETTINGS_H
