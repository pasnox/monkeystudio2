#ifndef GCCPARSER_H
#define GCCPARSER_H

#include "BasePlugin.h"

class GccParser : public BasePlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin )

public:
	GccParser();
	~GccParser();
	// BasePlugin
	virtual bool setEnabled( bool );

};

#endif // GCCPARSER_H
