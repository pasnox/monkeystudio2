#ifndef QFILEBROWSER_H
#define QFILEBROWSER_H

#include "BasePlugin.h"

class Q_MONKEY_EXPORT QFileBrowser : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	QFileBrowser();
	virtual ~QFileBrowser();
	virtual bool setEnabled( bool );

};

#endif
