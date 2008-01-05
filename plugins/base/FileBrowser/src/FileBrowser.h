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
	virtual QWidget* settingsWidget();

protected slots:
	void saveSettings();
	void restoreSettings();
};

#endif
