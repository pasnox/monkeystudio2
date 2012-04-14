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
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/XUPProjectModel.h"
#include "coremanager/MonkeyCore.h"
#include "pMenuBar.h"
#include "pluginsmanager/PluginsManager.h"
#include "pMonkeyStudio.h"
#include "maininterface/UIMain.h"
#include "pluginsmanager/XUPPlugin.h"
#include "xupmanager/core/XUPProjectItemHelper.h"
#include "xupmanager/core/XUPDynamicFolderItem.h"
#include "xupmanager/core/XUPProjectItemCache.h"
#include "workspace/pFileManager.h"
#include "xupmanager/gui/UIXUPEditor.h"
#include "consolemanager/pCommand.h"

#include <pQueuedMessageToolBar.h>

#include <QTextCodec>
#include <QDir>
#include <QRegExp>
#include <QProcess>
#include <QFileDialog>
#include <QLibrary>
#include <QMessageBox>

#include <pVersion.h>

#include <QDebug>

XUPProjectItemCache XUPProjectItem::mProjectsCache;
XUPProjectItemCacheBackend XUPProjectItem::mProjectsCacheBackend = XUPProjectItemCacheBackend( &XUPProjectItem::mProjectsCache );

XUPProjectItem::XUPProjectItem()
    : XUPItem( QDomElement(), 0 )
{
}

XUPProjectItem::~XUPProjectItem()
{
    XUPProjectItem::cache()->clear( this );
}

void XUPProjectItem::showError( const QString& error )
{
    if ( !error.trimmed().isEmpty() ) {
        MonkeyCore::messageManager()->appendMessage( error );
    }
}

QString XUPProjectItem::fileName() const
{
    return mFileName;
}

QString XUPProjectItem::path() const
{
    return QFileInfo( fileName() ).path();
}

QString XUPProjectItem::filePath( const QString& _fn ) const
{
    const QString fn = unquotedValue( _fn );
    
    if ( fn.isEmpty() ) {
        return QString::null;
    }
    
    return QDir::cleanPath(
        QFileInfo( fn ).isRelative()
            ? QString( "%1/%2" ).arg( path() ).arg( fn )
            : fn
    );
}

QString XUPProjectItem::relativeFilePath( const QString& _fn ) const
{
    const QString fn = unquotedValue( _fn );
    
    if ( fn.isEmpty() ) {
        return QString::null;
    }
    
    QDir dir( path() );
    
    return QDir::cleanPath(
        dir.relativeFilePath( fn )
    );
}

QStringList XUPProjectItem::sourceFiles() const
{
    QSet<QString> entries;
    
    foreach ( const QString& name, documentFilters().fileVariables() ) {
        XUPItemList variables = getVariables( this, name );
        
        foreach ( XUPItem* variable, variables ) {
            foreach( XUPItem* item, variable->childrenList() ) {
                if ( item->type() == XUPItem::File ) {
                    entries << filePath( item->cacheValue( "content" ) );
                }
            }
        }
    }
    
    // get dynamic files
    const XUPDynamicFolderSettings settings = XUPProjectItemHelper::projectDynamicFolderSettings( const_cast<XUPProjectItem*>( this ) );
    
    if ( settings.Active && !settings.AbsolutePath.isEmpty() && QFile::exists( settings.AbsolutePath ) ) {
        QDir dir( settings.AbsolutePath );
        QFileInfoList files = pMonkeyStudio::getFiles( dir, settings.FilesPatterns, true );
        
        foreach ( const QFileInfo& fi, files ) {
            entries << fi.absoluteFilePath();
        }
    }
    
    return entries.toList();
}

QStringList XUPProjectItem::topLevelProjectSourceFiles() const
{
    QStringList files;

    foreach ( XUPProjectItem* project, childrenProjects( true ) ) {
        files << project->sourceFiles();
    }

    // remove duplicates
    return files.toSet().toList();
}

XUPPlugin* XUPProjectItem::driver() const
{
    return MonkeyCore::pluginsManager()->plugin<XUPPlugin*>( PluginsManager::stAll, projectType() );
}

QStringList XUPProjectItem::autoActivatePlugins() const
{
    return driver()->infos().dependencies;
}

void XUPProjectItem::addFiles( const QStringList& files, XUPItem* _scope )
{
    const DocumentFilterMap& filters = documentFilters();
    const QString op = defaultOperator();
    XUPItem* scope = _scope ? _scope : this;
    XUPProjectItem* project = scope ? scope->project() : this;
    QStringList notImported;
    
    foreach ( const QString& file, files ) {
        const QString variableName = filters.fileNameVariable( unquotedValue( file ) );
        QString filePath = project->filePath( file );
        
        if ( filePath.isEmpty() ) {
            continue;
        }
        
        if ( variableName.isEmpty() || pMonkeyStudio::isSameFile( filePath, project->fileName() ) ) {
            notImported << file;
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
            
            filePath = quotedValue( project->relativeFilePath( file ) );
            XUPItem* valueItem = variableItem->addChild( XUPItem::File );
            valueItem->setContent( filePath );
        }
    }
    
    if ( !notImported.isEmpty() ) {
        showError( tr( "Don't know how to add files:\n%1" ).arg( notImported.join( "\n" ) ) );
    }
}

void XUPProjectItem::removeValue( XUPItem* item, bool deleteFiles )
{
    switch ( item->type() ) {
        case XUPItem::Variable: {
            if ( deleteFiles ) {
                item->setCacheValue( "markDeleted", "1" );
                
                foreach ( XUPItem* value, item->childrenList() ) {
                    removeValue( value, deleteFiles );
                }
            }
            
            break;
        }
        case XUPItem::File: {
            if ( deleteFiles ) {
                const QString content = item->cacheValue( "content" );
                const QString filePath = this->filePath( content );
                QFile file( filePath );
                
                if ( file.exists() ) {
                    MonkeyCore::fileManager()->closeFile( filePath );
                    
                    if ( !file.remove() ) {
                        MonkeyCore::messageManager()->appendMessage( tr( "Can't delete file '%1': %2" ).arg( filePath ).arg( file.errorString() ) );
                    }
                }
            }
            
            break;
        }
        case XUPItem::Path:
        case XUPItem::Value: {
            break;
        }
        default:
            qWarning() << Q_FUNC_INFO << "Ask to remove bad item" << item->type() << item->displayText();
            Q_ASSERT( 0 );
            break;
    }
    
    XUPItem* parentItem = item->parent();
    parentItem->removeChild( item );
    
    if ( parentItem->type() == XUPItem::Variable && parentItem->cacheValue( "markDeleted" ) != "1" ) {
        if ( !parentItem->hasChildren() ) {
            parentItem->parent()->removeChild( parentItem );
        }
    }
}

QString XUPProjectItem::quoteString() const
{
    return QString::null;
}

QString XUPProjectItem::defaultOperator() const
{
    return QString::null;
}

QFileInfoList XUPProjectItem::findFile( const QString& partialFilePath ) const
{
    const QString searchFileName = QFileInfo( partialFilePath ).fileName();
    QFileInfoList files;
    
    foreach( XUPProjectItem* project, childrenProjects( true ) ) {
        foreach( const QString& file, project->sourceFiles() ) {
            const QFileInfo fileInfo( project->filePath( file ) );
            
            if ( fileInfo.exists() && fileInfo.fileName() == searchFileName ) {
                files << fileInfo;
            }
        }
    }
    
    return files;
}

XUPProjectItem* XUPProjectItem::parentProject() const
{
    if ( mParentItem )
        return mParentItem->project();
    return const_cast<XUPProjectItem*>( this );
}

XUPProjectItem* XUPProjectItem::topLevelProject() const
{
    if ( mParentItem )
        return mParentItem->project()->topLevelProject();
    return const_cast<XUPProjectItem*>( this );
}

XUPProjectItem* XUPProjectItem::rootIncludeProject() const
{
    if ( mParentItem && mParentItem->type() == XUPItem::Function && mParentItem->attribute( "name" ).toLower() == "include" )
        return mParentItem->project()->rootIncludeProject();
    return const_cast<XUPProjectItem*>( this );
}

XUPProjectItemList XUPProjectItem::childrenProjects( bool recursive ) const
{
    XUPProjectModel* model = this->model();
    XUPProjectItem* thisProject = project();
    QMap<QString, XUPProjectItem*> projects;

    if ( model )
    {
        const QModelIndexList projectIndexes = model->match( thisProject->index(), XUPProjectModel::TypeRole, XUPItem::Project, -1, Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive );

        foreach ( const QModelIndex& index, projectIndexes )
        {
            XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );
            XUPProjectItem* project = item->project();

            if ( recursive || project->parentProject() == thisProject )
            {
                projects[ project->fileName() ] = project;
            }
        }
    }

    return projects.values();
}

XUPItemList XUPProjectItem::getVariables( const XUPItem* root, const QString& variableName, bool recursive, const XUPItem* caller ) const
{
    XUPItemList variables;

    foreach(XUPItem* item, root->childrenList())
    {
        if ( XUPItem::Variable ==  item->type() && variableName == item->attribute( "name" ) )
        {
            variables << item;
        }
        else if ( XUPItem::Project == item->type() )
        {
            if ( recursive )
            {
                XUPItem* pItem = item->parent();
                if ( pItem->type() == XUPItem::Function)
                {
                    variables << getVariables( item, variableName );
                }
            }
        }
        else if ( recursive )
        {
            variables << getVariables( item, variableName );
        }
        
        if ( item == caller ) {
            break;
        }
    }

    return variables;
}

XUPItem* XUPProjectItem::getVariable( const XUPItem* root, const QString& variableName ) const
{
    foreach( XUPItem* item, root->childrenList() )
    {
        if ( XUPItem::Variable == item->type() && item->attribute( "name" ) == variableName )
        {
            return item;
        }
    }

    return 0;
}

QString XUPProjectItem::toXml() const
{
    return XUPProjectItemHelper::stripDynamicFolderFiles( mDocument ).toString( 4 );
    //return mDocument.toString( 4 );
}

QString XUPProjectItem::toNativeString() const
{
    return toXml();
}

bool XUPProjectItem::open( const QString& fileName, const QString& codec )
{
    // get QFile
    QFile file( fileName );

    // check existence
    if ( !file.exists() ) {
        showError( tr( "Can't open project '%1': file not exists" ).arg( fileName ) );
        return false;
    }

    // try open it for reading
    if ( !file.open( QIODevice::ReadOnly ) ) {
        showError( tr( "can't open '%1' for reading" ).arg( fileName ) );
        return false;
    }

    // decode content
    QTextCodec* c = QTextCodec::codecForName( codec.toUtf8() );
    QString buffer = c->toUnicode( file.readAll() );

    // parse content
    QString errorMsg;
    int errorLine;
    int errorColumn;
    
    if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) )
    {
        showError(
            tr( "Xml error in '%1':\n%2 on line: %3, column: %4" )
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
        showError( tr( "Invalid project '%1': no project node" ).arg( fileName ) );
        return false;
    }
    
    // check xup version
    const QString docVersion = mDomElement.attribute( "version" );
    
    if ( pVersion( docVersion ) < XUP_VERSION ) {
        showError(
            tr( "The document format is too old, current version is '%1', your document is '%2' for '%3'" )
                .arg( XUP_VERSION )
                .arg( docVersion )
                .arg( fileName )
        );
        return false;
    }

    // all is ok
    mCodec = codec;
    mFileName = fileName;
    file.close();
    
    const XUPDynamicFolderSettings dynamicFolderSettings = XUPProjectItemHelper::projectDynamicFolderSettings( this );
    
    if ( dynamicFolderSettings.Active && QFile::exists( dynamicFolderSettings.AbsolutePath ) ) {
        XUPDynamicFolderItem* dynamicFolderItem = XUPProjectItemHelper::projectDynamicFolderItem( this, true );
        dynamicFolderItem->setRootPath( dynamicFolderSettings.AbsolutePath, dynamicFolderSettings.FilesPatterns );
    }

    emitDataChanged();
    return true;
}

bool XUPProjectItem::save()
{
    // try open file for writing
    QFile file( mFileName );

    if ( !file.open( QIODevice::WriteOnly ) ) {
        return false;
    }

    // erase file content
    file.resize( 0 );

    // set xup version
    setAttribute( "version", XUP_VERSION );

    // encode content
    QTextCodec* codecObj = QTextCodec::codecForName( codec().toAscii ()  );
    QByteArray content = codecObj->fromUnicode( toNativeString() );

    // write content
    bool result = file.write( content ) != -1;
    file.close();

    // set error message if needed
    if ( !result ) {
        showError( tr( "Can't write content in project '%1'" ).arg( fileName() ) );
    }
    
    XUPProjectItem::cache()->build( rootIncludeProject() );

    return result;
}

QString XUPProjectItem::targetFilePath( XUPProjectItem::TargetType type )
{
    Q_UNUSED( type );
    return XUPProjectItemHelper::projectSettingsValue( this, "MAIN_FILE" );
}

QAction* XUPProjectItem::addSeparator( const QString& mnu )
{
    static int i = 0;
    const QString name = QString( "%1_generated_separator" ).arg( i++ );
    QAction* action = MonkeyCore::menuBar()->action( QString( "%1/%2" ).arg( mnu ).arg( name ) );
    action->setSeparator( true );
    Q_ASSERT( !mInstalledActions.keys().contains( name ) );
    mInstalledActions[ name ] = action;
    return action;
}

void XUPProjectItem::addCommands( const QString& mnu, const QList<pCommand>& cmds )
{
    foreach ( const pCommand& cmd, cmds ) {
        addCommand( mnu, cmd );
    }
}

pCommand XUPProjectItem::command( const QString& name ) const
{
    return mCommands.value( name );
}

pCommand XUPProjectItem::command( QAction* action ) const
{
    return action->data().value<pCommand>();
}

void XUPProjectItem::addCommand( const QString& mnu, const pCommand& cmd )
{
    if ( ( mCommands.contains( cmd.name() ) && !cmd.name().isNull() ) || !cmd.isValid() ) {
        return;
    }
    
    static int i = 0;
    const QString& name = cmd.name().isNull() ? cmd.text() : cmd.name();
    QAction* action = MonkeyCore::menuBar()->action( QString( "%1/%2" ).arg( mnu ).arg( name ), cmd.text() );
    pCommand c = cmd;
    
    if ( c.name().isNull() ) {
        c.setName( QString( "%1_generated_name" ).arg( i++ ) );
    }
    
    action->setStatusTip( c.text() );
    action->setData( QVariant::fromValue( c ) );
    
    mCommands[ c.name() ] = c;
    mInstalledActions[ c.name() ] = action;
    
    connect( action, SIGNAL( triggered() ), this, SLOT( projectCustomActionTriggered() ) );
    
    // update menu visibility
    MonkeyCore::mainWindow()->menu_CustomAction_aboutToShow();
}

void XUPProjectItem::installCommands()
{
    const MenuCommandListMap commands = XUPProjectItemHelper::projectCommands( this );
    
    foreach ( const QString& menu, commands.keys() ) {
        if ( MonkeyCore::menuBar()->menu( menu )->actions().count() > 0 ) {
            addSeparator( menu );
        }
        
        addCommands( menu, commands[ menu ] );
    }
}

void XUPProjectItem::uninstallCommands()
{
    mCommands.clear();
    qDeleteAll( mInstalledActions );
    mInstalledActions.clear();

    // update menu visibility
    MonkeyCore::mainWindow()->menu_CustomAction_aboutToShow();
}

QString XUPProjectItem::codec() const
{
    if (! mCodec.isNull())
        return mCodec;
    else
        return pMonkeyStudio::defaultCodec();
}

bool XUPProjectItem::edit()
{
    UIXUPEditor* dlg = newEditDialog();
    
    if ( !dlg ) {
        return false;
    }
    
    dlg->setAttribute( Qt::WA_DeleteOnClose );
    dlg->setupProject( this );
    
    return dlg->exec() == QDialog::Accepted;
}

bool XUPProjectItem::editProjectFiles()
{
    UIXUPEditor* dlg = newEditDialog();
    
    if ( !dlg ) {
        return false;
    }
    
    dlg->setAttribute( Qt::WA_DeleteOnClose );
    dlg->setupProject( this );
    dlg->showProjectFilesPage();
    
    return dlg->exec() == QDialog::Accepted;
}

const DocumentFilterMap& XUPProjectItem::documentFilters() const
{
    return MonkeyCore::projectTypesIndex()->documentFilters( projectType() );
}

XUPProjectItemCacheBackend* XUPProjectItem::cacheBackend() const
{
    return &XUPProjectItem::mProjectsCacheBackend;
}

QStringList XUPProjectItem::cachedVariableValues( const QString& variableName ) const
{
    return XUPProjectItem::cache()->values( rootIncludeProject(), variableName );
}

QString XUPProjectItem::cachedVariableValue( const QString& variableName ) const
{
    return cachedVariableValues( variableName ).join( " " );
}

XUPProjectItemCache* XUPProjectItem::cache()
{
    return &XUPProjectItem::mProjectsCache;
}

QString XUPProjectItem::quotedValue( const QString& value ) const
{
    const QString quote = quoteString();
    
    if ( !quote.isEmpty() && value.contains( " " ) && !value.startsWith( quote ) && !value.endsWith( quote ) ) {
        return QString( "%1%2%1" ).arg( quote ).arg( value );
    }
    
    return value;
}

QString XUPProjectItem::unquotedValue( const QString& _value ) const
{
    if ( _value.isEmpty() ) {
        return QString::null;
    }
    
    const QString quote = quoteString();
    QString value = _value;
    
    if ( !quote.isEmpty() && value.startsWith( quote ) && value.endsWith( quote ) ) {
        value.chop( 1 );
        value.remove( 0, 1 );
    }
    
    return value;
}

UIXUPEditor* XUPProjectItem::newEditDialog() const
{
    return new UIXUPEditor( MonkeyCore::mainWindow() );
}

void XUPProjectItem::executeCommand( const QString& name )
{
    QAction* action = mInstalledActions.value( name );
    
    if ( action ) {
        action->trigger();
    }
}

void XUPProjectItem::projectCustomActionTriggered()
{
    QAction* action = qobject_cast<QAction*>( sender() );
    
    // save opened files
    if ( pMonkeyStudio::saveFilesOnCustomAction() ) {
        MonkeyCore::workspace()->fileSaveAll_triggered();
    }
    
    const pCommand acmd = command( action );
    
    if ( !acmd.isValid() ) {
        return;
    }
    
    pCommand::List cmds = acmd.childCommands();
    
    if ( acmd.childCommands().isEmpty() ) {
        cmds << acmd;
    }
    
    foreach ( pCommand cmd, cmds ) {
        cmd = MonkeyCore::consoleManager()->processCommand( cmd );
        MonkeyCore::consoleManager()->addCommand( cmd );
    }
}
