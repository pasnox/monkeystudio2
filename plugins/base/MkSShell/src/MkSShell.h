#ifndef MKSSHELL_H
#define MKSSHELL_H

#include <BasePlugin.h>

class MkSShellDock;

class MkSShell : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	
public:
	PluginInfos infos() const;
	virtual bool setEnabled( bool );

protected:
	QPointer<MkSShellDock> mDock;
};

#endif // MKSSHELL_H
