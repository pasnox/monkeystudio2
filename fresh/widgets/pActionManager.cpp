/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pActionManager.cpp
** Date      : 2008-01-14T00:27:39
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



#include "pActionManager.h"
#include "pAction.h"
#include "pShortcutsEditor.h"
#include "pSettings.h"

#include <QApplication>

pActionManager::pActionManager( QObject* o )
	: QObject( o ), mError( QString() ), mSettings( 0 )
{}

void pActionManager::setSettings( pSettings* s )
{
	if ( instance()->mSettings != s )
	{
		instance()->mSettings = s;
		instance()->reloadSettings();
	}
}

pSettings* pActionManager::settings()
{ return instance()->mSettings; }

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

QAction* pActionManager::addAction( const QString& g, QAction* a )
{
	// need group
	Q_ASSERT( !g.isEmpty() );
	// need action
	Q_ASSERT( a );
	// action name is require
	Q_ASSERT( !a->objectName().isEmpty() );
	// addactions to group if it donesn't contains already
	if ( !actions()[g].contains( a ) )
	{
		a->setProperty( pAction::_GROUP_PROPERTY_, g );
		instance()->mActions[g] << a;
	}
	// return action
	return a;
}

void pActionManager::removeAction( const QString& g, QAction* a )
{
	// need group
	Q_ASSERT( !g.isEmpty() );
	// need action
	Q_ASSERT( a );
	// action name is require
	Q_ASSERT( !a->objectName().isEmpty() );
	// addactions to group if it donesn't contains already
	if ( actions()[g].contains( a ) )
		instance()->mActions[g].removeAll( a );
}

pHashActionList pActionManager::actions()
{ return instance()->mActions; }

QString pActionManager::globalGroup()
{ return tr( "Global" ); }

QKeySequence pActionManager::getShortcut( const QString& g, QAction* a, const QKeySequence& sc )
{
	// need group
	Q_ASSERT( !g.isEmpty() );
	// need action
	Q_ASSERT( a != 0 );
	// return defined shortcuts or the default one
	pSettings* s = instance()->settings();
	return s ? QKeySequence( s->value( QString( "%1/%2/%3" ).arg( pAction::_SETTINGS_SCOPE_ ).arg( g ).arg( a->objectName() ), sc ).toString() ) : QKeySequence();
}

bool pActionManager::setShortcut( QAction* action, const QKeySequence& sc )
{
	// need action
	Q_ASSERT( action != 0 );
	// try to found already used shortcut
	foreach ( pActionList al, instance()->mActions )
	{
		foreach ( QAction* a, al )
		{
			if ( a != action && a->shortcut() == sc && !sc.isEmpty() )
			{
				instance()->mError = tr( "Key Sequence '%1' already assigned to the '%2/%3'" ).arg( sc.toString() ).arg( a->property( pAction::_GROUP_PROPERTY_ ).toString() ).arg( a->text() );
				return false;
			}
		}
	}
	// remove old/set shortcut entry
	if ( pSettings* s = instance()->settings() )
	{
		if ( action->shortcut() == sc )
			s->remove( QString( "%1/%2/%3" ).arg( pAction::_SETTINGS_SCOPE_ ).arg( action->property( pAction::_GROUP_PROPERTY_ ).toString() ).arg( action->objectName() ) );
		else
			s->setValue( QString( "%1/%2/%3" ).arg( pAction::_SETTINGS_SCOPE_ ).arg( action->property( pAction::_GROUP_PROPERTY_ ).toString() ).arg( action->objectName() ), sc.toString() );
	}
	// set new shortcut to action
	action->setShortcut( sc );
	// return success
	return true;
}

QString pActionManager::lastError()
{ return instance()->mError; }

void pActionManager::showSettings()
{ pShortcutsEditor::instance( QApplication::activeWindow() )->exec(); }
