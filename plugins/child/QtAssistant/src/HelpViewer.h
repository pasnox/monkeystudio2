#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include <QWebView>
#include <QUrl>
#include <QVariant>
#include <QAction>

class QHelpEngine;
class QtAssistantBrowser;

class QPoint;
class QString;
class QKeyEvent;
class QMouseEvent;
class QContextMenuEvent;

class HelpViewer : public QWebView
{
	Q_OBJECT

public:
	HelpViewer( QHelpEngine* helpEngine, QtAssistantBrowser* parent = 0 );
	void setSource( const QUrl& url );

	inline QUrl source() const
	{ return url(); }

	inline QString documentTitle() const
	{ return title(); }

	inline bool hasSelection() const
	{ return !selectedText().isEmpty(); }

	void resetZoom();
	void zoomIn( int range = 1 );
	void zoomOut( int range = 1 );

	inline void copy()
	{ return triggerPageAction( QWebPage::Copy ); }

	inline bool isForwardAvailable() const
	{ return pageAction( QWebPage::Forward )->isEnabled(); }

	inline bool isBackwardAvailable() const
	{ return pageAction( QWebPage::Back )->isEnabled(); }

public slots:
	void home();
	void backward()
	{ back(); }

protected:
	virtual void wheelEvent( QWheelEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );

private slots:
	void actionChanged();

private:
	QHelpEngine* mHelpEngine;
	QtAssistantBrowser* mBrowser;
	QUrl homeUrl;

signals:
	void copyAvailable( bool enabled );
	void forwardAvailable( bool enabled );
	void backwardAvailable( bool enabled );
	void actionsChanged();
	void highlighted( const QString& );
	void sourceChanged( const QUrl& );
};

#endif // HELPVIEWER_H
