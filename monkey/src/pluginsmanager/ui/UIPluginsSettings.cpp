/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UIPluginsSettings.cpp
** Date      : 2007-11-04T22:45:25
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "UIPluginsSettings.h"
#include "PluginsManager.h"
#include "pSettings.h"

#include <QMetaEnum>

UIPluginsSettings::UIPluginsSettings( QWidget* p )
	: QDialog( p ), mPluginsManager( PluginsManager::instance() )
{
	// if no plugins manager break app
	Q_ASSERT( mPluginsManager != 0 );
	
	// setup dialog
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	
	// fill list with plugins type
	const QMetaObject mo = BasePlugin::staticMetaObject;
	QMetaEnum e = mo.enumerator( mo.indexOfEnumerator( "Type" ) );
	for ( int i = 0; i < e.keyCount() -1; i++ )
		cbType->addItem( e.key( i ), e.value( i ) );
	// update plugins list
	updateList();
	
	// activate first plugin
	if ( lwNames->count() )
		lwNames->item( 0 )->setSelected( true );
}

void UIPluginsSettings::clearInfos()
{
	leCaption->clear();
	leName->clear();
	leVersion->clear();
	leType->clear();
	leAuthor->clear();
	teDescription->clear();
	cbEnableUponStart->setChecked( false );
	cbEnabled->setChecked( false );
	swWidgets->setCurrentIndex( -1 );
}

void UIPluginsSettings::updateList()
{
	// clear pages
	while ( swWidgets->count() )
		delete swWidgets->widget( 0 );
	// clear items
	lwNames->clear();
	// create items
	for ( int i = 0; i < mPluginsManager->plugins().count(); i++ )
	{
		BasePlugin* bp = mPluginsManager->plugins().at( i ); // got plugin
		QListWidgetItem* it = new QListWidgetItem( bp->infos().Name, lwNames ); // create item
		it->setData( Qt::UserRole, i ); // plugin index
		it->setData( Qt::UserRole +1, swWidgets->addWidget( bp->settingsWidget() ) ); // settings widget index
		it->setData( Qt::UserRole +2, bp->infos().Type ); // plugin type
		it->setData( Qt::UserRole +3, pSettings::instance()->value( QString( "Plugins/%1" ).arg( bp->infos().Name ), true ).toBool() ); // plugin auto install
	}
}

void UIPluginsSettings::on_cbType_currentIndexChanged( int i )
{
	// show/hide item according to type
	for ( int j = 0; j < lwNames->count(); j++ )
	{
		QListWidgetItem* it = lwNames->item( j ); // get item
		int pt = it->data( Qt::UserRole +2 ).toInt(); // get item plugin type
		int ct = cbType->itemData( i ).toInt(); // get current visible type
		it->setHidden( ( ct != BasePlugin::iAll && ct != pt ) ? true : false ); // show or hide the plugin
		it->setSelected( false );
	}
	
	// select the first visible item if current is hidden
	if ( lwNames->count() )
	{
		for ( int j = 0; j < lwNames->count(); j++ )
		{
			QListWidgetItem* it = lwNames->item( j ); // get item
			if ( !it->isHidden() )
			{
				it->setSelected( true );
				return;
			}
		}
	}
	
	// in case of no entry/plugins
	clearInfos();
}

void UIPluginsSettings::on_lwNames_itemSelectionChanged()
{
	// get item
	QListWidgetItem* i = lwNames->selectedItems().value( 0 );
	
	// if not item return
	if ( !i )
		return;
	
	// update plugin infos
	BasePlugin::PluginInfos pi = mPluginsManager->plugins().at( i->data( Qt::UserRole ).toInt() )->infos();
	leCaption->setText( pi.Caption );
	leName->setText( pi.Name );
	leVersion->setText( pi.Version );
	leType->setText( QString::number( pi.Type ) );
	leAuthor->setText( pi.Author );
	teDescription->setPlainText( pi.Description );
	cbEnableUponStart->setChecked( i->data( Qt::UserRole +3 ).toBool() );
	cbEnabled->setChecked( pi.Enabled );
	swWidgets->setCurrentIndex( i->data( Qt::UserRole +1 ).toInt() );
}

void UIPluginsSettings::on_cbEnableUponStart_clicked( bool b )
{
	// get item
	QListWidgetItem* it = lwNames->selectedItems().value( 0 );
	
	// if item and visible
	if ( it && !it->isHidden() )
		it->setData( Qt::UserRole +3, b );
}

void UIPluginsSettings::on_cbEnabled_clicked( bool b )
{
	// get item
	QListWidgetItem* it = lwNames->selectedItems().value( 0 );
	
	// if item and visible
	if ( it && !it->isHidden() )
	{
		// get plugin
		BasePlugin* bp = mPluginsManager->plugins().at( lwNames->row( it ) );
		// enable it according to b
		bp->setEnabled( b );
		// update check state
		cbEnabled->setChecked( bp->isEnabled() );
	}
}

void UIPluginsSettings::accept()
{
	// save auto install states
	for ( int i = 0; i < lwNames->count(); i++ )
	{
		QListWidgetItem* it = lwNames->item( i );
		pSettings::instance()->setValue( QString( "Plugins/%1" ).arg( it->text() ), it->data( Qt::UserRole +3 ).toBool() );
	}
	
	// close dialog
	QDialog::accept();
}
