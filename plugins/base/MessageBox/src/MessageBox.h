#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "BasePlugin.h"

class MessageBox : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	MessageBox();
	virtual ~MessageBox();
	virtual bool setEnabled( bool );

protected slots:
	void saveSettings();
	void restoreSettings();

};

#endif
