#include "pSearch.h"

#include <qsciscintilla.h>
#include <QMessageBox>

pSearch::pSearch( QsciScintilla* p )
	: QDockWidget( p )
{
	// setup dock
	setupUi( this );

	// set maximum height
	setMaximumHeight( minimumSizeHint().height() );

	// set current editor manage for search
	setEditor( p );
}

bool pSearch::checkEditor()
{
	// enable/disable dock accoding to editor
	QList<QWidget*> l = wCentral->findChildren<QWidget*>();
	foreach ( QWidget* w, l )
		if ( w != tbClose )
			w->setEnabled( mEditor );
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

	// search
	bool b = mEditor->findFirst( leSearch->text(), cbRegExp->isChecked(), cbCaseSensitive->isChecked(), cbWholeWords->isChecked(), cbWrap->isChecked(), false, y, x -mEditor->selectedText().length() );

	// uncheck from start if needed
	if ( cbFromStart->isChecked() )
		cbFromStart->setChecked( false );

	// change background acording to found or not
	QPalette p = leSearch->palette();
	p.setColor( leSearch->backgroundRole(), b ? Qt::white : Qt::red );
	leSearch->setPalette( p );

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

	// search
	bool b = mEditor->findFirst( leSearch->text(), cbRegExp->isChecked(), cbCaseSensitive->isChecked(), cbWholeWords->isChecked(), cbWrap->isChecked(), true, y, x );

	// uncheck from start if needed
	if ( cbFromStart->isChecked() )
		cbFromStart->setChecked( false );

	// change background acording to found or not
	QPalette p = leSearch->palette();
	p.setColor( leSearch->backgroundRole(), b ? Qt::white : Qt::red );
	leSearch->setPalette( p );

	// return found state
	return b;
}
//
bool pSearch::on_tbReplace_clicked()
{
	// cancel if no editor
	if ( !checkEditor() )
		return false;

	// cancel if no replace text
	if ( leReplace->text().isEmpty() )
		return false;

	// if no selection and not found cancel
	if ( mEditor->selectedText().isEmpty()	&& !on_tbNext_clicked() )
		return false;

	//
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

	// return replace state
	return b;
}
//
void pSearch::on_tbReplaceAll_clicked()
{
	// cancel if no editor
	if ( !checkEditor() )
		return;

	// while o, repalce
	int i = 0;
	while ( on_tbReplace_clicked() )
		i++;

	// show occurence number replaced
	if ( i )
		QMessageBox::information( this, tr( "Information..." ), tr( "%1 occurences replaced" ).arg( i ) );
}
