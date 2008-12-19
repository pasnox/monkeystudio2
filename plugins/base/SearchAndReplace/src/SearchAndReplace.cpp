/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : SearchAndReplace.cpp
** Date      : 
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
/*!
	\file SearchAndReplace.cpp
	\date 
	\author Andrei Kopats
	\brief implementation of main class of SearchAndReplace plugin
*/
#include "MonkeyCore.h"
#include "UIMain.h"
#include "pWorkspace.h"
#include "pChild.h"
#include "pEditor.h"
#include "QueuedStatusBar.h"

#include "SearchAndReplace.h"
#include "SearchWidget.h"
#include "SearchResultsDock.h"
#include "SearchThread.h"

#include <QDebug>

/*!
	Constructor of class
*/
SearchAndReplace::SearchAndReplace()
  :	mWidget (NULL),
    mDock (NULL),
	mSearchThread (NULL),
	mOccurencesFound (0),
	mFilesProcessed (0)
	
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Search and Replace" );
	mPluginInfos.Description = tr( "Plugin for searching and replacing text" );
	mPluginInfos.Author = "Andrei Kopats aka hlamer <hlamer at tut by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Enabled = false;
	
	qRegisterMetaType<pConsoleManager::Step>("pConsoleManager::Step");
}

/*!
	Destructor of class. Uninstalls plugin from IDE
*/
SearchAndReplace::~SearchAndReplace()
{
	if ( isEnabled() )
		setEnabled( false );
}

/*!
	Enable/disable plugin
	
	If plugin is enabled - it visible on main window and it's actions are in 
	the main menu
	\param b Flag. Enable = true, Disable = false
	\return Status of process 
	\retval true Successfully enabled
	\retval false Some error ocurred
*/
bool SearchAndReplace::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// create docks
		mWidget = new SearchWidget ();
		MonkeyCore::workspace()->addSearhReplaceWidget (mWidget);
		connect (mWidget, SIGNAL (previousClicked()), this, SLOT (onPreviousClicked()));
		connect (mWidget, SIGNAL (nextClicked()), this, SLOT (onNextClicked()));
		connect (mWidget, SIGNAL (replaceClicked()), this, SLOT (onReplaceClicked()));
		connect (mWidget, SIGNAL (replaceAllClicked()), this, SLOT (onReplaceAllClicked()));

		
		mDock = new SearchResultsDock ();
		mDock->setVisible( false );
		
		connect (mDock, SIGNAL (resultActivated (const QString&, const QPoint&)), this, SLOT (makeGoTo (const QString&, const QPoint&)));
		
		MonkeyCore::mainWindow()->dockToolBar( Qt::BottomToolBarArea )->addDock( mDock, infos().Caption, QIcon(":/icons/tabsearch.png") );
		
		MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFile", tr( "&Replace in the file..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+R" ), tr( "Replace in the file..." ) )->setEnabled( true );
		//MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchProject", tr( "&Search in the project..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "" ), tr( "Search in the project..." ) )->setEnabled( true );
		//MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceProject", tr( "&Replace in the project..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "" ), tr( "Replace in the project..." ) )->setEnabled( true );
		MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFolder", tr( "&Search in the folder..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+Alt+F" ), tr( "Search in the folder..." ) )->setEnabled( true );
		MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFolder", tr( "&Replace in the folder..." ), QIcon( ":/edit/icons/edit/search.png" ), tr( "Ctrl+Alt+R" ), tr( "Replace in the folder..." ) )->setEnabled( true );
		MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchPrevious", tr( "Search Previous" ), QIcon( ":/edit/icons/edit/previous.png" ), tr( "Shift+F3" ), tr( "Search Previous" ) )->setEnabled( true );
		MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchNext", tr( "Search Next" ), QIcon( ":/edit/icons/edit/next.png" ), tr( "F3" ), tr( "Search Next" ) )->setEnabled( true );

		connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFile" ), SIGNAL( triggered() ), SLOT( showSearchFile() ) );
		connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFile" ), SIGNAL( triggered() ), SLOT( showReplaceFile() ) );
		//connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchProject" ), SIGNAL( triggered() ), SLOT( showSearchProject() ) );
		//connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceProject" ), SIGNAL( triggered() ), SLOT( showReplaceProject() ) );
		connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFolder" ), SIGNAL( triggered() ), SLOT( showSearchFolder() ) );
		connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFolder" ), SIGNAL( triggered() ), SLOT( showReplaceFolder() ) );
		connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchNext" ), SIGNAL( triggered() ), this, SLOT( onNextClicked() ) );		
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		delete mWidget;
		mWidget = NULL;
		delete mDock;
		mDock = NULL;
		
		delete MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFile" );
		//delete MonkeyCore::menuBar()->deleteAaction( "mEdit/mSearchReplace/aSearchProject" );
		//delete MonkeyCore::menuBar()->deleteAaction( "mEdit/mSearchReplace/aReplaceProject" );
		delete MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFolder" );
		delete MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFolder" );
		delete MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchPrevious" );
		delete MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchNext" );
		
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	// return default value
	return true;
}

/*!
	Get settings widget for configuring plugin
	\return Pointer to widget
*/
QWidget* SearchAndReplace::settingsWidget()
{ return NULL; }


bool SearchAndReplace::isSearchTextValid ()
{
	if (mWidget->isRegExp())
		if (!QRegExp (mWidget->searchText()).isValid())
		{
			MonkeyCore::statusBar()->appendMessage( tr( "Regular expression is invalid" ), 2500 );
			return false;
		}
	
	return true;
}

bool SearchAndReplace::isReplaceTextValid ()
{
	return true;
}

bool SearchAndReplace::isPathValid ()
{
	if (!QDir (mWidget->path()).exists())
	{
		MonkeyCore::statusBar()->appendMessage( tr( "Search path not exist" ), 2500 );
		return false;
	}
	return true;
}

bool SearchAndReplace::isMaskValid ()
{
	return true;
}

void SearchAndReplace::showMessage (QString status)
{
	if (!status.isNull())
		MonkeyCore::mainWindow()->statusBar()->showMessage (tr ("Search: %1").arg (status), 5000);
	else
		MonkeyCore::mainWindow()->statusBar()->showMessage ("");
}

void SearchAndReplace::updateSearchTextOnUI ()
{
	pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
	if (child && child->editor())
	{
		pEditor* editor = child->editor ();
		QString text = editor->selectedText ();
		if (!text.isNull() && text.indexOf ('\n') == -1)
			mWidget->setSearchText (text);
	}
}

bool SearchAndReplace::searchFile (bool next)
{
	QString text = mWidget->searchText();
	mWidget->searchAddToRecents(text);
	
	pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
	if (!child || !child->editor())
	{
		showMessage(tr( "No active editor" ) );
		return false;
	}
	pEditor* editor = child->editor ();

	// get cursor position
	int x, y;
	editor->getCursorPosition( &y, &x );

	if (!next)
	{
		int temp;
		editor->getSelection(&y, &x, &temp, &temp);
	}

	// search
	bool b = editor->findFirst( text, mWidget->isRegExp(), mWidget->isCaseSensetive(), false, false, next, y, x);
	// change background acording to found or not
	//QPalette p = cobSearch->palette();
	//p.setColor( cobSearch->backgroundRole(), b ? Qt::white : Qt::red );
	//cobSearch->setPalette( p ); // FIXME
	
	// show message if needed
	showMessage( b ? QString::null : tr( "Not Found" ) );

	// return found state
	return b;
}

int SearchAndReplace::replace(bool all)
{
	QString rtext = mWidget->replaceText();
	
	mWidget->searchAddToRecents(mWidget->searchText());
	mWidget->replaceAddToRecents(rtext);
	
	pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
	if (!child || !child->editor())
	{
		showMessage(tr( "No active editor" ) );
		return 0;
	}
	pEditor* editor = child->editor ();

	int count = 0;
	if (all)
	{
		int x, y;
		editor->getCursorPosition(&y, &x);

		editor->setCursorPosition(0, 0);
		while (searchFile(true)) //search next
		{
			editor->replace(rtext);
			count++;
		};
		editor->setCursorPosition(y, x); //restore cursor position
	}
	else
	{ 		//do replace one time
			int x, y, temp;
			editor->getSelection(&y, &x, &temp, &temp);
			editor->setCursorPosition(y, x);

		if (searchFile(true))
			{
				editor->replace (rtext);
				count = 1;
				editor->findNext(); //move selection to next item
			}
	}
	
	return count;
}

void SearchAndReplace::showSearchFile () 
{
	if (dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild()))
	{
		mMode = SEARCH_FILE;
		updateSearchTextOnUI ();
		mWidget->show (mMode);
	}
};

void SearchAndReplace::showReplaceFile () 
{
	mMode = REPLACE_FILE;
	updateSearchTextOnUI ();
	mWidget->show (mMode);
};

#if 0
	void SearchAndReplace::showSearchProject () 
	{
		mMode = SEARCH_PROJECT;
		updateSearchTextOnUI ();
		mWidget->show (mMode);
	};

	void SearchAndReplace::showReplaceProject () 
	{
		mMode = REPLACE_PROJECT;
		updateSearchTextOnUI ();
		mWidget->show (mMode);
	};
#endif

void SearchAndReplace::showSearchFolder () 
{
	mMode = SEARCH_DIRRECTORY;
	updateSearchTextOnUI ();
	mWidget->show (mMode);};

void SearchAndReplace::showReplaceFolder () 
{
	mMode = REPLACE_DIRRECTORY;
	updateSearchTextOnUI ();
	mWidget->show (mMode);
};

void SearchAndReplace::onPreviousClicked()
{
	searchFile (false);
}

void SearchAndReplace::onNextClicked()
{
	switch (mMode)
	{
		case SEARCH_FILE:
		case REPLACE_FILE:
			if (! isPathValid ())
				return;
			
			searchFile (true);
		break;
		
		case SEARCH_DIRRECTORY:
		case REPLACE_DIRRECTORY:
			if (!isSearchTextValid () || !isPathValid ())
				return;
			
			if (mSearchThread && mSearchThread->isRunning ())
			{ // need to stop search process
				mSearchThread->setTermEnabled (true);
			}
			else
			{ // need to start searching
				mDock->clearSearchResults ();
				mWidget->pathAddToRecents (mWidget->path());
				mWidget->maskAddToRecents (mWidget->mask());
				mOccurencesFound = 0;
				mFilesProcessed = 0;
				fileProcessed (0);
				QString path = mWidget->path();
				QString mask = mWidget->mask();
				QString text = mWidget->searchText ();
				
				bool caseSensetive = mWidget->isCaseSensetive ();
				bool regexp = mWidget->isRegExp ();
				
				if (mMode == SEARCH_DIRRECTORY)
					mSearchThread = new SearchThread(SearchThread::SEARCH, path, mask, text, "", caseSensetive, regexp, this);
				else
					mSearchThread = new SearchThread(SearchThread::REPLACE, path, mask, text, "", caseSensetive, regexp, this);
				
				mWidget->setNextButtonText (tr("&Stop"));
				mWidget->setNextButtonIcon (QIcon(":/console/icons/console/stop.png"));
				
				connect (mSearchThread, SIGNAL (appendSearchResult( const pConsoleManager::Step& )), mDock, SLOT (appendSearchResult( const pConsoleManager::Step& )));
				connect (mSearchThread, SIGNAL (appendSearchResult( const pConsoleManager::Step& )), this, SLOT (occurenceFound ()));
				connect (mSearchThread, SIGNAL (changeProgress(int)), this, SLOT (fileProcessed (int)));
				connect (mSearchThread, SIGNAL (finished ()), this, SLOT (threadFinished()));
				mSearchThread->start();    
			}
		break;
	}
	return;
}

void SearchAndReplace::onReplaceClicked()
{
	/* Check replace text */
	if (!isReplaceTextValid ())
		return;
	
	replace (false);	
}

void SearchAndReplace::onReplaceAllClicked()
{
	/* Check replace text */
	if (!isReplaceTextValid ())
		return;

	pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
	if (!child && !child->editor())
		return;
	pEditor* editor = child->editor ();
	
	// begin undo global action
	editor->beginUndoAction();

	int count = replace (true);
	// end undo global action
	editor->endUndoAction();

	// show occurence number replaced
	showMessage( count ? tr( "%1 occurences replaced" ).arg( count ) : tr( "Nothing To Repalce" ) );
}

void SearchAndReplace::makeGoTo (const QString& file, const QPoint& position)
{
	MonkeyCore::workspace()->goToLine(file, position, true, pMonkeyStudio::defaultCodec());
}

void SearchAndReplace::threadFinished ()
{
	mWidget->setNextButtonText (tr("Search"));
	mWidget->setNextButtonIcon (QIcon(":/edit/icons/edit/search.png"));
	
	delete mSearchThread;
	mSearchThread = NULL;
	showMessage ("Searching finished");
}

void SearchAndReplace::occurenceFound ()
{
	mOccurencesFound ++;
	showMessage(tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFound));
}

void SearchAndReplace::fileProcessed (int count)
{
	mFilesProcessed = count;
	showMessage (tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFound));
}

Q_EXPORT_PLUGIN2( BaseMessageBox, SearchAndReplace )
