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
#include "pAction.h"

const char* pAction::_DEFAULT_SHORTCUT_PROPERTY_ = "Default Shortcut";
const char* pAction::_GROUP_PROPERTY_ = "Group";
const char* pAction::_SETTINGS_SCOPE_ = "Shortcuts Manager";

/*!
	\brief Construct a new pAction
	\param name The action internal name
	\param text The action text
	\param keySequence The action shortcut
	\param group The action group
*/
pAction::pAction( const QString& name, const QString& text, const QKeySequence& keySequence, const QString& group )
	: QAction( pActionManager::instance() )
{
	// set object name
	setObjectName( name );

	// add to group
	pActionManager::addAction( group, this );

	// set action text
	setText( text );

	// set action shortcut
	setProperty( _DEFAULT_SHORTCUT_PROPERTY_, keySequence.toString() );
	setShortcut( pActionManager::getShortcut( group, this, keySequence ) );
}

/*!
	\brief Construct a new pAction
	\param name The action internal name
	\param icon The action icon
	\param text The action text
	\param keySequence The action shortcut
	\param group The action group
*/
pAction::pAction( const QString& name, const QIcon& icon, const QString& text, const QKeySequence& keySequence, const QString& group )
	: QAction( pActionManager::instance() )
{
	// set object name
	setObjectName( name );

	// fixed group
	QString cg = fixedString( group );

	// add to group
	pActionManager::addAction( cg, this );

	// set icon
	setIcon( icon );

	// set action text
	setText( text );

	// set action shortcut
	setProperty( _DEFAULT_SHORTCUT_PROPERTY_, keySequence.toString() );
	setShortcut( pActionManager::getShortcut( cg, this, keySequence ) );
}

pAction::~pAction()
{
	// remove from group
	pActionManager::removeAction( property( _GROUP_PROPERTY_ ).toString(), this );
}

/*!
	\details Return a string that is proper to show
	\param string The string to fix
*/
QString pAction::fixedString( const QString& string )
{
	QString p = string;
	
	// remove trailing /
	if ( p.endsWith( "/" ) )
		p.chop( 1 );

	// remove &
	p.replace( "&", "" );

	// return correct string
	return p;
}
