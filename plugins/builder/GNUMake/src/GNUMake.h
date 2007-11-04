/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : GNUMake.h
** Date      : 2007-11-04T22:50:51
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef GNUMAKE_H
#define GNUMAKE_H

#include "BuilderPlugin.h"

class GNUMake : public BuilderPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin BuilderPlugin CLIToolPlugin )

public:
    GNUMake();
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

#endif // GNUMAKE_H
