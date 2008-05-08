#ifndef PQUEUEDMESSAGEWIDGET_H
#define PQUEUEDMESSAGEWIDGET_H

#include "../objects/MonkeyExport.h"

#include <QWidget>
#include <QHash>
#include <QDialogButtonBox>
#include <QPointer>
#include <QVariant>

class QLabel;
class QDialogButtonBox;

struct Q_MONKEY_EXPORT pQueuedMessage
{
	pQueuedMessage()
	{
		MilliSeconds = 0;
		Object = 0;
		Slot = 0;
	}
	
	bool operator==( const pQueuedMessage& o ) const
	{
		return Message == o.Message && MilliSeconds == o.MilliSeconds &&
			( Pixmap.cacheKey() == o.Pixmap.cacheKey() || ( Pixmap.isNull() && o.Pixmap.isNull() ) ) &&
			Background == o.Background && Foreground == o.Foreground && Buttons == o.Buttons &&
			Object == o.Object && Slot == o.Slot && UserData == o.UserData;
	}
	
	QString Message;
	int MilliSeconds;
	QPixmap Pixmap;
	QBrush Background;
	QBrush Foreground;
	QHash<QDialogButtonBox::StandardButton, QString> Buttons; // StandardButton, Button Text ( empty for standard )
	QPointer<QObject> Object;
	const char* Slot;
	QVariant UserData;
};

class Q_MONKEY_EXPORT pQueuedMessageWidget : public QWidget
{
	Q_OBJECT
	
public:
	pQueuedMessageWidget( QWidget* parent = 0 );
	
	int messagesCount() const;

protected:
	QHash<int,pQueuedMessage> mMessages;
	static int pQueuedMessageUniqueId;
	QLabel* lPixmap;
	QLabel* lMessage;
	QDialogButtonBox* dbbButtons;

public slots:
	int append( const pQueuedMessage& message );
	int append( const QString& message, int milliseconds = 0, const QPixmap pixmap = QPixmap(), const QBrush& background = QBrush( QColor( 255, 0, 0, 20 ) ), const QBrush& foreground = QBrush() );
	void remove( const pQueuedMessage& message );
	void remove( int id );
	void clear();

protected slots:
	void clicked( QAbstractButton* button );
	void showMessage();
	void closeMessage();

signals:
	void messageShown( const pQueuedMessage& message );
	void messageClosed( const pQueuedMessage& message );
	void cleared();
};

#endif // PQUEUEDMESSAGEWIDGET_H
