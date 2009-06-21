/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : QMakeItemDelegate.cpp
** Date      : 2008-01-14T00:54:11
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
#include "QMakeItemDelegate.h"
#include "UISettingsQMake.h"

#include <QMetaEnum>

QMakeItemDelegate::QMakeItemDelegate( QWidget* p )
	: QItemDelegate( p )
{}

QWidget* QMakeItemDelegate::createEditor( QWidget* w, const QStyleOptionViewItem&, const QModelIndex& i ) const
{
	if ( !i.isValid() || i.column() == 0 )
		return 0;
	switch ( i.sibling( i.row(), 0 ).data( Qt::UserRole +1 ).toInt() )
	{
		case ProjectItem::TypeRole:
		{
			QComboBox* cb = new QComboBox( w );
			const QMetaObject mo = ProjectItem::staticMetaObject;
			QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeType" ) );
			for ( int j = 0; j < me.keyCount(); j++ )
				if ( me.value( j ) > ProjectItem::FirstType && me.value( j ) < ProjectItem::LastType )
					cb->addItem( me.key( j ), me.value( j ) );
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
		}
		case ProjectItem::ValueRole:
		case ProjectItem::CommentRole:
		{
			QValueFileFolderEdit* vffe = new QValueFileFolderEdit( w, i );
			vffe->setText( i.data().toString() );
			return vffe;
		}
		case ProjectItem::OperatorRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItems( QStringList() << QString::null << pMonkeyStudio::availableOperators() );
			cb->setCurrentIndex( cb->findText( i.data().toString() ) );
			return cb;
		}
	}
	return 0;
}

void QMakeItemDelegate::setModelData( QWidget* w, QAbstractItemModel* m, const QModelIndex& i ) const
{
	if ( !w || !m || !i.isValid() || i.column() == 0 )
		return;
	switch ( i.sibling( i.row(), 0 ).data( Qt::UserRole +1 ).toInt() )
	{
		case ProjectItem::TypeRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->itemData( cb->currentIndex() ), Qt::DisplayRole );
			return;
		}
		case ProjectItem::ValueRole:
		case ProjectItem::CommentRole:
		{
			QValueFileFolderEdit* vffe = qobject_cast<QValueFileFolderEdit*>( w );
			m->setData( i, vffe->text(), Qt::DisplayRole );
			return;
		}
		case ProjectItem::OperatorRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->currentText(), Qt::DisplayRole );
			return;
		}
	}
}
