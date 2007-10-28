#ifndef QTDESIGNER_H
#define QTDESIGNER_H

#include "BasePlugin.h"

class QtDesigner : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	QtDesigner();
	virtual ~QtDesigner();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );
	
};

#endif // QTDESIGNER_H
