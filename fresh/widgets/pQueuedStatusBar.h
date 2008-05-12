#ifndef PQUEUEDSTATUSBAR_H
#define PQUEUEDSTATUSBAR_H

#include <QStatusBar>

#include "pQueuedMessageWidget.h"

class Q_MONKEY_EXPORT pQueuedStatusBar : public QStatusBar
{
	Q_OBJECT
	
public:
	pQueuedStatusBar( QWidget* parent = 0 );

protected:
	QPalette mDefaultPalette;
	pQueuedMessageWidget* mQueuedWidget;

public slots:
	int appendMessage( const pQueuedMessage& message );
	int appendMessage( const QString& message, int milliseconds = 0, const QPixmap pixmap = QPixmap(), const QBrush& background = QBrush( QColor( 255, 0, 0, 20 ) ), const QBrush& foreground = QBrush() );
	void removeMessage( const pQueuedMessage& message );
	void removeMessage( int id );

protected slots:
	void messageShown( const pQueuedMessage& message );
	void messageCleared();
};

#endif // PQUEUEDSTATUSBAR_H
