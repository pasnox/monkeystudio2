#include "UIRegExpEditor.h"

#include <monkey.h>

#include <QTime>
#include <QDebug>

UIRegExpEditor::UIRegExpEditor( QWidget* w )
	: QMainWindow( w, Qt::Tool )
{
	// init widget
	setupUi( this );
	// add regexp syntax in combobox
	cbSyntax->addItem( "RegExp", QRegExp::RegExp );
	cbSyntax->addItem( "RegExp2", QRegExp::RegExp2 );
	cbSyntax->addItem( "Wildcard", QRegExp::Wildcard );
	cbSyntax->addItem( "FixedString", QRegExp::FixedString );
	// default configuration
	cbSyntax->setCurrentIndex( cbSyntax->findData( QRegExp::RegExp ) );
	cbCaseSensitive->setChecked( true );
	cbGreedy->setChecked( true );
}

void UIRegExpEditor::on_tbFind_clicked()
{
	// get pattern and text
	const QString pattern = leRegExp->text().trimmed();
	const QString text = pteTestBuffer->toPlainText().trimmed();
	// create the regexp
	QRegExp regexp( pattern, cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive, cbSyntax->itemData( cbSyntax->currentIndex() ).value<QRegExp::PatternSyntax>() );
	regexp.setMinimal( !cbGreedy->isChecked() );
	// check null
	if ( pattern.isEmpty() || text.isEmpty() )
		return;
	// clear tree
	twResults->clear();
	// tracking time  elapsed in case of infinite loop
	QTime elapsedTime;
	elapsedTime.start();
	// searching
	int count = 0;
	int pos = 0;
	while ( ( pos = regexp.indexIn( text, pos ) ) != -1 )
	{
		// parent item
		QTreeWidgetItem* parent = new QTreeWidgetItem( twResults );
		parent->setText( 0, regexp.cap( 0 ) );
		parent->setToolTip( 0,QString( "Main capture on iteration %1" ).arg( count ) );
		parent->setExpanded( true );
		// child item
		for ( int i = 1; i < regexp.numCaptures(); i++ )
		{
			QTreeWidgetItem* child = new QTreeWidgetItem( parent );
			child->setText( 0,regexp.cap( i ) );
			child->setToolTip( 0, QString( "Capture %1 on iteration %2" ).arg( i ).arg( count ) );
			child->setExpanded( true );
		}
		// continue
		++count;
		pos += regexp.matchedLength();
		// check infinite loop
		if ( count %1000 == 0 && pMonkeyStudio::question( tr( "Freeze ?!" ), tr( "The regular expression seem to recurse infinitely, do you want to stop searching ?" ), window() ) )
			break;
	}
	// tell about time
	statusBar()->showMessage( tr( "Elapsed time: %1" ).arg( (float)elapsedTime.elapsed() /1000.0 ) );
}
