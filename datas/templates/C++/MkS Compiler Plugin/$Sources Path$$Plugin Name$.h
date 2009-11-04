#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <CompilerPlugin.h>

class $Plugin Name$ : public CompilerPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin CompilerPlugin CLIToolPlugin )

public:
	virtual pCommand defaultCompileCommand() const;
	virtual pCommandList defaultCommands() const;

protected:
	virtual void fillPluginInfos();

	virtual bool install();
	virtual bool uninstall();
};

#endif // $Plugin Name.upper$_H
