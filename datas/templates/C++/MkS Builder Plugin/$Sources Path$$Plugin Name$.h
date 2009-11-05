#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <BuilderPlugin.h>

class $Plugin Name$ : public BuilderPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin BuilderPlugin CLIToolPlugin )

public:
	virtual pCommand defaultBuildCommand() const;
	virtual pCommandList defaultCommands() const;

protected:
	virtual void fillPluginInfos();

	virtual bool install();
	virtual bool uninstall();
};

#endif // $Plugin Name.upper$_H
