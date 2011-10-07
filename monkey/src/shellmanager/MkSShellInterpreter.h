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
#ifndef MKSSHELLINTERPRETER_H
#define MKSSHELLINTERPRETER_H

#include <MonkeyExport.h>
#include "pConsoleCommand.h"

#include <QPointer>
#include <QObject>
#include <QHash>

/*
Pointer to function
QString commandImplementation( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter, void* data )
*/
typedef QString (*CommandImplementationPtr)(const QString&, const QStringList&, int*, class MkSShellInterpreter*, void* );

class Q_MONKEY_EXPORT MkSShellInterpreter : public QObject, public pConsoleCommand
{
    Q_OBJECT
    
public:
    enum Error
    {
        NoError = 0,
        InvalidCommand = -1,
        NoResultVariable = -2,
        UnknowError = -3
    };
    
    static MkSShellInterpreter* instance( QObject* parent = 0 );
    
    bool loadScript( const QString& fileName );
    void loadHomeScripts();
    QString usage( const QString& command ) const;
    QString interpret( const QString& command, int* result ) const;
    
    void addCommandImplementation( const QString& command, CommandImplementationPtr function, const QString& help = QString::null, void* data = 0 );
    void removeCommandImplementation( const QString& command );
    void setCommandHelp( const QString& command, const QString& help );
    
protected:
    static QPointer<MkSShellInterpreter> mInstance;
    QHash<QString, CommandImplementationPtr> mCommandImplementations;
    QHash<QString, void*> mCommandImplementationsData;
    QHash<QString, QString> mCommandHelps;
    
    MkSShellInterpreter( QObject* parent = 0 );
    static QString interpretHelp( const QString&, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter, void* data );
    static QString interpretEcho( const QString&, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter, void* data );
    
signals:
    void commandExecuted( const QString& command, const QString& output, int result );
};

#endif // MKSSHELLINTERPRETER_H
