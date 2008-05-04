#ifndef PQUEUEDSTATUSBAR_H
#define PQUEUEDSTATUSBAR_H

#include <QStatusBar>

#include "pQueuedMessageWidget.h"

class pQueuedStatusBar : public QStatusBar
{
	Q_OBJECT
	
public:
	pQueuedStatusBar( QWidget* parent = 0 );

protected:
	pQueuedMessageWidget* mQueuedWidget;

public slots:
	int appendMessage( const pQueuedMessage& message );
	int appendMessage( const QString& message, int milliseconds = -1, const QPixmap pixmap = QPixmap(), const QBrush& background = QBrush( QColor( 255, 0, 0, 20 ) ), const QBrush& foreground = QBrush() );
	void removeMessage( const pQueuedMessage& message );
	void removeMessage( int id );

protected slots:
	void messageChanged();
};

#endif // PQUEUEDSTATUSBAR_H
