/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pMenuBar.h
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



#ifndef PMENUBAR_H
#define PMENUBAR_H

#include "MonkeyExport.h"

#include <QMenuBar>
#include <QHash>
#include <QStack>
#include <QMenu>
#include <QAction>
#include <QIcon>

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

class Q_MONKEY_EXPORT pMenuBar : public QMenuBar
{
	Q_OBJECT

public:
	pMenuBar( QWidget* = 0 );
	static QString normalizedKey( const QString& );
	void beginGroupOrArray( const pMenuBarGroup& );
	void beginGroup( const QString& );
	QString group() const;
	void endGroup();

	QString fixedPath( const QString&, bool = false );

	QAction* action( const QString&, const QString& = QString::null, const QIcon& = QIcon(), const QString& = QString::null, const QString& = QString::null );
	QMenu* menu( const QString& = QString::null, const QString& = QString::null, const QIcon& = QIcon() );

	void clearMenu( const QString& = QString::null );
	void deleteMenu( const QString& = QString::null );

private:
	QAction* searchAction( QMenu*, const QString& );

	QString mMenuGroup;
	QHash<QString, QMenu*> mMenus;
	QStack<pMenuBarGroup> groupStack;
	QString groupPrefix;

};

#endif // PMENUBAR_H
