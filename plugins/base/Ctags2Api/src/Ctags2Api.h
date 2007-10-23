#ifndef CTAGS2API_H
#define CTAGS2API_H

#include "BasePlugin.h"

class Ctags2Api : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	Ctags2Api();
	~Ctags2Api();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );

protected slots:
	void UICtags2Api_show();
	
};

#endif // CTAGS2API_H
