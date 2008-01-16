/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIDesktopTools.cpp
** Date      : 2008-01-14T00:37:14
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
#include "UIDesktopTools.h"
#include "pToolsManager.h"
#include "pSettings.h"
#include "pMonkeyStudio.h"
#include "MonkeyCore.h"

#include <QTimer>
#include <QCloseEvent>

UIDesktopTools::UIDesktopTools( QWidget* w )
	: QDialog( w )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	twLeft->headerItem()->setHidden( true );
	pbLoading->setVisible( false );
	setProperty( "Populated", false );
	setProperty( "Modified", false );
	if ( !mStartMenu.categoriesAvailable() )
	{
		lCategoriesFilters->hide();
		leCategoriesFilters->hide();
	}
	// connection
	connect( twLeft, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( on_tbRight_clicked() ) );
	connect( lwRight, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), this, SLOT( on_tbLeft_clicked() ) );
}

UIDesktopTools::~UIDesktopTools()
{}

void UIDesktopTools::showEvent( QShowEvent* e )
{
	QDialog::showEvent( e );
	dbbButtons->button( QDialogButtonBox::Ok )->setDefault( false );
	QTimer::singleShot( 0, this, SLOT( populateList() ) );
}

void UIDesktopTools::closeEvent( QCloseEvent* e )
{
	if ( property( "Modified" ).toBool() && !pMonkeyStudio::question( tr( "Tools Editor..." ), tr( "You're about to discard all changes. Are you sure ?" ), this ) )
		e->ignore();
}

void UIDesktopTools::populateTree( QTreeWidgetItem* i, pDesktopFolder* f )
{
	if ( !f )
		return;
	// Folders
	foreach ( QString s, f->Folders.keys() )
	{
		pDesktopFolder* df = f->Folders.value( s );
		QTreeWidgetItem* it = 0;
		if ( i )
			it = new QTreeWidgetItem( i );
		else
			it = new QTreeWidgetItem( twLeft );
		it->setText( 0, s );
		it->setIcon( 0, QPixmap( df->Icon ) );
		if ( it->icon( 0 ).isNull() )
			it->setIcon( 0, pToolsManager::iconProvider()->icon( df->Path ) );
		it->setData( 0, pDesktopApplications::dtType, pDesktopApplications::dtFolder );
		populateTree( it, df );
	}
	// Applications
	foreach ( QString s, f->Applications.keys() )
	{
		QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
		pDesktopApplication* a = f->Applications.value( s );
		QTreeWidgetItem* it = 0;
		if ( i )
			it = new QTreeWidgetItem( i );
		else
			it = new QTreeWidgetItem( twLeft );
		it->setText( 0, a->Name );
		it->setIcon( 0, QPixmap( a->Icon ) );
		if ( it->icon( 0 ).isNull() )
			it->setIcon( 0, pToolsManager::iconProvider()->icon( s ) );
		it->setToolTip( 0, QString( "<b>%1</b><br />%2" ).arg( a->GenericName.isEmpty() ? a->Name : a->GenericName ).arg( a->Comment.isEmpty() ? QObject::tr( "No available comment" ) : a->Comment ) );
		it->setStatusTip( 0, s );
		it->setData( 0, pDesktopApplications::dtType, pDesktopApplications::dtApplication );
		it->setData( 0, pDesktopApplications::dtIcon, a->Icon );
		it->setData( 0, pDesktopApplications::dtCategories, a->Categories );
		it->setData( 0, pDesktopApplications::dtInUse, false );
		pbLoading->setValue( pbLoading->value() +1 );
	}
}

void UIDesktopTools::populateList()
{
	if ( !property( "Populated" ).toBool() )
	{
		// show progressbar
		pbLoading->setVisible( true );
		// set temp progressbar for loading application
		pbLoading->setRange( 0, 0 );
		// populate applications
		mStartMenu.populateApplications();
		// set progressbar range
		pbLoading->setRange( 0, mStartMenu.count() );
		// clear tree
		twLeft->clear();
		// disable update
		twLeft->setUpdatesEnabled( false );
		// populate tree
		populateTree( 0, mStartMenu.startMenu() );
		// enable update
		twLeft->setUpdatesEnabled( true );
		// hide progressbar
		pbLoading->setVisible( false );
		// restore to right added applications
		QStringList l;
		foreach ( pTool t, pToolsManager::tools( pToolsManager::ttDesktopEntry ) )
			l << t.FilePath;
		foreach ( QTreeWidgetItem* it, twLeft->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		{
			if ( l.contains( it->statusTip( 0 ) ) )
				it->setSelected( true );
		}
		// simulate click to add items to right
		tbRight->click();
		// update property
		setProperty( "Populated", true );
		setProperty( "Modified", false );
		// resize windows fucking this Windows XP style bug for progressbar width
		resize( 640, 480 );
	}
}

void UIDesktopTools::on_leNameFilter_returnPressed()
{
	QList<QTreeWidgetItem*> l = twLeft->findItems( leNameFilter->text(), Qt::MatchContains | Qt::MatchRecursive );
	foreach ( QTreeWidgetItem* it, twLeft->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		if ( !it->data( 0, pDesktopApplications::dtInUse ).toBool() && it->data( 0, pDesktopApplications::dtType ).toInt() == pDesktopApplications::dtApplication )
			it->setHidden( !l.contains( it ) );
}

void UIDesktopTools::on_leCategoriesFilters_returnPressed()
{
	foreach ( QTreeWidgetItem* it, twLeft->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
	{
		if ( it->data( 0, pDesktopApplications::dtInUse ).toBool() || it->data( 0, pDesktopApplications::dtType ).toInt() == pDesktopApplications::dtFolder )
			continue;
		// get item data
		QStringList l = it->data( 0, pDesktopApplications::dtCategories ).toStringList();
		// check if need to be visible according to filter
		bool b = leCategoriesFilters->text().isEmpty();
		if ( !b )
		{
			foreach ( QString s, leCategoriesFilters->text().split( ";", QString::SkipEmptyParts ) )
			{
				if ( l.contains( s, Qt::CaseInsensitive ) )
					b = true;
				if ( !b )
					break;
			}
		}
		// set item visibility
		it->setHidden( !b );
	}
}

void UIDesktopTools::on_tbRight_clicked()
{
	foreach ( QTreeWidgetItem* it, twLeft->selectedItems() )
	{
		if ( !it->isHidden() && it->data( 0, pDesktopApplications::dtType ).toInt() == pDesktopApplications::dtApplication )
		{
			QListWidgetItem* i = new QListWidgetItem( lwRight );
			i->setText( it->text( 0 ) );
			i->setIcon( it->icon( 0 ) );
			i->setToolTip( it->toolTip( 0 ) );
			i->setStatusTip( it->statusTip( 0 ) );
			i->setData( pDesktopApplications::dtIcon, it->data( 0, pDesktopApplications::dtIcon ).toString() );
			i->setData( pDesktopApplications::dtPointer, reinterpret_cast<quintptr>( it ) );
			it->setData( 0, pDesktopApplications::dtInUse, true );
			it->setHidden( true );
			// modified state
			setProperty( "Modified", true );
		}
	}
}

void UIDesktopTools::on_tbLeft_clicked()
{
	foreach ( QListWidgetItem* it, lwRight->selectedItems() )
	{
		QTreeWidgetItem* i = reinterpret_cast<QTreeWidgetItem*>( it->data( pDesktopApplications::dtPointer ).value<quintptr>() );
		if ( i )
		{
			i->setHidden( false );
			i->setData( 0, pDesktopApplications::dtInUse, false );
			// modified state
			setProperty( "Modified", true );
		}
		delete it;
	}
}

void UIDesktopTools::on_pbUp_clicked()
{
	if ( lwRight->selectedItems().count() > 1 )
	{
		pMonkeyStudio::warning( tr( "Move Up..." ), tr( "Only one item can be move up, please select only one item." ) );
		return;
	}
	
	QListWidgetItem* it = lwRight->selectedItems().value( 0 );
	if ( !it || lwRight->row( it ) == 0 )
		return;
	int id = lwRight->row( it );
	it = lwRight->takeItem( id );
	lwRight->insertItem( id -1, it );
	lwRight->setCurrentRow( id -1 );
	// modified state
	setProperty( "Modified", true );
}

void UIDesktopTools::on_pbDown_clicked()
{
	if ( lwRight->selectedItems().count() > 1 )
	{
		pMonkeyStudio::warning( tr( "Move Down..." ), tr( "Only one item can be move down, please select only one item." ) );
		return;
	}
	
	QListWidgetItem* it = lwRight->selectedItems().value( 0 );
	if ( !it || lwRight->row( it ) == lwRight->count() -1 )
		return;
	int id = lwRight->row( it );
	it = lwRight->takeItem( id );
	lwRight->insertItem( id +1, it );
	lwRight->setCurrentRow( id +1 );
	// modified state
	setProperty( "Modified", true );
}

void UIDesktopTools::accept()
{
	if ( property( "Modified" ).toBool() )
	{
		// get desktop entry
		QList<pTool> l = pToolsManager::tools( pToolsManager::ttUserEntry );
		// get settings
		pSettings* s = MonkeyCore::settings();
		// remove all tools entries
		s->remove( "Tools" );
		// begin array
		s->beginWriteArray( "Tools" );
		int i = 0;
		// write desktop entry
		for ( i = 0; i < lwRight->count(); i++ )
		{
			s->setArrayIndex( i );
			s->setValue( "Caption", lwRight->item( i )->text() );
			s->setValue( "FileIcon", lwRight->item( i )->data( pDesktopApplications::dtIcon ).toString() );
			s->setValue( "FilePath", lwRight->item( i )->statusTip() );
			s->setValue( "WorkingPath", "" );
			s->setValue( "DesktopEntry", true );
		}
		// write user entry
		foreach ( pTool t, l )
		{
			s->setArrayIndex( i );
			s->setValue( "Caption", t.Caption );
			s->setValue( "FileIcon", t.FileIcon );
			s->setValue( "FilePath", t.FilePath );
			s->setValue( "WorkingPath", t.WorkingPath );
			s->setValue( "DesktopEntry", false );
			i++;
		}
		// end array
		s->endArray();
	}
	// close dialog
	QDialog::accept();
}
