#ifndef REPLACETHREAD_H
#define REPLACETHREAD_H

#include <QThread>

class ReplaceThread : public QThread
{
	Q_OBJECT
	
public:
	ReplaceThread( QObject* parent = 0 );
	virtual ~ReplaceThread();

protected:
	void run();
};

#endif // REPLACETHREAD_H
