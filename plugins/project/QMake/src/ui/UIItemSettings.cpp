#include "UIItemSettings.h"
#include "ProjectItem.h"
#include "ProjectsModel.h"
#include "UIQMakeProjectSettings.h"
#include "QMakeItemDelegate.h"

#include <QMetaEnum>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QFontMetrics>

QPointer<UIItemSettings> UIItemSettings::mSelf = 0L;

UIItemSettings* UIItemSettings::edit( ProjectsModel* p, ProjectItem* i, QWidget* w )
{
	if ( !mSelf )
		mSelf = new UIItemSettings( p, i, w );
	return mSelf;
}
//
UIItemSettings::UIItemSettings( ProjectsModel* p, ProjectItem* pi, QWidget* w )
	: QDialog( w ), mItem( pi ), mProject( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
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
//
void UIItemSettings::accept()
{
	/*
	// got parent window
	UIQMakeProjectSettings* d = qobject_cast<UIQMakeProjectSettings*>( parentWidget() );
	// if no valid index, create one
	if ( !mItem && d )
	{
		// get the current index in the project settings
		mItem = static_cast<ProjectItem*>( mProject->itemFromIndex( d->currentIndex() ) );
		// get it s row and rowcount
		int r = mItem ? mItem->row() : 0;
		int rc = mItem ? mItem->rowCount() : mProject->rowCount();
		// if item is a scope, remove 1 to rc
		if ( mItem && ( mItem->data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ScopeType || mItem->data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::NestedScopeType ) )
			rc--;
		// if parent is a scope, remove 1 to r
		if ( mItem && ( mItem->data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::ScopeType || mItem->data( ProjectsModel::TypeRole ).toInt() == ProjectsModel::NestedScopeType ) )
			r--;
		// create the new item
		ProjectsModel::NodeType mNodeType = ( ProjectsModel::NodeType )twValueRoles->topLevelItem( 0 )->data( 1, Qt::DisplayRole ).toInt();
		ProjectItem* cItem = new ProjectItem( mNodeType );
		// ask which kind of item we must create
		if ( QMessageBox::question( this, tr( "New Item.." ), tr( "Create item as a child of the current item ?" ), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes )
		// as child
			mProject->insertRow( rc, cItem, mItem );
		else
			mProject->insertRow( r, cItem, mItem ? mItem->parent() : 0 );
		// if item is a scope add it s endscope
		if ( mNodeType == ProjectsModel::ScopeType || mNodeType == ProjectsModel::NestedScopeType )
			(void) new ProjectItem( ProjectsModel::ScopeEndType, cItem );
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
	*/
	QDialog::accept();
}
