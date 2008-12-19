/****************************************************************************
**
**         Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : SearchWidget.cpp
** Date      : 2008-01-14T00:37:03
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

#include <QKeyEvent>
#include <QDir>
#include <QStatusBar>
#include <QFileDialog>

#include <QGridLayout>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QCompleter>
#include <QDirModel>

#include <QDebug>

#include "pWorkspace.h"

#include "SearchAndReplace.h"

#include "SearchWidget.h"

SearchWidget::SearchWidget( QWidget* parent )
	: QWidget( parent )
{
	layout = new QGridLayout (this);
	layout->setContentsMargins (0, 0, 0, 0);
	layout->setSpacing (2);

	//search
	lSearchText = new QLabel (tr("Searc&h:"));
	lSearchText->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	lSearchText->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobSearch = new QComboBox ();
	cobSearch->setEditable (true);
	cobSearch->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	cobSearch->completer()->setCaseSensitivity (Qt::CaseSensitive);
	lSearchText->setBuddy (cobSearch);

	tbPrevious = new QPushButton ();
	tbPrevious->setText (tr("&Previous"));
	tbPrevious->setIcon (QIcon (":/edit/icons/edit/previous.png"));
	tbPrevious->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbPrevious->setFlat (true);
	
	tbNext = new QPushButton ();
	tbNext->setText (tr("&Next"));
	tbNext->setIcon (QIcon(":/edit/icons/edit/next.png"));
	tbNext->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbNext->setFlat (true);
	
	cbCaseSensitive = new QCheckBox ();
	cbCaseSensitive->setText (tr("&Case"));
	cbCaseSensitive->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	
	cbRegExp = new QCheckBox ();
	cbRegExp->setText (tr("Re&gExp"));
	cbRegExp->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	
	//replace
	lReplaceText = new QLabel (tr("R&eplace:"));
	lReplaceText->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobReplace = new QComboBox ();
	cobReplace->setEditable (true);
	cobReplace->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	cobReplace->completer()->setCaseSensitivity (Qt::CaseSensitive);
	lReplaceText->setBuddy (cobReplace);
	
	tbReplace = new QPushButton ();
	tbReplace->setText (tr("&Replace"));
	tbReplace->setIcon (QIcon (":/edit/icons/edit/replace.png"));
	tbReplace->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbReplace->setFlat (true);
	
	tbReplaceAll = new QPushButton ();
	tbReplaceAll->setText (tr("Replace &all"));
	tbReplaceAll->setIcon (QIcon (":/edit/icons/edit/replace.png"));
	tbReplaceAll->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbReplaceAll->setFlat (true);

	//folder
	lPath = new QLabel  (tr("&Path:"));
	lPath->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobPath = new QComboBox();
	cobPath->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	cobPath->setEditable (true);
	cobPath->lineEdit()->setCompleter (new QCompleter (new QDirModel()));
	lPath->setBuddy (cobPath);
	
	tbPath = new QToolButton ();
	tbPath->setText ("...");
	lMask = new QLabel  (tr("&Mask:"));
	lMask->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobMask = new QComboBox ();
	cobMask->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	cobMask->setEditable (true);
	cobMask->setToolTip (tr("Spase separated list of wildcards. Example:<br> <i>*.h *.cpp file???.txt</i>"));
	cobMask->completer()->setCaseSensitivity (Qt::CaseSensitive);
	lMask->setBuddy (cobMask);

	connect(tbPath, SIGNAL( clicked() ), this, SLOT( onPathClicked() ));
	
	connect (tbNext, SIGNAL (clicked()), this, SIGNAL (nextClicked ()));
//	connect (cobSearch->lineEdit(), SIGNAL (textEdited (const QString&)), this, SIGNAL (nextClicked ()));
	
	connect (tbPrevious, SIGNAL (clicked()), this, SIGNAL (previousClicked ()));
	connect (tbReplace, SIGNAL (clicked()), this, SIGNAL (replaceClicked ()));
	connect (tbReplaceAll, SIGNAL (clicked()), this, SIGNAL (replaceAllClicked ()));
	
#if 0	

	qRegisterMetaType<pConsoleManager::Step>("pConsoleManager::Step");

	mSearchThread = NULL;
	
	setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Maximum);
	
	mOperType = SEARCH;
	mWhereType = FILE;
#endif

	mDefaultEditColor = cobSearch->lineEdit()->palette().color (QPalette::Base);
}

void SearchWidget::show (SearchAndReplace::Mode mode)
{
	mMode = mode;
	removeSearchFromLayout ();
	removeReplaceFromLayout ();
	removeFolderFromLayout ();
	
	addSearchToLayout (0);
	int i = 1;
	if (mode == SearchAndReplace::REPLACE_FILE || mode == SearchAndReplace::REPLACE_DIRRECTORY)
		addReplaceToLayout (i++);
	if (mode == SearchAndReplace::SEARCH_DIRRECTORY || mode == SearchAndReplace::REPLACE_DIRRECTORY)
		addFolderToLayout (i++);

	tbPrevious->hide();
	tbReplace->hide();

	if (mode == SearchAndReplace::SEARCH_FILE || mode == SearchAndReplace::REPLACE_FILE)
	{
		tbNext->setText (tr("&Next"));
		tbNext->setIcon (QIcon(":/edit/icons/edit/next.png"));
		tbPrevious->show();
	}
	else
	{
		tbNext->setText (tr("&Search"));
		tbNext->setIcon (QIcon(":/edit/icons/edit/search.png"));
	}
	
	if (mode == SearchAndReplace::REPLACE_FILE)
	{
		tbReplace->show ();
	}

	cobPath->lineEdit()->setText (QDir::current().absolutePath());
	
	cobSearch->setFocus();

	switch (mode)
	{
		case SearchAndReplace::REPLACE_FILE:
			QWidget::setTabOrder (cobSearch->lineEdit(), cobReplace->lineEdit());
		break;
#if 0
		SEARCH_PROJECT = 2,
		REPLACE_PROJECT = 3
#endif
		case SearchAndReplace::SEARCH_DIRRECTORY:
			QWidget::setTabOrder (cobSearch->lineEdit(), cobPath->lineEdit());
		break;
		case SearchAndReplace::REPLACE_DIRRECTORY:
			QWidget::setTabOrder (cobSearch->lineEdit(), cobReplace->lineEdit());
			QWidget::setTabOrder (cobReplace->lineEdit(), cobPath->lineEdit());
		break;
		default:
		break;
	}
	
	cobSearch->lineEdit()->selectAll ();
	
	QWidget::show ();
}

void SearchWidget::addSearchToLayout (int row)
{
	layout->addWidget (lSearchText, row, 0, 1, 1);
	layout->addWidget (cobSearch, row, 1, 1, 2);
	layout->addWidget (tbPrevious, row, 3, 1, 1);
	layout->addWidget (tbNext, row, 4, 1, 1);
	layout->addWidget (cbCaseSensitive, row, 5, 1, 1);
	layout->addWidget (cbRegExp, row, 6, 1, 1);
}

void SearchWidget::addReplaceToLayout (int row)
{
	layout->addWidget (lReplaceText, row, 0, 1, 1);
	layout->addWidget (cobReplace, row, 1, 1, 2);
	layout->addWidget (tbReplace, row, 3, 1, 1);
	layout->addWidget (tbReplaceAll, row, 4, 1, 1);
	
	lReplaceText->show();
	cobReplace->show();
	tbReplace->show();
	tbReplaceAll->show();
}

void SearchWidget::addFolderToLayout (int row)
{
	layout->addWidget (lPath, row, 0, 1, 1);
	layout->addWidget (cobPath, row, 1, 1, 1);
	layout->addWidget (tbPath, row, 2, 1, 1);
	layout->addWidget (lMask, row, 3, 1, 1);
	layout->addWidget (cobMask, row, 4, 1, 3);
	
	lPath->show ();
	cobPath->show ();
	tbPath->show ();
	lMask->show ();
	cobMask->show ();
}

void SearchWidget::removeSearchFromLayout ()
{
	layout->removeWidget (lSearchText);
	layout->removeWidget(cobSearch);
	layout->removeWidget (tbPrevious);
	layout->removeWidget (tbNext);
	layout->removeWidget (cbCaseSensitive);
	layout->removeWidget (cbRegExp);
}

void SearchWidget::removeReplaceFromLayout ()
{
	layout->removeWidget (lReplaceText);
	layout->removeWidget (cobReplace);
	layout->removeWidget (tbReplace);
	layout->removeWidget (tbReplaceAll);
	
	lReplaceText->hide();
	cobReplace->hide();
	tbReplace->hide();
	tbReplaceAll->hide();
}

void SearchWidget::removeFolderFromLayout ()
{
	layout->removeWidget (lPath);
	layout->removeWidget (cobPath);
	layout->removeWidget (tbPath);
	layout->removeWidget (lMask);
	layout->removeWidget (cobMask);
	
	lPath->hide ();
	cobPath->hide ();
	tbPath->hide ();
	lMask->hide ();
	cobMask->hide ();
}

void SearchWidget::keyPressEvent( QKeyEvent* e )
{
	switch (e->key())
	{
		case Qt::Key_Escape:
			hide();
			MonkeyCore::workspace()->focusToEditor_triggered ();
		break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			if (mMode == SearchAndReplace::SEARCH_FILE || 
				mMode == SearchAndReplace::SEARCH_DIRRECTORY ||
				mMode == SearchAndReplace::REPLACE_DIRRECTORY)
					emit nextClicked();
			else /* replace */
				emit replaceClicked();
		break;
	}
	QWidget::keyPressEvent( e );
}

void SearchWidget::searchAddToRecents (QString text)
{
	if (searchRecents.isEmpty() || searchRecents[0] != text)
	{
		int i;
		while ((i = searchRecents.indexOf(text)) != -1)
			searchRecents.removeAt (i);
		searchRecents.prepend (text);
		if (searchRecents.size() > 10)
		{
			searchRecents.removeLast ();
		}
		cobSearch->clear();
		cobSearch->addItems (searchRecents);
	}
}

void SearchWidget::replaceAddToRecents (QString text)
{
	if (replaceRecents.isEmpty() || replaceRecents[0] != text)
	{
		int i;
		while ((i = replaceRecents.indexOf(text)) != -1)
			replaceRecents.removeAt (i);
		replaceRecents.prepend (text);
		if (replaceRecents.size() > 10)
		{
			replaceRecents.removeLast ();
		}
		cobReplace->clear();
		cobReplace->addItems (replaceRecents);
	}
}

void SearchWidget::pathAddToRecents (QString text)
{
	if (pathRecents.isEmpty() || pathRecents[0] != text)
	{
		int i;
		while ((i = pathRecents.indexOf(text)) != -1)
			pathRecents.removeAt (i);
		pathRecents.prepend (text);
		if (pathRecents.size() > 10)
		{
			pathRecents.removeLast ();
		}
		cobPath->clear();
		cobPath->addItems (pathRecents);
	}
}

void SearchWidget::maskAddToRecents (QString text)
{
	if (maskRecents.isEmpty() || maskRecents[0] != text)
	{
		int i;
		while ((i = maskRecents.indexOf(text)) != -1)
			maskRecents.removeAt (i);
		maskRecents.prepend (text);
		if (maskRecents.size() > 10)
		{
			maskRecents.removeLast ();
		}
		cobMask->clear();
		cobMask->addItems (maskRecents);
	}
}

void SearchWidget::setSearchLineEditColor (LineEditColor color)
{
	QPalette palette = cobSearch->lineEdit()->palette();
	switch (color)
	{
		case GREEN:
			palette.setColor (QPalette::Base, Qt::green);
		break;
		case RED:
			palette.setColor (QPalette::Base, Qt::green);
		break;
		case DEFAULT:
			palette.setColor (QPalette::Base, mDefaultEditColor);
		break;
	}
}

void SearchWidget::setSearchText (const QString& text)
{
	cobSearch->lineEdit()->setText (text);
}

void SearchWidget::setNextButtonText (const QString& text)
{
	tbNext->setText (text);
}

void SearchWidget::setNextButtonIcon (const QIcon& icon)
{
	tbNext->setIcon (icon);
}

bool SearchWidget::isRegExp ()
{
	return cbRegExp->isChecked();
}

bool SearchWidget::isCaseSensetive ()
{
	return cbCaseSensitive->isChecked();
}

QString SearchWidget::searchText()
{
	return cobSearch->lineEdit()->text();
}

QString SearchWidget::replaceText()
{
	return cobReplace->lineEdit()->text();
}

QString SearchWidget::path()
{
	return cobPath->lineEdit()->text();
}

QString SearchWidget::mask()
{
	return cobMask->lineEdit()->text();
}

#if 0
bool SearchWidget::isProjectAvailible ()
{
	if (MonkeyCore::projectsManager()->currentProject())
		return true;
	else
		return false;
}


bool SearchWidget::searchFile (bool next, bool wrap)
{
	QString text = cobSearch->currentText ();
	
	searchAddToRecents(text);
	
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
	bool b = editor->findFirst( text, cbRegExp->isChecked(), cbCaseSensitive->isChecked(), false, wrap, next, y, x);

	// change background acording to found or not
	QPalette p = cobSearch->palette();
	p.setColor( cobSearch->backgroundRole(), b ? Qt::white : Qt::red );
	cobSearch->setPalette( p );
	
	// show message if needed
	showMessage( b ? QString::null : tr( "Not Found" ) );

	// return found state
	return b;
}

//
int SearchWidget::replace(bool all)
{
	QString rtext = cobReplace->currentText();
	
	searchAddToRecents(cobSearch->currentText());
	replaceAddToRecents(rtext);
	
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
		while (searchFile(true, false)) //search next, wrap switched off
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

		if (on_tbNext_clicked())
			{
				editor->replace (rtext);
				count = 1;
				editor->findNext(); //move selection to next item
			}
	}
	
	return count;
}

void SearchWidget::threadFinished ()
{
	tbNext->setText (tr("Search"));
	tbNext->setIcon (QIcon(":/edit/icons/edit/search.png"));
	delete mSearchThread;
	mSearchThread = NULL;
	showMessage ("Searching finished");
}

void SearchWidget::occurenceFinded ()
{
	mOccurencesFinded ++;
	showMessage(tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFinded));
}

void SearchWidget::fileProcessed (int count)
{
	mFilesProcessed = count;
	showMessage (tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFinded));
}

void SearchWidget::showMessage (QString status)
{
	if (!status.isNull())
		MonkeyCore::mainWindow()->statusBar()->showMessage (tr ("Search: %1").arg (status), 30000);
	else
		MonkeyCore::mainWindow()->statusBar()->showMessage ("");
}


#endif

void SearchWidget::onPathClicked ()
{
	QString text = QFileDialog::getExistingDirectory ( this, tr("Search path"), cobPath->currentText(), 0);
	if (!text.isNull())
		cobPath->lineEdit ()->setText (text);
}
