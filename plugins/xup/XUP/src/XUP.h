#ifndef XUP_H
#define XUP_H

#include <pluginsmanager/XUPPlugin.h>
#include <xupmanager/core/ProjectTypesIndex.h>
#include <xupmanager/core/XUPProjectItem.h>

class GenericXUPProjectItem : public XUPProjectItem
{
	Q_OBJECT

public:
	Q_INVOKABLE GenericXUPProjectItem();
	
	virtual QString projectType() const;
};

class XUP : public XUPPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin XUPPlugin )

protected:
	DocumentFilterMap mFilters;
	
	virtual void fillPluginInfos();
	virtual bool install();
	virtual bool uninstall();
};

#endif // XUP_H
