/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UIPluginsSettings.cpp
** Date      : 2008-01-14T00:37:00
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
#include "UIPluginsSettings.h"
#include "UIPluginsSettingsElement.h"
#include "../../coremanager/MonkeyCore.h"
#include "../PluginsManager.h"

#include <QMetaEnum>
#include <QDebug>

UIPluginsSettings::UIPluginsSettings( QWidget* p )
	: QDialog( p ), mPluginsManager( MonkeyCore::pluginsManager() )
{
	// setup dialog
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	
	// fill list with plugins type
	const QMetaObject mo = BasePlugin::staticMetaObject;
	QMetaEnum e = mo.enumerator( mo.indexOfEnumerator( "Type" ) );
	for ( int i = 0; i < e.keyCount() -1; i++ )
		cbPluginType->addItem( e.key( i ), e.value( i ) );
	
	// update plugins list
	updateList();
}

void UIPluginsSettings::updateList()
{
	// clear list
	lwPlugins->clear();
	
	// create items and editor foreach plugin
	foreach ( BasePlugin* bp, mPluginsManager->plugins() )
	{
		UIPluginsSettingsElement* pse = new UIPluginsSettingsElement( bp, this );
		QListWidgetItem* item = new QListWidgetItem( lwPlugins );
		item->setSizeHint( pse->sizeHint() );
		lwPlugins->setItemWidget( item, pse );
	}
}

void UIPluginsSettings::on_cbPluginType_currentIndexChanged( int id )
{
	// clear selection
	lwPlugins->clearSelection();
	lwPlugins->setCurrentItem( 0 );
	// get current type
	BasePlugin::Type mType = (BasePlugin::Type)cbPluginType->itemData( id ).toInt();
	// show/hide item according to type
	for ( int i = 0; i < lwPlugins->count(); i++ )
	{
		QListWidgetItem* item = lwPlugins->item( i );
		UIPluginsSettingsElement* pse = qobject_cast<UIPluginsSettingsElement*>( lwPlugins->itemWidget( item ) );
		item->setHidden( mType != BasePlugin::iAll && !pse->plugin()->infos().Type.testFlag( mType ) );
		qWarning() << "plugin:" << pse->plugin()->infos().Caption << "type:" << pse->plugin()->infos().Type << "mtype:" << mType << "test:" << pse->plugin()->infos().Type.testFlag( mType );
	}
}

void UIPluginsSettings::on_lwNames_itemSelectionChanged()
{
	/*
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
	*/
}

void UIPluginsSettings::on_cbEnableUponStart_clicked( bool b )
{
	/*
	// get item
	QListWidgetItem* it = lwNames->selectedItems().value( 0 );
	
	// if item and visible
	if ( it && !it->isHidden() )
		it->setData( Qt::UserRole +3, b );
		*/
}

void UIPluginsSettings::on_cbEnabled_clicked( bool b )
{
	/*
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
	*/
}

void UIPluginsSettings::accept()
{
	/*
	// save auto install states
	for ( int i = 0; i < lwNames->count(); i++ )
	{
		QListWidgetItem* it = lwNames->item( i );
		MonkeyCore::settings()->setValue( QString( "Plugins/%1" ).arg( it->text() ), it->data( Qt::UserRole +3 ).toBool() );
	}
	
	// close dialog
	QDialog::accept();
	*/
}
