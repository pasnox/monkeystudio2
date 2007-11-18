#ifndef MSVCCOMPILER_H
#define MSVCCOMPILER_H

#include "CompilerPlugin.h"
#include "CLIToolPlugin.h"

class MSVCCompiler : public CompilerPlugin, public CLIToolPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin CompilerPlugin CLIToolPlugin )

public:
	MSVCCompiler();
	~MSVCCompiler();
	// BasePlugin
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	// CompilerPlugin
	virtual QString setUp( const QString& ) const { return QString(); }
	// CLIToolPlugin
	virtual pCommandList defaultCommands() const;
	virtual pCommandList userCommands() const;
	virtual void setUserCommands( const pCommandList& ) const;
	
public slots:
	virtual void commandTriggered();

};

#endif // MSVCCOMPILER_H
