#ifndef QTASSISTANTDOCK_H
#define QTASSISTANTDOCK_H

#include "ui_QtAssistantDock.h"

#include <QtHelp>

class QtAssistantBrowser;
class MkSQtDocInstaller;
class BookmarkManager;
class BookmarkWidget;

class QtAssistantDock : public pDockWidget, public Ui::QtAssistantDock
{
	Q_OBJECT

public:
	QtAssistantDock( QWidget* parent = 0 );
	~QtAssistantDock();
	
	inline QHelpEngine* helpEngine() const
	{ return mHelpEngine; }

protected:
	QHelpEngine* mHelpEngine;
	MkSQtDocInstaller* mDocInstaller;
	BookmarkManager* mBookmarkManager;
	BookmarkWidget* bwBookmarks;
	QtAssistantBrowser* mBrowser;
	QAction* aOpenContent;
	QAction* aOpenContentInNewTab;
	QAction* aOpenIndex;
	QAction* aOpenIndexInNewTab;
	
	bool eventFilter( QObject* obj, QEvent* e );
	void hideEvent( QHideEvent* e );

protected slots:
	void showBrowser();
	void onCurrentFilterChanged( const QString& filter );
	void on_cbFilter_currentIndexChanged( const QString& filter );
	void disableSearchLineEdit();
	void enableSearchLineEdit();
	void filterIndices( const QString& filter );
	void openContent();
	void openIndex();
	void addBookmark();
	void indexingStarted();
	void indexingFinished();
};

#endif // QTASSISTANTDOCK_H
