/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QMakeItemDelegate.cpp
** Date      : 2007-11-04T22:53:42
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "QMakeItemDelegate.h"
#include "UISettingsQMake.h"
#include "pMonkeyStudio.h"

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
