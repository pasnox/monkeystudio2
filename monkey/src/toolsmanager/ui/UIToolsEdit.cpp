/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIToolsEdit.cpp
** Date      : 2008-01-14T00:37:15
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
**
****************************************************************************/
#include "UIToolsEdit.h"
#include "../pToolsManager.h"
#include "../../pMonkeyStudio.h"
#include "../../coremanager/MonkeyCore.h"
#include "../../settingsmanager/Settings.h"

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
	foreach ( pTool tool, pToolsManager::tools( pToolsManager::ttUserEntry ) )
	{
		// create item
		QListWidgetItem* it = new QListWidgetItem( QIcon( tool.FileIcon ), tool.Caption, lwTools );
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
	}
	// modified state
	setWindowModified( false );
	// connection
	connect( bbDialog, SIGNAL( helpRequested() ), this, SLOT( helpRequested() ) );
}

void UIToolsEdit::closeEvent( QCloseEvent* e )
{
	if ( isWindowModified() && !pMonkeyStudio::question( tr( "Tools Editor..." ), tr( "You're about to discard all changes. Are you sure ?" ), this ) )
		e->ignore();
}

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
	if ( !it && !pMonkeyStudio::question( tr( "Adding..." ), tr( "There is no current tool, do you want to add a new one ?" ), this ) )
		return true; // true else the default drop event will be call
	else if ( !it )
		it = new QListWidgetItem( tr( "new Tool" ), lwTools );
	// get link info
	QFileInfo f( d->mimeData()->urls().at( 0 ).toLocalFile() );
	// drag for tbFileIcon
	pTool tool = it->data( Qt::UserRole ).value<pTool>();
	if ( tool.Caption.isEmpty() )
		tool.Caption = it->text();
	if ( o == tbFileIcon )
	{
		if ( f.isFile() )
			tool.FileIcon = f.absoluteFilePath();
	}
	// others
	else
	{
		if ( f.isFile() )
		{
			tool.Caption = f.baseName();
			tool.FilePath = f.absoluteFilePath();
			tool.WorkingPath = f.absolutePath();
		}
		else if ( f.isDir() )
			tool.WorkingPath = f.absoluteFilePath();
	}
	// update item data
	it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
	updateGui( it, true );
	// modified state
	setWindowModified( true );
	// we finish
	return true;
}

void UIToolsEdit::updateGui( QListWidgetItem* item, bool makeCurrent )
{
	const pTool tool = item->data( Qt::UserRole ).value<pTool>();
	item->setText( tool.Caption );
	item->setIcon( QIcon( tool.FileIcon ) );
	leCaption->setText( tool.Caption );
	tbFileIcon->setIcon( item->icon() );
	leFilePath->setText( tool.FilePath );
	leWorkingPath->setText( tool.WorkingPath );
	cbUseConsoleManager->setChecked( tool.UseConsoleManager );
	//
	if ( makeCurrent )
	{
		lwTools->clearSelection();
		lwTools->setCurrentItem( item );
		item->setSelected( true );
	}
}

void UIToolsEdit::on_lwTools_itemSelectionChanged()
{
	if ( QListWidgetItem* item = lwTools->selectedItems().value( 0 ) )
		updateGui( item );
}

void UIToolsEdit::on_pbNew_clicked()
{
	QListWidgetItem* item = new QListWidgetItem( tr( "new Tool" ), lwTools );
	pTool tool = item->data( Qt::UserRole ).value<pTool>();
	tool.Caption = item->text();
	item->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
	updateGui( item, true );
	// modified state
	setWindowModified( true );
}

void UIToolsEdit::on_pbDelete_clicked()
{
	delete lwTools->selectedItems().value( 0 );
	if ( lwTools->count() )
		updateGui( lwTools->item( 0 ), true );
	// modified state
	setWindowModified( true );
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
	setWindowModified( true );
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
	setWindowModified( true );
}

void UIToolsEdit::helpRequested()
{
	QString s = tr( "<b>Tools Editor</b> give you the possibility to use variables<br><br>"
		"<b>$cpp$</b> : Current project path<br>"
		"<b>$cp$</b> : Current project filepath<br>"
		"<b>$cfp$</b> : Current tab path<br>"
		"<b>$cf$</b> : Current tab filepath<br>"
		"<b>$cip$</b> : Current item path<br>"
		"<b>$ci$</b> : Current item filepath" );
	QWhatsThis::showText( mapToGlobal( rect().center() ), s );
}

void UIToolsEdit::on_leCaption_editingFinished()
{
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		pTool tool = it->data( Qt::UserRole ).value<pTool>();
		tool.Caption = leCaption->text();
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
		updateGui( it );
		// modified state
		setWindowModified( true );
	}
}

void UIToolsEdit::on_tbFileIcon_clicked()
{
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		pTool tool = it->data( Qt::UserRole ).value<pTool>();
		QString s = pMonkeyStudio::getImageFileName( tr( "Choose an icon for this tool" ), tool.FileIcon, this );
		if ( s.isEmpty() )
			return;
		tool.FileIcon = s;
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
		updateGui( it );
		// modified state
		setWindowModified( true );
	}
}

void UIToolsEdit::on_leFilePath_editingFinished()
{
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		pTool tool = it->data( Qt::UserRole ).value<pTool>();
		tool.FilePath = leFilePath->text();
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
		updateGui( it );
		setWindowModified( true );
	}
}

void UIToolsEdit::on_tbFilePath_clicked()
{
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		pTool tool = it->data( Qt::UserRole ).value<pTool>();
		QString s = pMonkeyStudio::getOpenFileName( tr( "Choose the file to execute for this tool" ), tool.FilePath, QString::null, this );
		if ( s.isEmpty() )
			return;
		tool.FilePath = s;
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
		updateGui( it );
		leFilePath->setFocus();
		setWindowModified( true );
	}
}

void UIToolsEdit::on_tbUpdateWorkingPath_clicked()
{	
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		QFileInfo f( leFilePath->text() );
		if ( f.exists() && f.absolutePath() != leWorkingPath->text() )
		{
			pTool tool = it->data( Qt::UserRole ).value<pTool>();
			tool.WorkingPath = f.absolutePath();
			it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
			updateGui( it );
			leWorkingPath->setFocus();
			setWindowModified( true );
		}
	}
}

void UIToolsEdit::on_leWorkingPath_editingFinished()
{
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		pTool tool = it->data( Qt::UserRole ).value<pTool>();
		tool.WorkingPath = leWorkingPath->text();
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
		updateGui( it );
		setWindowModified( true );
	}
}

void UIToolsEdit::on_tbWorkingPath_clicked()
{
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		pTool tool = it->data( Qt::UserRole ).value<pTool>();
		QString s = pMonkeyStudio::getExistingDirectory( tr( "Choose the working path for this tool" ), tool.WorkingPath, this );
		if ( s.isEmpty() )
			return;
		tool.WorkingPath = s;
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
		updateGui( it );
		leWorkingPath->setFocus();
		setWindowModified( true );
	}
}

void UIToolsEdit::on_cbUseConsoleManager_clicked( bool clicked )
{
	if ( QListWidgetItem* it = lwTools->selectedItems().value( 0 ) )
	{
		pTool tool = it->data( Qt::UserRole ).value<pTool>();
		tool.UseConsoleManager = clicked;
		it->setData( Qt::UserRole, QVariant::fromValue<pTool>( tool ) );
		updateGui( it );
		setWindowModified( true );
	}
}

void UIToolsEdit::accept()
{
	if ( isWindowModified() )
	{
		// get desktop entry
		const QList<pTool> tools = pToolsManager::tools( pToolsManager::ttDesktopEntry );
		// get settings
		pSettings* s = MonkeyCore::settings();
		// remove all tools entries
		s->remove( "Tools" );
		// begin array
		s->beginWriteArray( "Tools" );
		int i = 0;
		// write user entry
		for ( i = 0; i < lwTools->count(); i++ )
		{
			s->setArrayIndex( i );
			const pTool tool = lwTools->item( i )->data( Qt::UserRole ).value<pTool>();
			s->setValue( "Caption", tool.Caption );
			s->setValue( "FileIcon", tool.FileIcon );
			s->setValue( "FilePath", tool.FilePath );
			s->setValue( "WorkingPath", tool.WorkingPath );
			s->setValue( "DesktopEntry", false );
			s->setValue( "UseConsoleManager", tool.UseConsoleManager );
		}
		// write desktop entry
		foreach ( const pTool& tool, tools )
		{
			s->setArrayIndex( i );
			s->setValue( "Caption", tool.Caption );
			s->setValue( "FileIcon", tool.FileIcon );
			s->setValue( "FilePath", tool.FilePath );
			s->setValue( "WorkingPath", tool.WorkingPath );
			s->setValue( "DesktopEntry", true );
			s->setValue( "UseConsoleManager", tool.UseConsoleManager );
			i++;
		}
		// end array
		s->endArray();
	}
	// close dialog
	QDialog::accept();
}
