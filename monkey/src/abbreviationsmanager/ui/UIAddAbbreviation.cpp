#include "UIAddAbbreviation.h"
#include "pMonkeyStudio.h"

#include <QTreeWidget>
#include <QMessageBox>

void UIAddAbbreviation::edit( QTreeWidget* t )
{
	// create dialog
	UIAddAbbreviation d( t->window(), t );
	// execute dialog
	d.exec();
}

UIAddAbbreviation::UIAddAbbreviation( QWidget* w, QTreeWidget* t )
	: QDialog( w ), mTree( t )
{
	setupUi( this );
	// fill combo with language
	cbLanguages->addItems( pMonkeyStudio::availableLanguages() );
	// set focus on template lineedit
	leTemplate->setFocus();
}

UIAddAbbreviation::~UIAddAbbreviation()
{
}

void UIAddAbbreviation::accept()
{
	// check if item already exists for same name
	QTreeWidgetItem* it = mTree->findItems( leTemplate->text(), Qt::MatchFixedString ).value( 0 );
	// if not or differente language
	if ( !it || it->text( 2 ) != cbLanguages->currentText() )
	{
		// create item
		it = new QTreeWidgetItem( mTree );
		// fill it
		it->setText( 0, leTemplate->text() );
		it->setText( 1, leDescription->text() );
		it->setText( 2, cbLanguages->currentText() );
		it->setData( 0, Qt::UserRole, QString() );
		// set current item new one
		mTree->setCurrentItem( it );
		// accept
		QDialog::accept();
	}
	else
		QMessageBox::warning( window(), tr( "Warning..." ), tr( "This template already exists for this language." ) );
}
