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

UISaveFiles::UISaveFiles( pAbstractChild* p, QCloseEvent* e )
	: QDialog( p ), mChild( p ), mEvent( e )
{
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
	dbbButtons->setStandardButtons( QDialogButtonBox::Save | QDialogButtonBox::Close | QDialogButtonBox::Cancel );
	vl->addWidget( dbbButtons );

	// add files list
	foreach ( QString s, mChild->files() )
	{
		addFile( s, mChild->isModified( s ) );
		if ( !isWindowModified() && mChild->isModified( s ) )
			setWindowModified( true );
	}

	// connections
	connect( dbbButtons, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( clicked( QAbstractButton* ) ) );
}
//
void UISaveFiles::addFile( const QString& s, bool b )
{
	// crete file item
	QListWidgetItem* i = new QListWidgetItem( QFileInfo( s ).fileName(), lwFiles );
	i->setData( Qt::UserRole, s );
	i->setCheckState( Qt::Unchecked );
	if ( b )
		i->setCheckState( Qt::Checked );
}
//
void UISaveFiles::clicked( QAbstractButton* ab )
{
	// get clicked button
	QPushButton* pb = qobject_cast<QPushButton*>( ab );

	// if button was save
	if ( pb  == dbbButtons->button( QDialogButtonBox::Save ) )
	{
		for ( int i = 0; i < lwFiles->count(); i++ )
			if ( lwFiles->item( i )->checkState() != Qt::Unchecked )
				mChild->saveFile( lwFiles->item( i )->data( Qt::UserRole ).toString() );
	}
	// else cancel changing child event
	else if ( pb == dbbButtons->button( QDialogButtonBox::Cancel ) )
		mEvent->ignore();

	// close dialog
	close();
}
//
void UISaveFiles::execute( pAbstractChild* c, QCloseEvent* e )
{
	// if no files return
	if ( !c || !e || !c->files().count() )
		return;

	// create dialog
	UISaveFiles d( c, e );

	// execute dialog only if needed
	if ( d.isWindowModified() )
		d.exec();
}
