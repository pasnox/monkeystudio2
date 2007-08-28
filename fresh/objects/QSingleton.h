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

#include <QApplication>
#include <QWidget>
#include <QPointer>

class Q_MONKEY_EXPORT Base
{
protected:
	static QMap<QMetaObject, QObject*> mInstances;
};

template <class T>
class QSingleton
{
protected:
	QSingleton() {};
	virtual ~QSingleton() {};

public:
	template <typename P>
	static T* instance( P* );
	static T* instance();

private:
	static QPointer<T> mInstance;

};

template <class T>
QPointer<T> QSingleton<T>::mInstance = 0L;

template <class T>
template <typename P>
T* QSingleton<T>::instance( P* p )
{
	if ( !mInstances.contains( *T::staticMetaObject )
	{
		T *t = new T( P );
		map[*T::staticMetaObject] = t;
	}
	return mInstances[*T::staticMetaObject];
}

template <class T>
T* QSingleton<T>::instance()
{
	if ( !mInstances.contains( *T::staticMetaObject )
	{
		T *t = new T;
		map[*T::staticMetaObject] = t;
	}
	return mInstances[*T::staticMetaObject];
}

#endif // QSINGLETON_H
