#include "UIItemSettings.h"
#include "ProjectItem.h"
#include "ProjectsModel.h"
#include "QMakeItemDelegate.h"

#include <QMetaEnum>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QFontMetrics>
#include <QPushButton>

bool UIItemSettings::edit( ProjectsModel* p, ProjectItem* i, QWidget* w )
{
	UIItemSettings d( p, i, w );
	return d.exec();
}

UIItemSettings::UIItemSettings( ProjectsModel* p, ProjectItem* pi, QWidget* w )
	: QDialog( w ), mItem( pi ), mProject( p )
{
	setupUi( this );
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/Icons/Icons/buttonok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/Icons/Icons/buttoncancel.png" ) );
	twValueRoles->setItemDelegate( new QMakeItemDelegate( twValueRoles ) );
	// fill list with node roles
	const QMetaObject mo = ProjectsModel::staticMetaObject;
	QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeRole" ) );
	QFontMetrics fm( twValueRoles->font() );
	int pMax = 0;
	for ( int i = 0; i < me.keyCount(); i++ )
	{
		if ( me.value( i ) == ProjectsModel::FirstRole || me.value( i ) == ProjectsModel::LastRole )
			continue;
		QTreeWidgetItem* it = new QTreeWidgetItem( twValueRoles );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		it->setText( 0, me.key( i ) );
		it->setData( 0, Qt::UserRole +1, me.value( i ) );
		if ( pi )
			it->setText( 1, mItem->data( me.value( i ) ).toString() );
		// longest string in pixel for resizing the first column
		if ( fm.width( it->text( 0 ) ) > pMax )
			pMax = fm.width( it->text( 0 ) );
	}
	twValueRoles->setColumnWidth( 0, pMax +10 );
}

void UIItemSettings::accept()
{
	for ( int i = 0; i < twValueRoles->topLevelItemCount(); i++ )
		mItem->setData( twValueRoles->topLevelItem( i )->text( 1 ), twValueRoles->topLevelItem( i )->data( 0, Qt::UserRole +1 ).toInt() );
	
	QDialog::accept();
}
