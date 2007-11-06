#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include "BasePlugin.h"

class FileBrowser : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	FileBrowser();
	virtual ~FileBrowser();
	virtual bool setEnabled( bool );

protected slots:
	void saveSettings();
	void restoreSettings();

};

#endif
