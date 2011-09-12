#include "xupmanager/gui/UIXUPEditor.h"
#include "ui_UIXUPEditor.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "XUPPageEditor.h"
#include "MainEditor.h"
#include "FilesEditor.h"
#include "VariablesEditor.h"
#include "CommandsEditor.h"

UIXUPEditor::UIXUPEditor( QWidget* parent )
    : QDialog( parent )
{
    ui = new Ui_UIXUPEditor;
    mProject = 0;
    
    ui->setupUi( this );
    ui->lwPages->setAttribute( Qt::WA_MacShowFocusRect, false );
}

UIXUPEditor::~UIXUPEditor()
{
    delete ui;
}

int UIXUPEditor::insertPage( int _index, XUPPageEditor* page )
{
    const int count = mPages.count();
    const int index = _index > 0 && _index < count ? _index : count;
    
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint( QSize( ui->lwPages->width() -( ui->lwPages->lineWidth() *2 ) -( ui->lwPages->spacing() *2 ), 50 ) );
    item->setIcon( page->windowIcon() );
    item->setText( page->windowTitle() );
    
    foreach ( QWidget* w, page->findChildren<QWidget*>() ) {
        w->setAttribute( Qt::WA_MacSmallSize );
        w->setAttribute( Qt::WA_MacShowFocusRect, false );
    }
    
    mPages.insert( index, page );
    ui->lwPages->insertItem( index, item );
    ui->swPages->insertWidget( index, page );
    
    return index;
}

int UIXUPEditor::addPage( XUPPageEditor* page )
{
    return insertPage( -1, page );
}

void UIXUPEditor::addPages( const QList<XUPPageEditor*>& pages )
{
    foreach ( XUPPageEditor* page, pages ) {
        insertPage( -1, page );
    }
}

XUPPageEditor* UIXUPEditor::page( int index ) const
{
    return mPages.value( index );
}

void UIXUPEditor::removePage( int index )
{
    if ( index < 0 || index >= mPages.count() ) {
        return;
    }
    
    mPages.removeAt( index );
    delete ui->lwPages->item( index );
    delete ui->swPages->widget( index );
}

QList<XUPPageEditor*> UIXUPEditor::pages() const
{
    return mPages;
}

void UIXUPEditor::setCurrentPage( int index )
{
    ui->lwPages->setCurrentRow( index );
}

int UIXUPEditor::currentPage() const
{
    return ui->lwPages->currentRow();
}

void UIXUPEditor::setupProject( XUPProjectItem* project )
{
    const QList<XUPPageEditor*> pages = QList<XUPPageEditor*>()
        << new MainEditor
        << new FilesEditor
        << new VariablesEditor
        << new CommandsEditor
        ;
    
    addPages( pages );
    setup( project );
}

bool UIXUPEditor::showProjectFilesPage()
{
    for ( int i = 0; i < mPages.count(); i++ ) {
        const XUPPageEditor* page = mPages.at( i );
        
        if ( page->inherits( "FilesEditor" ) ) {
            setCurrentPage( i );
            return true;
        }
    }
    
    return false;
}

void UIXUPEditor::setup( XUPProjectItem* project )
{
    mProject = project;
    
    setWindowIcon( project->displayIcon() );
    setWindowTitle( tr( "Project Editor - %2" ).arg( project->displayText() ) );
    
    foreach ( XUPPageEditor* editor, mPages ) {
        editor->setup( project );
    }
    
    setCurrentPage( 0 );
}

void UIXUPEditor::finalize()
{
    foreach ( XUPPageEditor* editor, mPages ) {
        editor->finalize();
    }
}

void UIXUPEditor::on_lwPages_currentRowChanged( int row )
{
    QListWidgetItem* item = ui->lwPages->item( row );
    ui->lTitle->setText( item ? item->text() : QString::null );
    ui->lIcon->setPixmap( item ? item->icon().pixmap( QSize( 18, 18 ) ) : QPixmap() );
    ui->swPages->setCurrentIndex( row );
}

void UIXUPEditor::accept()
{
    finalize();
    QDialog::accept();
}
