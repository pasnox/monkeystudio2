#include "UIItemSettings.h"
#include "QMakeProjectItem.h"
#include "QMakeProjectModel.h"
#include "UIQMakeProjectSettings.h"
#include "QMakeProjectItemDelegate.h"
//
#include <QMetaEnum>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QFontMetrics>
//
QPointer<UIItemSettings> UIItemSettings::mSelf = 0L;
//
UIItemSettings* UIItemSettings::edit( QMakeProjectModel* p, QMakeProjectItem* i, QWidget* w )
{
	if ( !mSelf )
		mSelf = new UIItemSettings( p, i, w );
	return mSelf;
}
//
UIItemSettings::UIItemSettings( QMakeProjectModel* p, QMakeProjectItem* pi, QWidget* w )
	: QDialog( w ), mItem( pi ), mProject( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	dbbButtons->button( QDialogButtonBox::Ok )->setIcon( QPixmap( ":/Icons/Icons/buttonok.png" ) );
	dbbButtons->button( QDialogButtonBox::Cancel )->setIcon( QPixmap( ":/Icons/Icons/buttoncancel.png" ) );
	twValueRoles->setItemDelegate( new QMakeProjectItemDelegate( twValueRoles ) );
	// fill list with node roles
	const QMetaObject mo = AbstractProjectModel::staticMetaObject;
	QMetaEnum me = mo.enumerator( mo.indexOfEnumerator( "NodeRole" ) );
	QFontMetrics fm( twValueRoles->font() );
	int pMax = 0;
	for ( int i = 0; i < me.keyCount(); i++ )
	{
		if ( me.value( i ) == AbstractProjectModel::FirstRole || me.value( i ) == AbstractProjectModel::LastRole )
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
//
void UIItemSettings::accept()
{
	// got parent window
	UIQMakeProjectSettings* d = qobject_cast<UIQMakeProjectSettings*>( parentWidget() );
	// if no valid index, create one
	if ( !mItem && d )
	{
		// get the current index in the project settings
		mItem = static_cast<QMakeProjectItem*>( mProject->itemFromIndex( d->currentIndex() ) );
		// get it s row and rowcount
		int r = mItem ? mItem->row() : 0;
		int rc = mItem ? mItem->rowCount() : mProject->rowCount();
		// if item is a scope, remove 1 to rc
		if ( mItem && ( mItem->data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ScopeType || mItem->data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::NestedScopeType ) )
			rc--;
		// if parent is a scope, remove 1 to r
		if ( mItem && ( mItem->data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::ScopeType || mItem->data( AbstractProjectModel::TypeRole ).toInt() == AbstractProjectModel::NestedScopeType ) )
			r--;
		// create the new item
		AbstractProjectModel::NodeType mNodeType = ( AbstractProjectModel::NodeType )twValueRoles->topLevelItem( 0 )->data( 1, Qt::DisplayRole ).toInt();
		QMakeProjectItem* cItem = new QMakeProjectItem( mNodeType );
		// ask which kind of item we must create
		if ( QMessageBox::question( this, tr( "New Item.." ), tr( "Create item as a child of the current item ?" ), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes )
		// as child
			mProject->insertRow( rc, cItem, mItem );
		else
			mProject->insertRow( r, cItem, mItem ? mItem->parent() : 0 );
		// if item is a scope add it s endscope
		if ( mNodeType == AbstractProjectModel::ScopeType || mNodeType == AbstractProjectModel::NestedScopeType )
			(void) new QMakeProjectItem( AbstractProjectModel::ScopeEndType, cItem );
		// transfer pointer
		mItem = cItem;
	}
	// update item data
	if ( mItem )
		for ( int i = 0; i < twValueRoles->topLevelItemCount(); i++ )
			mItem->setData( twValueRoles->topLevelItem( i )->text( 1 ), twValueRoles->topLevelItem( i )->data( 0, Qt::UserRole +1 ).toInt() );
	// set the item the current one
	if ( d )
		d->setCurrentIndex( mProject->indexFromItem( mItem ) );
	//
	QDialog::accept();
}
