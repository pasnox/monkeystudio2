#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H

#include "xupmanager/core/XUPProjectItem.h"
#include <xupmanager/core/XUPProjectItemCache.h>

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
    friend class QMakeProjectItemCacheBackend;
    
public:
    Q_INVOKABLE QMakeProjectItem();
    virtual ~QMakeProjectItem();
    
    virtual void removeValue( XUPItem* item, bool deleteFiles );
    virtual QString quoteString() const;
    virtual QString defaultOperator() const;
    virtual QString toNativeString() const;
    virtual QString projectType() const;
    virtual bool open( const QString& fileName, const QString& codec );
    virtual QString targetFilePath( XUPProjectItem::TargetType type = XUPProjectItem::DefaultTarget );
    virtual void installCommands();
    virtual XUPProjectItemCacheBackend* cacheBackend() const;
    
protected:
    enum ActionTypeFlag {
        NoFlag = 0x0,
        BuildFlag = 0x1,
        CleanFlag = 0x2,
        DistcleanFlag = 0x4,
        ExecuteFlag = 0x8,
        ReleaseFlag = 0x10,
        DebugFlag = 0x20,
        AllFlag = 0x40,
        QMakeFlag = 0x80,
        LUpdateFlag = 0x100,
        LReleaseFlag = 0x200
    };
    
    enum ActionType {
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
    
    static QMakeProjectItemCacheBackend mCacheBackend;
    
    virtual UIXUPEditor* newEditDialog() const;
    
    // guess subdirs project fileChanged
    QString guessSubProjectFilePath( const QString& subdirsValue ) const;
    // handle the inclusion of include files
    bool handleIncludeFile( XUPItem* function );
    // handle inclusion of subdirs projects
    bool handleSubdirs( XUPItem* subdirs );
    
    CLIToolPlugin* builder() const;

protected slots:
    virtual void projectCustomActionTriggered();
};

#endif // QMAKEPROJECTITEM_H
