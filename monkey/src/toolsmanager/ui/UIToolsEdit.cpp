#include "UIToolsEdit.h"
#include "pSettings.h"
#include "pToolsManager.h"
#include "pMonkeyStudio.h"

#include <QFileDialog>
#include <QWhatsThis>
#include <QCloseEvent>
#include <QFileInfo>
#include <QUrl>
#include <QMimeData>

UIToolsEdit::UIToolsEdit( QWidget* p )
	: QDialog( p )
{
	// init dialog
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	leCaption->installEventFilter( this );
	tbFileIcon->installEventFilter( this );
	leFilePath->installEventFilter( this );
	leWorkingPath->installEventFilter( this );
	// create items
	foreach ( pTool t, pToolsManager::tools( pToolsManager::ttUserEntry ) )
	{
		// create item
		QListWidgetItem* it = new QListWidgetItem( lwTools );
		// fill item infos
		it->setData( idCaption, t.Caption );
		it->setData( idFileIcon, t.FileIcon );
		it->setData( idFilePath, t.FilePath );
		it->setData( idWorkingPath, t.WorkingPath );
		it->setIcon( QIcon( t.FileIcon ) );
	}
	setProperty( "Modified" , false );
}
//
void UIToolsEdit::closeEvent( QCloseEvent* e )
{
	if ( property( "Modified" ).toBool() && !pMonkeyStudio::question( tr( "Tools Editor..." ), tr( "You're about to discard all changes. Are you sure ?" ), this ) )
		e->ignore();
}
//
bool UIToolsEdit::eventFilter( QObject* o, QEvent* e )
{
	// accept drag enter event
	if ( e->type() == QEvent::DragEnter )
		e->accept();
	// if not a drop event, return
	if ( e->type() != QEvent::Drop )
		return QDialog::eventFilter( o, e );
	// got the event
	QDropEvent* d = static_cast<QDropEvent*>( e );
	// if no url drop it
	if ( !d || !d->mimeData()->hasUrls() )
		return QDialog::eventFilter( o, e );
	// if there is no current item selected, ask to create one
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( !it && pMonkeyStudio::question( tr( "Adding..." ), tr( "There is no current tool, do you want to add a new one ?" ), this ) )
		return true; // true else the default drop event will be call
	else if ( !it )
		it = new QListWidgetItem( tr( "new Tool" ), lwTools );
	// get link info
	QFileInfo f( d->mimeData()->urls().at( 0 ).toLocalFile() );
	// drag for tbFileIcon
	if ( o == tbFileIcon )
	{
		if ( f.isFile() )
		{
			it->setData( idFileIcon, f.canonicalFilePath() );
			tbFileIcon->setIcon( QIcon( f.canonicalFilePath() ) );
			it->setIcon( tbFileIcon->icon() );
		}
	}
	// others
	else
	{
		if ( f.isFile() )
		{
			leCaption->setText( f.baseName() );
			it->setData( idCaption, leCaption->text() );
			leFilePath->setText( f.canonicalFilePath() );
			it->setData( idFilePath, leFilePath->text() );
			leWorkingPath->setText( f.canonicalPath() );
		}
		else if ( f.isDir() )
			leWorkingPath->setText( f.canonicalFilePath() );
		it->setData( idWorkingPath, leWorkingPath->text() );
	}
	// select the current item
	lwTools->clearSelection();
	lwTools->setCurrentItem( it );
	it->setSelected( true );
	// modified state
	setProperty( "Modified" , true );
	// we finish
	return true;
}

void UIToolsEdit::on_lwTools_itemSelectionChanged()
{
	// get current selected item
	QListWidgetItem* i = lwTools->selectedItems().value( 0 );
	// cancel if no item
	if ( !i )
		return;
	// fill dialog infos
	leCaption->setText( i->text() );
	tbFileIcon->setIcon( i->icon() );
	leFilePath->setText( i->data( idFilePath ).toString() );
	leWorkingPath->setText( i->data( idWorkingPath ).toString() );
}

void UIToolsEdit::on_pbNew_clicked()
{
	QListWidgetItem* it = new QListWidgetItem( tr( "new Tool" ), lwTools );
	lwTools->clearSelection();
	lwTools->setCurrentItem( it );
	it->setSelected( true );
	on_lwTools_itemSelectionChanged();
	// modified state
	setProperty( "Modified" , true );
}

void UIToolsEdit::on_pbDelete_clicked()
{
	delete lwTools->selectedItems().value( 0 );
	if ( lwTools->count() )
	{
		lwTools->clearSelection();
		lwTools->setCurrentRow( 0 );
		lwTools->currentItem()->setSelected( true );
		on_lwTools_itemSelectionChanged();
	}
	// modified state
	setProperty( "Modified" , true );
}

void UIToolsEdit::on_pbUp_clicked()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( !it || lwTools->row( it ) == 0 )
		return;
	int id = lwTools->row( it );
	it = lwTools->takeItem( id );
	lwTools->insertItem( id -1, it );
	lwTools->setCurrentRow( id -1 );
	// modified state
	setProperty( "Modified" , true );
}

void UIToolsEdit::on_pbDown_clicked()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( !it || lwTools->row( it ) == lwTools->count() -1 )
		return;
	int id = lwTools->row( it );
	it = lwTools->takeItem( id );
	lwTools->insertItem( id +1, it );
	lwTools->setCurrentRow( id +1 );
	// modified state
	setProperty( "Modified" , true );
}

void UIToolsEdit::on_tbHelp_clicked()
{
	QString s = tr( "<b>Tools Editor</b> give you the possibility to use variables<br><br>"
		"<b>$cpp$</b> : Current project path<br>"
		"<b>$cp$</b> : Current project filepath<br>"
		"<b>$cfp$</b> : Current tab path<br>"
		"<b>$cf$</b> : Current tab filepath<br>"
		"<b>$cip$</b> : Current item path<br>"
		"<b>$ci$</b> : Current item filepath" );
	QWhatsThis::showText( tbHelp->mapToGlobal( QPoint( 0, 0 ) ), s );
}

void UIToolsEdit::on_leCaption_editingFinished()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( it )
	{
		it->setData( idCaption, leCaption->text() );
		// modified state
		setProperty( "Modified" , true );
	}
}

void UIToolsEdit::on_tbFileIcon_clicked()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( !it )
		return;
	QString s = pMonkeyStudio::getImageFileName( tr( "Choose an icon for this tool" ), QString::null, this );
	if ( s.isEmpty() )
		return;
	it->setData( idFileIcon, s );
	tbFileIcon->setIcon( QIcon( s ) );
	it->setIcon( tbFileIcon->icon() );
	// modified state
	setProperty( "Modified" , true );
}

void UIToolsEdit::on_leFilePath_editingFinished()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( it )
	{
		it->setData( idFilePath, leFilePath->text() );
		// modified state
		setProperty( "Modified" , true );
	}
}

void UIToolsEdit::on_tbFilePath_clicked()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( !it )
		return;
	QString s = pMonkeyStudio::getOpenFileName( tr( "Choose the file to execute for this tool" ), QString::null, QString::null, this );
	if ( s.isEmpty() )
		return;
	leFilePath->setText( s );
	leFilePath->setFocus();
	// modified state
	setProperty( "Modified" , true );
}

void UIToolsEdit::on_tbUpdateWorkingPath_clicked()
{
	if ( !lwTools->selectedItems().value( 0 ) )
		return;
	QFileInfo f( leFilePath->text() );
	if ( f.exists() && f.absolutePath() != leWorkingPath->text() )
	{
		leWorkingPath->setText( f.absolutePath() );
		leWorkingPath->setFocus();
		// modified state
		setProperty( "Modified" , true );
	}
}

void UIToolsEdit::on_leWorkingPath_editingFinished()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( it )
	{
		it->setData( idWorkingPath, leWorkingPath->text() );
		// modified state
		setProperty( "Modified" , true );
	}
}

void UIToolsEdit::on_tbWorkingPath_clicked()
{
	QListWidgetItem* it = lwTools->selectedItems().value( 0 );
	if ( !it )
		return;
	QString s = pMonkeyStudio::getExistingDirectory( tr( "Choose the working path for this tool" ), QString::null, this );
	if ( s.isEmpty() )
		return;
	leWorkingPath->setText( s );
	leWorkingPath->setFocus();
	// modified state
	setProperty( "Modified" , true );
}

void UIToolsEdit::accept()
{
	if ( property( "Modified" ).toBool() )
	{
		// get desktop entry
		QList<pTool> l = pToolsManager::tools( pToolsManager::ttDesktopEntry );
		// get settings
		QSettings* s = pSettings::instance();
		// remove all tools entries
		s->remove( "Tools" );
		// begin array
		s->beginWriteArray( "Tools" );
		int i = 0;
		// write user entry
		for ( i = 0; i < lwTools->count(); i++ )
		{
			s->setArrayIndex( i );
			s->setValue( "Caption", lwTools->item( i )->data( idCaption ).toString() );
			s->setValue( "FileIcon", lwTools->item( i )->data( idFileIcon ).toString() );
			s->setValue( "FilePath", lwTools->item( i )->data( idFilePath ).toString() );
			s->setValue( "WorkingPath", lwTools->item( i )->data( idWorkingPath ).toString() );
			s->setValue( "DesktopEntry", false );
		}
		// write desktop entry
		foreach ( pTool t, l )
		{
			s->setArrayIndex( i );
			s->setValue( "Caption", t.Caption );
			s->setValue( "FileIcon", t.FileIcon );
			s->setValue( "FilePath", t.FilePath );
			s->setValue( "WorkingPath", t.WorkingPath );
			s->setValue( "DesktopEntry", true );
			i++;
		}
		// end array
		s->endArray();
	}
	// close dialog
	QDialog::accept();
}
