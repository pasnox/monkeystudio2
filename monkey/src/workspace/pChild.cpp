/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pChild.cpp
** Date      : 2008-01-14T00:37:20
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
#include "pChild.h"
#include "../qscintillamanager/pEditor.h"
#include "../coremanager/MonkeyCore.h"

#include <qscintilla.h>

#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QApplication>

pChild::pChild()
	: pAbstractChild()
{
	// create textedit
	mEditor = new pEditor;
	mEditor->setAttribute( Qt::WA_MacSmallSize );
	mEditor->setFrameStyle( QFrame::NoFrame | QFrame::Plain );

	setWidget( mEditor );
	setFocusProxy( mEditor );
	setWindowIcon( QIcon() );

	// connections
	connect( mEditor, SIGNAL( cursorPositionChanged( const QPoint& ) ), this, SIGNAL( cursorPositionChanged( const QPoint& ) ) );
	connect( mEditor, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( undoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( redoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( copyAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( pasteAvailable( bool ) ), this, SIGNAL( pasteAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( modificationChanged( bool ) ), this, SLOT( setWindowModified( bool ) ) );
	connect( mEditor, SIGNAL( modificationChanged( bool ) ), this, SIGNAL( modifiedChanged( bool ) ) );
	connect( mEditor, SIGNAL( textChanged() ), this, SIGNAL( contentChanged() ) );
}

pChild::~pChild()
{
}

void pChild::cursorPositionChanged()
{
	emit pAbstractChild::cursorPositionChanged( cursorPosition() );
}

QString pChild::language() const
{
	// return the editor language
	if ( mEditor->lexer() )
	{
		return mEditor->lexer()->language();
	}

	// return nothing
	return QString();
}

QString pChild::fileBuffer() const
{
	return mEditor->text();
}

QString pChild::context() const
{
	return QLatin1String( "Coding" );
}

void pChild::initializeContext( QToolBar* tb )
{
	Q_UNUSED( tb );
}

QPoint pChild::cursorPosition() const
{
	return mEditor->cursorPosition();
}

pEditor* pChild::editor() const
{
	return mEditor;
}

bool pChild::isModified() const
{
	return mEditor->isModified();
}

bool pChild::isUndoAvailable() const
{
	return mEditor->isUndoAvailable();
}

void pChild::invokeSearch () 
{
	/*MonkeyCore::searchWidget()->showSearchFile ();*/
}

void pChild::undo()
{
	mEditor->undo();
}

bool pChild::isRedoAvailable() const
{
	return mEditor->isRedoAvailable();
}

void pChild::redo()
{
	mEditor->redo();
}

void pChild::cut()
{
	mEditor->cut();
}

void pChild::copy()
{
	mEditor->copy();
}

void pChild::paste()
{
	mEditor->paste();
}

void pChild::goTo()
{
	mEditor->invokeGoToLine();
}

void pChild::goTo( const QPoint& pos, bool highlight )
{
	Q_UNUSED( highlight );
	mEditor->setCursorPosition( pos.y() -1, pos.x() );
	mEditor->ensureLineVisible( pos.y() -1 );
	mEditor->setFocus();
}

bool pChild::isCopyAvailable() const
{
	return mEditor->copyAvailable();
}

bool pChild::isPasteAvailable() const
{
	return mEditor->canPaste();
}

bool pChild::isGoToAvailable() const
{
	return true;
}

bool pChild::isPrintAvailable() const
{
	return true;
}

void pChild::saveFile()
{
	mEditor->saveFile( filePath() );
}

void pChild::backupFileAs( const QString& s )
{
	mEditor->saveBackup( s );
}

bool pChild::openFile( const QString& fileName, const QString& codec )
{
	// if already open file, cancel
	if ( !filePath().isEmpty() )
	{
		return false;
	}
	
	const bool locked = mEditor->blockSignals( true );

	// open file
	if ( !mEditor->openFile( fileName, codec ) )
	{
		mEditor->blockSignals( locked );
		return false;
	}
	
	mEditor->blockSignals( locked );
	
	// set window modified state
	setFilePath( fileName );
	setWindowModified( mEditor->isModified() );
	
	if ( !mCodec )
	{
		mCodec = QTextCodec::codecForName( codec.toUtf8() );
	}

	emit fileOpened();
	return true;
}

void pChild::closeFile()
{
	mEditor->closeFile();
	setFilePath( QString::null );

	emit fileClosed();
}

void pChild::printFile()
{
	mEditor->print();
}

void pChild::quickPrintFile()
{
	// print file
	mEditor->quickPrint();
}
