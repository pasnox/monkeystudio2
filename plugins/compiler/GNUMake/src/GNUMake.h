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
	virtual QWidget* settingsWidget()
	{ return commandsEditorWidget( this ); }
	// CLIToolPlugin
	virtual pCommandList globalCommands() const;
	virtual pCommandList userCommands() const;
	virtual void setUserCommands( const pCommandList& ) const;
	virtual QStringList availableParsers() const;
	virtual pCommandParser* getParser( const QString& );
	
public slots:
	virtual void commandTriggered();
	
};

#endif //GNUMAKE_H
