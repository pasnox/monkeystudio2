#include "pEditor.h"
#include "pSearch.h"
#include "pMonkeyStudio.h"
#include "qSciShortcutsManager.h"

#include "qsciprinter.h"

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

bool pEditor::mPasteAvailableInit = false;
bool pEditor::mPasteAvailable = false;

pEditor::pEditor( QWidget* p )
	: QsciScintilla( p )
{
	// register image for auto completion
	registerImage( riClass, QPixmap( ":/editor/icons/editor/class.png" ) );
	registerImage( riEnum, QPixmap( ":/editor/icons/editor/enum.png" ) );
	registerImage( riFunction, QPixmap( ":/editor/icons/editor/function.png" ) );
	registerImage( riMember, QPixmap( ":/editor/icons/editor/member.png" ) );
	registerImage( riNamespace, QPixmap( ":/editor/icons/editor/namespace.png" ) );
	registerImage( riStruct, QPixmap( ":/editor/icons/editor/struct.png" ) );
	registerImage( riTypedef, QPixmap( ":/editor/icons/editor/typedef.png" ) );
	registerImage( riVariable, QPixmap( ":/editor/icons/editor/variable.png" ) );
	
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
	SendScintilla( QsciScintilla::SCI_CLEARALLCMDKEYS );
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
		}
		return;
	}
	QsciScintilla::keyPressEvent( e );
}

bool pEditor::lineNumbersMarginEnabled() const
{
	return marginLineNumbers( 0 );
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
	setLexer( pMonkeyStudio::lexerForFilename( s ) );

	// set properties
	pMonkeyStudio::setEditorProperties( this );

	// load file
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QTextStream i( &f );
	if ( i.codec()->name() != qPrintable( pMonkeyStudio::defaultEncoding() ) )
		i.setCodec( qPrintable( pMonkeyStudio::defaultEncoding() ) );
	setText( i.readAll() );
	setModified( false );
	QApplication::restoreOverrideCursor();

	// convert tabs if needed
	if ( pMonkeyStudio::convertTabsUponOpen() )
		convertTabs();
		
	// make backup if needed
	if ( pMonkeyStudio::createBackupUponOpen() )
		makeBackup();

	// convert eol
	if ( pMonkeyStudio::autoEolConversion() )
		convertEols( eolMode() );

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
	
	// check if file exists
	/*
	if ( QFile::exists( s ) && !pMonkeyStudio::question( tr( "Save backup..." ), tr( "The file already exists, are you sure you want to overwrite it ?" ) ) )
		return false;
	*/
	
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
	QApplication::setOverrideCursor( Qt::WaitCursor );
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

void pEditor::invokeSearchReplace()
{
	if ( !pSearch::instance()->isVisible() )
		pSearch::instance()->setVisible( true );
	while ( pSearch::instance()->isFloating() && QApplication::activeWindow() != pSearch::instance()->window() )
	{
		QApplication::processEvents();
		pSearch::instance()->activateWindow();
	}
	pSearch::instance()->leSearch->setFocus();
	pSearch::instance()->leSearch->selectAll();
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

void pEditor::convertTabs( int i )
{
	int x, y;
	getCursorPosition( &y, &x );
	if ( i == -1 )
		i = tabWidth();
	bool b = findFirst( "\t", false, true, false, true, true );
	if ( b )
	{
		QString r = QString().fill( ' ', i );
		replace( r );
		while ( findNext() )
			replace( r );
	}
	setCursorPosition( y, x );
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
