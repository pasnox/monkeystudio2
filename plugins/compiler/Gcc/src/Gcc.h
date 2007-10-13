#ifndef GNUMAKE_H
#define GNUMAKE_H

#include "CompilerPlugin.h"
#include "CLIToolPlugin.h"

class Gcc : public CompilerPlugin, public CLIToolPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin CompilerPlugin CLIToolPlugin )

public:
	Gcc();
	// BasePlugin
	virtual bool setEnabled( bool );
	virtual QWidget* settingsWidget();
	// CompilerPlugin
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

#endif //GNUMAKE_H
