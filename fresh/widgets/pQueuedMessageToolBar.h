#ifndef PQUEUEDMESSAGETOOLBAR_H
#define PQUEUEDMESSAGETOOLBAR_H

#include "../objects/MonkeyExport.h"
#include "pQueuedMessageWidget.h"

#include <QToolBar>

class Q_MONKEY_EXPORT pQueuedMessageToolBar : public QToolBar
{
	Q_OBJECT
	
public:
	pQueuedMessageToolBar( QWidget* parent = 0 );
	virtual ~pQueuedMessageToolBar();

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

#endif // PQUEUEDMESSAGETOOLBAR_H
