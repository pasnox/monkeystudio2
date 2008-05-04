#ifndef PQUEUEDMESSAGEWIDGET_H
#define PQUEUEDMESSAGEWIDGET_H

#include "../objects/MonkeyExport.h"

#include <QWidget>
#include <QHash>

class QLabel;
class QDialogButtonBox;

struct Q_MONKEY_EXPORT pQueuedMessage
{
	pQueuedMessage()
	{
		MilliSeconds = -1;
	}
	
	bool operator==( const pQueuedMessage& o ) const
	{
		return Message == o.Message && MilliSeconds == o.MilliSeconds &&
			Pixmap.cacheKey() == o.Pixmap.cacheKey() && Background == o.Background &&
			Foreground == o.Foreground;
	}
	
	QString Message;
	int MilliSeconds;
	QPixmap Pixmap;
	QBrush Background;
	QBrush Foreground;
};

class Q_MONKEY_EXPORT pQueuedMessageWidget : public QWidget
{
	Q_OBJECT
	
public:
	pQueuedMessageWidget( QWidget* parent = 0 );
	
	QSize sizeHint() const;

protected:
	QHash<int,pQueuedMessage> mMessages;
	static int pQueuedMessageUniqueId;
	QLabel* lPixmap;
	QLabel* lMessage;
	QDialogButtonBox* dbbButtons;
	QPalette mDefaultPalette;

public slots:
	int append( const pQueuedMessage& message );
	int append( const QString& message, int milliseconds = -1, const QPixmap pixmap = QPixmap(), const QBrush& background = QBrush( QColor( 255, 0, 0, 20 ) ), const QBrush& foreground = QBrush() );
	void remove( const pQueuedMessage& message );
	void remove( int id );

protected slots:
	void startMessage();
	void stopMessage();

signals:
	void messageShown();
	void messageClosed();
};

#endif // PQUEUEDMESSAGEWIDGET_H
