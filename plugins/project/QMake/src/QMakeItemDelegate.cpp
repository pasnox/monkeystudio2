#include "QMakeItemDelegate.h"
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
