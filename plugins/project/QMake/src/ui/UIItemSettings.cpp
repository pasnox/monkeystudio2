#include "UIItemSettings.h"
#include "ProjectItem.h"
#include "ProjectsModel.h"
#include "QMakeItemDelegate.h"

#include <QMetaEnum>
#include <QFontMetrics>
#include <QPushButton>

bool UIItemSettings::edit( ProjectItem* i, QWidget* w )
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
	const QMetaObject mo = ProjectsModel::staticMetaObject;
	QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeRole" ) );
	QFontMetrics fm( twValueRoles->font() );
	int pMax = 0;
	
	for ( int i = 0; i < me.keyCount(); i++ )
	{
		// don t create item for these roles
		if ( me.value( i ) > ProjectsModel::FirstRole && me.value( i ) < ProjectsModel::ModifiedRole )
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
			case ProjectsModel::TypeRole:
				mItem->setType( (ProjectsModel::NodeType)twValueRoles->topLevelItem( i )->text( 1 ).toInt() );
				break;
			case ProjectsModel::OperatorRole:
				mItem->setOperator( twValueRoles->topLevelItem( i )->text( 1 ) );
				break;
			case ProjectsModel::ValueRole:
				mItem->setValue( twValueRoles->topLevelItem( i )->text( 1 ) );
				break;
			case ProjectsModel::MultiLineRole:
				mItem->setMultiLine( twValueRoles->topLevelItem( i )->text( 1 ).toInt() );
				break;
			case ProjectsModel::CommentRole:
				mItem->setComment( twValueRoles->topLevelItem( i )->text( 1 ) );
				break;
		}
	}
	
	// close dialog
	QDialog::accept();
}
