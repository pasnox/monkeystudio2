/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pEditor.cpp
** Date      : 2008-01-14T00:37:06
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
#include "pEditor.h"
#include "../pMonkeyStudio.h"
#include "../qscintillamanager/qSciShortcutsManager.h"
#include "../coremanager/MonkeyCore.h"

#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QFileInfo>
#include <QTextStream>
#include <QInputDialog>
#include <QPrintDialog>
#include <QDir>
#include <QDateTime>
#include <QTextCodec>
#include <QRegExp>

bool pEditor::mPasteAvailableInit = false;
bool pEditor::mPasteAvailable = false;

pEditor::pEditor( QWidget* p )
	: QsciScintilla( p )
{
	const QSize mPixSize = QSize( 16, 16 );
	// register image for auto completion
	registerImage( riClass, QPixmap( ":/editor/class.png" ).scaled( mPixSize ) );
	registerImage( riEnum, QPixmap( ":/editor/enum.png" ).scaled( mPixSize ) );
	registerImage( riFunction, QPixmap( ":/editor/function.png" ).scaled( mPixSize ) );
	registerImage( riMember, QPixmap( ":/editor/member.png" ).scaled( mPixSize ) );
	registerImage( riNamespace, QPixmap( ":/editor/namespace.png" ).scaled( mPixSize ) );
	registerImage( riStruct, QPixmap( ":/editor/struct.png" ).scaled( mPixSize ) );
	registerImage( riTypedef, QPixmap( ":/editor/typedef.png" ).scaled( mPixSize ) );
	registerImage( riVariable, QPixmap( ":/editor/variable.png" ).scaled( mPixSize ) );
	
	// register image for bookmarks
	markerDefine( QPixmap( ":/editor/bookmark.png" ).scaled( mPixSize ), mdBookmark );
	
	// register image for debugging
	markerDefine( QPixmap( ":/editor/break_enable.png" ).scaled( mPixSize ), mdEnabledBreak );
	markerDefine( QPixmap( ":/editor/break_disable.png" ).scaled( mPixSize ), mdDisabledBreak );
	markerDefine( QPixmap( ":/editor/break_conditionnal_enable.png" ).scaled( mPixSize ), mdEnabledConditionalBreak );
	markerDefine( QPixmap( ":/editor/break_conditionnal_disable.png" ).scaled( mPixSize ), mdDisabledConditionalBreak );
	markerDefine( QPixmap( ":/editor/play.png" ).scaled( mPixSize ), mdPlay );
	
	// deal with utf8
	setUtf8( true );

	// connection
	connect( this, SIGNAL( linesChanged() ), this, SLOT( linesChanged() ) );
	connect( this, SIGNAL( copyAvailable( bool ) ), this, SLOT( setCopyAvailable( bool ) ) );
	connect( this, SIGNAL( cursorPositionChanged( int, int ) ), this, SLOT( cursorPositionChanged( int, int ) ) );
	connect( this, SIGNAL( textChanged() ), this, SLOT( textChanged() ) );
	connect( QApplication::clipboard(), SIGNAL( dataChanged() ), this, SLOT( clipboardDataChanged() ) );

	// init pasteAvailable
	if ( !mPasteAvailableInit )
	{
		mPasteAvailableInit = true;
		mPasteAvailable = !QApplication::clipboard()->text().isEmpty();
	}
	
	// init qscishortcutsmanager if needed
	SendScintilla( QsciScintillaBase::SCI_CLEARALLCMDKEYS );
	SendScintilla( QsciScintillaBase::SCI_ASSIGNCMDKEY, SCK_TAB, SCI_TAB);
	SendScintilla( QsciScintillaBase::SCI_ASSIGNCMDKEY, SCK_ESCAPE, SCI_CANCEL);
	SendScintilla( QsciScintillaBase::SCI_ASSIGNCMDKEY, SCK_RETURN, SCI_NEWLINE);

	// By default control characters don't do anything (rather than insert the
	// control character into the text). (c) Phil
	for (int k = 'A'; k <= 'Z'; ++k)
	SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY,
			k + (QsciScintillaBase::SCMOD_CTRL << 16),
			QsciScintillaBase::SCI_NULL);

	// Create shortcuts manager, if not created
	qSciShortcutsManager::instance();
}

pEditor::~pEditor()
{}

void pEditor::keyPressEvent( QKeyEvent* e )
{
	if ( !e->isAutoRepeat() && e->modifiers() & Qt::ControlModifier && e->key() == Qt::Key_Space )
	{
		switch ( autoCompletionSource() )
		{
			case QsciScintilla::AcsAll:
				autoCompleteFromAll();
				break;
			case QsciScintilla::AcsAPIs:
				autoCompleteFromAPIs();
				break;
			case QsciScintilla::AcsDocument:
				autoCompleteFromDocument();
				break;
			default:
				break;
		}
		return;
	}
	QsciScintilla::keyPressEvent( e );
}

bool pEditor::lineNumbersMarginEnabled() const
{
	return marginLineNumbers( 0 );
}

void pEditor::autoDetectIndent ()
{
	QString currText = "\n" + text(); // \n for more simple RegExp
	QRegExp tabRe = QRegExp ("\n\\t");
	int matchIntex;
	matchIntex = tabRe.indexIn (currText);
	if (matchIntex != -1) // Use tabs
	{
		setIndentationsUseTabs (true);
		return;
	}
	
	QRegExp spaceRe = QRegExp ("\n( +)");
	matchIntex = spaceRe.indexIn (currText);
	if (matchIntex != -1) // Use spaces
	{
		setIndentationsUseTabs (false);
		return;
	}
}

int pEditor::lineNumbersMarginWidth() const
{
	return property( "LineNumbersMarginWidth" ).toInt();
}

bool pEditor::lineNumbersMarginAutoWidth() const
{
	return property( "LineNumbersMarginAutoWidth" ).toBool();
}

void pEditor::setLineNumbersMarginEnabled( bool b )
{
	setMarginLineNumbers( 0, b );
}

void pEditor::setLineNumbersMarginWidth( int i )
{
	int j = i;
	if ( i != 0 )
		j++;

	setProperty( "LineNumbersMarginWidth", i );
	setMarginWidth( 0, QString().fill( '0', j ) );
}

void pEditor::setLineNumbersMarginAutoWidth( bool b )
{
	setProperty( "LineNumbersMarginAutoWidth", b );
	emit linesChanged();
}

void pEditor::linesChanged()
{
	if ( lineNumbersMarginAutoWidth() )
		setLineNumbersMarginWidth( QString::number( lines() ).length() );
}

bool pEditor::copyAvailable()
{
	return mCopyAvailable;
}

bool pEditor::canPaste()
{
	return mPasteAvailable;
}

QPoint pEditor::cursorPosition() const
{
	return mCursorPosition;
}

bool pEditor::markerAtLine( int line, pEditor::MarkerDefineType markerId ) const
{
	return QsciScintilla::markersAtLine( line ) & ( 1 << markerId );
}

int pEditor::markerFindPrevious( int line, pEditor::MarkerDefineType markerId ) const
{
	line = QsciScintilla::markerFindPrevious( line, 1 << markerId );
	if ( line == -1 )
		line = QsciScintilla::markerFindPrevious( lines() -1, 1 << markerId );
	return line;
}

int pEditor::markerFindNext( int line, pEditor::MarkerDefineType markerId ) const
{
	line = QsciScintilla::markerFindNext( line, 1 << markerId );
	if ( line == -1 )
		line = QsciScintilla::markerFindNext( 0, 1 << markerId );
	return line;
}

void pEditor::setCopyAvailable( bool b )
{
	mCopyAvailable = b;
}

void pEditor::cursorPositionChanged( int l, int p )
{
	mCursorPosition = QPoint( p, l );
	emit cursorPositionChanged( mCursorPosition );
}

void pEditor::textChanged()
{
	emit undoAvailable( isUndoAvailable() );
	emit redoAvailable( isRedoAvailable() );
}

void pEditor::clipboardDataChanged()
{
	mPasteAvailable = !QApplication::clipboard()->text().isEmpty();
	emit pasteAvailable( canPaste() );
}

bool pEditor::openFile( const QString& s )
{
	if ( isModified() )
		return false;

	QApplication::setOverrideCursor( Qt::WaitCursor );
	
	// open file
	QFile f( s );
	if ( !f.open( QFile::ReadOnly ) )
	{
		pMonkeyStudio::warning( tr( "Open file..." ), tr( "Cannot read file %1:\n%2." ).arg( s ).arg( f.errorString() ), this );
		return false;
	}

	// remember filename
	setProperty( "fileName", s );

	// set lexer and apis
	setLexer( pMonkeyStudio::lexerForFileName( s ) );

	// set properties
	pMonkeyStudio::setEditorProperties( this );

	// load file
	QTextStream i( &f );
	if ( i.codec()->name() != qPrintable( pMonkeyStudio::defaultEncoding() ) )
		i.setCodec( qPrintable( pMonkeyStudio::defaultEncoding() ) );
	setText( i.readAll() );
	setModified( false );

	// convert tabs if needed
	if ( pMonkeyStudio::convertTabsUponOpen() )
		convertTabs();
	
	//autodetect indent, if need
	if (pMonkeyStudio::autoDetectIndent())
	{
		autoDetectIndent ();
	}
	
	// make backup if needed
	if ( pMonkeyStudio::createBackupUponOpen() )
		makeBackup();

	// convert eol
	if ( pMonkeyStudio::autoEolConversion() )
		convertEols( eolMode() );
	
	QApplication::restoreOverrideCursor();
	
	return true;
}

bool pEditor::saveFile( const QString& s )
{
	if ( !isModified() )
		return true;

	// get filename
	QString fn = s;
	if ( s.isEmpty() )
		fn = property( "fileName" ).toString();
	// get path
	QString fp = QFileInfo( fn ).path();

	// filename
	QFile f( fn );
	// filename dir
	QDir d;
	// create bak folder
	if ( !d.exists( fp ) )
		if ( !d.mkpath( fp ) )
			return false;

	// set correct path
	d.setPath( fp );
	// try open file to write in
	if ( !f.open( QFile::WriteOnly ) )
	{
		pMonkeyStudio::warning( tr( "Save file..." ), tr( "Cannot write file %1:\n%2." ).arg( fn ).arg( f.errorString() ), this );
		return false;
	}

	// writing file
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QTextStream o( &f );
	if ( o.codec()->name() != qPrintable( pMonkeyStudio::defaultEncoding() ) )
		o.setCodec( qPrintable( pMonkeyStudio::defaultEncoding() ) );
	o << text();
	setModified( false );
	QApplication::restoreOverrideCursor();

	// remember filename
	setProperty( "fileName", fn );

	return true;
}

bool pEditor::saveBackup( const QString& s )
{
	// if not filename, cancel
	if ( s.isEmpty() )
		return false;
	
	QApplication::setOverrideCursor( Qt::WaitCursor );
	
	// get path
	QString fp = QFileInfo( s ).path();

	// file
	QFile f( s );
	
	// filename dir
	QDir d;
	// create bak folder
	if ( !d.exists( fp ) )
		if ( !d.mkpath( fp ) )
			return false;

	// set correct path
	d.setPath( fp );
		
	// try open file to write in
	if ( !f.open( QFile::WriteOnly ) )
	{
		pMonkeyStudio::warning( tr( "Save backup..." ), tr( "Cannot write file %1:\n%2." ).arg( s ).arg( f.errorString() ), this );
		return false;
	}

	// writing file
	QTextStream o( &f );
	if ( o.codec()->name() != qPrintable( pMonkeyStudio::defaultEncoding() ) )
		o.setCodec( qPrintable( pMonkeyStudio::defaultEncoding() ) );
	o << text();
	
	QApplication::restoreOverrideCursor();

	return true;
}

void pEditor::closeFile()
{
	clear();
	setModified( false );

	// clear filename
	setProperty( "fileName", QVariant() );
}

void pEditor::print( bool b )
{
	// get printer
	QsciPrinter p;

	// set wrapmode
	p.setWrapMode( WrapWord );

	// if quick print
	if ( b )
	{
		// check if default printer is set
		if ( p.printerName().isEmpty() )
		{
			pMonkeyStudio::warning( tr( "Quick Print..." ), tr( "There is no defaullt printer, please set one before trying quick print" ), this );
			return;
		}
		
		// print and return
		p.printRange( this );
		return;
	}

	// printer dialog
	QPrintDialog d( &p );

	// if ok
	if ( d.exec() )
	{
		// print
		int f = -1, t = -1, i;
		if ( d.printRange() == QPrintDialog::Selection )
			getSelection( &f, &i, &t, &i );
		p.printRange( this, f, t );
	}
}

void pEditor::quickPrint()
{
	print( true );
}

void pEditor::selectNone()
{
	selectAll( false );
}

void pEditor::invokeGoToLine()
{
	bool b;
	int l, i;
	getCursorPosition( &l, &i );
	int j = QInputDialog::getInteger( this, tr( "Go To Line..." ), tr( "Enter the line you want to go:" ), l +1, 1, lines(), 1, &b );
	if ( b )
		setCursorPosition( j -1, 0 );
}

void pEditor::convertTabs()
{
	// get original text
	QString originalText = text();
	// all modifications must believe as only one action
	beginUndoAction();
	// get indent width
	const int indentWidth = indentationWidth() != 0 ? indentationWidth() : tabWidth();
	// iterate each line
	for ( int i = 0; i < lines(); i++ )
	{
		// remember if last line was troncate
		static bool lastLineWasTroncate = false;
		// get current line indent width
		int lineIndent = indentation( i );
		// check if need troncate
		int t = lineIndent /indentWidth;
		int r = lineIndent %indentWidth;
		if ( r != 0 && r != indentWidth )
		{
			r += indentWidth -r;
			lineIndent = ( t *indentWidth) +r;
			lastLineWasTroncate = true;
		}
		else if ( lastLineWasTroncate && lineIndent != 0 )
		{
			lastLineWasTroncate = indentation( i +1 ) == lineIndent;
			lineIndent	+= indentWidth;
		}
		// remove indentation
		setIndentation( i, 0 );
		// restore it with possible troncate indentation
		setIndentation( i, lineIndent );
	}
	// end global undo action
	endUndoAction();
	// compare original and newer text
	if ( originalText == text() )
	{
		// clear undo buffer
		SendScintilla( SCI_EMPTYUNDOBUFFER );
		// set unmodified
		setModified( false );
	}
}

void pEditor::makeBackup()
{
	// get filename
	const QString dn = ".bak";
	QFileInfo f( property( "fileName" ).toString() );
	const QString s = f.path().append( "/" ).append( dn ).append( "/" ).append( f.fileName() ).append( "." ).append( QDateTime::currentDateTime().toString( "yyyyMMdd_hhmmss" ) );

	// cancel if filename doesn't exists
	if ( !f.exists() )
		return;

	// filename dir
	QDir d( f.path() );

	// create bak folder
	if ( !d.exists( ".bak" ) )
		if ( !d.mkdir( ".bak" ) )
			return;

	QFile::copy( f.absoluteFilePath(), s );
}
