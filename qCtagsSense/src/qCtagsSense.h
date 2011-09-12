/****************************************************************************
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
#ifndef QCTAGSSENSE_H
#define QCTAGSSENSE_H

#include "qCtagsSenseExport.h"

#include <QObject>
#include <QMap>
#include <QPair>
#include <QStringList>
#include <QMetaType>

class qCtagsSenseSQL;
class qCtagsSenseIndexer;

struct QCTAGSSENSE_EXPORT qCtagsSenseProperties
{
    qCtagsSenseProperties( const QStringList& systemPaths = QStringList(), const QStringList filteredSuffixes = QStringList(),
        bool usePhysicalDatabase = false, const QString& databaseFileName = QString::null )
    {
        SystemPaths = systemPaths;
        FilteredSuffixes = filteredSuffixes;
        UsePhysicalDatabase = usePhysicalDatabase;
        DatabaseFileName = databaseFileName;
    }
    
    bool operator==( const qCtagsSenseProperties& other ) const
    {
        return SystemPaths == other.SystemPaths && FilteredSuffixes == other.FilteredSuffixes &&
            UsePhysicalDatabase == other.UsePhysicalDatabase && DatabaseFileName == other.DatabaseFileName;
    }
    
    bool operator!=( const qCtagsSenseProperties& other ) const
    {
        return !operator==( other );
    }
    
    QStringList SystemPaths;
    QStringList FilteredSuffixes;
    bool UsePhysicalDatabase;
    QString DatabaseFileName;
};

class QCTAGSSENSE_EXPORT qCtagsSense : public QObject
{
    Q_OBJECT
    
public:
    enum Kind
    {
        Unknow = -1,
        // A
        AllPatterns, // BETA
        Anchor, // HTML
        AutoCommandGroup, // Vim
        // B
        BlockData, // Fortran
        BlockCommon, // Fortran
        // C
        Class, // Vera, Tcl, Ruby, Python, PHP, JavaScript, Java, Eiffel, C#, C, C++
        Constant, // Verilog, PHP, Perl, Basic, Asp
        Cursor, // SQL
        // D
        Define, // Asm
        DerivatedType, // Fortran
        Domain, // SQL
        // E
        EntryPoint, // Fortran
        Enumeration, // Vera, Java, C#, C, C++, Basic
        Enumerator, // Vera, C#, C, C++
        EnumType, // Java
        Event, // Verilog, SQL, C#
        Exception, // SML
        // F
        Feature, // Eiffel
        Field, // Java, C#, SQL
        FileDescriptor, // Cobol
        Format, // Perl
        Fragment, // BETA
        Function, // Vim, Verilog, Vera, SQL, SML, SLang, Sh, Scheme, Python, PHP, Pascal, Lua, Lisp, JavaScript, Fortran, Erlang, C, C++, Basic, Awk, Asp
        Functor, // SML
        // G
        // H
        // I
        Index, // SQL
        Interface, // PHP, Java, Fortran, C#
        ItemData, // Cobol
        ItemGroup, // Cobol
        // J
        JavaScriptFunction, // PHP, HTML
        // K
        // L
        Label, // YACC, SQL, Perl, Fortran, Basic, Asm
        // M
        Macro, // Vera, Make, Erlang, C#, C, C++, Asm
        Map, // Vim
        Member, // Vera, Python, C, C++
        Method, // Tcl, Ruby, JavaScript, Java, C#
        MethodSingleton, // Ruby
        MobiLinkTableScript, // SQL
        MobiLinkConnScript, // SQL
        Module, // Verilog, Ruby, Fortran, Erlang
        // N
        NameList, // Fortran
        Namespace, // SLang, C#, C, C++
        NetDataType, // Verilog
        // O
        // P
        Package, // SQL, Perl, Java
        Paragraph, // Cobol
        Pattern, // BETA
        Port, // Verilog
        Procedure, // Tcl, SQL, Pascal
        Program, // Vera, Fortran
        ProgramId, // Cobol
        Property, // C#
        Prototype, // Vera, SQL, C, C++
        Publication, // SQL
        // Q
        // R
        Record, // SQL, Erlang
        RegisterDataType, // Verilog
        // S
        Section, // Cobol
        Service, // SQL
        Set, // Scheme
        Signature, // SML
        Slot, // BETA
        SubRoutine, // REXX, Perl, Fortran, Asp
        SubRoutineDeclaration, // Perl
        SubType, // SQL
        Structure, // SML, C#, C, C++
        Synonym, // SQL
        // T
        Table, // SQL
        Task, // Verilog, Vera
        Trigger, // SQL
        Type, // SML, Fortran, Basic, Asm
        Typedef, // Vera, C#, C, C++
        // U
        Union, // C, C++
        UserDefinedCommand, // Vim
        // V
        ValueBinding, // SML
        Variable, // Vim, Vera, SQL, PHP, Fortran, C, C++, Basic, Asp
        VariableLocal, // Vera, SQL, Java, Fortran, Eiffel, C#, C, C++
        VariableGlobal, // JavaScript, Vera, C, C++
        View, // SQL
        // W
        // X
        // Y
        // Z
    };
    
    qCtagsSense( QObject* parent = 0 );
    virtual ~qCtagsSense();
    
    bool isValid() const;
    qCtagsSenseProperties properties() const;
    qCtagsSenseSQL* sql() const;
    qCtagsSenseIndexer* indexer() const;

public slots:
    void setProperties( const qCtagsSenseProperties& properties );
    void tagEntry( const QString& fileName );
    void tagEntries( const QStringList& fileNames );
    void tagEntries( const QMap<QString, QString>& entries );

protected:
    bool mInitialized;
    qCtagsSenseProperties mProperties;
    qCtagsSenseSQL* mSQL;
    qCtagsSenseIndexer* mIndexer;

signals:
    void indexingStarted();
    void indexingProgress( int value, int max );
    void indexingFinished();
    void indexingChanged();
};

struct qCtagsSenseEntry;
typedef QList<qCtagsSenseEntry*> qCtagsSenseEntryList;

struct QCTAGSSENSE_EXPORT qCtagsSenseEntry
{
    qCtagsSenseEntry( qCtagsSenseEntry* pItem = 0, bool numberEntry = false, ulong number = -1,  bool scope = false, bool entry = false, bool truncate = false, qCtagsSense::Kind _kind = qCtagsSense::Unknow )
    {
        parent = pItem;
        lineNumberEntry = numberEntry;
        lineNumber = number;
        isFileScope = scope;
        isFileEntry = entry;
        truncateLine = truncate;
        kind = _kind;
    }
    
    ~qCtagsSenseEntry()
    {
        qDeleteAll( children );
    }
    
    qCtagsSenseEntry* parent;
    
    QString fileName;
    QString language;
    bool lineNumberEntry;
    ulong lineNumber;
    bool isFileScope; // is header file
    bool isFileEntry;
    bool truncateLine;
    QString name;
    qCtagsSense::Kind kind;
    // extended fields
    QString access;
    QString fileScope;
    QString implementation;
    QString inheritance;
    QPair<QString, QString> scope;
    QString signature;
    QPair<QString, QString> typeRef;
    // children
    qCtagsSenseEntryList children;
};

Q_DECLARE_METATYPE( qCtagsSenseEntry* )
Q_DECLARE_METATYPE( qCtagsSenseEntry )

#endif // QCTAGSSENSE_H
