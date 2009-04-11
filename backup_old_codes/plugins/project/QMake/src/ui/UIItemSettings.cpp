/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : UIItemSettings.cpp
** Date      : 2008-01-14T00:54:09
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
#include "UIItemSettings.h"
#include "../QMakeItemDelegate.h"

#include <QMetaEnum>
#include <QFontMetrics>
#include <QPushButton>

bool UIItemSettings::edit( ProjectItem* i, QWidget* )
{
	UIItemSettings* d = instance( i );
	d->mItem = i;
	return d->exec();
}

UIItemSettings::UIItemSettings( ProjectItem* i )
	: QDialog( QApplication::activeWindow() ), mItem( i )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/icons/icons/ok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/icons/icons/cancel.png" ) );
	// crete tree delegate
	twValueRoles->setItemDelegate( new QMakeItemDelegate( twValueRoles ) );
	// fill list with node roles
	const QMetaObject mo = ProjectItem::staticMetaObject;
	QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeRole" ) );
	QFontMetrics fm( twValueRoles->font() );
	int pMax = 0;
	//
	for ( int i = 0; i < me.keyCount(); i++ )
	{
		// don t create item for these roles
		if ( me.value( i ) > ProjectItem::FirstRole && me.value( i ) < ProjectItem::LastRole )
		{
			// create item role
			QTreeWidgetItem* it = new QTreeWidgetItem( twValueRoles );
			it->setFlags( it->flags() | Qt::ItemIsEditable );
			it->setText( 0, me.key( i ) );
			it->setData( 0, Qt::UserRole +1, me.value( i ) );
			it->setText( 1, mItem->data( me.value( i ) ).toString() );
			
			// longest string in pixel for resizing the first column
			if ( fm.width( it->text( 0 ) ) > pMax )
				pMax = fm.width( it->text( 0 ) );
		}
	}
	// resize first column so we can see the caption
	twValueRoles->setColumnWidth( 0, pMax +10 );
}

void UIItemSettings::accept()
{
	// apply modifications
	for ( int i = 0; i < twValueRoles->topLevelItemCount(); i++ )
	{
		switch ( twValueRoles->topLevelItem( i )->data( 0, Qt::UserRole +1 ).toInt() )
		{
			case ProjectItem::TypeRole:
				mItem->setType( (ProjectItem::NodeType)twValueRoles->topLevelItem( i )->text( 1 ).toInt() );
				break;
			case ProjectItem::OperatorRole:
				mItem->setOperator( twValueRoles->topLevelItem( i )->text( 1 ) );
				break;
			case ProjectItem::ValueRole:
				mItem->setValue( twValueRoles->topLevelItem( i )->text( 1 ) );
				break;
			case ProjectItem::MultiLineRole:
				mItem->setMultiLine( twValueRoles->topLevelItem( i )->text( 1 ).toInt() );
				break;
			case ProjectItem::CommentRole:
				mItem->setComment( twValueRoles->topLevelItem( i )->text( 1 ) );
				break;
		}
	}
	// close dialog
	QDialog::accept();
}
