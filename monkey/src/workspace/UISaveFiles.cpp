#include "UISaveFiles.h"
#include "pAbstractChild.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QFileInfo>

UISaveFiles::UISaveFiles( QWidget* w, bool b )
	: QDialog( w )
{
	// set no child modified
	setProperty( "ChildsModified", false );
	// set undefined button state
	setProperty( "ClickedButton", UISaveFiles::bDiscardAll );
	// set window title
	setWindowTitle( tr( "Save File(s) ?" ) );
	// resize window
	resize( 400, 300 );

	// create dialog layout
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 5 );
	vl->setSpacing( 3 );

	// create label
	QLabel* l = new QLabel;
	l->setText( tr( "Check the files you want to save :" ) );
	vl->addWidget( l );

	// create listwidget
	lwFiles = new QListWidget;
	vl->addWidget( lwFiles );

	// create buttons
	dbbButtons = new QDialogButtonBox;
	dbbButtons->setStandardButtons( QDialogButtonBox::Save | QDialogButtonBox::Discard );
	dbbButtons->button( QDialogButtonBox::Save )->setText( tr( "Save Selected" ) );
	dbbButtons->button( QDialogButtonBox::Discard )->setText( tr( "Discard All" ) );
	
	// if not forced, add cancel button
	if ( !b)
	{
		dbbButtons->addButton( QDialogButtonBox::Cancel );
		dbbButtons->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel Close" ) );
	}
	
	// add button to dialog
	vl->addWidget( dbbButtons );

	// connections
	connect( dbbButtons, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( clicked( QAbstractButton* ) ) );
}

void UISaveFiles::addFile( const QString& s, pAbstractChild* c )
{
	// create file item
	QListWidgetItem* i = new QListWidgetItem( QFileInfo( s ).fileName(), lwFiles );
	i->setToolTip( s );
	i->setData( Qt::UserRole, s );
	i->setData( Qt::UserRole +1, reinterpret_cast<quintptr>( c ) );
	i->setCheckState( Qt::Checked );
	
	// change dialog window modified if needed
	if ( !property( "ChildsModified" ).toBool() )
		setProperty( "ChildsModified", true );
}

void UISaveFiles::clicked( QAbstractButton* ab )
{
	// if button was save
	if ( ab  == dbbButtons->button( QDialogButtonBox::Save ) )
	{
		setProperty( "ClickedButton", UISaveFiles::bSaveSelected );
		for ( int i = 0; i < lwFiles->count(); i++ )
			if ( lwFiles->item( i )->checkState() != Qt::Unchecked )
				reinterpret_cast<pAbstractChild*>( lwFiles->item( i )->data( Qt::UserRole +1 ).value<quintptr>() )->saveFile( lwFiles->item( i )->data( Qt::UserRole ).toString() );
	}
	// else cancel changing child event
	else if ( ab == dbbButtons->button( QDialogButtonBox::Cancel ) )
		setProperty( "ClickedButton", UISaveFiles::bCancelClose );

	// close dialog
	close();
}

UISaveFiles::Buttons UISaveFiles::saveDocuments( QWidget* w, QList<pAbstractChild*> l, bool b )
{
	// create dialog
	UISaveFiles d( w, b );
	
	// add files
	foreach ( pAbstractChild* c, l )
		foreach ( QString s, c->files() )
			if ( c->isModified( s ) )
				d.addFile( s, c );
	
	// if at least one file is modified, ChildsModified is true, show it if needed
	if ( d.property( "ChildsModified" ).toBool() )
		d.exec();
	
	// return clicked button
	return static_cast<UISaveFiles::Buttons>( d.property( "ClickedButton" ).toInt() );
}

UISaveFiles::Buttons UISaveFiles::saveDocument( QWidget* w, pAbstractChild* c, bool b )
{ return saveDocuments( w, QList<pAbstractChild*>() << c, b ); }
