#include "qCtagsSenseBrowser.h"
#include "qCtagsSense.h"
#include "qCtagsSenseLanguagesModel.h"
#include "qCtagsSenseFilesModel.h"
#include "qCtagsSenseMembersModel.h"
#include "qCtagsSenseKindFinder.h"

#include "FileManager.h"

#include <QMenu>
#include <QFileInfo>
#include <QDebug>

qCtagsSenseBrowser::qCtagsSenseBrowser( QWidget* parent )
	: QFrame( parent )
{
	setupUi( this );
	pbIndexing->setVisible( false );
	setAttribute( Qt::WA_DeleteOnClose );
	tvMembers->setContextMenuPolicy( Qt::CustomContextMenu );
	
	mSense = new qCtagsSense( this );
	mLanguagesModel = new qCtagsSenseLanguagesModel( mSense->sql() );
	mFilesModel = new qCtagsSenseFilesModel( mSense->sql() );
	mMembersModel = new qCtagsSenseMembersModel( mSense->sql() );
	
	cbLanguages->setModel( mLanguagesModel );
	cbFileNames->setModel( mFilesModel );
	
	QTreeView* tv = new QTreeView( cbMembers );
	tv->setHeaderHidden( true );
	cbMembers->setView( tv );
	cbMembers->setModel( mMembersModel );
	
	tvMembers->setHeaderHidden( true );
	tvMembers->setModel( mMembersModel );
	
	mFileManager = new FileManager( this );
	
	connect( mSense, SIGNAL( indexingStarted() ), pbIndexing, SLOT( show() ) );
	connect( mSense, SIGNAL( indexingProgress( int, int ) ), this, SLOT( mSense_indexingProgress( int, int ) ) );
	connect( mSense, SIGNAL( indexingFinished() ), pbIndexing, SLOT( hide() ) );
	connect( mSense, SIGNAL( indexingChanged() ), this, SLOT( mSense_indexingChanged() ) );
	
	connect( mFileManager, SIGNAL( buffersModified( const QMap<QString, QString>& ) ), mSense, SLOT( tagEntries( const QMap<QString, QString>& ) ) );
	connect( this, SIGNAL( memberActivated( qCtagsSenseEntry* ) ), mFileManager, SLOT( memberActivated( qCtagsSenseEntry* ) ) );
	connect( mLanguagesModel, SIGNAL( ready() ), this, SLOT( mLanguagesModel_ready() ) );
	connect( mFilesModel, SIGNAL( ready() ), this, SLOT( mFilesModel_ready() ) );
	connect( mMembersModel, SIGNAL( ready() ), this, SLOT( mMembersModel_ready() ) );
	
	// *********************
#if defined Q_OS_MAC
	mSense->tagEntry( "/Developer/SDKs/MacOSX10.4u.sdk/usr/include" );
	mSense->tagEntry( "/Users/filipeazevedo/Documents/Development/Qt4/mks/classexplorer/qCtagsSense/src" );
#elif defined Q_OS_WIN
#else
	//mSense->tagEntry( "/usr/include" );
	//mSense->tagEntry( "/home/pasnox/Development/Qt4/mks/v2/branches/dev" );
	//mSense->tagEntry( "/usr/include/qt4" );
	mSense->tagEntry( "/home/pasnox/Development/Qt4/mks/classexplorer/qCtagsSense/src" );
#endif
}

qCtagsSenseBrowser::~qCtagsSenseBrowser()
{
	qApp->quit();
}

void qCtagsSenseBrowser::on_cbLanguages_currentIndexChanged( int id )
{
	mLanguage = mLanguagesModel->language( id );
	mFileName.clear();
	mFilesModel->refresh( mLanguage );
}

void qCtagsSenseBrowser::on_cbFileNames_currentIndexChanged( int id )
{
	mFileName = mFilesModel->fileName( id );
	mMembersModel->refresh( mFileName );
}

void qCtagsSenseBrowser::on_cbMembers_currentIndexChanged( int id )
{
	Q_UNUSED( id );
	QModelIndex index = cbMembers->view()->currentIndex();
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	emit memberActivated( entry );
}

void qCtagsSenseBrowser::on_tvMembers_activated( const QModelIndex& index )
{
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	emit memberActivated( entry );
}

void qCtagsSenseBrowser::mSense_indexingProgress( int value, int total )
{
	pbIndexing->setValue( value );
	pbIndexing->setMaximum( total );
}

void qCtagsSenseBrowser::mSense_indexingChanged()
{
	mLanguage = mLanguagesModel->language( cbLanguages->currentIndex() );
	mFileName = mFilesModel->fileName( cbFileNames->currentIndex() );
	mLanguagesModel->refresh();
}

void qCtagsSenseBrowser::mLanguagesModel_ready()
{
	bool languageLocked = cbLanguages->blockSignals( true );
	int id = mLanguagesModel->indexOf( mLanguage );
	
	if ( id == -1 && !cbLanguages->count() == 0 )
	{
		id = 0;
	}
	
	mLanguage = mLanguagesModel->language( id );
	cbLanguages->setCurrentIndex( id );
	mFilesModel->refresh( mLanguage );
	cbLanguages->blockSignals( languageLocked );
}

void qCtagsSenseBrowser::mFilesModel_ready()
{	
	bool fileLocked = cbFileNames->blockSignals( true );
	int id = mFilesModel->indexOf( mFileName );
	
	if ( id == -1 && !cbFileNames->count() == 0 )
	{
		id = 0;
	}
	
	mFileName = mFilesModel->fileName( id );
	cbFileNames->setCurrentIndex( id );
	mMembersModel->refresh( mFileName );
	cbFileNames->blockSignals( fileLocked );
}

void qCtagsSenseBrowser::mMembersModel_ready()
{
	qobject_cast<QTreeView*>( cbMembers->view() )->expandAll();
	tvMembers->expandAll();
}

void qCtagsSenseBrowser::on_tvMembers_customContextMenuRequested( const QPoint& pos )
{
	QModelIndex index = tvMembers->currentIndex();
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	//bool isCorCpp = entry->language == "C++" || entry->language == "C";
	QMenu menu( this );
	
	switch ( entry->kind )
	{
		case qCtagsSense::Prototype:
		case qCtagsSense::Function:
		{
			QAction* aDeclaration = menu.addAction( "Go to declaration" );
			aDeclaration->setData( qCtagsSense::Prototype );
			QAction* aImplementation = menu.addAction( "Go to implementation" );
			aImplementation->setData( qCtagsSense::Function );
			break;
		}
		default:
		{
			QAction* aDeclaration = menu.addAction( "Go to declaration" );
			aDeclaration->setData( entry->kind );
			break;
		}
	}
	
	QAction* aTriggered = menu.exec( tvMembers->mapToGlobal( pos ) );
	
	if ( aTriggered )
	{
		qCtagsSense::Kind kind = (qCtagsSense::Kind)aTriggered->data().toInt();
		
		if ( entry->kind == kind )
		{
			emit memberActivated( entry );
		}
		else
		{
			qCtagsSenseKindFinder* cpp = new qCtagsSenseKindFinder( mSense->sql() );
			connect( cpp, SIGNAL( memberActivated( qCtagsSenseEntry* ) ), this, SIGNAL( memberActivated( qCtagsSenseEntry* ) ) );
			cpp->goTo( kind, entry );
		}
	}
}
