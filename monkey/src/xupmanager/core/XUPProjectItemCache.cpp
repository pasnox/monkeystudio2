#include "xupmanager/core/XUPProjectItemCache.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/XUPProjectItemHelper.h"

#include <QDebug>

uint qHash( const XUPProjectItemCache::ProjectPointer& pointer )
{
    return qHash( pointer ? pointer->fileName() : QString::null );
}

// XUPProjectItemCacheBackend

XUPProjectItemCacheBackend::XUPProjectItemCacheBackend( XUPProjectItemCache* cache/*, const QString& separator*/ )
{
    mCache = cache;
}

XUPProjectItemCacheBackend::~XUPProjectItemCacheBackend()
{
}

QStringList XUPProjectItemCacheBackend::guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const
{
    if ( !mCache ) {
        return QStringList();
    }
    
    XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
    
    const QString name = QString( variable ).replace( '$', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
    
    // environment variable
    if ( variable.startsWith( "$(" ) ) {
        if ( name == "PWD" ) {
            const QString path = project->path();
            return path.isEmpty() ? QStringList() : QStringList( path );
        }
        else {
            const QString value = QString::fromLocal8Bit( qgetenv( name.toLocal8Bit().constData() ) );
            return value.isEmpty() ? QStringList() : QStringList( value );
        }
    }
    // cache variable
    else if ( variable.startsWith( "$$[" ) ) {
        if ( name == "PWD" ) {
            const QString path = variableProject->path();
            return path.isEmpty() ? QStringList() : QStringList( path );
        }
        else {
            return cachedData.value( project ).value( name );
        }
    }
    
    return QStringList();
}

QStringList XUPProjectItemCacheBackend::guessedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QStringList& content ) const
{
    const QRegExp rx( "(?:[^$]|^)(\\$[(\\[][\\w._ ]+[)\\]])" );
    QString loopContent = content.join( " " );
    QStringList guessed = content;
    int pos = 0;

    while ( ( pos = rx.indexIn( loopContent, pos ) ) != -1 ) {
        const QString guessedVariableContent = guessedVariable( project, valueProject, rx.cap( 1 ) ).join( " " );
        loopContent.replace( rx.cap( 1 ), guessedVariableContent );
        guessed.replaceInStrings( rx.cap( 1 ), guessedVariableContent );
        pos += rx.matchedLength();
    }

#ifndef QT_NO_DEBUG
    if ( guessed.join( " " ).contains( "$" ) ) {
        qWarning() << content;
        qWarning() << guessed;
    }
#endif

    return guessed;
}

void XUPProjectItemCacheBackend::updateVariable( XUPProjectItem* project, const QString& variable, const QStringList& values, const QString& op )
{
    Q_UNUSED( op );
    
    if ( !mCache ) {
        return;
    }
    
    XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
    cachedData[ project ][ variable ] = values;
}

void XUPProjectItemCacheBackend::recursiveScan( XUPProjectItem* project, XUPItem* _root )
{
    XUPItem* root = _root ? _root : project;
    QStringList values;
    
    //qWarning() << project->fileName() << root->project()->rootIncludeProject()->fileName() << root->displayText();
    Q_ASSERT( project == root->project()->rootIncludeProject() );
    
    foreach ( XUPItem* child, root->childrenList() ) {
        switch ( child->type() ) {
            case XUPItem::Value:
            case XUPItem::File:
            case XUPItem::Path: {
                const QString content = guessedContent( project, child->project(), QStringList( child->content() ) ).join( " " );
                child->setCacheValue( "content", content );
                values << content;
                break;
            }
            case XUPItem::DynamicFolder:
                continue;
            case XUPItem::Project: {
                if ( project != child->project()->rootIncludeProject() ) {
                    continue;
                }
                break;
            }
            case XUPItem::Function: {
                const QString parameters = guessedContent( project, child->project(), QStringList( child->attribute( "parameters" ) ) ).join( " " );
                child->setCacheValue( "parameters", parameters );
                break;
            }
            default:
                break;
        }
        
        recursiveScan( project, child );
    }
    
    if ( root->type() == XUPItem::Variable ) {
        updateVariable( project, root->attribute( "name" ), values, root->attribute( "operator" ) );
    }
    
    if ( root->type() != XUPItem::DynamicFolder ) {
        cacheRecursiveScanHook( project, root );
    }
}

bool XUPProjectItemCacheBackend::cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item )
{
    Q_UNUSED( project );
    Q_UNUSED( item );
    return false;
}

// XUPProjectItemCache

XUPProjectItemCache::ProjectCache& XUPProjectItemCache::cachedData() const
{
    return mCache;
}

void XUPProjectItemCache::build( XUPProjectItem* project )
{
//qWarning() << "*** BUILD" << project->fileName() << ( root ? root->displayText() : "" );
    clear( project );
    update( project, project );
}

void XUPProjectItemCache::update( XUPProjectItem* project, XUPItem* root )
{
//qWarning() << "*** UPDATE" << project->fileName() << ( root ? root->displayText() : "" );
    if ( project->cacheBackend() ) {
        project->cacheBackend()->recursiveScan( project, root );
    }
}

void XUPProjectItemCache::clear( XUPProjectItem* project )
{
    mCache.remove( project );
}

#ifndef QT_NO_DEBUG
void XUPProjectItemCache::debug( bool full ) const
{
    int indent = 0;
    
    foreach ( XUPProjectItem* project, mCache.keys() ) {
        qWarning() << QString( "%1Project cache - %2" ).arg( QString( indent, '\t' ) ).arg( project->fileName() ).toLocal8Bit();
        
        if ( full ) {
            indent++;
            
            foreach ( const QString& variable, mCache[ project ].keys() ) {
                qWarning() << QString( "%1%2" ).arg( QString( indent, '\t' ) ).arg( variable ).toLocal8Bit();
                
                indent++;
                
                foreach ( const QString& value, mCache[ project ][ variable ] ) {
                    qWarning() << QString( "%1%2" ).arg( QString( indent, '\t' ) ).arg( value ).toLocal8Bit();
                }
                
                indent--;
            }
            
            indent--;
        }
    }
}
#endif

QStringList XUPProjectItemCache::values( XUPProjectItem* project, const QString& variable ) const
{
    const XUPProjectItemCacheBackend* backend = project->cacheBackend();
    
    if ( !backend ) {
        return QStringList();
    }
    
    if ( !mCache.value( project ).contains( variable ) ) {
        const_cast<XUPProjectItemCache*>( this )->build( project );
    }
    
    return mCache.value( project ).value( variable );
}
