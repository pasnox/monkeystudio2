#ifndef QUEUEDSTATUSBAR_H
#define QUEUEDSTATUSBAR_H

#include "fresh.h"

class QueuedStatusBar : public pQueuedStatusBar
{
	Q_OBJECT
	
public:
	QueuedStatusBar( QWidget* parent = 0 );
};

#endif // QUEUEDSTATUSBAR_H
