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
#include "qCtagsSenseSearchPopup.h"

#include <ctags.h>

#include <QMenu>
#include <QFileInfo>
#include <QWidgetAction>
#include <QDebug>
#include <QScrollBar>
#include <QMovie>
#include <QComboBox>

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
	pbIndexing->setVisible( false );
	lSearch->setAttribute( Qt::WA_MacShowFocusRect, false );
	lSearch->setAttribute( Qt::WA_MacSmallSize );
	leSearch->setAttribute( Qt::WA_MacShowFocusRect, false );
	leSearch->setAttribute( Qt::WA_MacSmallSize );
	tvMembers->setAttribute( Qt::WA_MacShowFocusRect, false );
	tvMembers->setAttribute( Qt::WA_MacSmallSize );
	
	mSense = new qCtagsSense( this );
	mLanguagesModel = new qCtagsSenseLanguagesModel( mSense->sql() );
	mFilesModel = new qCtagsSenseFilesModel( mSense->sql() );
	mMembersModel = new qCtagsSenseMembersModel( mSense->sql() );
	mSearchModel = new qCtagsSenseSearchModel( mSense->sql() );
	
	tvMembers->setModel( mMembersModel );
	
	aMembers = new MembersAction( this );
	
	mLoading = new QMovie( this );
	mLoading->setFileName( ":/icons/loading.gif" );
	mLoading->setScaledSize( QSize( 16, 16 ) );
	mLoading->jumpToFrame( 0 );
	
	lLoading->setMovie( mLoading );
	lLoading->setVisible( false );
	
	mSearchTreeView = new qCtagsSenseSearchPopup( leSearch );
	mSearchTreeView->setModel( mSearchModel );
	
	connect( mSense, SIGNAL( indexingStarted() ), pbIndexing, SLOT( show() ) );
	connect( mSense, SIGNAL( indexingProgress( int, int ) ), this, SLOT( mSense_indexingProgress( int, int ) ) );
	connect( mSense, SIGNAL( indexingFinished() ), pbIndexing, SLOT( hide() ) );
	connect( mSense, SIGNAL( indexingChanged() ), this, SLOT( mSense_indexingChanged() ) );
	
	connect( mMembersModel, SIGNAL( ready() ), this, SLOT( mMembersModel_ready() ) );
	connect( leSearch, SIGNAL( textChanged( const QString& ) ), this, SLOT( mSearchModel_refresh( const QString& ) ) );
	connect( mSearchModel, SIGNAL( ready() ), this, SLOT( mSearchModel_ready() ) );
	connect( mSearchModel, SIGNAL( searching( bool ) ), this, SLOT( mSearchModel_searching( bool ) ) );
	
	connect( mSearchTreeView, SIGNAL( entryActivated( qCtagsSenseEntry* ) ), this, SIGNAL( memberActivated( qCtagsSenseEntry* ) ) );
	connect( mSearchTreeView, SIGNAL( fileNameActivated( const QString& ) ), this, SIGNAL( fileNameActivated( const QString& ) ) );
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
	
	if ( mSense->indexer()->isRunning() || mLanguagesModel->isRunning() || mFilesModel->isRunning() )
	{
		return;
	}
	
	// update model
	mMembersModel->refresh( mFileName );
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
	mMembersModel->refresh( mFileName );
}

void qCtagsSenseBrowser::mMembersModel_ready()
{
	tvMembers->expandAll();
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

void qCtagsSenseBrowser::mSearchModel_refresh( const QString& search )
{
	if ( search.length() < 3 )
	{
		mSearchModel->clear();
	}
	else
	{
		mSearchModel->refresh( search );
	}
}

void qCtagsSenseBrowser::mSearchModel_ready()
{
	//mSearchTreeView->expandAll();
	mSearchTreeView->showPopup();
}

void qCtagsSenseBrowser::on_tvMembers_customContextMenuRequested( const QPoint& pos )
{
	popupMenu( tvMembers, pos );
}

#include "qCtagsSenseBrowser.moc"
