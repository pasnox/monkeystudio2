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
#include "SearchAndReplace.h"
#include "SearchWidget.h"
#include "SearchResultsDock.h"
#include "SearchThread.h"

#include "MonkeyCore.h"
#include "UIMain.h"
#include "pWorkspace.h"
#include "pChild.h"
#include "pEditor.h"
#include "pFileManager.h"
#include "QueuedStatusBar.h"

/*!
	Constructor of class
*/
SearchAndReplace::SearchAndReplace()
	:	mWidget (NULL),
		mDock (NULL),
		mSearchThread (NULL)
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Search and Replace" );
	mPluginInfos.Description = tr( "Plugin for searching and replacing text" );
	mPluginInfos.Author = "Andrei Kopats aka hlamer <hlamer at tut by>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = PLUGIN_NAME;
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.FirstStartEnabled = true;
	
	qRegisterMetaType<SearchAndReplace::Occurence>("SearchAndReplace::Occurence");
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
		connect (mWidget, SIGNAL (searchTextEdited()), this, SLOT (onSearchTextEdited()));

		
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
		stateAction()->setChecked( true );
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
		stateAction()->setChecked( false );
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

/*!
	Check, if text for search is valid. Important for regular expressions
	\return true - valid. False - invalid
*/
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

/*!
	Check, if text for search is valid. Important for regular expressions
	\return true - valid. False - invalid
*/
bool SearchAndReplace::isReplaceTextValid ()
{
	return true;
}

/*!
	Check, if text for search is valid. Important for regular expressions
	\return true - valid. False - invalid
*/
bool SearchAndReplace::isPathValid ()
{
	if (!QDir (mWidget->path()).exists())
	{
		MonkeyCore::statusBar()->appendMessage( tr( "Search path not exist" ), 2500 );
		return false;
	}
	return true;
}

/*!
	Check, if file names mask is valid.
	\return true - valid. False - invalid
*/
bool SearchAndReplace::isMaskValid ()
{
	return true;
}

/*!
	Show for 5 seconds message on status bar
*/
void SearchAndReplace::showMessage (const QString& status)
{
	if (!status.isNull())
		MonkeyCore::mainWindow()->statusBar()->showMessage (tr ("Search: %1").arg (status), 5000);
	else
		MonkeyCore::mainWindow()->statusBar()->clearMessage();
}

/*!
	Set on UI (search widget) search text according with text, selected in the current editor. Executed, when search widget appears
*/
void SearchAndReplace::updateSearchTextOnUI ()
{
	pChild* child = qobject_cast<pChild*> (MonkeyCore::workspace()->currentChild());
	if (child && child->editor())
	{
		pEditor* editor = child->editor ();
		QString text = editor->selectedText ();
		if (!text.isNull() && text.indexOf ('\n') == -1)
			mWidget->setSearchText (text);
	}
}

/*!
	Search text in the file.atEnd
	Result of search - changed selection and cursor position in the editor.
	Method also will change color of search text input
	
	\param next - search forward. If false - will search backward
	\param incremental - Incremental search mode. (Search dirrectly when user types something.) 
			If true - will search from selection (previous search result), if false - from cursor position
	\param enableLooping -If true, will try to search from start, if nothing found from cursor to end of file
	\return true - something found
	\return false - nothing found
*/
bool SearchAndReplace::searchFile (bool next, bool incremental, bool enableLooping)
{
	QString text = mWidget->searchText();
	if (!incremental)
		mWidget->searchAddToRecents(text);
	
	pChild* child = qobject_cast<pChild*> (MonkeyCore::workspace()->currentChild());
	if (!child || !child->editor())
	{
		showMessage(tr( "No active editor" ) );
		return false;
	}
	pEditor* editor = child->editor ();

	// get cursor position
	int x, y;
	if (next && !incremental)
	{
		editor->getCursorPosition( &y, &x );
	}
	else // incremental search, or search backward
	{
		int temp;
		editor->getSelection(&y, &x, &temp, &temp);
	}
	
	// search
	bool b = editor->findFirst( text, mWidget->isRegExp(), mWidget->isCaseSensetive(), false, false, next, y, x);
	if (!b && enableLooping) // not found, try from start/end
	{
		if (next)
			b = editor->findFirst( text, mWidget->isRegExp(), mWidget->isCaseSensetive(), false, false, next, 0, 0);
		else
			b = editor->findFirst( text, mWidget->isRegExp(), mWidget->isCaseSensetive(), false, false, next, editor->lines(), 0);
	}
	
	// show message if needed
	showMessage( b ? QString::null : tr( "Not Found" ) );
	
	// change background acording to found or not
	if (b)
		mWidget->setSearchLineEditColor (SearchWidget::GREEN);
	else
		mWidget->setSearchLineEditColor (SearchWidget::RED);
	
	// return found state
	return b;
}

/*!
	Do replacement in the file.
	Will execute SearchAndReplace::searchFile for find text, which will be replaced
	
	\param all If false - one replacement will be made. If true - all possible
	\return count of replacements
*/
int SearchAndReplace::replace(bool all)
{
	QString rtext = mWidget->replaceText();
	
	mWidget->searchAddToRecents(mWidget->searchText());
	mWidget->replaceAddToRecents(rtext);
	
	pChild* child = qobject_cast<pChild*> (MonkeyCore::workspace()->currentChild());
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
		while (searchFile(true, false, false)) //search next
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

		if (searchFile(true, false, true))
			{
				editor->replace (rtext);
				count = 1;
				editor->findNext(); //move selection to next item
			}
	}
	
	return count;
}

/*!
	Apply replacements in the dirrectory, according with selected on dock occcurences
*/
void SearchAndReplace::replaceInDirrectory()
{
	QMessageBox::StandardButton answer = 
		QMessageBox::question (NULL, 
								tr("Replace in dirrectory"), 
								tr("Rollback changes for replace in dirrectory is not available. You can permanently corrupt your files. Are you sure?"),
								(QMessageBox::Yes | QMessageBox::No),
								QMessageBox::No);
	if (QMessageBox::Yes != answer)
		return;
	
	int replacementsCount = 0;
	
	for (int fileIndex = 0; fileIndex < mDock->filesWithOccurencesCount (); fileIndex++) // for files
	{
		Occurence occ = mDock->occurence (fileIndex, 0); // every file has at least one occurence
		pAbstractChild* currFileChild = MonkeyCore::fileManager()->childForFile (occ.fileName); 
		if (NULL != currFileChild && currFileChild->isModified(occ.fileName)) // going to replace in modified file
		{
			QMessageBox::critical (NULL, tr("Replace in dirrectory"), tr("File %1 is not saved. Save please all files before do replacing").arg (occ.fileName));
			return;
		}
	}
		
	for (int fileIndex = 0; fileIndex < mDock->filesWithOccurencesCount (); fileIndex++) // for files
	{
		QFile file;
		QStringList fileContents;
		
		for (int occurenceIndex = 0; occurenceIndex < mDock->oCcurencesCount (fileIndex); occurenceIndex++) // for occurences
		{
			Occurence occ = mDock->occurence (fileIndex, occurenceIndex);
			if (occ.checked)
			{
				if (! file.isOpen())  // open file, if nessesary
				{
					file.setFileName (occ.fileName);
					bool stat = file.open (QIODevice::ReadWrite);
					if (! stat)
					{
						QMessageBox::critical (NULL, "Replace in dirrectory", "Failed to open file: " + file.errorString());
						break;
					}
					// read all file to QStringList
					while ((! file.atEnd()) && file.error() == QFile::NoError)
						fileContents << file.readLine();
					
					if (file.error() != QFile::NoError)
					{
						QMessageBox::critical (NULL, "Replace in dirrectory", "Failed to read file: " + file.errorString());
						break;
					}
				}
				// Now all file is in the QStringList
				if (fileContents.size() <= occ.position.y()-1)
				{
					QMessageBox::critical (NULL, "Replace in dirrectory", "Line with occurence not found");
					break;
				}
				// line with occurence
				QString line = fileContents[occ.position.y()-1];
				
				Qt::CaseSensitivity cs = occ.isCaseSensetive ? Qt::CaseSensitive : Qt::CaseInsensitive;
				if (occ.isRegExp)  // TODO let's remember parametres in the Occurence, because user can change it on UI
				{
					line.replace (QRegExp (occ.searchText, cs), mWidget->replaceText());
				}
				else // not a reg exp
				{
					line.replace (occ.searchText, mWidget->replaceText(), cs);
				}
				replacementsCount ++;
				
				fileContents[occ.position.y()-1] = line;
			}
		}
		// finished processing file. Need to write and close it, if it had been opened
		if (file.isOpen())
		{
			file.resize (0);
			if (file.error() != QFile::NoError)
			{
				QMessageBox::critical (NULL, "Replace in dirrectory", "Failed to write file: " + file.errorString());
				break;
			}
			file.write (fileContents.join("").toUtf8());
			if (file.error() != QFile::NoError)
			{
				QMessageBox::critical (NULL, "Replace in dirrectory", "Failed to write file: " + file.errorString());
				break;
			}
			file.close();
			if (file.error() != QFile::NoError)
			{
				QMessageBox::critical (NULL, "Replace in dirrectory", "Failed to write file: " + file.errorString());
				break;
			}
			// Now reload file automatically, if it is opened
			pAbstractChild* currFileChild = MonkeyCore::fileManager()->childForFile (file.fileName()); 
			if (NULL != currFileChild) // file is opened
			{
				currFileChild->closeFile(file.fileName());
				currFileChild->openFile(file.fileName(), currFileChild->textCodec());
			}
		}
	}
	mDock->clearSearchResults();
	showMessage (tr("%1 replacements made").arg (replacementsCount));
}

/*!
	Show search widget in SEARCH_FILE mode
*/
void SearchAndReplace::showSearchFile ()
{
	if (mSearchThread && mSearchThread->isRunning())
		return;
	
	if (qobject_cast<pChild*> (MonkeyCore::workspace()->currentChild()))
	{
		mMode = SEARCH_FILE;
		updateSearchTextOnUI ();
		mWidget->show (mMode);
	}
}

/*!
	Show search widget in SEARCH_FILE mode
*/
void SearchAndReplace::showReplaceFile () 
{
	if (mSearchThread && mSearchThread->isRunning())
		return;
	
	mMode = REPLACE_FILE;
	updateSearchTextOnUI ();
	mWidget->show (mMode);
}

#if 0
/*!
	Show search widget in SEARCH_PROJECT mode
*/
void SearchAndReplace::showSearchProject () 
{
	if (mSearchThread && mSearchThread->isRunning())
		return;
	
	mMode = SEARCH_PROJECT;
	updateSearchTextOnUI ();
	mWidget->show (mMode);
}

/*!
	Show search widget in REPLACE_PROJECT mode
*/
void SearchAndReplace::showReplaceProject () 
{
	if (mSearchThread && mSearchThread->isRunning())
		return;
	
	mMode = REPLACE_PROJECT;
	updateSearchTextOnUI ();
	mWidget->show (mMode);
}
#endif

/*!
	Show search widget in SEARCH_DIRRECTORY mode
*/
void SearchAndReplace::showSearchFolder () 
{
	if (mSearchThread && mSearchThread->isRunning())
		return;
	
	mMode = SEARCH_DIRECTORY;
	updateSearchTextOnUI ();
	mWidget->show (mMode);
}

/*!
	Show search widget in REPLACE_DIRRECTORY mode
*/
void SearchAndReplace::showReplaceFolder () 
{
	if (mSearchThread && mSearchThread->isRunning())
		return;
	
	mMode = REPLACE_DIRECTORY;
	updateSearchTextOnUI ();
	mWidget->show (mMode);
}

/*!
	Handler of "search previous" button
*/
void SearchAndReplace::onPreviousClicked()
{
	searchFile (false, false, true);
}

/*!
	Handler of "search next" button
	Button used for search next in the file, for start search in the dirrectory
*/
void SearchAndReplace::onNextClicked()
{
	switch (mMode)
	{
		case SEARCH_FILE:
		case REPLACE_FILE:
			if (! isPathValid ())
				return;
			
			searchFile (true, false, true);
		break;
		
		case SEARCH_DIRECTORY:
		case REPLACE_DIRECTORY:
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
				QString path = mWidget->path();
				QString mask = mWidget->mask();
				QString searchText = mWidget->searchText ();
				QString replaceText = mWidget->replaceText ();
				
				bool caseSensetive = mWidget->isCaseSensetive ();
				bool regexp = mWidget->isRegExp ();
				
				if (mMode == SEARCH_DIRECTORY)
					mSearchThread = new SearchThread(SearchThread::SEARCH, path, mask, searchText, "", caseSensetive, regexp, this);
				else
					mSearchThread = new SearchThread(SearchThread::REPLACE, path, mask, searchText, replaceText, caseSensetive, regexp, this);
				
				mWidget->setNextButtonText (tr("&Stop"));
				mWidget->setNextButtonIcon (QIcon(":/console/icons/console/stop.png"));
				
				connect (mSearchThread, SIGNAL (readPleaseResults ()), this, SLOT (readThreadResults ()));
				connect (mSearchThread, SIGNAL (finished ()), this, SLOT (threadFinished()));
				mSearchThread->start();    
			}
		break;
	}
	return;
}

/*!
	Handler of "replace" button. Need do one replacement in the file
*/
void SearchAndReplace::onReplaceClicked()
{
	/* Check replace text */
	if (!isReplaceTextValid ())
		return;
	
	replace (false);	
}

/*!
	Do all replacements in the file/dirrectory (according with current mode)
*/
void SearchAndReplace::onReplaceAllClicked()
{
	/* Check replace text */
	if (!isReplaceTextValid ())
		return;

	if (mMode == REPLACE_FILE)
	{
		pChild* child = qobject_cast<pChild*> (MonkeyCore::workspace()->currentChild());
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
	else
	{
		replaceInDirrectory();
	}
}

/*!
	If mode is SEARCH_FILE - do incremental search
*/
void SearchAndReplace::onSearchTextEdited()
{
	if (mMode != SEARCH_FILE) // incremental search, only for search in file
		return;
	
	searchFile (true, true, true);
}

/*!
	Handler of click on search result. Open file with occurence, set cursor to it
*/
void SearchAndReplace::makeGoTo (const QString& file, const QPoint& position)
{
	MonkeyCore::workspace()->goToLine(file, position, true, pMonkeyStudio::defaultCodec());
}

/*!
	Handler of finish of search thread work
*/
void SearchAndReplace::threadFinished ()
{
	mWidget->setNextButtonText (tr("&Search"));
	mWidget->setNextButtonIcon (QIcon(":/edit/icons/edit/search.png"));

	mSearchThread->lockResultsAccessMutex ();
	showMessage (QString("Searching finished. %1 occurences").arg (mSearchThread->foundOccurencesCount()));
	mSearchThread->unlockResultsAccessMutex ();
	
	delete mSearchThread;
	mSearchThread = NULL;
}

/*!
	Read results from buffer in the search thread. Called by signal from thread (signal comes by timer).
	Blocks thread using mutex, read data, than unlocks mutex
*/
void SearchAndReplace::readThreadResults ()
{
	mSearchThread->lockResultsAccessMutex ();
	foreach (Occurence occurence, mSearchThread->newFoundOccurences())
		mDock->appendSearchResult (occurence);
	mSearchThread->clearNewFoundOccurences();
	showMessage(tr ("%1 files %2 occcurences").arg(mSearchThread->processedFilesCount()).arg(mSearchThread->foundOccurencesCount()));
	mSearchThread->unlockResultsAccessMutex ();
}

Q_EXPORT_PLUGIN2( BaseMessageBox, SearchAndReplace )
