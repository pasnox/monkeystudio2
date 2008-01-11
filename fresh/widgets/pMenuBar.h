/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pMenuBar.h
 * LICENSE      : GPL
 * COMMENTARY   : This class allow you creating a menu bar using path for menus & actions, ie :
	For creating action File->Open simply use :
		class->menu( "mFile", tr( "&File" ) );
		class->action( "mFile/aOpen" );
	For creating menu File->Recents simply use :
		class->menu( "mFile", tr( "&File" ) );
		class->menu( "mFile/mRecents", tr( "&Recents" ) );
	This class use some modified code ( group members ) from Trolltech SA (c) QSettings.
 ********************************************************************************************************/
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
