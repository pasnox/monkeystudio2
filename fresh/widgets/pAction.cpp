/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pAction.cpp
** Date      : 2008-01-14T00:27:38
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

pAction::~pAction()
{
	// remove from group
	pActionManager::removeAction( property( _GROUP_PROPERTY_ ).toString(), this );
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
