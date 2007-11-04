#include "pSearch.h"

#include "qsciscintilla.h"

#include <QKeyEvent>

pSearch::pSearch( QsciScintilla* p )
	: QDockWidget( p )
{
	// setup dock
	setupUi( this );

	// set fixed height
	setFixedHeight( minimumSizeHint().height() );
	
	// clear informations edit
	lInformations->clear();

	// set current editor manage for search
	setEditor( p );
}

void pSearch::keyPressEvent( QKeyEvent* e )
{
	if ( e->key() == Qt::Key_Escape )
		hide();
	QDockWidget::keyPressEvent( e );
}

bool pSearch::checkEditor()
{
	// enable/disable dock accoding to editor
	wCentral->setEnabled( mEditor );
	return mEditor;
}

QsciScintilla* pSearch::editor() const
{
	return mEditor;
}

void pSearch::setEditor( QsciScintilla* e )
{
	mEditor = e;
	checkEditor();
	lInformations->clear();
};

bool pSearch::on_tbPrevious_clicked()
{
	// cancel if no editor
	if ( !checkEditor() )
		return false;

	// get cursor position
	int x, y;
	mEditor->getCursorPosition( &y, &x );

	// reset position if search from start
	if ( cbFromStart->isChecked() )
	{
		x = 0;
		y = 0;
	}
	
	// if no text to search, and selected text, set the selected text as search text
	if ( leSearch->text().isEmpty() && !mEditor->selectedText().isEmpty() )
		leSearch->setText( mEditor->selectedText() );

	// search
	bool b = mEditor->findFirst( leSearch->text(), cbRegExp->isChecked(), cbCaseSensitive->isChecked(), cbWholeWords->isChecked(), cbWrap->isChecked(), false, y, x -mEditor->selectedText().length() );

	// uncheck from start if needed
	if ( cbFromStart->isChecked() )
		cbFromStart->setChecked( false );

	// change background acording to found or not
	QPalette p = leSearch->palette();
	p.setColor( leSearch->backgroundRole(), b ? Qt::white : Qt::red );
	leSearch->setPalette( p );
	
	// show message if needed
	lInformations->setText( b ? QString::null : tr( "Not Found" ) );

	// return found state
	return b;
}

bool pSearch::on_tbNext_clicked()
{
	// cancel if no editor
	if ( !checkEditor() )
		return false;

	// get cursor position
	int x, y;
	mEditor->getCursorPosition( &y, &x );

	// reset position if search from start
	if ( cbFromStart->isChecked() )
	{
		x = 0;
		y = 0;
	}
	
	// if no text to search, and selected text, set the selected text as search text
	if ( leSearch->text().isEmpty() && !mEditor->selectedText().isEmpty() )
		leSearch->setText( mEditor->selectedText() );

	// search
	bool b = mEditor->findFirst( leSearch->text(), cbRegExp->isChecked(), cbCaseSensitive->isChecked(), cbWholeWords->isChecked(), cbWrap->isChecked(), true, y, x );

	// uncheck from start if needed
	if ( cbFromStart->isChecked() )
		cbFromStart->setChecked( false );

	// change background acording to found or not
	QPalette p = leSearch->palette();
	p.setColor( leSearch->backgroundRole(), b ? Qt::white : Qt::red );
	leSearch->setPalette( p );
	
	// show message if needed
	lInformations->setText( b ? QString::null : tr( "Not Found" ) );

	// return found state
	return b;
}
//
bool pSearch::on_tbReplace_clicked()
{
	// cancel if no editor
	if ( !checkEditor() )
		return false;

	// if no selection and not found cancel
	if ( mEditor->selectedText().isEmpty() && !on_tbNext_clicked() )
		return false;

	// get selected text
	QString mSelection = mEditor->selectedText();
	QString mSearch = leSearch->text();
	bool b = false;

	// if not regexp replace
	if ( !cbRegExp->isChecked() )
	{
		// replace and go next
		if ( cbCaseSensitive->isChecked() && mSelection == mSearch )
		{
			mEditor->replace( leReplace->text() );
			b = true;
			on_tbNext_clicked();
		}
		else if ( !cbCaseSensitive->isChecked() && mSelection.toLower() == mSearch.toLower() )
		{
			mEditor->replace( leReplace->text() );
			b = true;
			on_tbNext_clicked();
		}
	}
	// regexp replace
	else
	{
		// repalce and go next
		if ( QRegExp( mSearch, cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive ).exactMatch( mSelection ) )
		{
			mEditor->replace( leReplace->text() );
			b = true;
			on_tbNext_clicked();
		}
	}
	
	// show message if needed
	lInformations->setText( b ? QString::null : tr( "Nothing To Replace" ) );

	// return replace state
	return b;
}
//
void pSearch::on_tbReplaceAll_clicked()
{
	// cancel if no editor
	if ( !checkEditor() )
		return;
	
	// begin undo global action
	mEditor->beginUndoAction();

	// while o, repalce
	int i = 0;
	while ( on_tbReplace_clicked() )
		i++;
	
	// end undo global action
	mEditor->endUndoAction();

	// show occurence number replaced
	lInformations->setText( i ? tr( "%1 occurences replaced" ).arg( i ) : tr( "Nothing To Repalce" ) );
}
