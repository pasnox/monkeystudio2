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
#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "xupmanager/core/XUPProjectItem.h"
#include <xupmanager/core/XUPProjectItemCache.h>
#include <consolemanager/pConsoleManager.h>

class CLIToolPlugin;

class QMakeProjectItemCacheBackend : public XUPProjectItemCacheBackend
{
public:
    QMakeProjectItemCacheBackend( XUPProjectItemCache* cache );
    
    virtual QStringList guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const;
    virtual QStringList guessedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QStringList& content ) const;
    virtual void updateVariable( XUPProjectItem* project, const QString& variable, const QStringList& values, const QString& op = QString::null );
    virtual bool cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item );
};

class QMakeProjectItem : public XUPProjectItem
{
    Q_OBJECT
    Q_ENUMS( ActionTypeFlag )
    friend class QMakeProjectItemCacheBackend;
    
public:
    Q_INVOKABLE QMakeProjectItem();
    virtual ~QMakeProjectItem();
    
    virtual void addFiles( const QStringList& files, XUPItem* scope = 0 );
    virtual void removeValue( XUPItem* item, bool deleteFiles );
    virtual QString quoteString() const;
    virtual QString defaultOperator() const;
    virtual QString toNativeString() const;
    virtual QString projectType() const;
    virtual bool open( const QString& fileName, const QString& codec );
    virtual QString targetFilePath( XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget );
    virtual void executeCommand( const QString& name );
    virtual void installCommandsV2();
    virtual void installCommands();
    virtual XUPProjectItemCacheBackend* cacheBackend() const;
    
    enum ActionTypeFlag {
        NoFlag = 0x0,
        BuildFlag = 0x1,
        CleanFlag = 0x2,
        DistcleanFlag = 0x4,
        InstallFlag = 0x8,
        UninstallFlag = 0x10,
        ExecuteFlag = 0x20,
        ReleaseFlag = 0x40,
        DebugFlag = 0x80,
        AllFlag = 0x100,
        QMakeFlag = 0x200,
        LUpdateFlag = 0x400,
        LReleaseFlag = 0x800,
    };
    
    enum DefaultActionType {
        BuildDebug = QMakeProjectItem::BuildFlag | QMakeProjectItem::DebugFlag,
        BuildRelease = QMakeProjectItem::BuildFlag | QMakeProjectItem::ReleaseFlag,
        BuildAll = QMakeProjectItem::BuildFlag | QMakeProjectItem::AllFlag,
        Build = QMakeProjectItem::BuildFlag,
        CleanDebug = QMakeProjectItem::CleanFlag | QMakeProjectItem::DebugFlag,
        CleanRelease = QMakeProjectItem::CleanFlag | QMakeProjectItem::ReleaseFlag,
        CleanAll = QMakeProjectItem::CleanFlag | QMakeProjectItem::AllFlag,
        Clean = QMakeProjectItem::CleanFlag,
        DistcleanDebug = QMakeProjectItem::DistcleanFlag | QMakeProjectItem::DebugFlag,
        DistcleanRelease = QMakeProjectItem::DistcleanFlag | QMakeProjectItem::ReleaseFlag,
        DistcleanAll = QMakeProjectItem::DistcleanFlag | QMakeProjectItem::AllFlag,
        Distclean = QMakeProjectItem::DistcleanFlag,
        InstallDebug = QMakeProjectItem::InstallFlag | QMakeProjectItem::DebugFlag,
        InstallRelease = QMakeProjectItem::InstallFlag | QMakeProjectItem::ReleaseFlag,
        InstallAll = QMakeProjectItem::InstallFlag | QMakeProjectItem::AllFlag,
        Install = QMakeProjectItem::InstallFlag,
        UninstallDebug = QMakeProjectItem::UninstallFlag | QMakeProjectItem::DebugFlag,
        UninstallRelease = QMakeProjectItem::UninstallFlag | QMakeProjectItem::ReleaseFlag,
        UninstallAll = QMakeProjectItem::UninstallFlag | QMakeProjectItem::AllFlag,
        Uninstall = QMakeProjectItem::UninstallFlag,
        RebuildDebug = QMakeProjectItem::DistcleanFlag | QMakeProjectItem::QMakeFlag | QMakeProjectItem::BuildFlag | QMakeProjectItem::DebugFlag,
        RebuildRelease = QMakeProjectItem::DistcleanFlag | QMakeProjectItem::QMakeFlag | QMakeProjectItem::BuildFlag | QMakeProjectItem::ReleaseFlag,
        RebuildAll = QMakeProjectItem::DistcleanFlag | QMakeProjectItem::QMakeFlag | QMakeProjectItem::BuildFlag | QMakeProjectItem::AllFlag,
        Rebuild = QMakeProjectItem::DistcleanFlag | QMakeProjectItem::QMakeFlag | QMakeProjectItem::BuildFlag,
        ExecuteDebug = QMakeProjectItem::ExecuteFlag | QMakeProjectItem::DebugFlag,
        ExecuteRelease = QMakeProjectItem::ExecuteFlag | QMakeProjectItem::ReleaseFlag,
        Execute = QMakeProjectItem::ExecuteFlag,
        QMake = QMakeProjectItem::QMakeFlag,
        LUpdate = QMakeProjectItem::LUpdateFlag,
        LRelease = QMakeProjectItem::LReleaseFlag
    };
    
    Q_DECLARE_FLAGS( ActionType, ActionTypeFlag )
    
protected:
    static QMakeProjectItemCacheBackend mCacheBackend;
    pCommand mLastCommand;
    
    virtual UIXUPEditor* newEditDialog() const;
    
    // guess subdirs project fileChanged
    QString guessSubProjectFilePath( const QString& subdirsValue ) const;
    // handle the inclusion of include files
    bool handleIncludeFile( XUPItem* function );
    // handle inclusion of subdirs projects
    bool handleSubdirs( XUPItem* subdirs );
    // return the makefile rules
    QList<QByteArray> makefileRules( const QString& filePath ) const;
    // return a list of QFileInfo pointing to makefile files
    QFileInfoList makefiles( const QString& directory ) const;
    // return a title cased string
    QString toTitleCase( const QString& string ) const;
    // return string version of flags
    QString actionTypeToString( QMakeProjectItem::ActionType type ) const;
    QString defaultActionTypeToString( QMakeProjectItem::DefaultActionType type ) const;
    // return a flag version of string
    QMakeProjectItem::ActionType stringToActionType( const QString& string ) const;
    // return a string text for command text based on type flags
    QString actionTypeToText( QMakeProjectItem::ActionType type ) const;
    QString defaultActionTypeToText( QMakeProjectItem::DefaultActionType type ) const;
    
    CLIToolPlugin* builder() const;
    
    QString shadowBuildPath() const;

protected slots:
    virtual void projectCustomActionTriggered();
    void consoleManager_commandFinished( const pCommand& cmd, int exitCode, QProcess::ExitStatus exitStatus );
};

Q_DECLARE_OPERATORS_FOR_FLAGS( QMakeProjectItem::ActionType )

#endif // QMAKEPROJECTITEM_H
