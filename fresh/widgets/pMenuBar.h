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
	\file pMenuBar.h
	\date 2008-01-14T00:27:47
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief An extended QMenuBar
*/
#ifndef PMENUBAR_H
#define PMENUBAR_H

#include "../objects/MonkeyExport.h"

#include <QMenuBar>
#include <QHash>
#include <QStack>
#include <QMenu>
#include <QAction>
#include <QIcon>

/*!
	\brief An internal class used by pMenuBar
	\details It allow to manage valued path ( ie: QSettings like )
*/
class Q_MONKEY_EXPORT pMenuBarGroup
{
public:
	inline pMenuBarGroup()
		: num( -1 ), maxNum( -1 ) {}
	inline pMenuBarGroup( const QString& s )
		: str( s ), num( -1 ), maxNum( -1 ) {}
	inline pMenuBarGroup( const QString& s, bool guessArraySize )
		: str( s ), num( 0 ), maxNum( guessArraySize ? 0 : -1 ) {}
	//
	inline QString name() const { return str; }
	inline QString toString() const;
	inline bool isArray() const { return num != -1; }
	inline int arraySizeGuess() const { return maxNum; }
	inline void setArrayIndex( int i )
		{ num = i +1; if ( maxNum != -1 && num > maxNum ) maxNum = num; }
	//
	QString str;
	int num;
	int maxNum;
};

inline QString pMenuBarGroup::toString() const
{
	QString result;
	result = str;
	if ( num > 0 )
	{
		result += QLatin1Char( '/' );
		result += QString::number( num );
	}
	return result;
}

/*!
	\brief An extended QMenuBar
	\details This menu bar is working like a QSettings, you can get action/menu on the fly
	\details with call like this : menu->action( "mFile/aSave" );
	\details If the path is not existing then the action is created.
*/
class Q_MONKEY_EXPORT pMenuBar : public QMenuBar
{
	Q_OBJECT

public:
	pMenuBar( QWidget* parent = 0 );
	
	QAction* searchAction( QMenu* menu, const QString& name );

	static QString normalizedKey( const QString& key );
	void beginGroupOrArray( const pMenuBarGroup& group );
	void beginGroup( const QString& group );
	QString group() const;
	void endGroup();

	QString fixedPath( const QString& path, bool prependPrefix = false );

	QAction* action( const QString& path, const QString& text = QString::null, const QIcon& icon = QIcon(), const QString& shortcut = QString::null, const QString& toolTip = QString::null );
	QMenu* menu( const QString& path = QString::null, const QString& title = QString::null, const QIcon& icon = QIcon() );

	void clearMenu( const QString& path = QString::null );
	void deleteMenu( const QString& path = QString::null );
	void setMenuEnabled( QMenu* menu, bool enabled );
	
	Qt::ShortcutContext defaultShortcutContext() const;
	void setDefaultShortcutContext( Qt::ShortcutContext context );

private:
	QString mMenuGroup;
	QHash<QString, QMenu*> mMenus;
	QStack<pMenuBarGroup> groupStack;
	QString groupPrefix;
	Qt::ShortcutContext mDefaultShortcutContext;
};

#endif // PMENUBAR_H
