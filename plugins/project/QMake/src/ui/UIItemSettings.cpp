/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIItemSettings.cpp
** Date      : 2007-11-04T22:53:36
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "UIItemSettings.h"
#include "ProjectItem.h"
#include "ProjectsModel.h"
#include "QMakeItemDelegate.h"

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
