


#ifndef IRC_H
#define IRC_H

#include <pluginsmanager.h>
#include "IrcDock.h"



class Irc : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	Irc();
	virtual ~Irc();

	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	virtual QPixmap pixmap() const;
private:
	IrcDock *mIrcDock;
};

#endif
