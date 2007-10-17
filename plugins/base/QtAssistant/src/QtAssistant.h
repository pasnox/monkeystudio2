#ifndef QTASSISTANT_H
#define QTASSISTANT_H

#include "WorkspacePlugin.h"

class QtAssistant : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	QtAssistant();
	virtual ~QtAssistant();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );
	
};

#endif // QTASSISTANT_H
