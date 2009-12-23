#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>

class SearchThread : public QThread
{
	Q_OBJECT
	
public:
	SearchThread( QObject* parent = 0 );
	virtual ~SearchThread();
};

#endif // SEARCHTHREAD_H
