#include "AddFilesDialog.h"

#include <QGridLayout>
#include <QFrame>
#include <QHBoxLayout>

AddFilesDialog::AddFilesDialog( AddFilesDialog::Type type, ScopedProjectItemModel* spim, XUPItem* pi, QWidget* w )
	: QFileDialog( w )
{
	Q_ASSERT( spim && pi );
	mScoped = spim;
	mType = type;
	// set label/mode
	switch ( mType )
	{
		case AddFilesDialog::Files:
			setWindowTitle( tr( "Choose the files to add to your project" ) );
			setFileMode( QFileDialog::ExistingFiles );
			break;
		case AddFilesDialog::Folder:
			setWindowTitle( tr( "Choose the folder to add to your project" ) );
			setFileMode( QFileDialog::DirectoryOnly );
			break;
	}
	// set directory
	setDirectory( pi->projectPath() );
	// set accept mode
	setAcceptMode( QFileDialog::AcceptOpen );
	setLabelText( QFileDialog::Accept, tr( "Add" ) );
	// create check boxLayout
	cbRecursive = new QCheckBox( tr( "Add folders recursively." ) );
	cbRecursive->setChecked( true );
	// create frame for comboboxes
	QFrame* f = new QFrame;
	// add layout to frame
	QHBoxLayout* hl = new QHBoxLayout( f );
	hl->setMargin( 0 );
	hl->setSpacing( 3 );
	// add projects/scopes groupbox
	QGroupBox* gb1 = new QGroupBox;
	gb1->setTitle( tr( "Scopes" ) );
	// set groupbox layout
	QHBoxLayout* hl1 = new QHBoxLayout( gb1 );
	hl1->setMargin( 5 );
	hl1->setSpacing( 0 );
	// create projects/scopes treecombobox
	tcbProjects = new pTreeComboBox;
	tcbProjects->setModel( mScoped );
	tcbProjects->setCurrentIndex( mScoped->mapFromSource( pi->project()->index() ) );
	hl1->addWidget( tcbProjects );
	// add operators groupbox
	QGroupBox* gb2 = new QGroupBox;
	gb2->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	gb2->setTitle( tr( "Operators" ) );
	// set groupbox layout
	QHBoxLayout* hl2 = new QHBoxLayout( gb2 );
	hl2->setMargin( 5 );
	hl2->setSpacing( 0 );
	// create operators combobox
	cbOperators = new QComboBox;
	cbOperators->addItems( pi->operators() );
	hl2->addWidget( cbOperators );
	// add groupbox to layout
	hl->addWidget( gb1 );
	hl->addWidget( gb2 );
	// add frame to layout
	QGridLayout* gridlayout = qobject_cast<QGridLayout*>( layout() );
	gridlayout->addWidget( cbRecursive, 4, 0, 1, 3 );
	gridlayout->addWidget( f, 5, 0, 1, 3 );
	// set recursive checkbox visibilityChanged
	cbRecursive->setVisible( mType == AddFilesDialog::Folder );
}
