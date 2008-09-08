#include "QtAssistantDock.h"
#include "QtAssistantBrowser.h"
#include "../MkSQtDocInstaller.h"
#include "../HelpViewer.h"
#include "../3rdparty/topicchooser.h"
#include "../3rdparty/bookmarkmanager.h"

#include <coremanager.h>

#include <QKeyEvent>

QtAssistantDock::QtAssistantDock( QWidget* parent )
	: pDockWidget( parent )
{
	setupUi( this );
	pbProgress->setVisible( false );

	// create help engine with default collection
	MkSQtDocInstaller::collectionFileDirectory( true );
	mHelpEngine = new QHelpEngine( MkSQtDocInstaller::defaultHelpCollectionFileName(), this );
	
	// set filters
	cbFilter->addItems( mHelpEngine->customFilters() );
	
	// create bookmarks manager
	mBookmarkManager = new BookmarkManager( mHelpEngine );
	bwBookmarks = new BookmarkWidget( mBookmarkManager, this );

	// create dock pages
	vlContents->addWidget( mHelpEngine->contentWidget() );
	vlIndex->addWidget( mHelpEngine->indexWidget() );
	vlBookmarks->addWidget( bwBookmarks );

	// create content actions
	mHelpEngine->contentWidget()->setContextMenuPolicy( Qt::ActionsContextMenu );
	aOpenContent = new QAction( tr( "Open Link" ), mHelpEngine->contentWidget() );
	aOpenContentInNewTab = new QAction( tr( "Open Link in New Tab" ), mHelpEngine->contentWidget() );
	mHelpEngine->contentWidget()->addAction( aOpenContent );
	mHelpEngine->contentWidget()->addAction( aOpenContentInNewTab );

	// create index actions
	mHelpEngine->indexWidget()->setContextMenuPolicy( Qt::ActionsContextMenu );
	aOpenIndex = new QAction( tr( "Open Link" ), mHelpEngine->indexWidget() );
	aOpenIndexInNewTab = new QAction( tr( "Open Link in New Tab" ), mHelpEngine->indexWidget() );
	mHelpEngine->indexWidget()->addAction( aOpenIndex );
	mHelpEngine->indexWidget()->addAction( aOpenIndexInNewTab );

	// create browser
	mBrowser = new QtAssistantBrowser( mHelpEngine );

	// connections
	connect( mBrowser, SIGNAL( showBrowserRequested() ), this, SLOT( showBrowser() ) );
	connect( mHelpEngine, SIGNAL( currentFilterChanged( const QString& ) ), this, SLOT( onCurrentFilterChanged( const QString& ) ) );
	connect( mHelpEngine->indexModel(), SIGNAL( indexCreationStarted() ),this, SLOT( disableSearchLineEdit() ) );
	connect( mHelpEngine->indexModel(), SIGNAL( indexCreated() ),this, SLOT( enableSearchLineEdit() ) );
	connect( leLookFor, SIGNAL( textChanged( const QString& ) ), this, SLOT( filterIndices( const QString& ) ) );
	connect( leLookFor, SIGNAL( returnPressed() ), mHelpEngine->indexWidget(), SLOT( activateCurrentItem() ) );
	connect( aOpenContent, SIGNAL( triggered() ), this, SLOT( openContent() ) );
	connect( aOpenContentInNewTab, SIGNAL( triggered() ), this, SLOT( openContent() ) );
	connect( mHelpEngine->contentWidget(), SIGNAL( linkActivated( const QUrl& ) ), mBrowser, SLOT( openUrl( const QUrl& ) ) );
	connect( aOpenIndex, SIGNAL( triggered() ), this, SLOT( openIndex() ) );
	connect( aOpenIndexInNewTab, SIGNAL( triggered() ), this, SLOT( openIndex() ) );
	connect( mHelpEngine->indexWidget(), SIGNAL( linkActivated( const QUrl&, const QString& ) ), mBrowser, SLOT( openUrl( const QUrl& ) ) );
	connect( mHelpEngine->indexWidget(), SIGNAL( linksActivated( const QMap<QString, QUrl>&, const QString& ) ), this, SLOT( openIndex() ) );
	connect( mHelpEngine->searchEngine(), SIGNAL( indexingStarted() ), this, SLOT( indexingStarted() ) );
	connect( mHelpEngine->searchEngine(), SIGNAL( indexingFinished() ), this, SLOT( indexingFinished() ) );
	connect( bwBookmarks, SIGNAL( requestShowLink( const QUrl& ) ), mBrowser, SLOT( openUrl( const QUrl& ) ) );
	connect( bwBookmarks, SIGNAL( requestShowLinkInNewTab( const QUrl& ) ), mBrowser, SLOT( openInNewTabUrl( const QUrl& ) ) );
	connect( bwBookmarks, SIGNAL( addBookmark() ), this, SLOT( addBookmark() ) );
	
	// browser child connection
	MonkeyCore::workspace()->initChildConnections( mBrowser );

	// install event filters
	leLookFor->installEventFilter( this );
	mHelpEngine->indexWidget()->installEventFilter( this );

	// show browser
	showBrowser();

	// init documentation
	mDocInstaller = new MkSQtDocInstaller( mHelpEngine );
	if ( mDocInstaller->checkDocumentation() )
		mBrowser->restoreLastShownPages();
}

QtAssistantDock::~QtAssistantDock()
{
	mBrowser->close();
	delete mBrowser;
}

void QtAssistantDock::hideEvent( QHideEvent* e )
{
	mBookmarkManager->saveBookmarks();

	pDockWidget::hideEvent( e );
}

bool QtAssistantDock::eventFilter( QObject* obj, QEvent* e )
{
	if ( obj == leLookFor && e->type() == QEvent::KeyPress )
	{
		QKeyEvent* ke = static_cast<QKeyEvent*>( e );
		QModelIndex idx = mHelpEngine->indexWidget()->currentIndex();
		switch ( ke->key() )
		{
			case Qt::Key_Up:
				idx = mHelpEngine->indexModel()->index( idx.row() -1, idx.column(), idx.parent() );
				if ( idx.isValid() )
					mHelpEngine->indexWidget()->setCurrentIndex( idx );
				break;
			case Qt::Key_Down:
				idx = mHelpEngine->indexModel()->index( idx.row() +1, idx.column(), idx.parent() );
				if ( idx.isValid() )
					mHelpEngine->indexWidget()->setCurrentIndex( idx );
				break;
			case Qt::Key_Escape:
				mBrowser->twPages->currentWidget()->setFocus();
				break;
			default:
				break;
		}
	}
#ifdef Q_OS_MAC
	else if ( obj == mHelpEngine->indexWidget() && e->type() == QEvent::KeyPress )
	{
		QKeyEvent* ke = static_cast<QKeyEvent*>( e );
		if ( ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter )
			mHelpEngine->indexWidget()->activateCurrentItem();
	}
#endif
	return pDockWidget::eventFilter( obj, e );
}

void QtAssistantDock::showBrowser()
{
	pWorkspace* workspace = MonkeyCore::workspace();
	if ( !workspace->children().contains( mBrowser ) )
	{
		workspace->addDocument( mBrowser, tr( "Qt Assistant" ) );
		mBrowser->setAttribute( Qt::WA_DeleteOnClose, false );
	}
	if ( workspace->currentChild() != mBrowser )
		workspace->setCurrentDocument( mBrowser );
}

void QtAssistantDock::onCurrentFilterChanged( const QString& filter )
{
	cbFilter->setCurrentIndex( cbFilter->findText( filter ) );
}

void QtAssistantDock::on_cbFilter_currentIndexChanged( const QString& filter )
{
	mHelpEngine->setCurrentFilter( filter );
}

void QtAssistantDock::disableSearchLineEdit()
{
	leLookFor->setDisabled( true );
}

void QtAssistantDock::enableSearchLineEdit()
{
	leLookFor->setEnabled( true );
	filterIndices( leLookFor->text() );
}

void QtAssistantDock::filterIndices( const QString& filter )
{
	if ( filter.contains( '*' ) )
		mHelpEngine->indexWidget()->filterIndices( filter, filter );
	else
		mHelpEngine->indexWidget()->filterIndices( filter, QString() );
}

void QtAssistantDock::openContent()
{
	QHelpContentItem* ci = mHelpEngine->contentModel()->contentItemAt( mHelpEngine->contentWidget()->currentIndex() );
	if ( ci )
	{
		if ( sender() == aOpenContentInNewTab )
			mBrowser->openInNewTabUrl( ci->url() );
		else
			mBrowser->openUrl( ci->url() );
	}
}

void QtAssistantDock::openIndex()
{
	const QModelIndex idx = mHelpEngine->indexWidget()->currentIndex();
	QString keyword = mHelpEngine->indexModel()->data( idx, Qt::DisplayRole ).toString();
	QMap<QString, QUrl> links = mHelpEngine->indexModel()->linksForKeyword( keyword );
	if ( links.count() == 1 )
	{
		if ( sender() == aOpenIndexInNewTab )
			mBrowser->openInNewTabUrl( links.constBegin().value() );
		else
			mBrowser->openUrl( links.constBegin().value() );
	}
	else
	{
		TopicChooser tc( this, keyword, links );
		if ( tc.exec() == QDialog::Accepted )
		{
			if ( sender() == aOpenIndexInNewTab )
				mBrowser->openInNewTabUrl( tc.link() );
			else
				mBrowser->openUrl( tc.link() );
		}
	}
}

void QtAssistantDock::addBookmark()
{
	if ( HelpViewer* hv = mBrowser->currentTab( false ) )
	{
		if ( hv->source().isEmpty() )
			return;
		mBookmarkManager->showBookmarkDialog( this, hv->documentTitle(), hv->source().toString() );
	}
}

void QtAssistantDock::indexingStarted()
{
	pbProgress->setFormat( "Updating search index %p%" );
	pbProgress->setRange( 0, 0 );
	pbProgress->setVisible( true );
}

void QtAssistantDock::indexingFinished()
{
	pbProgress->setVisible( false );
}
