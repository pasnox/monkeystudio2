#ifndef MKSSHELL_H
#define MKSSHELL_H

#include <BasePlugin.h>

class MkSShellDock;

class MkSShell : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
protected:	
	void fillPluginInfos();
public:
	virtual bool install();
	virtual bool uninstall();

protected:
	QPointer<MkSShellDock> mDock;
};

#endif // MKSSHELL_H