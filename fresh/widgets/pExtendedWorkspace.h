/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Framework
** FileName  : pExtendedWorkspace.h
** Date      : 2008-01-14T00:27:43
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#ifndef PEXTENDEDWORKSPACE_H
#define PEXTENDEDWORKSPACE_H

#include "MonkeyExport.h"
#include "pTabBar.h"

#include <QWidget>
#include <QList>
#include <QIcon>

class QBoxLayout;
class QStackedLayout;
class QStackedWidget;
class QMdiArea;
class QMdiSubWindow;
class pAction;
class pFilesListWidget;

class Q_MONKEY_EXPORT pExtendedWorkspace : public QWidget
{
	Q_OBJECT
	Q_ENUMS( TabMode DocumentMode )
	
public:
	enum DocumentMode { dmSDI = 0, dmMDI, dmTopLevel };

	pExtendedWorkspace( QWidget* , pExtendedWorkspace::DocumentMode = pExtendedWorkspace::dmSDI );
	~pExtendedWorkspace();
	
	virtual bool eventFilter( QObject*, QEvent* );

	//getters
	pTabBar* tabBar() const;
	pFilesListWidget* listWidget() const;
	
	pExtendedWorkspace::DocumentMode docMode() const;
	QTabBar::Shape tabShape() const;

	QWidgetList documents() const;	
	QWidget* document( int ) const;
	int count() const;
	
	int currentIndex() const;
	QWidget* currentDocument() const;
	int indexOf( QWidget* ) const;

	void setBackground( const QPixmap& );
	void setBackground( const QString& );
	
	int addDocument( QWidget*, const QString&,  const QIcon& = QIcon() );
	int insertDocument( int, QWidget*, const QString&, const QIcon& = QIcon() );
	
	QWidget* takeDocument( int );
	void removeDocument( int );
	void moveDocument( int, int );
	
public slots:
	void setDocMode( pExtendedWorkspace::DocumentMode );
	void setTabShape( QTabBar::Shape );

	void setCurrentIndex( int );
	void setCurrentDocument( QWidget* );

	/*
	Do not make this functions virtual!!
	closeAllDocuments must not call functions of child classes
	*/
	virtual void closeDocument( QWidget* document );
	virtual void closeDocument( int index );
	virtual bool closeAllDocuments();
	virtual void closeCurrentDocument();

	void activateNextDocument();
	void activatePreviousDocument();

    void setSDI();
    void setMDI();
    void setTopLevel();
    void cascade();
    void tile();
    void minimize();
    void restore();

protected:
	// workspace properties
	pExtendedWorkspace::DocumentMode mDocMode;

	QList<QWidget*> mDocuments;

	// main layout
	QBoxLayout* mLayout;
	// tab widget
	pTabBar* mTabBar;
	QBoxLayout* mTabLayout;
	//list widget
	pFilesListWidget* mFilesList;
	// document widget
	QStackedLayout* mStackedLayout;
	/* Stacked widget used for SDI mode, because we can't use maximized windows on QMdiArea on Mac*/
	QStackedWidget* mStackedWidget; 
	QMdiArea* mMdiAreaWidget;

protected slots:
	void mdiArea_subWindowActivated( QMdiSubWindow* );

signals:
	void documentInserted( int, const QString&, const QIcon& );
	void documentAboutToBeRemoved( int );
	void documentAboutToClose( int );
	// -1 if last file was closed
	void currentChanged( int );
	void tabShapeChanged( QTabBar::Shape );
	void docModeChanged( pExtendedWorkspace::DocumentMode );
	void modifiedChanged( int, bool );
	void docTitleChanged( int, const QString& );
	
//	void aboutToCloseAll ();
    
};

#endif // PEXTENDEDWORKSPACE_H
