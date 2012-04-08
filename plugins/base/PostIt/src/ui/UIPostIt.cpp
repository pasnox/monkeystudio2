#include "UIPostIt.h"

#include "notesManager.h"
#include <MonkeyCore.h>
#include <Settings.h>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>

#include <QtDebug>

//constructor
UIPostIt::UIPostIt( QWidget* w )
    : QMainWindow( w, Qt::Tool )
{
    // init widget
    setupUi( this );

    //init mNotesManager
    mNotesManager = new notesManager( QString(QFileInfo( MonkeyCore::settings()->fileName() ).absolutePath() + "/postit.xml") );
    mNotesManager->readDocument();

    //load note list
    loadNotes();
    
    if ( lstNotes->currentRow() != -1 ) {
        lstNotes->setCurrentRow( 0 );
        txtContent->setFocus();
    }
}

//destructor
UIPostIt::~UIPostIt()
{
    delete mNotesManager;
}

//interface : add note
void UIPostIt::on_tbAdd_clicked()
{
    //create new note
    bool inputResult;
    QString title = QInputDialog::getText(this, "New note", "Enter new title note :", QLineEdit::Normal, "", &inputResult );
    
    if ( inputResult ) {
        if ( !title.isEmpty() )
            mNotesManager->addElement( title, QString("") );
        else
            mNotesManager->addElement( QString("New note..."), QString(" ") );
            
        loadNotes();
        lstNotes->setCurrentRow( lstNotes->count() - 1 );
        txtContent->setFocus();
        this->activateWindow();
    }
}

//load note from QDomDocument
void UIPostIt::loadNotes()
{
    lstNotes->clear();
    
    QStringList list = mNotesManager->getElements();

    for ( uint i = 0; i < list.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem( lstNotes );
        item->setText( list.at( i ) );
    }
}

//edit title on doubleclicked
void UIPostIt::on_tbEdit_clicked ()
{
    int row = lstNotes->currentRow();
    if ( row != -1 ) {
        bool inputResult;
        QString title = QInputDialog::getText(this, "Edit title", "Enter new title note :", QLineEdit::Normal, mNotesManager->getTitleElement( lstNotes->currentRow() ), &inputResult );
        
        if ( inputResult ) {
            mNotesManager->setTitleElement( lstNotes->currentRow(), title );
            loadNotes();
            lstNotes->setCurrentRow( row );
            txtContent->setFocus();
            this->activateWindow();
        }
    }
}

//delete selected note
void UIPostIt::on_tbDelete_clicked()
{
    if ( lstNotes->currentRow() != -1 ) {
        uint i = lstNotes->currentRow();
        mNotesManager->removeElement( i );
        txtContent->clear();
        loadNotes();
    }
}

//when PostIt closing
void UIPostIt::closeEvent( QCloseEvent *event )
{
    mNotesManager->writeDocument();
}

//remove all notes
void UIPostIt::on_tbClear_clicked()
{
    if ( mNotesManager->getNotesCount() > 0 ) {
        QMessageBox m;
        m.setText( "Delete all notes ?" );
        m.setStandardButtons( QMessageBox::Cancel | QMessageBox::Ok );
        m.setDefaultButton( QMessageBox::Ok );
        int res = m.exec();

        if ( res == QMessageBox::Ok ) {
            mNotesManager->removeAllElements();
            txtContent->clear();
            loadNotes();
        }
    }
}

//save txtcontent
void UIPostIt::on_lstNotes_currentItemChanged( QListWidgetItem * current, QListWidgetItem * previous )
{
    if ( lstNotes->row( previous ) != -1 )
        mNotesManager->setElement( lstNotes->row( previous ), txtContent->toPlainText() );
        
    if ( lstNotes->row( current ) != -1 ) {
        txtContent->setPlainText( mNotesManager->getElement( lstNotes->row( current ) ).text() );
        txtContent->setFocus();
        txtContent->moveCursor( QTextCursor::End );
    }
}
