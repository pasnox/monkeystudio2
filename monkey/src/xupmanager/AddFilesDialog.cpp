#include "AddFilesDialog.h"
#include "../pMonkeyStudio.h"

#include <QGridLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QTreeView>
#include <QListView>
#include <QFileSystemWatcher>

AddFilesDialog::AddFilesDialog( ScopedProjectItemModel* spim, XUPItem* pi, QWidget* w )
	: QFileDialog( w )
{
	Q_ASSERT( spim && pi );
	mScoped = spim;
	// dialog options
	setWindowTitle( tr( "Choose the folders/files to add to your project" ) );
	setFileMode( QFileDialog::ExistingFiles );
	// set directory
	setDirectory( pi->projectPath() );
	// set accept mode
	setAcceptMode( QFileDialog::AcceptOpen );
	setLabelText( QFileDialog::Accept, tr( "Add" ) );
	
	// gay hack :)
	// listview
	if ( ( lvFiles = findChild<QListView*>( "listView" ) ) )
	{
	//qWarning( "listView ok" );
		lvFiles->disconnect();
		connect( lvFiles, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( doubleClicked( const QModelIndex& ) ) );
		connect( lvFiles->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
	}
	// treeview
	if ( ( tvFiles = findChild<QTreeView*>( "treeView" ) ) )
	{
	//qWarning( "treeView ok" );
		tvFiles->disconnect();
		connect( tvFiles, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( doubleClicked( const QModelIndex& ) ) );
		connect( tvFiles->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
	}
	// lineedit files
	leFiles = findChild<QLineEdit*>();
	// accept button
	pbAdd = 0;
	foreach ( QPushButton* pb, findChildren<QPushButton*>() )
	{
		if ( pb->text() == labelText( QFileDialog::Accept ) )
		{
		//qWarning( "pbAdd ok" );
			pbAdd = pb;
			pbAdd->setEnabled( false );
			pbAdd->disconnect();
			connect( pbAdd, SIGNAL( clicked() ), this, SLOT( addClicked() ) );
			break;
		}
	}
	// model
	mModel = qobject_cast<QFileSystemModel*>( lvFiles->model() );
	//qWarning( "mModel ok: %i", mModel );

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
	// create import widgets
	gbCopy = new QGroupBox( tr( "Import external files" ), this );
	gbCopy->setCheckable( true );
	gbCopy->setChecked( false );
	gbCopy->setToolTip( tr( "Imported files will by default be copied to project path unless you provide a relative only path from project path" ) );
	cbCopy = new QComboBox( this );
	cbCopy->setSizeAdjustPolicy( QComboBox::AdjustToMinimumContentsLength );
	cbCopy->setEditable( true );
	QHBoxLayout* hlCopy = new QHBoxLayout( gbCopy );
	hlCopy->addWidget( cbCopy );
	// add widgets to global layout
	QGridLayout* gridlayout = qobject_cast<QGridLayout*>( layout() );
	gridlayout->addWidget( cbRecursive, 4, 0, 1, 3 );
	gridlayout->addWidget( f, 5, 0, 1, 3 );
	gridlayout->addWidget( gbCopy, 6, 0, 1, 3 );
	
	// connections
	connect( tcbProjects, SIGNAL( currentChanged( const QModelIndex& ) ), this, SLOT( projects_currentChanged( const QModelIndex& ) ) );
	
	
	// select current scope in the project combobox
	tcbProjects->setCurrentIndex( mScoped->mapFromSource( pi->project()->index() ) );
}

QStringList AddFilesDialog::selectedFilesFolders() const
{
	QStringList files = selectedFiles();
	files.removeAll( mModel->rootPath() );
	files.removeAll( mModel->rootPath() +"/" );
	files.removeAll( mModel->rootPath() +"\\" );
	return files;
}

void AddFilesDialog::doubleClicked( const QModelIndex& index )
{
	if ( mModel->isDir( index ) )
		setDirectory( mModel->rootPath() +"/" +mModel->fileName( index ) );	
}

void AddFilesDialog::selectionChanged( const QItemSelection&, const QItemSelection& )
{
	leFiles->clear();
	QStringList files;
	foreach ( const QModelIndex& index, lvFiles->selectionModel()->selectedIndexes() )
		if ( index.column() == 0 && mModel->rootDirectory().dirName() != mModel->fileName( index ) )
			files << "\"" +mModel->fileName( index ) +"\"";
	leFiles->setText( files.join( " " ) );
	pbAdd->setEnabled( files.count() > 0 );
}

void AddFilesDialog::addClicked()
{
	if ( selectedFilesFolders().count() > 0 )
		QDialog::accept();
}

void AddFilesDialog::projects_currentChanged( const QModelIndex& index )
{
	const QModelIndex idx = mScoped->mapToSource( index );
	const XUPItem* it = mScoped->model()->itemFromIndex( idx )->project();
	const QString projectPath = it->projectPath();
	const QString currentPath = cbCopy->currentText();
	// update combobox
	QDir dir( projectPath );
	cbCopy->clear();
	foreach ( const QFileInfo& fi, pMonkeyStudio::getFolders( dir, QStringList() ) )
		cbCopy->addItem( dir.relativeFilePath( fi.filePath() ) );
	// restore last path if possible
	cbCopy->setCurrentIndex( cbCopy->findText( currentPath ) );
}
