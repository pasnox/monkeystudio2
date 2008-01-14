/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pFileListEditor.cpp
** Date      : 2008-01-14T00:27:43
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
#include "pFileListEditor.h"

#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QListWidgetItem>

pFileListEditor::pFileListEditor( QWidget* p, const QString& t, const QString& pa, const QString& f ):
	pStringListEditor( p, t ), mPath( pa ), mFilter( f )
{ findChild<QToolBar*>()->actions().last()->setIcon( QIcon( ":/listeditor/icons/listeditor/file.png" ) ); }

void pFileListEditor::onAddItem()
{
	// get files
	QStringList l = QFileDialog::getOpenFileNames( window(), tr( "Choose file(s)" ), mPath, mFilter );
	
	if ( !l.isEmpty() )
	{
		foreach ( QString s, l )
		{
			QListWidgetItem* it = new QListWidgetItem( s, mList );
			it->setFlags( it->flags() | Qt::ItemIsEditable );
			mList->setCurrentItem( it );
			mList->scrollToItem( it );
		}
		emit edited();
	}
}

void pFileListEditor::onEditItem()
{
	if ( QListWidgetItem* it = mList->selectedItems().value( 0 ) )
	{
		QString s= QFileDialog::getOpenFileName( window(), tr( "Choose file" ), mPath, mFilter );
		if ( !s.isEmpty() )
		{
			it->setText( s );
			emit edited();
		}
	}
}

void pFileListEditor::setPath( const QString& s )
{ mPath = s; }
