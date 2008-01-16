/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : QSingleton.h
** Date      : 2008-01-14T00:27:37
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
/*
	How to use :

	// single sur un QPushButton
	class MyPushButon : public QPushButton, public QSingleton<MyPushButon>
	{
  		friend class QSingleton<MyPushButon>;

	protected:
		MyPushButon( QWidget* p )
		: QPushButton( p ), QSingleton<MyPushButon>()
		{};
	};
	// in cpp
	MyPushButton* pb = MyPushButton::instance( parentWidget ); // initialise object
	// then u can retreive it with MyPushButton::instance() or using pb pointer
*/
#ifndef QSINGLETON_H
#define QSINGLETON_H

#include "MonkeyExport.h"

#include <QHash>
#include <QMetaObject>
#include <QApplication>

class Q_MONKEY_EXPORT QSingletonExpose
{
protected:
	static QHash<const QMetaObject*, QObject*> mInstances;
};

template <class T>
class QSingleton : public QSingletonExpose
{
protected:
	QSingleton() {};
	virtual ~QSingleton()
	{ mInstances.remove( &T::staticMetaObject ); }

public:
	template <typename P>
	static T* instance( P* );
	static T* instance();
	static bool instanceAvailable()
	{ return mInstances.contains( &T::staticMetaObject ); }
	static void cleanInstance()
	{ if ( instanceAvailable() ) delete mInstances[ &T::staticMetaObject ]; }

};

template <class T>
template <typename P>
T* QSingleton<T>::instance( P* p )
{
	T* t = qobject_cast<T*>( mInstances.value( &T::staticMetaObject ) );
	if ( !t )
		mInstances[&T::staticMetaObject] = ( t = new T( p ) );
	return t;
}

template <class T>
T* QSingleton<T>::instance()
{
	T* t = qobject_cast<T*>( mInstances.value( &T::staticMetaObject ) );
	if ( !t )
#ifdef Q_CC_GNU
		mInstances[&T::staticMetaObject] = ( t = new T );
#else
		mInstances[&T::staticMetaObject] = ( t = new T( 0 ) );
#endif
	return t;
}

#endif // QSINGLETON_H
