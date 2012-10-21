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
#include "QMakeProjectItem.h"
#include "QtVersionManager.h"
#include "QMake.h"
#include "QMake2XUP.h"
#include "editor/UIQMakeEditor.h"

#include <xupmanager/core/XUPProjectItemHelper.h>
#include <maininterface/UIMain.h>
#include <pluginsmanager/CLIToolPlugin.h>
#include <pQueuedMessageToolBar.h>
#include <pluginsmanager/PluginsManager.h>
#include <xupmanager/gui/XUPProjectManager.h>

#include <pVersion.h>

#include <QProcess>
#include <QLibrary>
#include <QFileDialog>
#include <QMetaEnum>
#include <QDebug>

#if defined( Q_OS_WIN )
    #define PLATFORM_TYPE_STRING "WINDOWS_PLATFORM"
#elif defined( Q_OS_MAC )
    #define PLATFORM_TYPE_STRING "MAC_PLATFORM"
#else
    #define PLATFORM_TYPE_STRING "OTHERS_PLATFORM"
#endif

static QSet<QString> QMakeNativeFunctions = QSet<QString>() // set better for contains test
    << "basename"
    << "CONFIG"
    << "contains"
    << "count"
    << "dirname"
    << "error"
    << "eval"
    << "exists"
    << "find"
    << "for"
    << "include"
    << "infile"
    << "isEmpty"
    << "join"
    << "member"
    << "message"
    << "prompt"
    << "quote"
    << "replace"
    << "sprintf"
    << "system"
    << "unique"
    << "warning"
    ;

QDebug operator<<( QDebug dbg, const QFileInfo& fi )
{
    dbg.nospace() << fi.absoluteFilePath();
    return dbg.space();
}

QMakeProjectItemCacheBackend QMakeProjectItem::mCacheBackend = QMakeProjectItemCacheBackend( QMakeProjectItem::cache() );

// QMakeProjectItemCacheBackend

QMakeProjectItemCacheBackend::QMakeProjectItemCacheBackend( XUPProjectItemCache* cache )
    : XUPProjectItemCacheBackend( cache )
{
}

QStringList QMakeProjectItemCacheBackend::guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const
{
    /*
        $$[QT_INSTALL_HEADERS] : read content from qt conf
        $${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
        $$(QT_INSTALL_HEADERS) : read content from environment variable when qmake run
        $(QTDIR) : read from generated makefile - Hmm what does that mean ?!
    */
    
    const QString name = QString( variable ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
    
    if ( QMakeNativeFunctions.contains( name ) ) {
        return QStringList( variable );
    }
    
    if ( !mCache ) {
        return QStringList();
    }
    
    XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
    
    // environment variable
    if ( variable.startsWith( "$$(" ) || variable.startsWith( "$(" ) ) {
        if ( name == "PWD" ) {
            const QString path = project->path();
            return path.isEmpty() ? QStringList() : QStringList( path );
        }
        else {
            const QString value = QString::fromLocal8Bit( qgetenv( name.toLocal8Bit().constData() ) );
            return value.isEmpty() ? QStringList() : QStringList( value );
        }
    }
    // qmake variable
    else if ( variable.startsWith( "$$[" ) ) {
        // caching result for now, we may disable if needed as user can change qmake variable outside
        if ( cachedData.value( project ).contains( name ) ) {
            return cachedData[ project ][ name ];
        }
        
        const QtVersion version = QMake::versionManager()->version( XUPProjectItemHelper::projectSettingsValue( project, "QT_VERSION" ) );
        QString output;
        
        if ( version.isValid() ) {
            QProcess query;
            query.start( QString( "%1 -query %2" ).arg( version.qmake() ).arg( name ) );
            query.waitForFinished();
            output = QString::fromLocal8Bit( query.readAll() ).trimmed();
            
            if ( output == "**Unknown**" ) {
                output.clear();
            }
        }
        
        // caching result for now, we may disable if needed as user can change qmake variable outside
        cachedData[ project ][ name ] = QStringList( output );
        return QStringList( output );
    }
    // project variable
    else {
        if ( name == "PWD" ) {
            const QString path = variableProject->path();
            return path.isEmpty() ? QStringList() : QStringList( path );
        }
        else if ( name == "_PRO_FILE_" ) {
            const QString fileName = project->fileName();
            return fileName.isEmpty() ? QStringList() : QStringList( fileName );
        }
        else if ( name == "_PRO_FILE_PWD_" ) {
            const QString path = project->path();
            return path.isEmpty() ? QStringList() : QStringList( path );
        }
        else {
            return cachedData.value( project ).value( name );
        }
    }
    
    return QStringList();
}

QStringList QMakeProjectItemCacheBackend::guessedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QStringList& content ) const
{
    if ( !mCache ) {
        return QStringList();
    }
    
    //const QRegExp rx( "\\$\\$?[\\{\\(\\[]?([\\w\\.]+(?!\\w*\\s*\\{\\[\\(\\)\\]\\}))[\\]\\)\\}]?" );
    const QRegExp rx( "(?:[^$]|^)(\\${1,2}(?!\\$+)[{(\\[]?[\\w._]+[})\\]]?)" );
    XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
    QString loopContent = content.join( " " );
    QStringList guessed = content;
    int pos = 0;

    while ( ( pos = rx.indexIn( loopContent, pos ) ) != -1 ) {
        const QString capture = rx.cap( 1 );
        const QString guessedVariableContent = guessedVariable( project, valueProject, capture ).join( " " );
        loopContent.replace( capture, guessedVariableContent );
        guessed.replaceInStrings( capture, guessedVariableContent );
        pos += guessedVariableContent.length();
    }
    
#ifndef QT_NO_DEBUG
    QString s = guessed.join( " " );
    
    foreach ( const QString& f, QMakeNativeFunctions ) {
        s.replace( QString( "$$%1" ).arg( f ), QString::null );
    }
    
    if ( s.contains( "$" ) ) {
        qWarning() << "Failed guessing";
        qWarning() << content;
        qWarning() << loopContent;
        qWarning() << guessed;
        qWarning() << cachedData.value( project );
        //Q_ASSERT( 0 );
    }
#endif

    return guessed;
}

void QMakeProjectItemCacheBackend::updateVariable( XUPProjectItem* project, const QString& variable, const QStringList& values, const QString& op )
{
    if ( !mCache ) {
        return;
    }
    
    XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
    
    if ( op == "=" || op.isEmpty() ) {
        cachedData[ project ][ variable ] = values;
    }
    else if ( op == "-=" ) {
        const DocumentFilterMap& filter = project->documentFilters();
        const QStringList opValue = filter.splitValue( values.join( " " ) ).toSet().toList();
        
        foreach ( const QString& value, opValue ) {
            cachedData[ project ][ variable ].replaceInStrings( QRegExp( QString( "\\b%1\\b" ).arg( value ) ), QString::null );
        }
    }
    else if ( op == "+=" ) {
        cachedData[ project ][ variable ] << values;
    }
    else if ( op == "*=" ) {
        const DocumentFilterMap& filter = project->documentFilters();
        QSet<QString> currentValues = filter.splitValue( cachedData[ project ][ variable ].join( " " ) ).toSet();
        
        foreach ( const QString& value, values ) {
            const QStringList newValues = filter.splitValue( value ).toSet().toList();
            QStringList parts;
            
            foreach ( const QString& value, newValues ) {
                if ( !currentValues.contains( value ) ) {
                    currentValues << value;
                    parts << value;
                }
            }
            
            if ( !parts.isEmpty() ) {
                cachedData[ project ][ variable ] << parts.join( " " );
            }
        }
    }
    else if ( op == "~=" ) {
        project->showError( QMakeProjectItem::tr( "Don't know how to interpret ~= operator" ) );
    }
}

bool QMakeProjectItemCacheBackend::cacheRecursiveScanHook( XUPProjectItem* _project, XUPItem* item )
{
    QMakeProjectItem* project = qobject_cast<QMakeProjectItem*>( _project );
    
    if ( !project ) {
        return false;
    }
    
    bool changed = false;
    
    // handle include projects
    if ( item->type() == XUPItem::Function && item->attribute( "name" ).toLower() == "include" ) {
        if ( project->handleIncludeFile( item ) ) {
            changed = true;
        }
    }
    
    // handle sub projects
    if ( item->type() == XUPItem::Variable && item->attribute( "name" ) == "SUBDIRS" ) {
        if ( project->handleSubdirs( item ) ) {
            changed = true;
        }
    }
    
    return changed;
}

// QMakeProjectItem

QMakeProjectItem::QMakeProjectItem()
    : XUPProjectItem()
{
    connect( MonkeyCore::consoleManager(), SIGNAL( commandFinished( const pCommand&, int, QProcess::ExitStatus ) ), this, SLOT( consoleManager_commandFinished( const pCommand&, int, QProcess::ExitStatus ) ) );
}

QMakeProjectItem::~QMakeProjectItem()
{
}

void QMakeProjectItem::addFiles( const QStringList& _files, XUPItem* _scope )
{
    const DocumentFilterMap& filters = documentFilters();
    const QString op = defaultOperator();
    XUPItem* scope = _scope ? _scope : this;
    XUPProjectItem* project = scope ? scope->project() : this;
    QSet<QString> files = _files.toSet();
    
    foreach ( const QString& _file, files ) {
        const QString file = unquotedValue( _file );
        const QString variableName = filters.fileNameVariable( file );
        
        if ( variableName != "SUBDIRS" || file.isEmpty() ) {
            continue;
        }
        
        files.remove( _file );
        
        QString filePath = project->filePath( file );
        
        if ( pMonkeyStudio::isSameFile( filePath, project->fileName() ) ) {
            continue;
        }
        
        XUPItem* variableItem = project->getVariable( scope, variableName );
        bool fileExists = false;
        
        if ( variableItem ) {
            foreach ( XUPItem* item, variableItem->childrenList() ) {
                switch ( item->type() ) {
                    case XUPItem::Value:
                    case XUPItem::File:
                    case XUPItem::Path:
                        fileExists = pMonkeyStudio::isSameFile( filePath, project->filePath( item->cacheValue( "content" ) ) );
                        break;
                    default:
                        break;
                }
                
                if ( fileExists ) {
                    break;
                }
            }
        }
        
        if ( !fileExists ) {
            if ( !variableItem ) {
                variableItem = scope->addChild( XUPItem::Variable );
                variableItem->setAttribute( "name", variableName );
                
                if ( !op.isEmpty() ) {
                    variableItem->setAttribute( "operator", op );
                }
            }
            
            filePath = quotedValue( project->relativeFilePath( QFileInfo( file ).absolutePath() ) );
            XUPItem* valueItem = variableItem->addChild( XUPItem::File );
            valueItem->setContent( filePath );
        }
    }
    
    XUPProjectItem::addFiles( files.toList(), _scope );
}

void QMakeProjectItem::removeValue( XUPItem* item, bool deleteFiles )
{
    switch ( item->type() ) {
        case XUPItem::Variable: {
            if ( item->attribute( "name" ) == "SUBDIRS" ) {
                item->setCacheValue( "markDeleted", "1" ); // see XUPProjectItem::removeValue
                
                foreach ( XUPItem* value, item->childrenList() ) {
                    removeValue( value, false );
                }
            }
            
            break;
        }
        case XUPItem::File: {
            if ( item->parent()->attribute( "name" ) == "SUBDIRS" ) {
                XUPProjectItem* project = item->project();
                const DocumentFilterMap& filter = project->documentFilters();
                QStringList cacheFns = filter.splitValue( item->cacheValue( "content" ) );
                QSet<QString> projects;
                
                foreach ( const QString& cacheFn, cacheFns ) {
                    const QString filePath = guessSubProjectFilePath( cacheFn );
                    
                    if ( cacheFn.isEmpty() ) {
                        continue;
                    }
                    
                    if ( !projects.contains( filePath ) ) {
                        projects << filePath;
                    }
                }
                
                foreach ( XUPProjectItem* proj, project->childrenProjects( false ) ) {
                    const QString projectFilePath = QDir::cleanPath( QDir::toNativeSeparators( proj->fileName() ) );
                    
                    if ( projects.contains( projectFilePath ) ) {
                        projects.remove( projectFilePath );
                        project->removeChild( proj );
                    }
                }
            }
            
            break;
        }
        default:
            break;
    }
    
    XUPProjectItem::removeValue( item, deleteFiles );
}

QString QMakeProjectItem::quoteString() const
{
    return "\"";
}

QString QMakeProjectItem::defaultOperator() const
{
    return "*=";
}

QString QMakeProjectItem::toNativeString() const
{
    return QMake2XUP::convertToPro( mDocument );
}

QString QMakeProjectItem::projectType() const
{
    return PLUGIN_NAME;
}

bool QMakeProjectItem::open( const QString& fileName, const QString& codec )
{
    QString buffer = QMake2XUP::convertFromPro( fileName, codec );
    
    // parse content
    QString errorMsg;
    int errorLine;
    int errorColumn;
    
    if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) ) {
        showError( tr( "Xml error in '%1':\n%2 on line %3, column %4" )
            .arg( fileName )
            .arg( errorMsg )
            .arg( errorLine )
            .arg( errorColumn )
        );
        return false;
    }
    
    // check project validity
    mDomElement = mDocument.firstChildElement( "project" );
    
    if ( mDomElement.isNull() ) {
        showError( tr( "Invalid project: no project node in '%1'" ).arg( fileName ) );
        return false;
    }
    
    // check xup version
    const QString docVersion = mDomElement.attribute( "version" );
    
    if ( pVersion( docVersion ) < XUP_VERSION ) { 
        showError( tr( "The document format is too old, current version is '%1', your document is '%2' in '%3'" )
            .arg( XUP_VERSION )
            .arg( docVersion )
            .arg( fileName )
        );
        return false;
    }
    
    // all is ok
    mCodec = codec;
    mFileName = fileName;
    
    const XUPItem* parent = XUPItem::parent();
    const bool isIncludeProject = parent && parent->type() == XUPItem::Function && parent->attribute( "name" ) == "include";
    
    if ( isIncludeProject ) {
        XUPProjectItem::cache()->update( parent->project()->rootIncludeProject(), this );
    }
    else {
        XUPProjectItem::cache()->build( this );
    }
    
    emitDataChanged();
    return true;
}

QString QMakeProjectItem::targetFilePath( XUPProjectItem::TargetType targetType )
{
    QString targetTypeString;
    
    switch ( targetType ) {
        case XUPProjectItem::DefaultTarget:
            targetTypeString = QLatin1String( "TARGET_DEFAULT" );
            break;
        case XUPProjectItem::DebugTarget:
            targetTypeString = QLatin1String( "TARGET_DEBUG" );
            break;
        case XUPProjectItem::ReleaseTarget:
            targetTypeString = QLatin1String( "TARGET_RELEASE" );
            break;
        default:
            return QString::null;
    }

    XUPProjectItem* tlProject = topLevelProject();
    const QString key = QString( "%1_%2" ).arg( PLATFORM_TYPE_STRING ).arg( targetTypeString );
    QString target = tlProject->filePath( XUPProjectItemHelper::projectSettingsValue( tlProject, key ) );
    QFileInfo targetInfo( target );
    
    if ( !targetInfo.exists() || ( !targetInfo.isExecutable() && !QLibrary::isLibrary( target ) ) ) {
        QString type;
        
        switch ( targetType ) {
            case XUPProjectItem::DebugTarget:
                type = tr( "debug" ) +" ";
                break;
            case XUPProjectItem::ReleaseTarget:
                type = tr( "release" ) +" ";
                break;
            default:
                break;
        }
        
        const QString userTarget = QFileDialog::getOpenFileName( MonkeyCore::mainWindow(), tr( "Point please project %1target" ).arg( type ), tlProject->path() );
        targetInfo.setFile( userTarget );
        
        if ( !userTarget.isEmpty() ) {
            target = userTarget;
        }
        
        if ( targetInfo.exists() ) {
            XUPProjectItemHelper::setProjectSettingsValue( tlProject, key, tlProject->relativeFilePath( userTarget ) );
            tlProject->save();
        }
    }
    
    return target;
}

QString QMakeProjectItem::guessSubProjectFilePath( const QString& subdirsValue ) const
{
    if ( subdirsValue.isEmpty() ) {
        return QString::null;
    }
    
    QFileInfo file( filePath( subdirsValue ) );
    
    if ( file.isDir() ) {
        QDir dir( file.absoluteFilePath() );
        const QString mask = QString( "%1.pro" ).arg( file.fileName() );
        const QFileInfoList files = pMonkeyStudio::getFiles( dir, QStringList( mask ), false );
        file.setFile( files.value( 0 ).absoluteFilePath() );
    }
    
    return QDir::cleanPath( QDir::toNativeSeparators( file.absoluteFilePath() ) );
}

bool QMakeProjectItem::handleIncludeFile( XUPItem* function )
{
    XUPProjectItem* project = function->project();
    const QString filePath = project->filePath( function->cacheValue( "parameters" ) );
    QStringList projects;
    
    // search already handled include project
    foreach ( XUPItem* child, function->childrenList() ) {
        if ( child->type() == XUPItem::Project ) {
            projects << child->project()->fileName();
        }
    }
    
    // check if project is already handled
    if ( projects.contains( filePath ) ) {
        return false;
    }

    // open project
    XUPProjectItem* includeProject = new QMakeProjectItem();
    function->addChild( includeProject );

    // remove and delete project if can't open
    if ( !includeProject->open( filePath, project->codec() ) ) {
        function->removeChild( includeProject );
        showError( tr( "Failed to handle include file '%1'" ).arg( filePath ) );
        return false;
    }
    
    return true;
}

bool QMakeProjectItem::handleSubdirs( XUPItem* subdirs )
{
    XUPProjectItem* project = subdirs->project();
    const DocumentFilterMap& filter = project->documentFilters();
    QStringList projects;
    bool created = false;
    
    // search all sub project to handle
    foreach ( XUPItem* child, subdirs->childrenList() ) {
        if ( child->type() == XUPItem::File ) {
            QStringList cacheFns = filter.splitValue( child->cacheValue( "content" ) );
            
            foreach ( const QString& cacheFn, cacheFns ) {
                const QString filePath = guessSubProjectFilePath( cacheFn );
                
                if ( cacheFn.isEmpty() ) {
                    continue;
                }
                
                if ( !projects.contains( filePath ) ) {
                    projects << filePath;
                }
            }
        }
    }
    
    // remove already handled sub projects
    foreach ( XUPItem* child, project->childrenList() ) {
        if ( child->type() == XUPItem::Project ) {
            const QString filePath = QDir::cleanPath( QDir::toNativeSeparators( child->project()->fileName() ) );
            
            if ( projects.contains( filePath ) ) {
                projects.removeAll( filePath );
            }
        }
    }
    
    // open missing sub projects
    foreach ( const QString& filePath, projects ) {
        // open project
        XUPProjectItem* subProject = new QMakeProjectItem();
        project->addChild( subProject );
        
        // remove and delete project if can't open
        if ( subProject->open( filePath, project->codec() ) ) {
            created = true;
        }
        else {
            project->removeChild( subProject );
            showError( tr( "Failed to handle subdirs file '%1'" ).arg( filePath ) );
            continue;
        }
    }
    
    return created;
}

QList<QByteArray> QMakeProjectItem::makefileRules( const QString& filePath ) const
{
    QFile file( filePath );
    
    if ( !file.open( QIODevice::ReadOnly ) ) {
        return QList<QByteArray>();
    }
    
    int i = 0;
    QHash<QByteArray, int> orderedKeepRules;
    orderedKeepRules[ QT_TR_NOOP( "build" ) ] = i++; // fake
    orderedKeepRules[ QT_TR_NOOP( "clean" ) ] = i++;
    orderedKeepRules[ QT_TR_NOOP( "distclean" ) ] = i++;
    orderedKeepRules[ QT_TR_NOOP( "rebuild" ) ] = i++; // fake
    orderedKeepRules[ QT_TR_NOOP( "execute" ) ] = i++; // fake
    orderedKeepRules[ QT_TR_NOOP( "install" ) ] = i++;
    orderedKeepRules[ QT_TR_NOOP( "uninstall" ) ] = i++;
    //orderedKeepRules[ QT_TR_NOOP( "all" ) ] = i++;
    
    const QSet<QByteArray> keepRules = orderedKeepRules.keys().toSet();
    const QRegExp targetRex( "^([\\w\\-_\\d]+):.*" );
    QHash<int, QByteArray> rules;
    
    while ( !file.atEnd() ) {
        const QByteArray line = file.readLine();
        const int pos = targetRex.indexIn( line );
        const QByteArray rule = pos != -1 ? targetRex.cap( 1 ).toAscii() : QByteArray();
        const int id = orderedKeepRules.value( rule, -1 );
        
        if ( id != -1 ) {
            rules[ id ] = rule;
        }
    }
    
    if ( !rules.isEmpty() ) {
        rules[ orderedKeepRules.value( "build" ) ] = "build";
        rules[ orderedKeepRules.value( "rebuild" ) ] = "rebuild";
        rules[ orderedKeepRules.value( "execute" ) ] = "execute";
    }
    
    return rules.values();
}

QFileInfoList QMakeProjectItem::makefiles() const
{
    QDir dir( path() );
    dir.refresh();
    return pMonkeyStudio::getFiles( dir, QStringList( "*Makefile*" ), false );
}

QString QMakeProjectItem::toTitleCase( const QString& string ) const
{
    QString s = string.trimmed().toLower();
    
    if ( !s.isEmpty() ) {
        s[ 0 ] = s[ 0 ].toTitleCase();
    }
    
    return s;
}

QString QMakeProjectItem::actionTypeToString( QMakeProjectItem::ActionType type ) const
{
    const QMetaObject* mo = &QMakeProjectItem::staticMetaObject;
    const int index = mo->indexOfEnumerator( "ActionTypeFlag" );
    const QMetaEnum me = mo->enumerator( index );
    return QString::fromAscii( me.valueToKeys( type ) ).replace( "|", "_" );
}

QString QMakeProjectItem::defaultActionTypeToString( QMakeProjectItem::DefaultActionType type ) const
{
    return actionTypeToString( QMakeProjectItem::ActionType( type ) );
}

QMakeProjectItem::ActionType QMakeProjectItem::stringToActionType( const QString& string ) const
{
    const QMetaObject* mo = &QMakeProjectItem::staticMetaObject;
    const int index = mo->indexOfEnumerator( "ActionTypeFlag" );
    const QMetaEnum me = mo->enumerator( index );
    return QMakeProjectItem::ActionType( me.keysToValue( QString( string ).replace( "_", "|" ).toAscii() ) );
}

QString QMakeProjectItem::actionTypeToText( QMakeProjectItem::ActionType type ) const
{
    switch ( type ) {
        // Qt tools
        case QMakeProjectItem::QMake:
            return tr( "Qt qmake" );
        case QMakeProjectItem::LUpdate:
            return tr( "Qt lupdate" );
        case QMakeProjectItem::LRelease:
            return tr( "Qt lrelease" );
        // Generic
        case QMakeProjectItem::Build:
            return tr( "Build" );
        case QMakeProjectItem::Clean:
            return tr( "Clean" );
        case QMakeProjectItem::Distclean:
            return tr( "Distclean" );
        case QMakeProjectItem::Install:
            return tr( "Install" );
        case QMakeProjectItem::Uninstall:
            return tr( "Uninstall" );
        case QMakeProjectItem::Rebuild:
            return tr( "Rebuild" );
        case QMakeProjectItem::Execute:
            return tr( "Execute" );
        // Release
        case QMakeProjectItem::BuildRelease:
            return tr( "Build Release" );
        case QMakeProjectItem::CleanRelease:
            return tr( "Clean Release" );
        case QMakeProjectItem::DistcleanRelease:
            return tr( "Distclean Release" );
        case QMakeProjectItem::InstallRelease:
            return tr( "Install Release" );
        case QMakeProjectItem::UninstallRelease:
            return tr( "Uninstall Release" );
        case QMakeProjectItem::RebuildRelease:
            return tr( "Rebuild Release" );
        case QMakeProjectItem::ExecuteRelease:
            return tr( "Execute Release" );
        // Debug
        case QMakeProjectItem::BuildDebug:
            return tr( "Build Debug" );
        case QMakeProjectItem::CleanDebug:
            return tr( "Clean Debug" );
        case QMakeProjectItem::DistcleanDebug:
            return tr( "Distclean Debug" );
        case QMakeProjectItem::InstallDebug:
            return tr( "Install Debug" );
        case QMakeProjectItem::UninstallDebug:
            return tr( "Uninstall Debug" );
        case QMakeProjectItem::RebuildDebug:
            return tr( "Rebuild Debug" );
        case QMakeProjectItem::ExecuteDebug:
            return tr( "Execute Debug" );
        //
        /*case QMakeProjectItem::BuildAll:
        case QMakeProjectItem::CleanAll:
        case QMakeProjectItem::DistcleanAll:
        case QMakeProjectItem::InstallAll:
        case QMakeProjectItem::UninstallAll:
        case QMakeProjectItem::RebuildAll:
            break;*/
        default:
            break;
    }
    
    const QMetaObject* mo = &QMakeProjectItem::staticMetaObject;
    const int index = mo->indexOfEnumerator( "ActionTypeFlag" );
    const QMetaEnum me = mo->enumerator( index );
    return QString::fromAscii( me.valueToKeys( type ) ).replace( "|", "_" );
}

QString QMakeProjectItem::defaultActionTypeToText( QMakeProjectItem::DefaultActionType type ) const
{
    return actionTypeToText( QMakeProjectItem::ActionType( type ) );
}

void QMakeProjectItem::installCommandsV2()
{
    const QString suffix = QFileInfo( fileName() ).suffix().toLower();
    
    // Only call qmake and generate commands for true projects, and not include project files
    if ( suffix != "pro" ) {
        // install defaults commands
        XUPProjectItem::installCommands();
        return;
    }
    
    XUPProjectItem* tlProject = topLevelProject();
    const CLIToolPlugin* bp = builder();
    const QtVersionManager* manager = QMake::versionManager();
    const QtVersion version = manager->version( XUPProjectItemHelper::projectSettingsValue( tlProject, "QT_VERSION" ) );
    const bool showQtWarning = XUPProjectItemHelper::projectSettingsValue( tlProject, "SHOW_QT_VERSION_WARNING", "1" ) == "1";
    pCommand builderCommand = bp ? bp->command() : pCommand();
    
    // Qt tools commands
    if ( version.isValid() ) {
        pCommand cmd;
        
        // qmake command
        cmd = pCommand();
        cmd.setName( defaultActionTypeToString( QMakeProjectItem::QMake ) );
        cmd.setText( defaultActionTypeToText( QMakeProjectItem::QMake ) );
        cmd.setCommand( QString( "%1 %2 $cp$" ).arg( version.qmake() ).arg( version.qmakeParameters() ) );
        cmd.setWorkingDirectory( "$cpp$" );
        cmd.setProject( this );
        cmd.setSkipOnError( false );
        cmd.addParser( "QMake" );
        addCommand( "mBuilder", cmd );
        
        // lupdate command
        cmd = pCommand();
        cmd.setName( defaultActionTypeToString( QMakeProjectItem::LUpdate ) );
        cmd.setText( defaultActionTypeToText( QMakeProjectItem::LUpdate ) );
        cmd.setCommand( QString( "%1 $cp$" ).arg( version.lupdate() ) );
        cmd.setWorkingDirectory( "$cpp$" );
        cmd.setProject( this );
        cmd.setSkipOnError( false );
        cmd.addParser( "QMake" );
        addCommand( "mBuilder", cmd );
        
        // lrelease command
        cmd = pCommand();
        cmd.setName( defaultActionTypeToString( QMakeProjectItem::LRelease ) );
        cmd.setText( defaultActionTypeToText( QMakeProjectItem::LRelease ) );
        cmd.setCommand( QString( "%1 $cp$" ).arg( version.lrelease() ) );
        cmd.setWorkingDirectory( "$cpp$" );
        cmd.setProject( this );
        cmd.setSkipOnError( false );
        cmd.addParser( "QMake" );
        addCommand( "mBuilder", cmd );
    }
    else if ( showQtWarning ) {
        XUPProjectItemHelper::setProjectSettingsValue( tlProject, "SHOW_QT_VERSION_WARNING", "0" );
        tlProject->save();
        MonkeyCore::messageManager()->appendMessage( tr( "Qt was not found, some actions have been disabled.\n"
            "Edit the QMake plugin configuration to fix the problem." ) );
    }
    
    if ( !bp || !builderCommand.isValid() ) {
        return;
    }
    
    builderCommand.setProject( this );
    builderCommand.setWorkingDirectory( path() );
    builderCommand.setSkipOnError( false );
    
    QDir dir( path() );
    const pCommand makeCommand = builderCommand;
    const QFileInfoList files = makefiles();
    
    // call qmake to generate makefiles
    if ( files.isEmpty() ) {
        if ( version.isValid() ) {
            executeCommand( defaultActionTypeToString( QMakeProjectItem::QMake ) );
        }
    }
    // iterate each makefiles
    else {
        foreach ( const QFileInfo& file, files ) {
            const QList<QByteArray> rules = makefileRules( file.absoluteFilePath() );
            
            if ( rules.isEmpty() ) {
                continue;
            }
            
            const QString fileName = file.fileName();
            const QString filePath = pConsoleManager::quotedFilePath( file.absoluteFilePath() );
            QMakeProjectItem::ActionType type = QMakeProjectItem::NoFlag;
            
            if ( fileName.contains( "release", Qt::CaseInsensitive ) ) {
                type |= QMakeProjectItem::ReleaseFlag;
            }
            else if ( fileName.contains( "debug", Qt::CaseInsensitive ) ) {
                type |= QMakeProjectItem::DebugFlag;
            }
            
            addSeparator( "mBuilder" );
            
            // iterate each rules
            foreach ( QByteArray rule, rules ) {
                QMakeProjectItem::ActionType actionType = type;
                pCommand cmd = makeCommand;
                
                if ( rule == "build" ) {
                    actionType |= QMakeProjectItem::BuildFlag;
                    rule.clear();
                }
                else if ( rule == "clean" ) {
                    actionType |= QMakeProjectItem::CleanFlag;
                }
                else if ( rule == "distclean" ) {
                    actionType |= QMakeProjectItem::DistcleanFlag;
                }
                else if ( rule == "rebuild" ) {
                    actionType |= QMakeProjectItem::DistcleanFlag | QMakeProjectItem::QMakeFlag | QMakeProjectItem::BuildFlag;
                    
                    if ( version.isValid() ) {
                        cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::QMake ) ) );
                        
                        cmd.setName( actionTypeToString( actionType ) );
                        cmd.setText( actionTypeToText( actionType ) );
                        cmd.setCommand( QString::null );
                        cmd.setWorkingDirectory( QString::null );
                        
                        if ( type & QMakeProjectItem::DebugFlag ) {
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::DistcleanDebug ) ) );
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::QMake ) ) );
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::BuildDebug ) ) );
                        }
                        else if ( type & QMakeProjectItem::ReleaseFlag ) {
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::DistcleanRelease ) ) );
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::QMake ) ) );
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::BuildRelease ) ) );
                        }
                        else {
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::Distclean ) ) );
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::QMake ) ) );
                            cmd.addChildCommand( command( defaultActionTypeToString( QMakeProjectItem::Build ) ) );
                        }
                        
                        addCommand( "mBuilder", cmd );
                    }
                    
                    continue;
                }
                else if ( rule == "execute" ) {
                    actionType |= QMakeProjectItem::ExecuteFlag;
                    
                    cmd.setName( actionTypeToString( actionType ) );
                    cmd.setText( actionTypeToText( actionType ) );
                    cmd.setCommand( "$target$" );
                    cmd.setWorkingDirectory( QString::null );
                    cmd.setParsers( QStringList() );
                    cmd.setTryAllParsers( false );
                    
                    if ( actionType & QMakeProjectItem::ReleaseFlag ) {
                        cmd.setExecutableCheckingType( XUPProjectItem::ReleaseTarget );
                    }
                    else if ( actionType & QMakeProjectItem::DebugFlag ) {
                        cmd.setExecutableCheckingType( XUPProjectItem::DebugTarget );
                    }
                    else {
                        cmd.setExecutableCheckingType( XUPProjectItem::DefaultTarget );
                    }
                    
                    addCommand( "mBuilder", cmd );
                    continue;
                }
                else if ( rule == "install" ) {
                    actionType |= QMakeProjectItem::InstallFlag;
                }
                else if ( rule == "uninstall" ) {
                    actionType |= QMakeProjectItem::UninstallFlag;
                }
                /*else if ( rule == "all" ) {
                    actionType |= QMakeProjectItem::BuildFlag | QMakeProjectItem::AllFlag;
                    continue;
                }*/
                
                cmd.setName( actionTypeToString( actionType ) );
                cmd.setText( actionTypeToText( actionType ) );
                cmd.setCommand( QString( "%1 -f %2 %3" ).arg( cmd.command() ).arg( filePath ).arg( QString::fromAscii( rule ) ) );
                addCommand( "mBuilder", cmd );
            }
        }
    }
    
    // create default version
    if ( !version.isValid() && mInstalledActions.isEmpty() ) {
        pCommand cmd = makeCommand;
        
        // build
        cmd.setName( actionTypeToString( QMakeProjectItem::BuildFlag ) );
        cmd.setText( actionTypeToText( QMakeProjectItem::BuildFlag ) );
        cmd.setCommand( cmd.command() );
        addCommand( "mBuilder", cmd );
        
        // clean
        cmd.setName( actionTypeToString( QMakeProjectItem::CleanFlag ) );
        cmd.setText( actionTypeToText( QMakeProjectItem::CleanFlag ) );
        cmd.setCommand( QString( "%1 clean" ).arg( cmd.command() ) );
        addCommand( "mBuilder", cmd );
        
        // distclean
        cmd.setName( actionTypeToString( QMakeProjectItem::DistcleanFlag ) );
        cmd.setText( actionTypeToText( QMakeProjectItem::DistcleanFlag ) );
        cmd.setCommand( QString( "%1 distclean" ).arg( cmd.command() ) );
        addCommand( "mBuilder", cmd );
        
        // execute
        cmd.setName( actionTypeToString( QMakeProjectItem::ExecuteFlag ) );
        cmd.setText( actionTypeToText( QMakeProjectItem::ExecuteFlag ) );
        cmd.setCommand( "$target$" );
        cmd.setWorkingDirectory( QString::null );
        cmd.setParsers( QStringList() );
        cmd.setTryAllParsers( false );
        cmd.setExecutableCheckingType( XUPProjectItem::DefaultTarget );
        addCommand( "mBuilder", cmd );
    }
    
    // install defaults commands
    XUPProjectItem::installCommands();
}

void QMakeProjectItem::installCommands()
{
    installCommandsV2();
    return;
    
    /*// get plugins
    CLIToolPlugin* bp = builder();
    
    // config variable
    QMakeProjectItem* riProject = qobject_cast<QMakeProjectItem*>( rootIncludeProject() );
    QMakeProjectItem* tlProject = qobject_cast<QMakeProjectItem*>( topLevelProject() );
    const QStringList config = documentFilters().splitValue( riProject->cachedVariableValue( "CONFIG" ) );
    const bool haveDebug = config.contains( "debug" );
    const bool haveRelease = config.contains( "release" );
    const bool haveDebugRelease = config.contains( "debug_and_release" );
    // get qt version
    const QtVersionManager* manager = QMake::versionManager();
    const QtVersion version = manager->version( XUPProjectItemHelper::projectSettingsValue( tlProject, "QT_VERSION" ) );
    const bool showQtWarning = XUPProjectItemHelper::projectSettingsValue( tlProject, "SHOW_QT_VERSION_WARNING", "1" ) == "1";
    // temp variables
    QMap<QMakeProjectItem::DefaultActionType, pCommand> cmds; // all created commands
    pCommand cmd;
    QString s;
    
    // build command
    if ( bp ) {
        cmd = bp->command();
    }
    
    cmd.setProject( this );
    cmd.setWorkingDirectory( path() );
    cmd.setProject( this );
    cmd.setSkipOnError( false );
    cmd.setExecutableCheckingType( XUPProjectItem::NoTarget );
    
    // base command
    const pCommand cmdBuild = cmd;
    
    // evaluate some variables
    s = cachedVariableValue( "TARGET" );
    
    if ( s.isEmpty() ) {
        s = QFileInfo( fileName() ).baseName();
    }
    
    const QString target = s;
    
    s = cachedVariableValue( "DESTDIR" );
    
    if ( s.isEmpty() ) {
        s = cachedVariableValue( "DLLDESTDIR" );
    }
    
    if ( QDir( s ).isRelative() ) {
        s.prepend( "$cpp$/" );
    }
    
    if ( s.endsWith( '/' ) ) {
        s.chop( 1 );
    }
    
    const QString destdir = s;
    
    // compiler
    if ( bp && cmdBuild.isValid() ) {
        // build debug
        if ( haveDebug || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Build Debug" ) );
            if ( haveDebugRelease ) {
                cmd.setCommand( QString( "%1 debug" ).arg( cmd.command() ) );
            }
            cmds[ QMakeProjectItem::BuildDebug ] = cmd;
        }
        
        // build release
        if ( haveRelease || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Build Release" ) );
            if ( haveDebugRelease ) {
                cmd.setCommand( QString( "%1 release" ).arg( cmd.command() ) );
            }
            cmds[ QMakeProjectItem::BuildRelease ] = cmd;
        }
        
        // build all
        if ( haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Build All" ) );
            cmd.setCommand( QString( "%1 all" ).arg( cmd.command() ) );
            cmds[ QMakeProjectItem::BuildAll ] = cmd;
        }
        
        // default build
        if ( /!( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease )/ 0 == 0 ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Build" ) );
            cmds[ QMakeProjectItem::Build ] = cmd;
        }
        
        // clean debug
        if ( haveDebug || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Clean Debug" ) );
            if ( haveDebugRelease ) {
                cmd.setCommand( QString( "%1 debug-clean" ).arg( cmd.command() ) );
            }
            else {
                cmd.setCommand( QString( "%1 clean" ).arg( cmd.command() ) );
            }
            cmds[ QMakeProjectItem::CleanDebug ] = cmd;
        }
        
        // clean release
        if ( haveRelease || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Clean Release" ) );
            if ( haveDebugRelease ) {
                cmd.setCommand( QString( "%1 release-clean" ).arg( cmd.command() ) );
            }
            else {
                cmd.setCommand( QString( "%1 clean" ).arg( cmd.command() ) );
            }
            cmds[ QMakeProjectItem::CleanRelease ] = cmd;
        }
        
        // clean all
        if ( haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Clean All" ) );
            cmd.setCommand( QString( "%1 clean" ).arg( cmd.command() ) );
            cmds[ QMakeProjectItem::CleanAll ] = cmd;
        }
        
        // default clean
        if ( /!( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease )/ 0 == 0 ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Clean" ) );
            cmd.setCommand( QString( "%1 clean" ).arg( cmd.command() ) );
            cmds[ QMakeProjectItem::Clean ] = cmd;
        }
        
        // distclean debug
        if ( haveDebug || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Distclean Debug" ) );
            if ( haveDebugRelease ) {
                cmd.setCommand( QString( "%1 debug-distclean" ).arg( cmd.command() ) );
            }
            else {
                cmd.setCommand( QString( "%1 distclean" ).arg( cmd.command() ) );
            }
            cmds[ QMakeProjectItem::DistcleanDebug ] = cmd;
        }
        
        // distclean release
        if ( haveRelease || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Distclean Release" ) );
            if ( haveDebugRelease ) {
                cmd.setCommand( QString( "%1 release-distclean" ).arg( cmd.command() ) );
            }
            else {
                cmd.setCommand( QString( "%1 distclean" ).arg( cmd.command() ) );
            }
            cmds[ QMakeProjectItem::DistcleanRelease ] = cmd;
        }
        
        // distclean all
        if ( haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Distclean All" ) );
            cmd.setCommand( QString( "%1 distclean" ).arg( cmd.command() ) );
            cmds[ QMakeProjectItem::DistcleanAll ] = cmd;
        }
        
        // default distclean
        if ( /!( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease )/ 0 == 0 ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Distclean" ) );
            cmd.setCommand( QString( "%1 distclean" ).arg( cmd.command() ) );
            cmds[ QMakeProjectItem::Distclean ] = cmd;
        }
        
        if ( version.isValid() ) {
            // qmake command
            cmd = pCommand();
            cmd.setText( tr( "QMake" ) );
            cmd.setCommand( QString( "%1 %2" ).arg( version.qmake() ).arg( version.qmakeParameters() ) );
            cmd.setWorkingDirectory( "$cpp$" );
            cmd.setProject( this );
            cmd.setSkipOnError( false );
            cmd.addParser( "QMake" );
            cmds[ QMakeProjectItem::QMake ] = cmd;
            
            // rebuild debug
            if ( haveDebug || haveDebugRelease ) {
                cmd = pCommand( tr( "Rebuild Debug" ) );
                cmd.addChildCommand( cmds[ QMakeProjectItem::DistcleanDebug ] );
                cmd.addChildCommand( cmds[ QMakeProjectItem::QMake ] );
                cmd.addChildCommand( cmds[ QMakeProjectItem::BuildDebug ] );
                cmds[ QMakeProjectItem::RebuildDebug ] = cmd;
            }
            
            // rebuild release
            if ( haveRelease || haveDebugRelease ) {
                cmd = pCommand( tr( "Rebuild Release" ) );
                cmd.addChildCommand( cmds[ QMakeProjectItem::DistcleanRelease ] );
                cmd.addChildCommand( cmds[ QMakeProjectItem::QMake ] );
                cmd.addChildCommand( cmds[ QMakeProjectItem::BuildRelease ] );
                cmds[ QMakeProjectItem::RebuildRelease ] = cmd;
            }
            
            // rebuild all
            if ( haveDebugRelease ) {
                cmd = pCommand( tr( "Rebuild All" ) );
                cmd.addChildCommand( cmds[ QMakeProjectItem::DistcleanAll ] );
                cmd.addChildCommand( cmds[ QMakeProjectItem::QMake ] );
                cmd.addChildCommand( cmds[ QMakeProjectItem::BuildAll ] );
                cmds[ QMakeProjectItem::RebuildAll ] = cmd;
            }
            
            // default rebuild
            if ( /!( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease )/ 0 == 0 ) {
                cmd = pCommand( tr( "Rebuild" ) );
                cmd.addChildCommand( cmds[ QMakeProjectItem::Distclean ] );
                cmd.addChildCommand( cmds[ QMakeProjectItem::QMake ] );
                cmd.addChildCommand( cmdBuild );
                cmds[ QMakeProjectItem::Rebuild ] = cmd;
            }
        }
        
        // execute debug
        if ( haveDebug || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Execute Debug" ) );
            cmd.setCommand( "$target$" );
            cmd.setWorkingDirectory( QString::null);
            cmd.setParsers( QStringList() );
            cmd.setTryAllParsers( false );
            cmd.setExecutableCheckingType( XUPProjectItem::DebugTarget );
            cmds[ QMakeProjectItem::ExecuteDebug ] = cmd;
        }
        
        // execute release
        if ( haveRelease || haveDebugRelease ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Execute Release" ) );
            cmd.setCommand( "$target$" );
            cmd.setWorkingDirectory( QString::null );
            cmd.setParsers( QStringList() );
            cmd.setTryAllParsers( false );
            cmd.setExecutableCheckingType( XUPProjectItem::ReleaseTarget );
            cmds[ QMakeProjectItem::ExecuteRelease ] = cmd;
        }
        
        // execute
        if ( /!( haveDebug || haveDebugRelease ) && !( haveRelease || haveDebugRelease )/ 0 == 0 ) {
            cmd = cmdBuild;
            cmd.setText( tr( "Execute" ) );
            cmd.setCommand( "$target$" );
            cmd.setWorkingDirectory( QString::null );
            cmd.setParsers( QStringList() );
            cmd.setTryAllParsers( false );
            cmd.setExecutableCheckingType( XUPProjectItem::DefaultTarget );
            cmds[ QMakeProjectItem::Execute ] = cmd;
        }
        
        // add qt commands only if possible
        if ( version.isValid() ) {
            // lupdate command
            cmd = pCommand();
            cmd.setText( tr( "lupdate" ) );
            cmd.setCommand( QString( "%1 $cp$" ).arg( version.lupdate() ) );
            cmd.setWorkingDirectory( "$cpp$" );
            cmd.setProject( this );
            cmd.setSkipOnError( false );
            cmd.addParser( "QMake" );
            cmds[ QMakeProjectItem::LUpdate ] = cmd;
            
            // lrelease command
            cmd = pCommand();
            cmd.setText( tr( "lrelease" ) );
            cmd.setCommand( QString( "%1 $cp$" ).arg( version.lrelease() ) );
            cmd.setWorkingDirectory( "$cpp$" );
            cmd.setProject( this );
            cmd.setSkipOnError( false );
            cmd.addParser( "QMake" );
            cmds[ QMakeProjectItem::LRelease ] = cmd;
        }
        else if ( showQtWarning ) {
            XUPProjectItemHelper::setProjectSettingsValue( tlProject, "SHOW_QT_VERSION_WARNING", "0" );
            tlProject->save();
            MonkeyCore::messageManager()->appendMessage( tr( "Qt was not found, some actions have been disabled.\n"
                "Edit the QMake plugin configuration to fix the problem." ) );
        }
    }
    
    const QList<QMakeProjectItem::DefaultActionType> typesOrder = QList<QMakeProjectItem::DefaultActionType>()
        // Default
        << QMakeProjectItem::Build
        << QMakeProjectItem::Clean
        << QMakeProjectItem::Distclean
        << QMakeProjectItem::Rebuild
        << QMakeProjectItem::Execute
        // Debug
        << QMakeProjectItem::BuildDebug
        << QMakeProjectItem::CleanDebug
        << QMakeProjectItem::DistcleanDebug
        << QMakeProjectItem::RebuildDebug
        << QMakeProjectItem::ExecuteDebug
        // Release
        << QMakeProjectItem::BuildRelease
        << QMakeProjectItem::CleanRelease
        << QMakeProjectItem::DistcleanRelease
        << QMakeProjectItem::RebuildRelease
        << QMakeProjectItem::ExecuteRelease
        // All
        << QMakeProjectItem::BuildAll
        << QMakeProjectItem::CleanAll
        << QMakeProjectItem::DistcleanAll
        << QMakeProjectItem::RebuildAll
        // Qt Tools
        << QMakeProjectItem::QMake
        << QMakeProjectItem::LUpdate
        << QMakeProjectItem::LRelease
        ;
    
    foreach ( const QMakeProjectItem::DefaultActionType& type, typesOrder ) {
        if ( !cmds.contains( type ) ) {
            continue;
        }
        
        pCommand& cmd = cmds[ type ];
        cmd.setName( defaultActionTypeToString( type ) );
        addCommand( "mBuilder", cmd );
        
        if ( ( type & QMakeProjectItem::ExecuteFlag ) || type == QMakeProjectItem::RebuildAll ) {
            addSeparator(  "mBuilder" );
        }
    }
    
    // install defaults commands
    XUPProjectItem::installCommands();*/
}

XUPProjectItemCacheBackend* QMakeProjectItem::cacheBackend() const
{
    return &QMakeProjectItem::mCacheBackend;
}

UIXUPEditor* QMakeProjectItem::newEditDialog() const
{
    return new UIQMakeEditor( MonkeyCore::mainWindow() );
}

CLIToolPlugin* QMakeProjectItem::builder() const
{
    XUPProjectItem* tlProject = topLevelProject();
    const QtVersionManager* manager = QMake::versionManager();
    const QtVersion version = manager->version( XUPProjectItemHelper::projectSettingsValue( tlProject, "QT_VERSION" ) );
    const QString name = version.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ) ? "MSVCMake" : "GNUMake";
    return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, name );
}

void QMakeProjectItem::projectCustomActionTriggered()
{
    QAction* action = qobject_cast<QAction*>( sender() );
    
    if ( !action ) {
        return;
    }
    
    const pCommand cmd = command( action );
    QDir dir( path() );
    
    switch ( stringToActionType( cmd.name() ) ) {
        case QMakeProjectItem::QMake:
        case QMakeProjectItem::LUpdate:
        case QMakeProjectItem::LRelease: {
            break;
        }
        case QMakeProjectItem::ExecuteDebug: {
            const QString executable = targetFilePath( XUPProjectItem::DebugTarget );
            
            if ( !QFile::exists( executable ) ) {
                executeCommand( defaultActionTypeToString( QMakeProjectItem::BuildDebug ) );
            }
            
            break;
        }
        case QMakeProjectItem::ExecuteRelease: {
            const QString executable = targetFilePath( XUPProjectItem::ReleaseTarget );
            
            if ( !QFile::exists( executable ) ) {
                executeCommand( defaultActionTypeToString( QMakeProjectItem::BuildRelease ) );
            }
            
            break;
        }
        case QMakeProjectItem::Execute: {
            const QString executable = targetFilePath( XUPProjectItem::DefaultTarget );
            
            if ( !QFile::exists( executable ) ) {
                executeCommand( defaultActionTypeToString( QMakeProjectItem::Build ) );
            }
            
            break;
        }
        default: {
            const QFileInfoList files = makefiles();
            
            if ( files.isEmpty() ) {
                executeCommand( defaultActionTypeToString( QMakeProjectItem::QMake ) );
            }
            
            break;
        }
    }
    
    XUPProjectItem::projectCustomActionTriggered();
}

void QMakeProjectItem::consoleManager_commandFinished( const pCommand& cmd, int exitCode, QProcess::ExitStatus exitStatus )
{
    Q_UNUSED( exitCode );
    Q_UNUSED( exitStatus );
    
    if ( cmd.project() != this || cmd.project() != MonkeyCore::projectsManager()->currentProject() ) {
        return;
    }
    
    if ( stringToActionType( cmd.name() ) != QMakeProjectItem::QMakeFlag ) {
        mLastCommand = pCommand();
        return;
    }
    
    if ( exitStatus == QProcess::CrashExit || exitCode != 0 ) {
        if ( mLastCommand == cmd ) {
            mLastCommand = pCommand();
            return;
        }
    }
    
    uninstallCommands();
    installCommands();
}
