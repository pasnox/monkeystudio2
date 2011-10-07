/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef ENVIRONMENTVARIABLESMANAGER_H
#define ENVIRONMENTVARIABLESMANAGER_H

#include "MonkeyExport.h"

#include <pEnvironmentVariablesManager.h>

class MkSShellInterpreter;

class Q_MONKEY_EXPORT EnvironmentVariablesManager : public pEnvironmentVariablesManager
{
public:
    EnvironmentVariablesManager();
    
    // interpreter commands
    void setCommand( const QString& name, const QString& value );
    void unsetCommand( const QString& name );
    void clearCommand();
    void enableCommand( const QString& name, bool enabled );

protected:
    // pEnvironmentVariablesManager reimplementations
    virtual bool writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
    virtual bool readVariables( pEnvironmentVariablesModel::Variables& variables ) const;
    
    void initializeInterpreterCommands();
    static QString commandInterpreter( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter, void* data );
};

#endif // ENVIRONMENTVARIABLESMANAGER_H
