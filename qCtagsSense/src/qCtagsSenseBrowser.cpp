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
#include "qCtagsSenseSearchModel.h"

#include <ctags.h>

#include <QMenu>
#include <QFileInfo>
#include <QWidgetAction>
#include <QDebug>
#include <QScrollBar>
#include <QMovie>
#include <QComboBox>
#include <QTimer>
#include <QActionGroup>

#define SEARCH_TIMEOUT 500
#define SEARCH_MIN_CHARS 3

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
		mTree->setContextMenuPolicy( Qt::CustomContextMenu );
		
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
		emit entryActivated( entry );
	}

signals:
	void entryActivated( qCtagsSenseEntry* entry );
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
		connect( combo, SIGNAL( entryActivated( qCtagsSenseEntry* ) ), mBrowser, SIGNAL( entryActivated( qCtagsSenseEntry* ) ) );
		connect( combo->view(), SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( _q_tree_customContextMenuRequested( const QPoint& ) ) );
		
		return combo;
	}

protected slots:
	void _q_tree_customContextMenuRequested( const QPoint& pos )
	{
		QTreeView* view = qobject_cast<QTreeView*>( sender() );
		
		mBrowser->popupMenu( view, pos );
	}
};

qCtagsSenseBrowser::qCtagsSenseBrowser( QWidget* parent )
	: QFrame( parent )
{
	setupUi( this );
	lSearch->setAttribute( Qt::WA_MacShowFocusRect, false );
	lSearch->setAttribute( Qt::WA_MacSmallSize );
	leSearch->setAttribute( Qt::WA_MacShowFocusRect, false );
	leSearch->setAttribute( Qt::WA_MacSmallSize );
	tvMembers->setAttribute( Qt::WA_MacShowFocusRect, false );
	tvMembers->setAttribute( Qt::WA_MacSmallSize );
	tvSearchResult->setAttribute( Qt::WA_MacShowFocusRect, false );
	tvSearchResult->setAttribute( Qt::WA_MacSmallSize );
	
	QActionGroup* group = new QActionGroup( this );
	group->addAction( aBrowser );
	group->addAction( aSearchResults );
	
	mLoading = new QMovie( this );
	mLoading->setFileName( ":/icons/loading.gif" );
	mLoading->setScaledSize( QSize( 16, 16 ) );
	mLoading->jumpToFrame( 0 );
	
	lLoading->setMovie( mLoading );
	
	pbIndexing->setVisible( false );
	lLoading->setVisible( false );
	
	mSense = new qCtagsSense( this );
	mLanguagesModel = new qCtagsSenseLanguagesModel( mSense->sql() );
	mFilesModel = new qCtagsSenseFilesModel( mSense->sql() );
	mMembersModel = new qCtagsSenseMembersModel( mSense->sql() );
	mSearchModel = new qCtagsSenseSearchModel( mSense->sql() );
	
	aMembers = new MembersAction( this );
	
	tvMembers->setModel( mMembersModel );
	tvSearchResult->setModel( mSearchModel );
	
	mSearchTimer = new QTimer( this );
	
	connect( mSense, SIGNAL( indexingStarted() ), pbIndexing, SLOT( show() ) );
	connect( mSense, SIGNAL( indexingProgress( int, int ) ), this, SLOT( mSense_indexingProgress( int, int ) ) );
	connect( mSense, SIGNAL( indexingFinished() ), pbIndexing, SLOT( hide() ) );
	connect( mSense, SIGNAL( indexingChanged() ), this, SLOT( mSense_indexingChanged() ) );
	
	connect( mMembersModel, SIGNAL( ready() ), this, SLOT( mMembersModel_ready() ) );
	connect( mSearchTimer, SIGNAL( timeout() ), this, SLOT( mSearchTimer_timeout() ) );
	connect( mSearchModel, SIGNAL( ready() ), this, SLOT( mSearchModel_ready() ) );
	connect( mSearchModel, SIGNAL( searching( bool ) ), this, SLOT( mSearchModel_searching( bool ) ) );
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

QAction* qCtagsSenseBrowser::viewBrowserAction() const
{
	return aBrowser;
}

QAction* qCtagsSenseBrowser::viewSearchResultsAction() const
{
	return aSearchResults;
}

void qCtagsSenseBrowser::popupMenu( QTreeView* view, const QPoint& pos )
{
	QModelIndex index = view->currentIndex();
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
	
	QAction* aTriggered = menu.exec( view->mapToGlobal( pos ) );
	
	if ( aTriggered )
	{
		qCtagsSense::Kind kind = (qCtagsSense::Kind)aTriggered->data().toInt();
		
		if ( entry->kind == kind )
		{
			emit entryActivated( entry );
		}
		else
		{
			qCtagsSenseKindFinder* cpp = new qCtagsSenseKindFinder( mSense->sql() );
			connect( cpp, SIGNAL( entryActivated( qCtagsSenseEntry* ) ), this, SIGNAL( entryActivated( qCtagsSenseEntry* ) ) );
			cpp->goTo( kind, entry );
		}
	}
}

void qCtagsSenseBrowser::tagEntry( const QString& fileName )
{
	mSense->tagEntry( fileName );
}

void qCtagsSenseBrowser::tagEntries( const QMap<QString, QString>& entries )
{
	mSense->tagEntries( entries );
}

void qCtagsSenseBrowser::setSystemPaths( const QStringList& paths, const QStringList& oldPaths )
{
	mSense->setSystemPaths( paths, oldPaths );
}

void qCtagsSenseBrowser::setFilteredSuffixes( const QStringList& suffixes )
{
	mSense->indexer()->setFilteredSuffixes( suffixes );
}

void qCtagsSenseBrowser::setCurrentFileName( const QString& fileName )
{
	mLanguage = getFileNameLanguageName( fileName.toLocal8Bit().constData() );
	mFileName = fileName;
	
	// update model
	mMembersModel->refresh( mFileName );
}

void qCtagsSenseBrowser::on_aBrowser_toggled( bool checked )
{
	Q_UNUSED( checked );
	swPages->setCurrentIndex( 0 );
}

void qCtagsSenseBrowser::on_aSearchResults_toggled( bool checked )
{
	Q_UNUSED( checked );
	swPages->setCurrentIndex( 1 );
}

void qCtagsSenseBrowser::on_tvMembers_activated( const QModelIndex& index )
{
	qCtagsSenseEntry* entry = static_cast<qCtagsSenseEntry*>( index.internalPointer() );
	emit entryActivated( entry );
}

void qCtagsSenseBrowser::on_tvSearchResult_activated( const QModelIndex& index )
{
	switch ( index.data( qCtagsSenseSearchModel::TypeRole ).toInt() )
	{
		case qCtagsSenseSearchModel::FileName:
			emit fileNameActivated( index.data( qCtagsSenseSearchModel::DataRole ).toString() );
			break;
		case qCtagsSenseSearchModel::Entry:
			emit entryActivated( index.data( qCtagsSenseSearchModel::DataRole ).value<qCtagsSenseEntry*>() );
			break;
	}
}

void qCtagsSenseBrowser::mSense_indexingProgress( int value, int total )
{
	pbIndexing->setValue( value );
	pbIndexing->setMaximum( total );
}

void qCtagsSenseBrowser::mSense_indexingChanged()
{
	mMembersModel->refresh( mFileName );
}

void qCtagsSenseBrowser::mMembersModel_ready()
{
	tvMembers->expandAll();
	aBrowser->setChecked( true );
}

void qCtagsSenseBrowser::on_leSearch_textChanged( const QString& search )
{
	if ( search.length() < SEARCH_MIN_CHARS )
	{
		mSearchModel->clear();
	}
	else
	{
		mSearchTimer->start( SEARCH_TIMEOUT );
	}
}

void qCtagsSenseBrowser::mSearchModel_searching( bool searching )
{
	lLoading->setVisible( searching );
	
	if ( searching )
	{
		mLoading->start();
	}
	else
	{
		mLoading->stop();
		mLoading->jumpToFrame( 0 );
	}
}

void qCtagsSenseBrowser::mSearchTimer_timeout()
{
	mSearchTimer->stop();
	mSearchModel->refresh( leSearch->text() );
}

void qCtagsSenseBrowser::mSearchModel_ready()
{
	const QModelIndex index = mSearchModel->index( mFileName );
	
	if ( index.isValid() )
	{
		tvSearchResult->expand( index );
		tvSearchResult->setCurrentIndex( index );
		tvSearchResult->scrollTo( index, QAbstractItemView::PositionAtTop );
	}
	
	if ( mSearchModel->rowCount() > 0 )
	{
		aSearchResults->setChecked( true );
	}
	else
	{
		aBrowser->setChecked( true );
	}
}

void qCtagsSenseBrowser::on_tvMembers_customContextMenuRequested( const QPoint& pos )
{
	popupMenu( tvMembers, pos );
}

#include "qCtagsSenseBrowser.moc"
