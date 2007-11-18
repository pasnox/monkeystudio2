#ifndef MSVC_H
#define MSVC_H

#include "CompilerPlugin.h"
#include "CLIToolPlugin.h"

class MSVC : public CompilerPlugin, public CLIToolPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin CompilerPlugin CLIToolPlugin )

public:
	MSVC();
	~MSVC();
	// BasePlugin
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	// CompilerPlugin
	virtual pCommand defaultCompileCommand() const;
	virtual pCommand compileCommand() const;
	virtual void setCompileCommand( const pCommand& );
	virtual QString setUp( const QString& ) const { return QString(); }
	// CLIToolPlugin
	virtual pCommandList defaultCommands() const;
	virtual pCommandList userCommands() const;
	virtual void setUserCommands( const pCommandList& ) const;
	virtual QStringList availableParsers() const;
	virtual pCommandParser* getParser( const QString& );
	
public slots:
	virtual void commandTriggered();

};

#endif // MSVC_H
