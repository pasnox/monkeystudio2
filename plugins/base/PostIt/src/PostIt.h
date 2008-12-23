#ifndef REGEXPEDITOR_H
#define REGEXPEDITOR_H

#include <PluginsManager.h>

class UIPostIt;

class PostIt : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	PostIt();
	~PostIt();
	virtual bool setEnabled( bool );

protected:
	QPointer<UIPostIt> mPostIt;

protected slots:
	void action_triggered();
};

#endif // POSTIT_H
