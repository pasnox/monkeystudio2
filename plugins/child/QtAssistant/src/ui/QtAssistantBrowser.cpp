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
#include "QtAssistantBrowser.h"
#include "../HelpViewer.h"

#include <MonkeyCore.h>
#include <QueuedStatusBar.h>

#include <QTextBrowser>
#include <QClipboard>
#include <QtHelp>
#include <QCheckBox>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>
#include <QWebFrame>
#include <QKeyEvent>

QtAssistantBrowser::QtAssistantBrowser( QHelpEngine* engine, QWidget* parent )
	: pAbstractChild( parent ), mHelpEngine( engine )
{
	mLastHelpView = -1;
	mSearchZoomCount = 0;
	setupUi( this );
	isSearch->setVisible( false );
	
	// corner widgets
	tbCloneTab = new QToolButton( this );
	tbCloneTab->setAutoRaise( true );
	tbCloneTab->setDefaultAction( aAddNewPage );
	tbCloseTab = new QToolButton( this );
	tbCloseTab->setAutoRaise( true );
	tbCloseTab->setDefaultAction( aCloseCurrentPage );
	
	// set corner widgets
	twPages->setCornerWidget( tbCloneTab, Qt::TopLeftCorner );
	twPages->setCornerWidget( tbCloseTab, Qt::TopRightCorner );
	
	// search help context menu
	QTextBrowser* browser = qFindChild<QTextBrowser*>( mHelpEngine->searchEngine()->resultWidget() );
	browser->setContextMenuPolicy( Qt::CustomContextMenu );
	
	// insert search & result widget
	vlSearch->addWidget( mHelpEngine->searchEngine()->queryWidget() );
	vlSearch->addWidget( mHelpEngine->searchEngine()->resultWidget() );
	
	// change icons of action
	QWebView wv;
	aPrevious->setIcon( wv.pageAction( QWebPage::Back )->icon() );
	aNext->setIcon( wv.pageAction( QWebPage::Forward )->icon() );
	
	// set keysequence
	aPrevious->setShortcut( QKeySequence::Back );
	aNext->setShortcut( QKeySequence::Forward );
	//aHome->setShortcut( QKeySequence:: );
	//aFindInText->setShortcut( QKeySequence::Find ); // this conflict with global find menu action
	aZoomIn->setShortcut( QKeySequence::ZoomIn );
	aZoomOut->setShortcut( QKeySequence::ZoomOut );
	//aZoomReset->setShortcut( QKeySequence:: );
	aAddNewPage->setShortcut( QKeySequence::AddTab );
	aCloseCurrentPage->setShortcut( QKeySequence::Close );
	//aNextTab->setShortcut( QKeySequence::NextChild ); // this conflict with global next tab menu action
	aNextTab->setShortcut( Qt::CTRL +Qt::Key_Right );
	//aPreviousTab->setShortcut( QKeySequence::PreviousChild ); // this conflict with global previous tab menu action
	aPreviousTab->setShortcut( Qt::CTRL +Qt::Key_Left );
	
	// update actions state
	on_twPages_currentChanged( 0 );
	
	// event filter event
	foreach ( QLineEdit* le, twPages->widget( 0 )->findChildren<QLineEdit*>() )
		le->installEventFilter( this );
	
	// connections
	connect( aPrevious, SIGNAL( triggered() ), this, SLOT( previousPage() ) );
	connect( aNext, SIGNAL( triggered() ), this, SLOT( nextPage() ) );
	connect( aHome, SIGNAL( triggered() ), this, SLOT( homePage() ) );
	connect( aZoomIn, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );
	connect( aZoomOut, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );
	connect( aZoomReset, SIGNAL( triggered() ), this, SLOT( zoomReset() ) );
	connect( aFindInText, SIGNAL( triggered() ), this, SLOT( invokeSearch() ) );
	connect( aPreviousTab, SIGNAL( triggered() ), this, SLOT( previousTab() ) );
	connect( aNextTab, SIGNAL( triggered() ), this, SLOT( nextTab() ) );
	connect( tbCloneTab, SIGNAL( clicked() ), this, SLOT( cloneTab() ) );
	connect( tbCloseTab, SIGNAL( clicked() ), this, SLOT( closeTab() ) );
	connect( mHelpEngine->searchEngine(), SIGNAL( searchingStarted() ), this, SLOT( searchingStarted() ) );
	connect( mHelpEngine->searchEngine(), SIGNAL( searchingFinished( int ) ), this, SLOT( searchingFinished( int ) ) );
	connect( mHelpEngine->searchEngine()->queryWidget(), SIGNAL( search() ), this, SLOT( search() ) );
	connect( mHelpEngine->searchEngine()->resultWidget(), SIGNAL( requestShowLink( const QUrl& ) ), this, SLOT( openUrl( const QUrl& ) ) );
	connect( browser, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( search_customContextMenuRequested( const QPoint& ) ) );
	connect( isSearch->toolNext, SIGNAL( clicked() ), this, SLOT( findNext() ) );
	connect( isSearch->editFind, SIGNAL( returnPressed() ), this, SLOT( findNext() ) );
	connect( isSearch->editFind, SIGNAL( textChanged( const QString& ) ), this, SLOT( findCurrentText( const QString& ) ) );
	connect( isSearch->toolPrevious, SIGNAL( clicked() ), this, SLOT( findPrevious() ) );
}

QtAssistantBrowser::~QtAssistantBrowser()
{
}

void QtAssistantBrowser::hideEvent( QHideEvent* e )
{
	QString zoomCount;
	QString currentPages;
	QLatin1Char sep( '|' );
	for ( int i = 1; i < twPages->count(); ++i )
	{
		HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->widget( i ) );
		if ( hv && hv->source().isValid() )
		{
			currentPages.append( hv->source().toString() ).append( sep );
			zoomCount.append( QString::number( hv->textSizeMultiplier() ) ).append( sep );
		}
	}
	mHelpEngine->setCustomValue( QLatin1String( "LastTabPage" ), mLastHelpView );
	mHelpEngine->setCustomValue( QLatin1String( "LastShownPages" ), currentPages );
	mHelpEngine->setCustomValue( QLatin1String( "LastPagesZoomWebView" ), zoomCount );

	pAbstractChild::hideEvent( e );
}

QPoint QtAssistantBrowser::cursorPosition() const
{
	return QPoint( -1, -1 );
}

QString QtAssistantBrowser::currentFile() const
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
		return hv->source().toString();
	return QString::null;
}

QString QtAssistantBrowser::currentFileName() const
{
	return QFileInfo( currentFile() ).fileName();
}

pEditor* QtAssistantBrowser::currentEditor() const
{
	return 0;
}

bool QtAssistantBrowser::isModified() const
{
	return false;
}

bool QtAssistantBrowser::isUndoAvailable() const
{
	return false;
}

bool QtAssistantBrowser::isRedoAvailable() const
{
	return false;
}

bool QtAssistantBrowser::isCopyAvailable() const
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
		return hv->pageAction( QWebPage::Copy )->isEnabled();
	return false;
}

bool QtAssistantBrowser::isPasteAvailable() const
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
		return hv->pageAction( QWebPage::Paste )->isEnabled();
	return false;
}

bool QtAssistantBrowser::isGoToAvailable() const
{
	return false;
}

bool QtAssistantBrowser::isModified( const QString& /*filename*/ ) const
{
	return false;
}

bool QtAssistantBrowser::isPrintAvailable() const
{
	return true;
}

void QtAssistantBrowser::showFile( const QString& filename )
{
	for ( int i = 1; i < twPages->count(); i++ )
	{
		HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->widget( i ) );
		if ( hv && hv->source().toString() == filename )
		{
			twPages->setCurrentIndex( i );
			return;
		}
	}
}

void QtAssistantBrowser::undo()
{}

void QtAssistantBrowser::redo()
{}

void QtAssistantBrowser::cut()
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
		hv->pageAction( QWebPage::Cut )->trigger();
}

void QtAssistantBrowser::copy()
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
		hv->pageAction( QWebPage::Copy )->trigger();
}

void QtAssistantBrowser::paste()
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
		hv->pageAction( QWebPage::Paste )->trigger();
}

void QtAssistantBrowser::goTo()
{}

void QtAssistantBrowser::goTo( const QString& /*filename*/, const QPoint& /*pos*/, bool /*highlight*/ )
{}

void QtAssistantBrowser::invokeSearch()
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
	{
		isSearch->show();
		isSearch->editFind->selectAll();
		isSearch->editFind->setFocus( Qt::ShortcutFocusReason );
	}
	else if ( twPages->currentIndex() == 0 )
		mHelpEngine->searchEngine()->queryWidget()->setFocus( Qt::ShortcutFocusReason );
}

void QtAssistantBrowser::saveFile( const QString& /*filename*/ )
{}

void QtAssistantBrowser::backupCurrentFile( const QString& filename )
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( !hv )
		return;

	QFile file( filename );
	if ( !file.open( QIODevice::WriteOnly ) )
	{
		MonkeyCore::statusBar()->appendMessage( tr( "Can't open file for writing when creating backup file." ) );
		return;
	}
	file.resize( 0 );
	if ( file.write( hv->page()->mainFrame()->toHtml().toUtf8() ) != -1 )
		file.close();
	else
		MonkeyCore::statusBar()->appendMessage( tr( "Can't write file content when creating backup." ) );
}

void QtAssistantBrowser::saveFiles()
{}

bool QtAssistantBrowser::openFile( const QString& /*filename*/, const QString& /*codec*/ )
{
	return false;
}

void QtAssistantBrowser::closeFile( const QString& filename )
{
	for ( int i = 1; i < twPages->count(); i++ )
	{
		HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->widget( i ) );
		if ( hv && hv->source().toString() == filename )
		{
			twPages->removeTab( twPages->indexOf( hv ) );
			hv->deleteLater();
			return;
		}
	}
}

void QtAssistantBrowser::closeFiles()
{
	while ( twPages->count() != 1 )
	{
		QWidget* w = twPages->widget( 1 );
		twPages->removeTab( 1 );
		w->deleteLater();
	}
}

void QtAssistantBrowser::printFile( const QString& /*filename*/ )
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( !hv )
		return;

	QPrinter printer( QPrinter::HighResolution );
	QPrintDialog dlg( &printer, this );
	dlg.addEnabledOption( QAbstractPrintDialog::PrintPageRange );
	dlg.addEnabledOption( QAbstractPrintDialog::PrintCollateCopies );
	dlg.setWindowTitle( tr( "Print Document" ) );
	if ( dlg.exec() == QDialog::Accepted )
		hv->print( &printer );
}

void QtAssistantBrowser::quickPrintFile( const QString& /*filename*/ )
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( !hv )
		return;
		
	QPrinter printer( QPrinter::HighResolution );
	if ( printer.printerName().isEmpty() )
		MonkeyCore::statusBar()->appendMessage( tr( "There is no default printer, please set one before trying quick print" ) );
	else
		hv->print( &printer );
}

bool QtAssistantBrowser::eventFilter( QObject* obj, QEvent* e )
{
	if ( twPages->count() > 1 )
	{
		if ( e->type() == QEvent::KeyPress )
		{
			QKeyEvent* se = static_cast<QKeyEvent*>( e );
			if ( se->modifiers() != Qt::NoModifier )
			{
				const QKeySequence shortcut( se->modifiers() | se->key() );
				if ( aPreviousTab->shortcut().matches( shortcut ) )
					aPreviousTab->trigger();
				else if ( aNextTab->shortcut().matches( shortcut ) )
					aNextTab->trigger();
			}
		}
	}
	return pAbstractChild::eventFilter( obj, e );
}

HelpViewer* QtAssistantBrowser::newEmptyTab( qreal zoom )
{
	HelpViewer* hv = new HelpViewer( mHelpEngine, this );
	if ( mHelpEngine->customValue( QLatin1String( "useBrowserFont" ) ).toBool() )
	{
		QFont font = qVariantValue<QFont>( mHelpEngine->customValue( QLatin1String( "browserFont" ) ) );
		hv->setFont( font );
	}
	hv->setTextSizeMultiplier( zoom );
	twPages->setCurrentIndex( twPages->addTab( hv, QString::null ) );
	connect( hv, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( helpViewer_sourceChanged( const QUrl& ) ) );
	connect( hv, SIGNAL( actionsChanged() ), this, SLOT( helpViewer_actionsChanged() ) );
	return hv;
}

HelpViewer* QtAssistantBrowser::currentTab( bool create )
{
	if ( mLastHelpView != -1 )
		return qobject_cast<HelpViewer*>( twPages->widget( mLastHelpView ) );
	if ( create )
		return newEmptyTab();
	return 0;
}

void QtAssistantBrowser::cloneTab()
{
	HelpViewer* hv = currentTab( false );
	if ( hv )
	{
		const QUrl url = hv->source();
		newEmptyTab()->setSource( url );
	}
}

void QtAssistantBrowser::closeTab()
{
	HelpViewer* hv = currentTab( false );
	if ( hv )
	{
		twPages->removeTab( twPages->indexOf( hv ) );
		hv->deleteLater();
	}
}

void QtAssistantBrowser::openUrl( const QUrl& url )
{
	if ( !isVisible() )
		emit showBrowserRequested();
	HelpViewer* hv = currentTab( true );
	hv->setSource( url );
	twPages->setCurrentWidget( hv );
}

void QtAssistantBrowser::openInNewTabUrl( const QUrl& url )
{
	if ( !isVisible() )
		emit showBrowserRequested();
	HelpViewer* hv = newEmptyTab();
	hv->setSource( url );
}

void QtAssistantBrowser::previousTab()
{
	int index = twPages->currentIndex() -1;
	if ( index >= 0 )
		twPages->setCurrentIndex( index );
	else
		twPages->setCurrentIndex( twPages->count() -1 );
	updateActions();
}

void QtAssistantBrowser::nextTab()
{
	if ( twPages->currentIndex() < twPages->count() -1 )
		twPages->setCurrentIndex( twPages->currentIndex() +1 );
	else
		twPages->setCurrentIndex( 0 );
	updateActions();
}

void QtAssistantBrowser::previousPage()
{
	HelpViewer* hv = currentTab( false );
	if ( hv && twPages->currentWidget() == hv )
	{
		hv->back();
		updateActions();
	}
}

void QtAssistantBrowser::nextPage()
{
	HelpViewer* hv = currentTab( false );
	if ( hv && twPages->currentWidget() == hv )
	{
		hv->forward();
		updateActions();
	}
}

void QtAssistantBrowser::homePage()
{
	HelpViewer* hv = currentTab( false );
	if ( hv && twPages->currentWidget() == hv )
		hv->home();
}

void QtAssistantBrowser::searchZoomIn()
{
	QTextBrowser* browser = qFindChild<QTextBrowser*>( mHelpEngine->searchEngine()->resultWidget() );
	if ( browser && mSearchZoomCount != 10 )
	{
		mSearchZoomCount++;
		browser->zoomIn();
	}
}

void QtAssistantBrowser::searchZoomOut()
{
	QTextBrowser* browser = qFindChild<QTextBrowser*>( mHelpEngine->searchEngine()->resultWidget() );
	if ( browser && mSearchZoomCount != -5 )
	{
		mSearchZoomCount--;
		browser->zoomOut();
	}
}

void QtAssistantBrowser::searchResetZoom()
{
	if ( mSearchZoomCount == 0 )
		return;
		
	QTextBrowser* browser = qFindChild<QTextBrowser*>( mHelpEngine->searchEngine()->resultWidget() );
	if ( browser )
	{
		browser->zoomOut( mSearchZoomCount );
		mSearchZoomCount = 0;
	}
}
	
void QtAssistantBrowser::zoomIn()
{
	HelpViewer* hv = currentTab( false );
	if ( hv && twPages->currentWidget() == hv )
		hv->zoomIn();
	
	if ( twPages->currentIndex() == 0 )
		searchZoomIn();
}

void QtAssistantBrowser::zoomOut()
{
	HelpViewer* hv = currentTab( false );
	if ( hv && twPages->currentWidget() == hv )
		hv->zoomOut();
	
	if ( twPages->currentIndex() == 0 )
		searchZoomOut();
}

void QtAssistantBrowser::zoomReset()
{
	HelpViewer* hv = currentTab( false );
	if ( hv && twPages->currentWidget() == hv )
		hv->resetZoom();
	
	if ( twPages->currentIndex() == 0 )
		searchResetZoom();
}

void QtAssistantBrowser::updateActions()
{
	HelpViewer* hv = currentTab( false );
	aPrevious->setEnabled( hv && twPages->currentWidget() == hv ? hv->isBackwardAvailable() : false );
	aNext->setEnabled( hv && twPages->currentWidget() == hv ? hv->isForwardAvailable() : false );
	aHome->setEnabled( hv && twPages->currentWidget() == hv ? true : false );
	aPreviousTab->setEnabled( twPages->count() > 1 );
	aNextTab->setEnabled( twPages->count() > 1 );
	helpViewer_actionsChanged();
}

void QtAssistantBrowser::on_twPages_currentChanged( int index )
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	if ( hv )
		mLastHelpView = index;
	if ( twPages->count() == 1 )
		mLastHelpView = -1;

	twPages->cornerWidget( Qt::TopRightCorner )->setEnabled( twPages->currentIndex() != 0 );
	twPages->cornerWidget( Qt::TopLeftCorner )->setEnabled( twPages->currentIndex() != 0 );
	
	updateActions();
	
	emit currentFileChanged( currentFile() );
}

void QtAssistantBrowser::helpViewer_sourceChanged( const QUrl& )
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( sender() );
	if ( !hv )
		return;
	const int id = twPages->indexOf( hv );
	twPages->setTabText( id, hv->documentTitle() );
	updateActions();
}

void QtAssistantBrowser::helpViewer_actionsChanged()
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( sender() );
	emit pasteAvailableChanged( hv ? hv->pageAction( QWebPage::Paste )->isEnabled() : false );
	emit copyAvailableChanged( hv ? hv->pageAction( QWebPage::Copy )->isEnabled() : false );
}

void QtAssistantBrowser::searchingStarted()
{
	setCursor( Qt::WaitCursor );
}

void QtAssistantBrowser::searchingFinished( int hits )
{
	Q_UNUSED( hits )
	unsetCursor();
}

void QtAssistantBrowser::search() const
{
	const QList<QHelpSearchQuery> query = mHelpEngine->searchEngine()->queryWidget()->query();
	mHelpEngine->searchEngine()->search( query );
}

void QtAssistantBrowser::search_customContextMenuRequested( const QPoint& pos )
{
	QTextBrowser* browser = qobject_cast<QTextBrowser*>( sender() );
	if ( !browser )
		return;

	const QUrl link = browser->anchorAt( pos );
	QMenu menu;

	QAction* aCopy = menu.addAction( tr( "&Copy" ) );
	aCopy->setShortcut( QKeySequence::Copy );
	aCopy->setEnabled( browser->textCursor().hasSelection() );

	QAction* aCopyAnchor = menu.addAction( tr( "Copy &Link Location" ) );
	aCopyAnchor->setEnabled( !link.isEmpty() && link.isValid() );

	QAction* aNewTab = menu.addAction( tr( "Open Link in New Tab" ) );
	aNewTab->setEnabled( !link.isEmpty() && link.isValid() );

	menu.addSeparator();

	QAction* aSelectAll = menu.addAction( tr( "Select All" ) );
	aSelectAll->setShortcut( QKeySequence::SelectAll );

	QAction* aChoosen = menu.exec( browser->mapToGlobal( pos ) );
	if ( aChoosen == aCopy )
	{
		QTextCursor cursor = browser->textCursor();
		if ( !cursor.isNull() && cursor.hasSelection() )
		{
			QString selectedText = cursor.selectedText();
			QMimeData* data = new QMimeData();
			data->setText( selectedText );
			QApplication::clipboard()->setMimeData( data );
		}
	}
	else if ( aChoosen == aCopyAnchor )
		QApplication::clipboard()->setText( link.toString() );
	else if ( aChoosen == aNewTab )
		openInNewTabUrl( link );
	else if ( aChoosen == aSelectAll )
		browser->selectAll();
}

void QtAssistantBrowser::findNext()
{
	find( isSearch->editFind->text(), true, false );
}

void QtAssistantBrowser::findPrevious()
{
	find( isSearch->editFind->text(), false, true );
}

void QtAssistantBrowser::findCurrentText(const QString &text)
{
	find( text, false, false );
}

void QtAssistantBrowser::find( QString ttf, bool forward, bool backward )
{
	HelpViewer* hv = qobject_cast<HelpViewer*>( twPages->currentWidget() );
	QPalette p = isSearch->editFind->palette();
	p.setColor( QPalette::Active, QPalette::Base, Qt::white );

	Q_UNUSED( backward )

	if ( hv )
	{
		QWebPage::FindFlags options;
		if ( !forward )
			options |= QWebPage::FindBackward;

		if ( isSearch->checkCase->isChecked() )
			options |= QWebPage::FindCaseSensitively;

		bool found = hv->findText( ttf, options );
		isSearch->labelWrapped->hide();

		if ( !found )
		{
			options |= QWebPage::FindWrapsAroundDocument;
			found = hv->findText( ttf, options );

			if ( !found )
				p.setColor( QPalette::Active, QPalette::Base, QColor( 255, 102, 102 ) );
			else
				isSearch->labelWrapped->show();
		}
	}

	if ( !isSearch->isVisible() )
		isSearch->show();

	isSearch->editFind->setPalette( p );
}

void QtAssistantBrowser::restoreLastShownPages()
{
	QLatin1String zoom( "LastPagesZoomWebView" );
	const QStringList lastShownPageList = mHelpEngine->customValue( QLatin1String( "LastShownPages" ) ).toString().split( QLatin1Char( '|' ), QString::SkipEmptyParts );

	if ( !lastShownPageList.isEmpty() )
	{
		QVector<QString> zoomList = mHelpEngine->customValue( zoom ).toString().split( QLatin1Char( '|' ), QString::SkipEmptyParts ).toVector();
		if ( zoomList.isEmpty() )
			zoomList.fill( QLatin1String( "1.0" ), lastShownPageList.size() );

		QVector<QString>::const_iterator zIt = zoomList.constBegin();
		QStringList::const_iterator it = lastShownPageList.constBegin();
		for ( ; it != lastShownPageList.constEnd(); ++it, ++zIt )
		{
			HelpViewer* hv = newEmptyTab( (*zIt).toFloat() );
			hv->setSource( (*it) );
		}

		twPages->setCurrentIndex( mHelpEngine->customValue( QLatin1String( "LastTabPage" ), 1 ).toInt() );
	}
	/*
	else
		openUrl( QUrl( QLatin1String( "help" ) ) );
	*/
}
