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
#include "pActionManager.h"
#include "pAction.h"
#include "pShortcutsEditor.h"
#include "../objects/pSettings.h"

#include <QApplication>

/*!
	\details Create an action manager
	\param parent The parent object
*/
pActionManager::pActionManager( QObject* parent )
	: QObject( parent ), mError( QString() ), mSettings( 0 )
{}

/*!
	\details Set the settings object used by this action manager
	\param settings The settings object
*/
void pActionManager::setSettings( pSettings* settings )
{
	if ( instance()->mSettings != settings )
	{
		instance()->mSettings = settings;
		instance()->reloadSettings();
	}
}

/*!
	\details Return the settings object used by this action manager, can be null
*/
pSettings* pActionManager::settings()
{ return instance()->mSettings; }

/*!
	\details Reload all action properties
*/
void pActionManager::reloadSettings()
{
	// restore shortcut for each action
	if ( pSettings* s = instance()->settings() )
	{
		foreach ( pActionList al, instance()->mActions )
		{
			foreach ( QAction* a, al )
				a->setShortcut( QKeySequence( s->value( QString( "%1/%2/%3" ).arg( pAction::_SETTINGS_SCOPE_ ).arg( a->property( pAction::_GROUP_PROPERTY_ ).toString() ).arg( a->objectName() ), a->property( pAction::_DEFAULT_SHORTCUT_PROPERTY_ ) ).toString() ) );
		}
	}
}

/*!
	\details Add an action to a group
	\param group The group to add the action to
	\param action The action to add
	\return The added action
*/
QAction* pActionManager::addAction( const QString& group, QAction* action )
{
	// need group
	Q_ASSERT( !group.isEmpty() );
	// need action
	Q_ASSERT( action );
	// action name is require
	Q_ASSERT( !action->objectName().isEmpty() );
	// addactions to group if it donesn't contains already
	if ( !actions()[group].contains( action ) )
	{
		action->setProperty( pAction::_GROUP_PROPERTY_, group );
		instance()->mActions[group] << action;
	}
	// return action
	return action;
}

/*!
	\details Remove an action from a group
	\param group The group to remove the action of
	\param action The action to remove
*/
void pActionManager::removeAction( const QString& group, QAction* action )
{
	// need group
	Q_ASSERT( !group.isEmpty() );
	// need action
	Q_ASSERT( action );
	// action name is require
	Q_ASSERT( !action->objectName().isEmpty() );
	// addactions to group if it donesn't contains already
	if ( actions()[group].contains( action ) )
		instance()->mActions[group].removeAll( action );
}

/*!
	\details Return the hash actions container
*/
pHashActionList pActionManager::actions()
{ return instance()->mActions; }

/*!
	\details Return the default global group
*/
QString pActionManager::globalGroup()
{ return tr( "Global" ); }

/*!
	\details Return an action key sequence from settings or the default one.
	\details Return a null key sequence if action or group is invalid.
	\param group The action group
	\param action The action
	\param  keySequence The default key sequence returned if the settings don't contains entry for this group/action
*/
QKeySequence pActionManager::getShortcut( const QString& group, QAction* action, const QKeySequence& keySequence )
{
	// need group
	Q_ASSERT( !group.isEmpty() );
	// need action
	Q_ASSERT( action != 0 );
	// return defined shortcuts or the default one
	pSettings* settings = instance()->settings();
	return settings ? QKeySequence( settings->value( QString( "%1/%2/%3" ).arg( pAction::_SETTINGS_SCOPE_ ).arg( group ).arg( action->objectName() ), keySequence ).toString() ) : QKeySequence();
}

/*!
	\details Try to set a action key sequence, return true on success, else false.
	\details If there is an error, yo ucan read it using lastError.
	\param action The action to set the shortcut too
	\param keySequence The key sequence to set
*/
bool pActionManager::setShortcut( QAction* action, const QKeySequence& keySequence )
{
	// need action
	Q_ASSERT( action != 0 );
	// try to found already used shortcut
	foreach ( pActionList al, instance()->mActions )
	{
		foreach ( QAction* a, al )
		{
			if ( a != action && a->shortcut() == keySequence && !keySequence.isEmpty() )
			{
				instance()->mError = tr( "Key Sequence '%1' already assigned to the '%2/%3'" ).arg( keySequence.toString() ).arg( a->property( pAction::_GROUP_PROPERTY_ ).toString() ).arg( a->text() );
				return false;
			}
		}
	}
	// remove old/set shortcut entry
	if ( pSettings* s = instance()->settings() )
	{
		if ( action->shortcut() == keySequence )
			s->remove( QString( "%1/%2/%3" ).arg( pAction::_SETTINGS_SCOPE_ ).arg( action->property( pAction::_GROUP_PROPERTY_ ).toString() ).arg( action->objectName() ) );
		else
			s->setValue( QString( "%1/%2/%3" ).arg( pAction::_SETTINGS_SCOPE_ ).arg( action->property( pAction::_GROUP_PROPERTY_ ).toString() ).arg( action->objectName() ), keySequence.toString() );
	}
	// set new shortcut to action
	action->setShortcut( keySequence );
	// return success
	return true;
}

/*!
	\details Return the last error string
*/
QString pActionManager::lastError()
{ return instance()->mError; }

/*!
	\details Show the action key sequence editor
*/
void pActionManager::showSettings()
{ pShortcutsEditor::instance( QApplication::activeWindow() )->exec(); }
