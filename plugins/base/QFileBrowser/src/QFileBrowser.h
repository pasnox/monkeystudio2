#ifndef QFILEBROWSER_H
#define QFILEBROWSER_H

#include "BasePlugin.h"

class QFileBrowser : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	QFileBrowser();
	virtual ~QFileBrowser();
	virtual bool setEnabled( bool );

protected slots:
	void saveSettings();
	void restoreSettings();

};

#endif
