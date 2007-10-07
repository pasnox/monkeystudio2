#ifndef GNUMAKE_H
#define GNUMAKE_H

#include "CompilerPlugin.h"
#include "CLIToolPlugin.h"

class GNUMake : public CompilerPlugin, public CLIToolPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin CLIToolPlugin CompilerPlugin )

public:
	GNUMake ();
	// BasePlugin
	virtual bool setEnabled( bool );
	// CompilerPlugin
	virtual pCommandList globalCommands() const;
	virtual pCommandList userCommands() const;
	virtual void setUserCommands( const pCommandList& ) const;
	// CLIToolPlugin
	virtual QString name();
	virtual QString getCommand ();
	virtual QString getSwitches (QString oldSwitches = QString::null);
	virtual pCommandParser* getParser();

};

#endif //GNUMAKE_H
