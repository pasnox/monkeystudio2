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
	virtual QString setUp( const QString& ) const { return QString(); }
	// CLIToolPlugin
	virtual pCommandList defaultCommands() const;
	virtual pCommandList userCommands() const;
	virtual void setUserCommands( const pCommandList& ) const;
	
public slots:
	virtual void commandTriggered();

};

#endif // MSVC_H
