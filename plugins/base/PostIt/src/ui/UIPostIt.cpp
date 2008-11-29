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

    txtContent->installEventFilter( this );

    //init mNotesManager
    mNotesManager = new notesManager( QFileInfo( MonkeyCore::settings()->fileName() ).absolutePath() + "/postit.xml" );
    mNotesManager->readDocument();

    //load note list
    loadNotes();

    mTextChanged = false;
}

//destructor
UIPostIt::~UIPostIt()
{
    delete mNotesManager;
}

//interface : add note
void UIPostIt::on_tbAdd_clicked()
{
    bool inputResult;
    QString title = QInputDialog::getText(this, "New note", "Enter new title note :", QLineEdit::Normal, "", &inputResult );
    
    if ( inputResult ) {
        if ( !title.isEmpty() )
            mNotesManager->addElement( title, "" );
        else
            mNotesManager->addElement( "New note...", "" );
        loadNotes();
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

//interface : item of list activated
void UIPostIt::on_lstNotes_itemActivated ( QListWidgetItem *item )
{
    //load note content
    uint i = lstNotes->currentRow();
    txtContent->setPlainText( mNotesManager->getElement( i ).text() );
}

//edit title on doubleclicked
void UIPostIt::on_lstNotes_itemDoubleClicked ( QListWidgetItem * item )
{
    bool inputResult;
    QString title = QInputDialog::getText(this, "Edit title", "Enter new title note :", QLineEdit::Normal, "", &inputResult );
    
    if ( inputResult ) {
        mNotesManager->setTitleElement( lstNotes->currentRow(), title );
        loadNotes();
    }
}

//delete selected note
void UIPostIt::on_tbDelete_clicked()
{
    uint i = lstNotes->currentRow();
    mNotesManager->removeElement( i );
    txtContent->clear();
    loadNotes();
}

//when PostIt closing
void UIPostIt::closeEvent( QCloseEvent *event )
{
    if ( mTextChanged) {
        mNotesManager->setElement( lstNotes->currentRow(), txtContent->toPlainText() );
        mTextChanged = false;
    }

    mNotesManager->writeDocument();
}

//save note modification
void UIPostIt::on_txtContent_textChanged()
{
    mTextChanged = true;
}

bool UIPostIt::eventFilter( QObject *obj, QEvent *event )
 {
     if ( (obj == txtContent) && (event->type() == QEvent::FocusOut) && mTextChanged) {
        //save new value
        mNotesManager->setElement( lstNotes->currentRow(), txtContent->toPlainText() );
        loadNotes();
        mTextChanged = false;
        //return true;
     } else {
        // pass the event on to the parent class
        return QMainWindow::eventFilter( obj, event );
        //return false;
     }
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