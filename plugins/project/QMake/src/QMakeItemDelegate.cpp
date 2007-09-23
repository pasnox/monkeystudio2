#include "QMakeItemDelegate.h"
#include "ProjectsModel.h"
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
		case ProjectsModel::TypeRole:
		{
			QComboBox* cb = new QComboBox( w );
			const QMetaObject mo = ProjectsModel::staticMetaObject;
			QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeType" ) );
			for ( int j = 0; j < me.keyCount(); j++ )
				if ( me.value( j ) > ProjectsModel::FirstType && me.value( j ) < ProjectsModel::LastType )
					cb->addItem( me.key( j ), me.value( j ) );
			cb->setCurrentIndex( cb->findData( i.data() ) );
			return cb;
		}
		case ProjectsModel::ValueRole:
		case ProjectsModel::CommentRole:
		{
			QValueFileFolderEdit* vffe = new QValueFileFolderEdit( w, i );
			vffe->setText( i.data().toString() );
			return vffe;
		}
		case ProjectsModel::OperatorRole:
		{
			QComboBox* cb = new QComboBox( w );
			cb->addItems( QStringList() << "" << UISettingsQMake::readOperators() );
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
		case ProjectsModel::TypeRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->itemData( cb->currentIndex() ), Qt::DisplayRole );
			return;
		}
		case ProjectsModel::ValueRole:
		case ProjectsModel::CommentRole:
		{
			QValueFileFolderEdit* vffe = qobject_cast<QValueFileFolderEdit*>( w );
			m->setData( i, vffe->text(), Qt::DisplayRole );
			return;
		}
		case ProjectsModel::OperatorRole:
		{
			QComboBox* cb = qobject_cast<QComboBox*>( w );
			m->setData( i, cb->currentText(), Qt::DisplayRole );
			return;
		}
	}
}
