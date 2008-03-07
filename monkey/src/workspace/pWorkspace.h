/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pWorkspace.h
** Date      : 2008-01-14T00:37:22
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
#ifndef PWORKSPACE_H
#define PWORKSPACE_H

#include "MonkeyExport.h"
#include "pExtendedWorkspace.h"
#include "pConsoleManager.h"

class pAbstractChild;
class QMainWindow;

class Q_MONKEY_EXPORT pWorkspace : public pExtendedWorkspace
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	void loadSettings();
	pAbstractChild* currentChild() const;
	pAbstractChild* child( int ) const;
	QList<pAbstractChild*> children() const;

	// open a file and set cursor to position
	pAbstractChild* openFile( const QString& );
	// close the file
	void closeFile( const QString& );
	// goto to position inside file, highlight line according to bool, opening it if needed
	void goToLine( const QString&, const QPoint&, bool );

	void closeCurrentDocument();
	bool closeAllDocuments();

protected:
	virtual void closeDocument( QWidget* document );

private:
	pWorkspace( QMainWindow* = 0 );

private slots:
	void internal_currentFileChanged( const QString& );
	void internal_currentChanged( int );
	//void internal_aboutToCloseTab( int, QCloseEvent* );
	void internal_urlsDropped( const QList<QUrl>& );
	void internal_listWidget_customContextMenuRequested( const QPoint& );

public slots:
	// file menu
	void fileNew_triggered();
	void fileOpen_triggered();
	void fileSessionSave_triggered();
	void fileSessionRestore_triggered();
	void fileSaveCurrent_triggered();
	void fileSaveAll_triggered();
	void fileCloseCurrent_triggered();
	void fileCloseAll_triggered();
	void fileSaveAsBackup_triggered();
	void fileQuickPrint_triggered();
	void filePrint_triggered();
	void fileExit_triggered();

	// edit menu
	void editSettings_triggered();
	void editTranslations_triggered();
	void editUndo_triggered();
	void editRedo_triggered();
	void editCut_triggered();
	void editCopy_triggered();
	void editPaste_triggered();
	void editGoTo_triggered();
	void editExpandAbbreviation_triggered();
	void editPrepareAPIs_triggered();

	// view menu
	void agStyles_triggered( QAction* );

	// help menu
	void helpAboutApplication_triggered();
	void helpAboutQt_triggered();
#ifdef __COVERAGESCANNER__
	void helpTestReport_triggered();
#endif

signals:
	// a file has been opened
	void fileOpened( const QString& );
	// a file has been closed
	void fileClosed( const QString& );
	// current file changed
	void currentFileChanged( pAbstractChild*, const QString& );
	
	// signals for controling search results dock
	void appendSearchResult( const pConsoleManager::Step& );
	void clearSearchResults();
};

#endif // PWORKSPACE_H
