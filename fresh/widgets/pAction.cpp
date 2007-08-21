/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com ), Kopats Andrei ( hlamer@tut.by )
 * FILENAME     : pAction.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class allow to create action that can be manage by the shortcut manager
 ********************************************************************************************************/
#include "pAction.h"

const char* pAction::_DEFAULT_SHORTCUT_PROPERTY_ = "Default Shortcut";
const char* pAction::_GROUP_PROPERTY_ = "Group";
const char* pAction::_SETTINGS_SCOPE_ = "Shortcuts Manager";

pAction::pAction( const QString& n, const QString& s, const QKeySequence& sc, const QString& g )
	: QAction( pActionManager::instance() )
{
	// set object name
	setObjectName( n );

	// add to group
	pActionManager::addAction( g, this );

	// set action text
	setText( s );

	// set action shortcut
	setProperty( _DEFAULT_SHORTCUT_PROPERTY_, sc.toString() );
	setShortcut( pActionManager::getShortcut( g, this, sc ) );
}

pAction::pAction( const QString& n, const QIcon& i, const QString& s, const QKeySequence& sc, const QString& g )
	: QAction( pActionManager::instance() )
{
	// set object name
	setObjectName( n );

	// fixed group
	QString cg = fixedString( g );

	// add to group
	pActionManager::addAction( cg, this );

	// set icon
	setIcon( i );

	// set action text
	setText( s );

	// set action shortcut
	setProperty( _DEFAULT_SHORTCUT_PROPERTY_, sc.toString() );
	setShortcut( pActionManager::getShortcut( cg, this, sc ) );
}

QString pAction::fixedString( const QString& s )
{
	QString p = s;
	
	// remove trailing /
	if ( p.endsWith( "/" ) )
		p.chop( 1 );

	// remove &
	p.replace( "&", "" );

	// return correct string
	return p;
}
