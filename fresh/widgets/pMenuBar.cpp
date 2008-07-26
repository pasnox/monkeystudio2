/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pMenuBar.cpp
** Date      : 2008-01-14T00:27:47
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
#include "pMenuBar.h"
#include "pAction.h"

/*!
	\brief Create a new pMenuBar object
	\param parent The parent widget
*/
pMenuBar::pMenuBar( QWidget* parent )
	: QMenuBar( parent ), mMenuGroup( tr( "Main Menu" ) )
{
	mDefaultShortcutContext = Qt::WindowShortcut;
}

/*!
	\details Return an action having corresponding name in menu.
	\details If the name is like aSeparatorXXX or XXX represent a number
	\details a new separator is returned if it's not found.
	\details If name is not found, and don't contains separator mask, null is returned.
	\param menu The QMenu to scan
	\param name The action name to search
*/
QAction* pMenuBar::searchAction( QMenu* menu, const QString& name )
{
	foreach ( QAction* action, menu->actions() )
		if ( action->objectName().toLower() == name.toLower() )
			return action;

	if ( name.contains( QRegExp( "^aseparator\\d{1,2}$", Qt::CaseInsensitive ) ) )
		return menu->addSeparator();

	return 0;
}

QString pMenuBar::normalizedKey( const QString& key )
{
	QString r = key;

	int i = 0;
	while ( i < r.size() )
	{
		while ( r.at( i ) == QLatin1Char( '/' ) )
		{
			r.remove( i, 1 );
			if ( i == r.size() )
				goto after_loop;
		}
		while ( r.at( i ) != QLatin1Char( '/' ) )
		{
			++i;
			if ( i == r.size() )
				return r;
		}
		++i; // leave the slash alone
	}

after_loop:
	if ( !r.isEmpty() )
		r.truncate( i -1 ); // remove the trailing slash
	return r;
}

void pMenuBar::beginGroupOrArray( const pMenuBarGroup& group )
{
	groupStack.push( group );
	if ( !group.name().isEmpty() )
	{
		groupPrefix += group.name();
		groupPrefix += QLatin1Char( '/' );
	}
}

/*!
	\details Begin a \c group
	\see QSettings::beginGroup()
*/
void pMenuBar::beginGroup( const QString& group )
{
	beginGroupOrArray( pMenuBarGroup( normalizedKey( group ) ) );
}

/*!
	\details Return the current group
	\see QSettings::group()
*/
QString pMenuBar::group() const
{
	return groupPrefix.left( groupPrefix.size() -1 );
}

/*!
	\details End a group
	\see QSettings::endGroup()
*/
void pMenuBar::endGroup()
{
	if ( groupStack.isEmpty() )
	{
		qWarning( "pMenuBar::endGroup: No matching beginGroup()" );
		return;
	}

	pMenuBarGroup g = groupStack.pop();
	int len = g.toString().size();
	if ( len > 0 )
		groupPrefix.truncate( groupPrefix.size() -( len +1 ) );

	if ( g.isArray() )
		qWarning( "pMenuBar::endGroup: Expected endArray() instead" );
}

/*!
	\details Return a fixed path
	\param path The path to fix
	\param prependPrefix If true, the current prefix is prepend, else nothing is prepend.
*/
QString pMenuBar::fixedPath( const QString& path, bool prependPrefix )
{
	QString mString = path;
	if ( prependPrefix )
		mString.prepend( groupPrefix );
	mString.replace( '\\', '/' );
	if ( mString.endsWith( '/' ) )
		mString.chop( 1 );
	return mString;
}

/*!
	\details Return the action at \c path
	\details If the path doesn't exists, the action is created.
	\param path The path of the action to get
	\param text The action text 
	\param icon The action icon
	\param shortcut The action shortcut
	\param toolTip The action tooltip/statusTip
*/
QAction* pMenuBar::action( const QString& path, const QString& text, const QIcon& icon, const QString& shortcut, const QString& toolTip )
{
	QString mString = fixedPath( path );
	QString mName = mString.mid( mString.lastIndexOf( '/' ) +1 );
	QString mPath;

	if ( mString.contains( '/' ) )
		mPath = mString.mid( 0, mString.lastIndexOf( '/' ) );

	QMenu* foundMenu = menu( mPath );
	QMenu* tmpMenu = foundMenu;
	QAction* action = searchAction( foundMenu, mName );

	// create action if needed
	if ( !action )
	{
		// get group
		QString group;
		while ( tmpMenu && !tmpMenu->title().isEmpty() )
		{
			group.prepend( tmpMenu->title() +"/" );
			tmpMenu = qobject_cast<QMenu*>( tmpMenu->parentWidget() );
		}

		// add main menu group
		group.prepend( mMenuGroup +"/" );

		// create action
		action = new pAction( mName, icon, text, QKeySequence( shortcut ), group );
		action->setParent( foundMenu );
		action->setShortcutContext( mDefaultShortcutContext );
		foundMenu->addAction( action );

		if ( !toolTip.isEmpty() )
			action->setStatusTip( toolTip );

		if ( !toolTip.isEmpty() )
			action->setToolTip( toolTip );
	}

	// return action
	return action;
}

/*!
	\details Return a QMenu at \c path.
	\details If the path don't exists, the menu is created and
	\details title and icon are used as meu properties.
	\param path The QMenu path to get
	\param title The menu title
	\param icon The menu icon
*/
QMenu* pMenuBar::menu( const QString& path, const QString& title, const QIcon& icon )
{
	QString mString = fixedPath( path, true );
	QMenu* mMenu = 0L;
	if ( !mMenus.contains( mString ) )
	{
		QStringList mMenuList = mString.split( '/' );
		mString.clear();
		for ( int i = 0; i < mMenuList.count(); i++ )
		{
			mString += '/' + mMenuList[ i ];
			if ( mString[ 0 ] == '/' )
				mString.remove( 0, 1 );
			if ( !mMenus.contains( mString ) )
			{
				mMenus[ mString ] = !mString.contains( '/' ) ? addMenu( mMenuList[ i ] ) : mMenus[ mString.left( mString.lastIndexOf( '/' ) ) ]->addMenu( mMenuList[ i ] );
				mMenus[ mString ]->setObjectName( mMenuList[ i ] );
				mMenus[ mString ]->setTitle( mString );
			}
		}
	}
	mMenu = mMenus[ mString ];
	if ( !title.isEmpty() && mMenu->title() != title )
		mMenu->setTitle( title );
	if ( !icon.isNull() && mMenu->icon().cacheKey() != icon.cacheKey() )
		mMenu->setIcon( icon );

	return mMenu;
}

/*!
	\details Clear the menu at \c path
	\param path The menu path
*/
void pMenuBar::clearMenu( const QString& path )
{
	QString mPath = fixedPath( path, true );
	if ( mMenus.contains( mPath ) )
		mMenus[ mPath ]->clear();
}

/*!
	\details Delete the menu at \c path
	\param path The menu path
*/
void pMenuBar::deleteMenu( const QString& path )
{
	QString mPath = fixedPath( path, true );
	if ( mMenus.contains( mPath ) )
	{
		mMenus[ mPath ]->clear();
		delete mMenus.take( mPath );
	}
}

/*!
	\details Enable/Disable the menu at \c path.
	\details This cause all actions of a menu to be recursively disabled/enabled.
	\param menu The menu to enable/disable
	\param enabled If true, enable menu, else disable it
*/
void pMenuBar::setMenuEnabled( QMenu* menu, bool enabled )
{
	if ( menu )
	{
		foreach ( QAction* action, menu->actions() )
		{
			action->setEnabled( enabled );
			if ( action->menu() )
				setMenuEnabled( action->menu(), enabled );
		}
		//menu->menuAction()->setEnabled( enabled );
	}
}

/*!
	\details Return the default ShortcutContext used by actions created by the class
*/
Qt::ShortcutContext pMenuBar::defaultShortcutContext() const
{ return mDefaultShortcutContext; }

/*!
	\details Set the default ShortcutContext used by actions created by the class
	\param context The default ShortcutContext
*/
void pMenuBar::setDefaultShortcutContext( Qt::ShortcutContext context )
{ mDefaultShortcutContext = context; }
