/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com ), klorydryk ( benoit.piedallu@free.fr ), lud42fr, IrmatDen
 * FILENAME     : QSingleton.h
 * LICENSE      : GPL
 * COMMENTARY   : This template class allow to singletonize QObject herited classes
 ********************************************************************************************************/
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
#include <QWidget>
#include <QPointer>

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
		mInstances[&T::staticMetaObject] = ( t = new T );
	return t;
}

#endif // QSINGLETON_H
