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
#include "pEditor.h"
#include "MonkeyCore.h"

#include "qscilexer.h"

#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QApplication>

pChild::pChild()
	: pAbstractChild()
{
	// create textedit
	mEditor = new pEditor;

	// create layout
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );

	// add textedit
	vl->addWidget( mEditor );

	// connections
	connect( mEditor, SIGNAL( cursorPositionChanged( const QPoint& ) ), this, SIGNAL( cursorPositionChanged( const QPoint& ) ) );
	connect( mEditor, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( undoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( redoAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( copyAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( pasteAvailable( bool ) ), this, SIGNAL( pasteAvailableChanged( bool ) ) );
	connect( mEditor, SIGNAL( modificationChanged( bool ) ), this, SIGNAL( modifiedChanged( bool ) ) );
	connect( this, SIGNAL( modifiedChanged( bool ) ), window(), SLOT( setWindowModified( bool ) ) );
}

pChild::~pChild()
{}

pEditor* pChild::editor()
{
	return mEditor;
}

void pChild::cursorPositionChanged()
{ emit pAbstractChild::cursorPositionChanged( cursorPosition() ); }

QString pChild::language() const
{
	// return the editor language
	if ( mEditor->lexer() )
		return mEditor->lexer()->language();

	// return nothing
	return QString();
}

QPoint pChild::cursorPosition() const
{ return mEditor->cursorPosition(); }

void pChild::showFile( const QString& )
{}

QString pChild::currentFile() const
{ return mFiles.value( 0 ); }

QString pChild::currentFileName() const
{ return QFileInfo( currentFile() ).fileName(); }

pEditor* pChild::currentEditor() const
{ return mEditor; }

bool pChild::isModified() const
{ return mEditor->isModified(); }

bool pChild::isUndoAvailable() const
{ return mEditor->isUndoAvailable(); }

void pChild::undo()
{ mEditor->undo(); }

bool pChild::isRedoAvailable() const
{ return mEditor->isRedoAvailable(); }

void pChild::redo()
{ mEditor->redo(); }

void pChild::cut()
{ mEditor->cut(); }

void pChild::copy()
{ mEditor->copy(); }

void pChild::paste()
{ mEditor->paste(); }

void pChild::goTo()
{ mEditor->invokeGoToLine(); }

void pChild::goTo( const QString& s, const QPoint& p, bool )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	mEditor->setCursorPosition( p.y() -1, p.x() );
	mEditor->ensureLineVisible( p.y() -1 );
	mEditor->setFocus();
}

bool pChild::isCopyAvailable() const
{ return mEditor->copyAvailable(); }

bool pChild::isPasteAvailable() const
{ return mEditor->canPaste(); }

bool pChild::isGoToAvailable() const
{ return true; }

bool pChild::isModified( const QString& ) const
{ return isModified(); }

bool pChild::isPrintAvailable() const
{ return true; }

void pChild::saveFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	mEditor->saveFile( s );
}

void pChild::backupCurrentFile( const QString& s )
{ mEditor->saveBackup( s ); }

void pChild::saveFiles()
{ saveCurrentFile(); }

void pChild::openFile( const QString& s, QTextCodec* )
{
	// if already open file, cancel
	if ( !currentFile().isNull() )
		return;

	// open file
	if ( !mEditor->openFile( s ) )
		return;

	// add filename to list
	mFiles.append( s );

	// change window title
	setWindowTitle( s );

	emit fileOpened( s +"[*]" );
}

void pChild::closeFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	mEditor->closeFile();

	// remove from files list
	mFiles.removeAll( s );

	// change window title
	setWindowTitle( QString() );

	emit fileClosed( s );
}

void pChild::closeFiles()
{ closeCurrentFile(); }

void pChild::printFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	// print file
	mEditor->print();
}

void pChild::quickPrintFile( const QString& s )
{
	// if not exists cancel
	if ( !mFiles.contains( s ) )
		return;

	// print file
	mEditor->quickPrint();
}
