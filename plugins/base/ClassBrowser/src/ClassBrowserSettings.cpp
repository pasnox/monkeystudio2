/****************************************************************************
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
****************************************************************************/
/*!
	\file ClassBrowserSettings.cpp
	\date 2009-05-01
	\author Filipe AZEVEDO
	\brief Settings widget of ClassBrowser plugin
*/
#include "ClassBrowserSettings.h"
#include "ClassBrowser.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

/*!
	Creates settings widget
	\param plugin Pointer to ClassBrowser plugin
	\param parent Parent widget of settings widget
*/
ClassBrowserSettings::ClassBrowserSettings( ClassBrowser* plugin, QWidget* parent )
	: QWidget( parent )
{
	// retain plugin
	mPlugin = plugin;
	
	// list editor
	mEditor = new pPathListEditor( this, tr( "System include paths" ) );
	mEditor->setValues( plugin->systemPaths() );
	
	// apply button
	QDialogButtonBox* dbbApply = new QDialogButtonBox( this );
	dbbApply->addButton( QDialogButtonBox::Apply );
	
	// global layout
	QVBoxLayout* vbox = new QVBoxLayout( this );
	vbox->addWidget( mEditor );
	vbox->addWidget( dbbApply );
	
	// connections
	connect( dbbApply->button( QDialogButtonBox::Apply ), SIGNAL( clicked() ), this, SLOT( applySettings() ) );
}

/*!
	Handler of clicking Apply button. Applying settings
*/
void ClassBrowserSettings::applySettings()
{
	mPlugin->setSystemPaths( mEditor->values() );
}
