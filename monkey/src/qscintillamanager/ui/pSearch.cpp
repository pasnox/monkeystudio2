/****************************************************************************
**
**         Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pSearch.cpp
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
#include "pSearch.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"
#include "pWorkspace.h"
#include "UIProjectsManager.h"
#include "pChild.h"
#include "pEditor.h"
#include "UIMain.h"

#include "SearchThread.h"


#include "qsciscintilla.h"

#include <QKeyEvent>
#include <QDir>
#include <QStatusBar>

#include <QGridLayout>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include <QDebug>

pSearch::pSearch()
    : pDockWidget()
{
    
    // clear informations edit
    //lInformations->clear();
    QWidget * wgt = new QWidget (this);
	
	layout = new QGridLayout (wgt);
	layout->setContentsMargins (0, 0, 0, 0);
	layout->setSpacing (1);
	//search
	lSearchText = new QLabel (tr("Search:"));
	lSearchText->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	lSearchText->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	leSearch = new QLineEdit ();
	leSearch->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);

	tbPrevious = new QPushButton ();
	tbPrevious->setText (tr("Previous"));
	tbPrevious->setIcon (QIcon (":/edit/icons/edit/previous.png"));
	tbPrevious->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbPrevious->setFlat (true);
	
	tbNext = new QPushButton ();
	tbNext->setText (tr("Next"));
    tbNext->setIcon (QIcon(":/edit/icons/edit/next.png"));
	tbNext->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbNext->setFlat (true);
	
	cbCaseSensitive = new QCheckBox ();
	cbCaseSensitive->setText (tr("Case"));
	cbCaseSensitive->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	
	cbRegExp = new QCheckBox ();
	cbRegExp->setText (tr("RegExp"));
	cbRegExp->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	
	//folder
	lPath = new QLabel  (tr("Path:"));
	lePath = new QLineEdit ();
	lePath->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	lePath->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	tbPath = new QToolButton ();
	tbPath->setText ("...");
	lMask = new QLabel  (tr("Mask:"));
	lMask->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	leMask = new QLineEdit ();
	leMask->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);

	//replace
	lReplaceText = new QLabel (tr("Replace:"));
	lReplaceText->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	leReplaceText = new QLineEdit ();
	leReplaceText->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	tbReplace = new QPushButton ();
	tbReplace->setText (tr("Replace"));
	tbReplace->setIcon (QIcon (":/edit/icons/edit/replace.png"));
	tbReplace->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbReplace->setFlat (true);
	
	tbReplaceAll = new QPushButton ();
	tbReplaceAll->setText (tr("Replace all"));
	tbReplaceAll->setIcon (QIcon (":/edit/icons/edit/replace.png"));
	tbReplaceAll->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbReplaceAll->setFlat (true);
	
	wgt->setLayout (layout);
	
	setWidget (wgt);
	
    qRegisterMetaType<pConsoleManager::Step>("pConsoleManager::Step");
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFile" ), SIGNAL( triggered() ), SLOT( showSearchFile() ) );
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFile" ), SIGNAL( triggered() ), SLOT( showReplaceFile() ) );
    //connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchProject" ), SIGNAL( triggered() ), SLOT( showSearchProject() ) );
    //connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceProject" ), SIGNAL( triggered() ), SLOT( showReplaceProject() ) );
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFolder" ), SIGNAL( triggered() ), SLOT( showSearchFolder() ) );
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFolder" ), SIGNAL( triggered() ), SLOT( showReplaceFolder() ) );
    connect(this, SIGNAL (clearSearchResults ()), MonkeyCore::workspace(), SIGNAL (clearSearchResults ()));
    
	mSearchThread = NULL;
	
	widget()->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Maximum);
	addSearchToLayout (0);
	
	setAllowedAreas (Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
}

void pSearch::addSearchToLayout (int row)
{
	layout->addWidget (lSearchText, row, 0, 1, 1);
	layout->addWidget (leSearch, row, 1, 1, 2);
	layout->addWidget (tbPrevious, row, 3, 1, 1);
	layout->addWidget (tbNext, row, 4, 1, 1);
	layout->addWidget (cbCaseSensitive, row, 5, 1, 1);
	layout->addWidget (cbRegExp, row, 6, 1, 1);
}

void pSearch::addReplaceToLayout (int row)
{
	layout->addWidget (lReplaceText, row, 0, 1, 1);
	layout->addWidget (leReplaceText, row, 1, 1, 2);
	layout->addWidget (tbReplace, row, 3, 1, 1);
	layout->addWidget (tbReplaceAll, row, 4, 1, 1);
	
	lReplaceText->show();
	leReplaceText->show();
	tbReplace->show();
	tbReplaceAll->show();
}

void pSearch::addFolderToLayout (int row)
{
	layout->addWidget (lPath, row, 0, 1, 1);
	layout->addWidget (lePath, row, 1, 1, 1);
	layout->addWidget (tbPath, row, 2, 1, 1);
	layout->addWidget (lMask, row, 3, 1, 1);
	layout->addWidget (leMask, row, 4, 1, 3);
	
	lPath->show ();
	lePath->show ();
	tbPath->show ();
	lMask->show ();
	leMask->show ();
}

void pSearch::removeSearchFromLayout ()
{
	layout->removeWidget (lSearchText);
	layout->removeWidget(leSearch);
	layout->removeWidget (tbPrevious);
	layout->removeWidget (tbNext);
	layout->removeWidget (cbCaseSensitive);
	layout->removeWidget (cbRegExp);
}

void pSearch::removeReplaceFromLayout ()
{
	layout->removeWidget (lReplaceText);
	layout->removeWidget (leReplaceText);
	layout->removeWidget (tbReplace);
	layout->removeWidget (tbReplaceAll);
	
	lReplaceText->hide();
	leReplaceText->hide();
	tbReplace->hide();
	tbReplaceAll->hide();
}

void pSearch::removeFolderFromLayout ()
{
	layout->removeWidget (lPath);
	layout->removeWidget (lePath);
	layout->removeWidget (tbPath);
	layout->removeWidget (lMask);
	layout->removeWidget (leMask);
	
	lPath->hide ();
	lePath->hide ();
	tbPath->hide ();
	lMask->hide ();
	leMask->hide ();
}

bool pSearch::isProjectAvailible ()
{
    if (MonkeyCore::projectsManager()->currentProject())
        return true;
    else
        return false;
}

void pSearch::keyPressEvent( QKeyEvent* e )
{
    if ( e->key() == Qt::Key_Escape )
        hide();
    QDockWidget::keyPressEvent( e );
}


void pSearch::showSearchFile () 
{
	if (mOperType == REPLACE)
		removeReplaceFromLayout ();
	mOperType = SEARCH;
	
	if (mWhereType == FOLDER)
		removeFolderFromLayout ();
	mWhereType = FILE;
    show ();
};

void pSearch::showReplaceFile () 
{
	if (mWhereType == FOLDER)
		removeFolderFromLayout ();
	mWhereType = FILE;
	
	if (mOperType == SEARCH)
		addReplaceToLayout(1);
	mOperType = REPLACE;
	
    show ();
};

/*
void pSearch::showSearchProject () 
{
    mOperType = SEARCH;
    mWhereType = PROJECT;
    show ();
};

void pSearch::showReplaceProject () 
{
    mOperType = REPLACE;
    mWhereType = PROJECT;
    show ();
};
*/

void pSearch::showSearchFolder () 
{
	if (mOperType == REPLACE)
		removeReplaceFromLayout ();
	mOperType = SEARCH;
	
	if (mWhereType == FILE)
		addFolderToLayout(1);
	mWhereType = FOLDER;
    show ();
};

void pSearch::showReplaceFolder () 
{
	if (mOperType == SEARCH)
		addReplaceToLayout (1);
	mOperType = REPLACE;
	
	if (mWhereType == FILE)
		addFolderToLayout (2);
	mWhereType = FOLDER;
    show ();
};

void pSearch::show ()
{
	leSearch->setFocus();
    QDockWidget::show ();
}

bool pSearch::search (bool next)
{
    pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
    if (!child && !child->editor())
    {
        //lInformations->setText(tr( "No active editor" ) );
        return false;
    }
    pEditor* editor = child->editor ();

    // get cursor position
    int x, y;
    /*if (cbFromStart->isChecked())
    {
        x = 0;
        y = 0;
        cbFromStart->setChecked( false );
    }
    else
    {*/
        editor->getCursorPosition( &y, &x );
    /*}*/

    if (!next)
    {
        int temp;
        editor->getSelection(&y, &x, &temp, &temp);
    }

    // search
    bool b = editor->findFirst( leSearch->text(), cbRegExp->isChecked(), cbCaseSensitive->isChecked(), false, true, next, y, x);

    // change background acording to found or not
    QPalette p = leSearch->palette();
    p.setColor( leSearch->backgroundRole(), b ? Qt::white : Qt::red );
    leSearch->setPalette( p );
    
    // show message if needed
    //lInformations->setText( b ? QString::null : tr( "Not Found" ) );

    // return found state
    return b;
}

bool pSearch::on_tbPrevious_clicked()
{
    return search (false);
}

bool pSearch::on_tbNext_clicked()
{
    switch (mWhereType)
    {
        case FILE:
            return search (true);
        case FOLDER:
            if (mSearchThread && mSearchThread->isRunning ())
            { // need to stop searching
                mSearchThread->setTermEnabled (true);
            }
            else
            { // need to start searching
                emit clearSearchResults ();
                mOccurencesFinded = 0;
                mFilesProcessed = 0;
                fileProcessed (0);
                QString path = lePath->text();
                QString text = leSearch->text();
                //bool whole = cbWholeWords->isChecked ();
                bool match = cbCaseSensitive->isChecked();
                bool regexp = cbRegExp->isChecked ();
                mSearchThread = new SearchThread(path, text, true, match, regexp, this);
                tbNext->setText (tr("Stop"));
                tbNext->setIcon (QIcon(":/console/icons/console/stop.png"));
                
                qWarning () << __LINE__;
                connect (mSearchThread, SIGNAL (appendSearchResult( const pConsoleManager::Step& )), MonkeyCore::workspace(), SIGNAL (appendSearchResult( const pConsoleManager::Step& )));
                connect (mSearchThread, SIGNAL (finished ()), this, SLOT (threadFinished()));
                connect (mSearchThread, SIGNAL (appendSearchResult( const pConsoleManager::Step& )), this, SLOT (occurenceFinded ()));
                connect (mSearchThread, SIGNAL (changeProgress(int)), this, SLOT (fileProcessed (int)));
                qWarning () << __LINE__;
                mSearchThread->start();    
            }
        break;
    }
    return true;
}

//
int pSearch::replace(bool all)
{
    pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
    if (!child && !child->editor())
    {
        //lInformations->setText(tr( "No active editor" ) );
        return 0;
    }
    pEditor* editor = child->editor ();


    int x, y, temp;
    editor->getSelection(&y, &x, &temp, &temp);
    editor->setCursorPosition(y, x);
    
    QString rtext = leReplaceText->text();
    int count;
    if (on_tbNext_clicked())
    {
        editor->replace (rtext);
        count = 1;
    }
    
    if (all)
    {
        while (on_tbNext_clicked())
        {
            editor->replace(rtext);
            count++;
        };
    }
    else
    {
        editor->findNext(); //move selection to next item
    }
    
    return count;
}

void pSearch::on_tbReplace_clicked()
{
    replace (false);
    
}
//
void pSearch::on_tbReplaceAll_clicked()
{
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
    //lInformations->setText( count ? tr( "%1 occurences replaced" ).arg( count ) : tr( "Nothing To Repalce" ) );
}

void pSearch::threadFinished ()
{
    qWarning () << mSearchThread->isFinished();
    tbNext->setText (tr("Search"));
    tbNext->setIcon (QIcon(":/edit/icons/edit/search.png"));
    delete mSearchThread;
    mSearchThread = NULL;
}

void pSearch::occurenceFinded ()
{
    mOccurencesFinded ++;
    //lInformations->setText (tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFinded));
}

void pSearch::fileProcessed (int count)
{
    mFilesProcessed = count;
    showMessage (tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFinded));
}

void pSearch::showMessage (QString status)
{
    MonkeyCore::mainWindow()->statusBar()->showMessage (tr ("Search: %1").arg (status), 30);
}
