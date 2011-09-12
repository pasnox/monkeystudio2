#include "DebugDockWidget.h"

#include <coremanager/MonkeyCore.h>
#include <xupmanager/gui/XUPProjectManager.h>
#include <xupmanager/core/XUPFilteredProjectModel.h>
#include <qscintillamanager/pEditor.h>
#include <workspace/pChild.h>
#include <workspace/pFileManager.h>
#include <xupmanager/core/ProjectTypesIndex.h>

#include <pDockWidgetTitleBar.h>

#include <QTreeView>
#include <QPlainTextEdit>

DebugDockWidget::DebugDockWidget( QWidget* parent )
    : pDockWidget( parent )
{
    setObjectName( metaObject()->className() );
    setupUi( this );

    titleBar()->addAction( aShowXml, 0 );
    titleBar()->addAction( aShowNativeString, 1 );
    titleBar()->addAction( aGenerateFakeProject, 2 );
    titleBar()->addAction( aChildInfos, 3 );
    titleBar()->addSeparator( 4 );
    
    connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem* ) ), this, SLOT( currentProjectChanged() ) );
    connect( aShowXml, SIGNAL( triggered() ), this, SLOT( showXml() ) );
    connect( aShowNativeString, SIGNAL( triggered() ), this, SLOT( showNativeString() ) );
    connect( aGenerateFakeProject, SIGNAL( triggered() ), this, SLOT( generateFakeProject() ) );
    connect( aChildInfos, SIGNAL( triggered() ), this, SLOT( childInfos() ) );
}

DebugDockWidget::~DebugDockWidget()
{
}

void DebugDockWidget::qtMessageHandler( QtMsgType type, const char* msg )
{
    switch ( type ) {
        case QtDebugMsg:
            pteDebug->appendPlainText( QString::fromLocal8Bit( msg ) );
            printf( "*** Debug: %s\n", msg );
            break;
        case QtWarningMsg:
            pteWarning->appendPlainText( QString::fromLocal8Bit( msg ) );
            printf( "*** Warning: %s\n", msg );
            break;
        case QtCriticalMsg:
            pteCritical->appendPlainText( QString::fromLocal8Bit( msg ) );
            printf( "*** Critical: %s\n", msg );
            break;
        case QtFatalMsg:
            pteFatal->appendPlainText( QString::fromLocal8Bit( msg ) );
            printf( "*** Fatal: %s\n", msg );
            abort();
    }
    
    fflush( stdout );
}

void DebugDockWidget::currentProjectChanged()
{
    tvProjects->setModel( MonkeyCore::projectsManager()->currentProjectModel() );
}

void DebugDockWidget::showXml()
{
    const QModelIndex index = tvProjects->selectionModel()->selectedIndexes().value( 0 );
    XUPItem* item = MonkeyCore::projectsManager()->currentProjectModel()->itemFromIndex( index );
    XUPProjectItem* project = item ? item->project() : 0;
    
    if ( !project ) {
        return;
    }
    
    pWorkspace* workspace = MonkeyCore::workspace();
    static pAbstractChild* document = 0;
    bool exists = document;
    
    if ( !document ) {
        document = new pChild;
    }
    
    pEditor* editor = document->editor();
    
    editor->setText( project->toXml() );
    editor->setModified( false );
    document->setFilePath( "xml content" );
    
    if ( !exists ) {
        workspace->handleDocument( document );
        emit document->fileOpened();
        document->showMaximized();
    }
    
    workspace->setCurrentDocument( document );
}

void DebugDockWidget::showNativeString()
{
    const QModelIndex index = tvProjects->selectionModel()->selectedIndexes().value( 0 );
    XUPItem* item = MonkeyCore::projectsManager()->currentProjectModel()->itemFromIndex( index );
    XUPProjectItem* project = item ? item->project() : 0;
    
    //qWarning() << Q_FUNC_INFO << project << project->toNativeString();
    
    if ( !project ) {
        return;
    }
    
    pWorkspace* workspace = MonkeyCore::workspace();
    static pAbstractChild* document = 0;
    bool exists = document;
    
    if ( !document ) {
        document = new pChild;
    }
    
    pEditor* editor = document->editor();
    
    editor->setText( project->toNativeString() );
    editor->setModified( false );
    document->setFilePath( "native content" );
    
    if ( !exists ) {
        workspace->handleDocument( document );
        emit document->fileOpened();
        document->showMaximized();
    }
    
    workspace->setCurrentDocument( document );
}

void DebugDockWidget::createAllScopes( XUPItem* parent )
{
    createScope( parent, false, false, false ); createScope( parent, true, true, true );
    createScope( parent, true, false, true ); createScope( parent, false, true, false );
    createScope( parent, true, false, false ); createScope( parent, false, true, true );
    createScope( parent, true, true, false ); createScope( parent, false, false, true );
}

void DebugDockWidget::createScope( XUPItem* parent, bool nested1, bool nested2, bool nested3, bool nested4, bool nested5 )
{
    // an empty line
    XUPItem* item = parent->addChild( XUPItem::EmptyLine );
    item->setAttribute( "count", "1" );
    
    // a comment
    XUPItem* comment = parent->addChild( XUPItem::Comment );
    comment->setAttribute( "value", QString( "# %1 %2 %3 %4 %5" ).arg( QVariant( nested1 ).toString() ).arg( QVariant( nested2 ).toString() ).arg( QVariant( nested3 ).toString() ).arg( QVariant( nested4 ).toString() ).arg( QVariant( nested5 ).toString() ) );
    
    // an empty line
    item = parent->addChild( XUPItem::EmptyLine );
    item->setAttribute( "count", "1" );
    
    // a function
    XUPItem* function = parent->addChild( XUPItem::Function );
    function->setAttribute( "name", "isEmpty" );
    function->setAttribute( "nested", QVariant( nested1 ).toString() );
    function->setAttribute( "parameters", "$${NAME}" );
    function->setAttribute( "comment", "# $${NAME} comment" );
    
    // a scope
    XUPItem* scope = function->addChild( XUPItem::Scope );
    scope->setAttribute( "nested", QVariant( nested2 ).toString() );
    scope->setAttribute( "name", "unix" );
    scope->setAttribute( "comment", "# unix comment" );
    
    // a comment
    comment = scope->addChild( XUPItem::Comment );
    comment->setAttribute( "value", QString( "# %1 %2 %3 %4 %5" ).arg( QVariant( nested1 ).toString() ).arg( QVariant( nested2 ).toString() ).arg( QVariant( nested3 ).toString() ).arg( QVariant( nested4 ).toString() ).arg( QVariant( nested5 ).toString() ) );
    
    // a variable
    XUPItem* variable = scope->addChild( XUPItem::Variable );
    variable->setAttribute( "operator", "-=" );
    variable->setAttribute( "name", "LIBS" );
    
    // values
    XUPItem* value = variable->addChild( XUPItem::Value );
    value->setContent( "-lz" );
    value->setAttribute( "comment", "# -lz comment" );
    
    // path
    value = variable->addChild( XUPItem::Path );
    value->setContent( "/usr/include" );
    value->setAttribute( "comment", "# /usr/include comment" );
    
    // a sub scope
    XUPItem* subScope = scope->addChild( XUPItem::Scope );
    subScope->setAttribute( "nested", QVariant( nested3 ).toString() );
    subScope->setAttribute( "name", "linux-*" );
    subScope->setAttribute( "comment", "# linux-* comment" );
    
    // a variable
    variable = subScope->addChild( XUPItem::Variable );
    variable->setAttribute( "operator", "*=" );
    variable->setAttribute( "name", "DEFINES" );
    variable->setAttribute( "comment", "# DEFINES comment" );
    
    // values
    value = variable->addChild( XUPItem::Value );
    value->setContent( "HAVE_TEST_H=1" );
    value->setAttribute( "comment", "# HAVE_TEST_H=1 comment" );
    
    if ( !parent->parent() ) {
        createAllScopes( subScope );
    }
}

void DebugDockWidget::generateFakeProject()
{
    XUPProjectItem* project = MonkeyCore::projectTypesIndex()->newProjectItemByType( "QMake" );
    XUPItem* item;
    XUPItem* variable;
    XUPItem* value;
    
    project->mDocument = QDomDocument( "XUPProject" );
    QDomElement projectElement = project->mDocument.createElement( "project" );
    project->mDocument.appendChild( projectElement );
    project->mDomElement = projectElement;
    
    // set project name
    project->setAttribute( "name", "My project" );
    
    // a comment
    item = project->addChild( XUPItem::Comment );
    item->setAttribute( "value", "#This is my comment" );
    
    // an empty line
    item = project->addChild( XUPItem::EmptyLine );
    item->setAttribute( "count", "1" );
    
    // a variable
    variable = project->addChild( XUPItem::Variable );
    variable->setAttribute( "operator", "*=" );
    variable->setAttribute( "name", "LIBS" );
    
    // values
    value = variable->addChild( XUPItem::Value );
    value->setContent( "-L/usr/include" );
    value->setAttribute( "comment", "# -L/usr/include comment" );
    
    value = variable->addChild( XUPItem::Value );
    value->setContent( "-lpng" );
    value->setAttribute( "comment", "# -lpng comment" );
    
    value = variable->addChild( XUPItem::Value );
    value->setContent( "-lz" );
    value->setAttribute( "comment", "# -lz comment" );
    
    createAllScopes( project );
    
    // finish project
    project->mCodec = "UTF-8";
    project->mFileName = QString( "Fake project %1" ).arg( qrand() % INT_MAX );
    
    MonkeyCore::projectsManager()->openProject( project );
}

void DebugDockWidget::childInfos()
{
    const QModelIndex index = tvProjects->selectionModel()->selectedIndexes().value( 0 );
    XUPItem* item = MonkeyCore::projectsManager()->currentProjectModel()->itemFromIndex( index );
    
    if ( !item ) {
        return;
    }
    
    XUPItem* variable = item->addChild( XUPItem::Variable );
    variable->setAttribute( "operator", "*=" );
    variable->setAttribute( "name", "LIBS" );
    
    variable->parent()->removeChild( variable );
}
