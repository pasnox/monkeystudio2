// TODO make it standalone application

#ifndef REGEXPEDITOR_H
#define REGEXPEDITOR_H

#include <BasePlugin.h>

class UIRegExpEditor;

class RegExpEditor : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	PluginInfos infos() const;
	~RegExpEditor();
	virtual bool setEnabled( bool );

protected:
	QPointer<UIRegExpEditor> mEditor;

protected slots:
	void action_triggered();
};

#endif // REGEXPEDITOR_H
