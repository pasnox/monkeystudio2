


#ifndef IRC_H
#define IRC_H

#include <BasePlugin.h>
#include "IrcDock.h"



class Irc : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

protected:	
	void fillPluginInfos();
	
	virtual bool setEnabled( bool );
private:
	IrcDock *mIrcDock;
};

#endif