#ifndef PASTYLE_H
#define PASTYLE_H

#include "BasePlugin.h"

class pAStyle : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	pAStyle();
	~pAStyle();
	virtual QWidget* settingsWidget();
	virtual bool setEnabled( bool );
	
protected slots:
	void applyFormatter();
	
};

#endif // PASTYLE_H
