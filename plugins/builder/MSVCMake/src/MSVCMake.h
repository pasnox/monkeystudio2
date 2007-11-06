#ifndef MSVCMAKE_H
#define MSVCMAKE_H

#include "BuilderPlugin.h"

class MSVCMake : public BuilderPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin BuilderPlugin CLIToolPlugin )

public:
    MSVCMake();
	~MSVCMake();
    // BasePlugin
    virtual bool setEnabled( bool );
    virtual QWidget* settingsWidget();
    // CLIToolPlugin
    virtual pCommandList defaultCommands() const;
    virtual pCommandList userCommands() const;
    virtual void setUserCommands( const pCommandList& ) const;
    virtual QStringList availableParsers() const;
    virtual pCommandParser* getParser( const QString& );
    // BuilderPlugin
    virtual pCommand defaultBuildCommand() const;
    virtual pCommand buildCommand() const;
    virtual void setBuildCommand( const pCommand& );

private slots:
    virtual void commandTriggered();
    
};

#endif // MSVCMAKE_H
