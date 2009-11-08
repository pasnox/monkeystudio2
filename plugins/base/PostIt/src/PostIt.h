#ifndef REGEXPEDITOR_H
#define REGEXPEDITOR_H

#include <PluginsManager.h>

class UIPostIt;

class PostIt : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
protected:	
	void fillPluginInfos();
	virtual bool install();
	virtual bool uninstall();

	QPointer<UIPostIt> mPostIt;

protected slots:
	void action_triggered();
};

#endif // POSTIT_H