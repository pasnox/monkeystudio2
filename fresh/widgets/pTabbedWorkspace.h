/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pTabbedWorkspace.h
 * LICENSE      : GPL
 * COMMENTARY   : This class provide a workspace that can at run time be switched in SDI / MDI or Top Level
 ********************************************************************************************************/
#ifndef PTABBEDWORKSPACE_H
#define PTABBEDWORKSPACE_H

#include "MonkeyExport.h"

#include <QWidget>
#include "pTabBar.h"
#include <QList>

class QBoxLayout;
class QStackedLayout;
class QStackedWidget;
class QWorkspace;
class pTabbedWorkspaceCorner;
class pAction;
class pTabBar;

class Q_MONKEY_EXPORT pTabbedWorkspace : public QWidget
{
	Q_OBJECT
	Q_ENUMS( TabMode DocumentMode )
	friend class pTabbedWorkspaceRightCorner;
	
public:
	enum TabMode { tmSDI = 0, tmMDI, tmTopLevel };
	enum DocumentMode { dmMaximized = 0, dmCascade, dmTile, dmIcons, dmMinimizeAll, dmRestoreAll };

	pTabbedWorkspace( QWidget* = 0, pTabbedWorkspace::TabMode = pTabbedWorkspace::tmMDI );
	~pTabbedWorkspace();
	
	virtual bool eventFilter( QObject*, QEvent* );

	pTabBar* tabBar() const;
	QTabBar::Shape tabShape() const;
	pTabbedWorkspace::TabMode tabMode() const;
	pTabbedWorkspace::DocumentMode documentMode() const;
	int currentIndex() const;
	QWidget* currentDocument() const;
	int indexOf( QWidget* ) const;
	QWidget* document( int ) const;
	int count() const;
	QList<QWidget*> documents() const;
	pTabbedWorkspaceCorner* cornerWidget( Qt::Corner = Qt::TopRightCorner ) const;
	int addTab( QWidget*, const QString& );
	int addTab( QWidget*, const QIcon&, const QString& );
	int insertTab( int, QWidget*, const QString& );
	int insertTab( int, QWidget*, const QIcon&, const QString& );
	bool tabsHaveShortcut() const;
	bool tabsElided() const;

public slots:
	void setBackground( const QPixmap& );
	void setBackground( const QString& );
	void setTabShape( QTabBar::Shape );
	void setTabMode( pTabbedWorkspace::TabMode );
	void setDocumentMode( pTabbedWorkspace::DocumentMode );
	void setCurrentIndex( int );
	void setCurrentDocument( QWidget* );
	void setCornerWidget( pTabbedWorkspaceCorner*, Qt::Corner = Qt::TopRightCorner );
	void removeTab( int );
	void removeDocument( QWidget* );
	void closeCurrentTab();
	void closeAllTabs( bool = false );
	void activateNextDocument();
	void activatePreviousDocument();
	void setTabsHaveShortcut( bool );
	void setTabsElided( bool );

protected:
	void updateCorners();
	void updateView( QWidget* = 0 );
	void addDocument( QWidget* d, int = -1 );

	// tell if tabs have shortcuts
	pAction* aTabbedTabsHaveShortcut;
	// tell if tabs are elided
	pAction* aTabbedTabsElided;
	// workspace properties
	pTabbedWorkspace::TabMode mTabMode;
	pTabbedWorkspace::DocumentMode mDocumentMode;
	// main layout
	QBoxLayout* mLayout;
	QList<QWidget*> mDocuments;
	// tab widget
	pTabBar* mTabBar;
	QBoxLayout* mTabLayout;
	// document widget
	QStackedLayout* mStackedLayout;
	QStackedWidget* mStackedWidget;
	QWorkspace* mWorkspaceWidget;

protected slots:
	void internal_midButtonPressed( int, const QPoint& );
	void internal_rightButtonPressed( int, const QPoint& );
	void internal_tabDropped( int, int );
	void internal_currentChanged( int );
	void workspaceWidget_windowActivated( QWidget* );
	void removeDocument( QObject* );
	void updateTabsNumber( int = -1 );

signals:
	void tabInserted( int );
	void aboutToCloseTab( int, QCloseEvent* );
	void aboutToCloseDocument( QWidget*, QCloseEvent* );
	void tabRemoved( int );
	void currentChanged( int );
	void tabShapeChanged( QTabBar::Shape );
	void tabModeChanged( pTabbedWorkspace::TabMode );
	void documentModeChanged( pTabbedWorkspace::DocumentMode );

};

#endif // PTABBEDWORKSPACE_H
