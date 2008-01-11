/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com ), Kopats Andrei ( hlamer@tut.by )
 * FILENAME     : pActionManager.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class manage the pAction so we can ease change action's shortcuts
 ********************************************************************************************************/
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
