/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "qCtagsSenseBrowser.h"
#include "qCtagsSense.h"
#include "qCtagsSenseIndexer.h"
#include "qCtagsSenseLanguagesModel.h"
#include "qCtagsSenseFilesModel.h"
#include "qCtagsSenseMembersModel.h"
#include "qCtagsSenseKindFinder.h"

#include <ctags.h>

#include <QMenu>
#include <QFileInfo>
#include <QWidgetAction>
#include <QDebug>
#include <QScrollBar>

class MembersActionComboBox : public QComboBox
{
	Q_OBJECT
	
public:
	MembersActionComboBox( QWidget* parent = 0 )
		: QComboBox( parent )
	{
		mTree = new QTreeView( this );
		mTree->setHeaderHidden( true );
		mTree->setMinimumWidth( 600 );
		setView( mTree );
		
		connect( this, SIGNAL( currentIndexChanged( int ) ), this, SLOT( _q_currentIndexChanged( int ) ) );
	}
	
	virtual QSize sizeHint() const
	{
		QSize size = QComboBox::sizeHint();
		
		if ( mTree )
		{
			QFontMetrics fm( font() );
			QStyleOptionComboBox options;
			
			options.initFrom( this );
			int width = fm.width( mTree->selectionModel()->selectedIndexes().value( 0 ).data().toString() );
			width += fm.width( mTree->iconSize().width() );
			width += mTree->indentation();
			width += style()->subControlRect( QStyle::CC_ComboBox, &options, QStyle::SC_ComboBoxArrow, this ).width() *3;
			
			size.setWidth( width );
		}
		
		return size;
	}
	

protected:
	QTreeView* mTree;

protected slots:
	void membersModel_ready()
	{
		if ( mTree )
		{
			mTree->expandAll();
		}
	}
	
	void _q_currentIndexChanged( int id )
	{
		Q_UNUSED( id );
		updateGeometry();
		QModelIndex index = view()->currentIndex();
		qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
		emit memberActivated( entry );
	}

signals:
	void memberActivated( qCtagsSenseEntry* entry );
};

class MembersAction : public QWidgetAction
{
	Q_OBJECT

public:
	MembersAction( qCtagsSenseBrowser* browser )
		: QWidgetAction( browser )
	{
		mBrowser = browser;
	}

protected:
	qCtagsSenseBrowser* mBrowser;
	
	virtual QWidget* createWidget( QWidget* parent )
	{
		QComboBox* combo = new MembersActionComboBox( parent );
		combo->setMaxVisibleItems( 50 );
		combo->setAttribute( Qt::WA_MacSmallSize );
		combo->setModel( mBrowser->membersModel() );
		
		connect( mBrowser->membersModel(), SIGNAL( ready() ), combo, SLOT( membersModel_ready() ) );
		connect( combo, SIGNAL( memberActivated( qCtagsSenseEntry* ) ), mBrowser, SIGNAL( memberActivated( qCtagsSenseEntry* ) ) );
		
		return combo;
	}
};

qCtagsSenseBrowser::qCtagsSenseBrowser( QWidget* parent )
	: QFrame( parent )
{
	setupUi( this );
	pbIndexing->setVisible( false );
	setAttribute( Qt::WA_DeleteOnClose );
	tvMembers->setContextMenuPolicy( Qt::CustomContextMenu );
	tvMembers->setAttribute( Qt::WA_MacShowFocusRect, false );
	
	// layouts
	QVBoxLayout* vl = qobject_cast<QVBoxLayout*>( layout() );
	vl->setMargin( 0 );
	vl->setSpacing( 4 );
	QGridLayout* gl = qobject_cast<QGridLayout*>( wTopContainer->layout() );
	gl->setMargin( 6 );
	gl->setSpacing( 6 );
	
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
	
	aMembers = new MembersAction( this );
	
	connect( mSense, SIGNAL( indexingStarted() ), pbIndexing, SLOT( show() ) );
	connect( mSense, SIGNAL( indexingProgress( int, int ) ), this, SLOT( mSense_indexingProgress( int, int ) ) );
	connect( mSense, SIGNAL( indexingFinished() ), pbIndexing, SLOT( hide() ) );
	connect( mSense, SIGNAL( indexingChanged() ), this, SLOT( mSense_indexingChanged() ) );
	
	connect( mLanguagesModel, SIGNAL( ready() ), this, SLOT( mLanguagesModel_ready() ) );
	connect( mFilesModel, SIGNAL( ready() ), this, SLOT( mFilesModel_ready() ) );
	connect( mMembersModel, SIGNAL( ready() ), this, SLOT( mMembersModel_ready() ) );
	
	connect( aMembers, SIGNAL( memberActivated( qCtagsSenseEntry* ) ), this, SIGNAL( memberActivated( qCtagsSenseEntry* ) ) );
}

qCtagsSenseBrowser::~qCtagsSenseBrowser()
{
	delete mSense;
}

qCtagsSense* qCtagsSenseBrowser::sense() const
{
	return mSense;
}

qCtagsSenseLanguagesModel* qCtagsSenseBrowser::languagesModel() const
{
	return mLanguagesModel;
}

qCtagsSenseFilesModel* qCtagsSenseBrowser::filesModel() const
{
	return mFilesModel;
}

qCtagsSenseMembersModel* qCtagsSenseBrowser::membersModel() const
{
	return mMembersModel;
}

QAction* qCtagsSenseBrowser::membersAction() const
{
	return aMembers;
}

void qCtagsSenseBrowser::tagEntry( const QString& fileName )
{
	mSense->tagEntry( fileName );
}

void qCtagsSenseBrowser::tagEntries( const QMap<QString, QString>& entries )
{
	mSense->tagEntries( entries );
}

void qCtagsSenseBrowser::setCurrentFileName( const QString& fileName )
{
	mLanguage = getFileNameLanguageName( fileName.toLocal8Bit().constData() );
	mFileName = fileName;
	
	if ( mSense->indexer()->isRunning() || mLanguagesModel->isRunning() || mFilesModel->isRunning() )
	{
		return;
	}
	
	// update languages combo
	bool languageLocked = cbLanguages->blockSignals( true );
	int lid = mLanguagesModel->indexOf( mLanguage );
	
	cbLanguages->setCurrentIndex( lid );
	cbLanguages->blockSignals( languageLocked );
	
	// update files combo
	bool fileLocked = cbFileNames->blockSignals( true );
	int fid = mFilesModel->indexOf( mFileName );
	
	cbFileNames->setCurrentIndex( fid );
	cbFileNames->blockSignals( fileLocked );
	
	// update view
	if ( fid != -1 )
	{
		mMembersModel->refresh( mFileName );
	}
	else
	{
		mFilesModel->refresh( mLanguage );
	}
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
	if ( mLanguage.isEmpty() || mFileName.isEmpty() )
	{
		mLanguage = mLanguagesModel->language( cbLanguages->currentIndex() );
		mFileName = mFilesModel->fileName( cbFileNames->currentIndex() );
	}
	
	mLanguagesModel->refresh();
}

void qCtagsSenseBrowser::mLanguagesModel_ready()
{
	bool languageLocked = cbLanguages->blockSignals( true );
	int id = mLanguagesModel->indexOf( mLanguage );
	
	if ( id == -1 && cbLanguages->count() != 0 )
	{
		id = 0;
		mLanguage = mLanguagesModel->language( 0 );
	}
	
	//mLanguage = mLanguagesModel->language( id );
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
		mFileName = mFilesModel->fileName( 0 );
	}
	
	//mFileName = mFilesModel->fileName( id );
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

#include "qCtagsSenseBrowser.moc"
