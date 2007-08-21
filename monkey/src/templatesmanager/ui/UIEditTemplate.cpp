#include "UIEditTemplate.h"
#include "pMonkeyStudio.h"
#include "pTemplatesManager.h"

#include <QTreeWidget>
#include <QAbstractItemModel>
#include <QFileInfo>

void UIEditTemplate::edit( QTreeWidget* t, QTreeWidgetItem* i )
{
	// create dialog
	UIEditTemplate d( t->window(), t, i );
	// execute dialog
	d.exec();
}

UIEditTemplate::UIEditTemplate( QWidget* w, QTreeWidget* t, QTreeWidgetItem* i )
	: QDialog( w ), mTree( t ), mItem( i )
{
	setupUi( this );
	setWindowTitle( tr( "Creating new template" ) );
	// fill combo with language
	cbLanguages->addItems( pMonkeyStudio::availableLanguages() );
	// fill type comboobox
	pTemplate::fillComboBox( cbTypes );
	// fill infos
	if ( i )
	{
		setWindowTitle( tr( "Editing template - %1" ).arg( i->text( 1 ) ) );
		cbLanguages->setCurrentIndex( cbLanguages->findText( i->text( 0 ), Qt::MatchFixedString ) );
		cbTypes->setCurrentIndex( cbTypes->findData( i->data( 0, Qt::UserRole +2 ).toInt(), Qt::UserRole, Qt::MatchFixedString ) );
		leName->setText( i->text( 1 ) );
		leDescription->setText( i->text( 2 ) );
		tbIcon->setIcon( i->icon( 0 ) );
		tbIcon->setToolTip( i->data( 0, Qt::UserRole ).toString() );
		foreach ( QString s, i->data( 0, Qt::UserRole +1 ).toStringList() )
		{
			QListWidgetItem* it = new QListWidgetItem( lwFiles );
			it->setText( QFileInfo( s ).fileName() );
			it->setToolTip( s );
		}
	}
	// set focus on name lineedit
	leName->setFocus();
}

UIEditTemplate::~UIEditTemplate()
{
}

bool UIEditTemplate::canAddFile( const QString& s )
{
	return !lwFiles->model()->match( lwFiles->model()->index( 0, 0 ), Qt::ToolTipRole, s, 1, Qt::MatchFixedString | Qt::MatchWrap ).count();
}

void UIEditTemplate::on_tbIcon_clicked()
{
	// choose image
	QString s = pMonkeyStudio::getImageFileName( tr( "Select an icon for this template type" ), tbIcon->toolTip(), this );
	// remember it
	if ( !s.isNull() )
	{
		tbIcon->setIcon( QIcon( s ) );
		tbIcon->setToolTip( s );
	}
}

void UIEditTemplate::on_pbAdd_clicked()
{
	// get a file
	QString s = pMonkeyStudio::getOpenFileName( tr( "Select a template file" ), pTemplatesManager::templatesPath(), QString(), window() );
	// remember it
	if ( !s.isNull() && canAddFile( s ) )
	{
		QListWidgetItem* it = new QListWidgetItem( lwFiles );
		it->setText( QFileInfo( s ).fileName() );
		it->setToolTip( s );
	}
}

void UIEditTemplate::on_pbEdit_clicked()
{
	// get item
	QListWidgetItem* it = lwFiles->selectedItems().value( 0 );
	// cancel if no item
	if ( !it )
		return;
	// get a file
	QString s = pMonkeyStudio::getOpenFileName( tr( "Select a template file" ), it->toolTip(), QString(), window() );
	// remember it
	if ( !s.isNull() && canAddFile( s ) )
	{
		it->setText( QFileInfo( s ).fileName() );
		it->setToolTip( s );
	}
}

void UIEditTemplate::on_pbRemove_clicked()
{
	delete lwFiles->selectedItems().value( 0 );
}

void UIEditTemplate::accept()
{
	// check if item already exists for same name
	QTreeWidgetItem* it = mTree->findItems( leName->text(), Qt::MatchFixedString, 1 ).value( 0 );
	// if not, create/update it
	if ( !it || it == mItem )
	{
		// create item if needed
		if ( !mItem )
			mItem = new QTreeWidgetItem( mTree );
		// fill item
		mItem->setText( 0, cbLanguages->currentText() );
		mItem->setIcon( 0, tbIcon->icon() );
		mItem->setText( 1, leName->text() );
		mItem->setText( 2, leDescription->text() );
		mItem->setText( 3, cbTypes->currentText() );
		mItem->setData( 0, Qt::UserRole, tbIcon->toolTip() );
		QStringList l;
		for ( int i = 0; i < lwFiles->count(); i++ )
			l << lwFiles->item( i )->toolTip();
		mItem->setData( 0, Qt::UserRole +1, l );
		mItem->setData( 0, Qt::UserRole +2, cbTypes->itemData( cbTypes->currentIndex() ).toInt() );
		// set current item new one
		mTree->setCurrentItem( mItem );
		// accept
		QDialog::accept();
	}
	else
		pMonkeyStudio::warning( tr( "Warning..." ), tr( "This template name is already used." ), window() );
}
