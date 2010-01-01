#include "SearchWidget.h"
#include "SearchThread.h"
#include "ReplaceThread.h"
#include "SearchResultsDock.h"

#include <MonkeyCore.h>
#include <UIMain.h>
#include <pFileManager.h>
#include <XUPProjectItem.h>
#include <pWorkspace.h>
#include <pAbstractChild.h>
#include <pChild.h>
#include <pEditor.h>
#include <pQueuedMessageToolBar.h>

#include <QTextCodec>
#include <QFileDialog>
#include <QCompleter>
#include <QDirModel>

SearchWidget::SearchWidget( QWidget* parent )
	: QFrame( parent )
{
	setupUi( this );
	cbSearch->completer()->setCaseSensitivity( Qt::CaseSensitive );
	cbReplace->completer()->setCaseSensitivity( Qt::CaseSensitive );
	cbPath->lineEdit()->setCompleter( new QCompleter( new QDirModel( this ) ) );
	cbMask->completer()->setCaseSensitivity( Qt::CaseSensitive );
	pbSearchStop->setVisible( false );
	pbReplaceCheckedStop->setVisible( false );
	
	// threads
	mSearchThread = new SearchThread( this );
	mReplaceThread = new ReplaceThread( this );
	
	mDock = 0;
	
	// mode actions
	QMenu* menuMode = new QMenu( pbMode );
	QActionGroup* groupMode = new QActionGroup( menuMode );
	
	mModeActions[ SearchAndReplaceV2::ModeSearch ] = menuMode->addAction( tr( "&Search in File" ) );
	mModeActions[ SearchAndReplaceV2::ModeReplace ] = menuMode->addAction( tr( "&Replace in File" ) );
	mModeActions[ SearchAndReplaceV2::ModeSearchDirectory ] = menuMode->addAction( tr( "Search in &Directory" ) );
	mModeActions[ SearchAndReplaceV2::ModeReplaceDirectory ] = menuMode->addAction( tr( "Repla&ce in Directory" ) );
	mModeActions[ SearchAndReplaceV2::ModeSearchProjectFiles ] = menuMode->addAction( tr( "Search in &Project" ) );
	mModeActions[ SearchAndReplaceV2::ModeReplaceProjectFiles ] = menuMode->addAction( tr( "R&eplace in Project" ) );
	mModeActions[ SearchAndReplaceV2::ModeSearchOpenedFiles ] = menuMode->addAction( tr( "Search in &Opened Files" ) );
	mModeActions[ SearchAndReplaceV2::ModeReplaceOpenedFiles ] = menuMode->addAction( tr( "Replace in Opened &Files" ) );
	
	foreach ( QAction* action, menuMode->actions() )
	{
		action->setCheckable( true );
		groupMode->addAction( action );
	}
	
	pbMode->setMenu( menuMode );
	
	// options actions
	QMenu* menuOptions = new QMenu( pbOptions );
	
	mOptionActions[ SearchAndReplaceV2::OptionCaseSensitive ] = menuOptions->addAction( tr( "&Case Sensitive" ) );
	mOptionActions[ SearchAndReplaceV2::OptionWholeWord ] = menuOptions->addAction( tr( "&Whole Word" ) );
	mOptionActions[ SearchAndReplaceV2::OptionWrap ] = menuOptions->addAction( tr( "Wra&p" ) );
	mOptionActions[ SearchAndReplaceV2::OptionRegularExpression ] = menuOptions->addAction( tr( "&Regular Expression" ) );
	
	foreach ( QAction* action, menuOptions->actions() )
	{
		action->setCheckable( true );
	}
	
	pbOptions->setMenu( menuOptions );
	
	// mac
	pMonkeyStudio::showMacFocusRect( this, false, true );
	pMonkeyStudio::setMacSmallSize( this, true, true );

#ifdef Q_OS_MAC
	const QSize size( 12, 12 );
	
	foreach ( QAbstractButton* button, findChildren<QAbstractButton*>() )
	{
		button->setIconSize( size );
		button->setFixedHeight( 24 );
	}
	
	vlMain->setSpacing( 0 );
#endif
	
	// codecs
	QStringList codecs;
	foreach ( const QString& codec, QTextCodec::availableCodecs() )
	{
		codecs << codec;
	}
	codecs.sort();
	cbCodec->addItems( codecs );
	
	cbCodec->setCurrentIndex( cbCodec->findText( pMonkeyStudio::defaultCodec() ) );
	
	// connections
	connect( groupMode, SIGNAL( triggered( QAction* ) ), this, SLOT( groupMode_triggered( QAction* ) ) );
	connect( cbSearch->lineEdit(), SIGNAL( textChanged( const QString& ) ), mSearchThread, SLOT( clear() ) );
	connect( mSearchThread, SIGNAL( started() ), this, SLOT( searchThread_stateChanged() ) );
	connect( mSearchThread, SIGNAL( finished() ), this, SLOT( searchThread_stateChanged() ) );
	connect( mReplaceThread, SIGNAL( started() ), this, SLOT( replaceThread_stateChanged() ) );
	connect( mReplaceThread, SIGNAL( finished() ), this, SLOT( replaceThread_stateChanged() ) );
	connect( mReplaceThread, SIGNAL( openedFileHandled( const QString&, const QString&, const QString& ) ), this, SLOT( replaceThread_openedFileHandled( const QString&, const QString&, const QString& ) ) );
	connect( mReplaceThread, SIGNAL( error( const QString& ) ), this, SLOT( replaceThread_error( const QString& ) ) );
	
	setMode( SearchAndReplaceV2::ModeSearch );
}

SearchWidget::~SearchWidget()
{
	delete mSearchThread;
	delete mReplaceThread;
}

SearchAndReplaceV2::Mode SearchWidget::mode() const
{
	return mMode;
}

SearchThread* SearchWidget::searchThread() const
{
	return mSearchThread;
}

void SearchWidget::setResultsDock( SearchResultsDock* dock )
{
	if ( mDock == dock )
	{
		return;
	}
	
	mDock = dock;
	
	// connections
	connect( mReplaceThread, SIGNAL( resultsHandled( const QString&, const SearchResultsModel::ResultList& ) ), mDock->model(), SLOT( thread_resultsHandled( const QString&, const SearchResultsModel::ResultList& ) ) );
}

void SearchWidget::setMode( SearchAndReplaceV2::Mode mode )
{	
	mSearchThread->stop();
	mReplaceThread->stop();
	
	mMode = mode;
	mModeActions[ mMode ]->setChecked( true );
	
	pAbstractChild* document = MonkeyCore::workspace()->currentDocument();
	pEditor* editor = document ? document->editor() : 0;
	const QString searchText = editor ? editor->selectedText() : QString::null;
	
	setVisible( mode != SearchAndReplaceV2::ModeNo );
	
	if ( isVisible() )
	{
		cbSearch->setEditText( searchText );
		cbSearch->lineEdit()->selectAll();
		
		if ( mode & SearchAndReplaceV2::ModeFlagSearch )
		{
			cbSearch->setFocus();
		}
		else
		{
			cbReplace->setFocus();
		}
	}
	
	switch ( mMode )
	{
		case SearchAndReplaceV2::ModeNo:
			wSearch->setVisible( false );
			wReplace->setVisible( false );
			wPath->setVisible( false );
			wOptions->setVisible( false );
			break;
		case SearchAndReplaceV2::ModeSearch:
			wSearch->setVisible( true );
			pbPrevious->setVisible( true );
			pbNext->setVisible( true );
			pbSearch->setVisible( false );
			
			wReplace->setVisible( false );
			
			wPath->setVisible( false );
			pbReplace->setVisible( false );
			pbReplaceAll->setVisible( false );
			pbReplaceChecked->setVisible( false );
			
			wOptions->setVisible( true );
			wMask->setVisible( false );
			wCodec->setVisible( false );
			break;
		case SearchAndReplaceV2::ModeReplace:
			wSearch->setVisible( true );
			pbPrevious->setVisible( true );
			pbNext->setVisible( true );
			pbSearch->setVisible( false );
			
			wReplace->setVisible( true );
			
			wPath->setVisible( false );
			pbReplace->setVisible( true );
			pbReplaceAll->setVisible( true );
			pbReplaceChecked->setVisible( false );
			
			wOptions->setVisible( true );
			wMask->setVisible( false );
			wCodec->setVisible( false );
			break;
		case SearchAndReplaceV2::ModeSearchDirectory:
			wSearch->setVisible( true );
			pbPrevious->setVisible( false );
			pbNext->setVisible( false );
			pbSearch->setVisible( true );
			
			wReplace->setVisible( false );
			
			wPath->setVisible( true );
			pbReplace->setVisible( false );
			pbReplaceAll->setVisible( false );
			pbReplaceChecked->setVisible( false );
			
			wOptions->setVisible( true );
			wMask->setVisible( true );
			wCodec->setVisible( true );
			break;
		case SearchAndReplaceV2::ModeReplaceDirectory:
			wSearch->setVisible( true );
			pbPrevious->setVisible( false );
			pbNext->setVisible( false );
			pbSearch->setVisible( true );
			
			wReplace->setVisible( true );
			
			wPath->setVisible( true );
			pbReplace->setVisible( false );
			pbReplaceAll->setVisible( false );
			pbReplaceChecked->setVisible( true );
			
			wOptions->setVisible( true );
			wMask->setVisible( true );
			wCodec->setVisible( true );
			break;
		case SearchAndReplaceV2::ModeSearchProjectFiles:
			wSearch->setVisible( true );
			pbPrevious->setVisible( false );
			pbNext->setVisible( false );
			pbSearch->setVisible( true );
			
			wReplace->setVisible( false );
			
			wPath->setVisible( false );
			pbReplace->setVisible( false );
			pbReplaceAll->setVisible( false );
			pbReplaceChecked->setVisible( false );
			
			wOptions->setVisible( true );
			wMask->setVisible( true );
			wCodec->setVisible( true );
			break;
		case SearchAndReplaceV2::ModeReplaceProjectFiles:
			wSearch->setVisible( true );
			pbPrevious->setVisible( false );
			pbNext->setVisible( false );
			pbSearch->setVisible( true );
			
			wReplace->setVisible( true );
			
			wPath->setVisible( false );
			pbReplace->setVisible( false );
			pbReplaceAll->setVisible( false );
			pbReplaceChecked->setVisible( true );
			
			wOptions->setVisible( true );
			wMask->setVisible( true );
			wCodec->setVisible( true );
			break;
		case SearchAndReplaceV2::ModeSearchOpenedFiles:
			wSearch->setVisible( true );
			pbPrevious->setVisible( false );
			pbNext->setVisible( false );
			pbSearch->setVisible( true );
			
			wReplace->setVisible( false );
			
			wPath->setVisible( false );
			pbReplace->setVisible( false );
			pbReplaceAll->setVisible( false );
			pbReplaceChecked->setVisible( false );
			
			wOptions->setVisible( true );
			wMask->setVisible( true );
			wCodec->setVisible( false );
			break;
		case SearchAndReplaceV2::ModeReplaceOpenedFiles:
			wSearch->setVisible( true );
			pbPrevious->setVisible( false );
			pbNext->setVisible( false );
			pbSearch->setVisible( true );
			
			wReplace->setVisible( true );
			
			wPath->setVisible( false );
			pbReplace->setVisible( false );
			pbReplaceAll->setVisible( false );
			pbReplaceChecked->setVisible( true );
			
			wOptions->setVisible( true );
			wMask->setVisible( true );
			wCodec->setVisible( false );
			break;
	}
	
	updateLabels();
	updateWidgets();
	initializeProperties();
}

void SearchWidget::keyPressEvent( QKeyEvent* event )
{
	if ( event->modifiers() == Qt::NoModifier )
	{
		switch ( event->key() )
		{
			case Qt::Key_Escape:
			{
				MonkeyCore::workspace()->focusEditor();
				hide();
				
				break;
			}
			case Qt::Key_Enter:
			case Qt::Key_Return:
			{
				switch ( mMode )
				{
					case SearchAndReplaceV2::ModeNo:
						break;
					case SearchAndReplaceV2::ModeSearch:
						pbNext->click();
						break;
					case SearchAndReplaceV2::ModeSearchDirectory:
					case SearchAndReplaceV2::ModeSearchProjectFiles:
					case SearchAndReplaceV2::ModeSearchOpenedFiles:
						pbSearch->click();
						break;
					case SearchAndReplaceV2::ModeReplace:
						pbReplace->click();
						break;
					case SearchAndReplaceV2::ModeReplaceDirectory:
					case SearchAndReplaceV2::ModeReplaceProjectFiles:
					case SearchAndReplaceV2::ModeReplaceOpenedFiles:
						pbReplaceChecked->click();
						break;
				}
				
				break;
			}
		}
	}
	
	QWidget::keyPressEvent( event );
}

void SearchWidget::updateLabels()
{
	int width = 0;
	
	if ( lSearch->isVisible() )
	{
		width = qMax( width, lSearch->minimumSizeHint().width() );
	}
	
	if ( lReplace->isVisible() )
	{
		width = qMax( width, lReplace->minimumSizeHint().width() );
	}
	
	if ( lPath->isVisible() )
	{
		width = qMax( width, lPath->minimumSizeHint().width() );
	}
	
	lSearch->setMinimumWidth( width );
	lReplace->setMinimumWidth( width );
	lPath->setMinimumWidth( width );
}

void SearchWidget::updateWidgets()
{
	int width = 0;
	
	if ( wSearchRight->isVisible() )
	{
		width = qMax( width, wSearchRight->minimumSizeHint().width() );
	}
	
	if ( wReplaceRight->isVisible() )
	{
		width = qMax( width, wReplaceRight->minimumSizeHint().width() );
	}
	
	if ( wPathRight->isVisible() )
	{
		width = qMax( width, wPathRight->minimumSizeHint().width() );
	}
	
	wSearchRight->setMinimumWidth( width );
	wReplaceRight->setMinimumWidth( width );
	wPathRight->setMinimumWidth( width );
}

void SearchWidget::initializeProperties()
{
	const QMap<QString, QStringList> suffixes = pMonkeyStudio::availableLanguagesSuffixes();
	const QStringList keys = suffixes.keys();
	mProperties.searchText = cbSearch->currentText();
	mProperties.replaceText = cbReplace->currentText();
	mProperties.searchPath = cbPath->currentText();
	mProperties.mode = mMode;
	mProperties.mask.clear();
	mProperties.codec = cbCodec->currentText();
	mProperties.options = SearchAndReplaceV2::Options( SearchAndReplaceV2::ModeNo );
	mProperties.openedFiles.clear();
	mProperties.project = MonkeyCore::fileManager()->currentProject();
	mProperties.sourcesFiles.clear();
	
	// update masks
	foreach ( const QString& part, cbMask->currentText().split( " ", QString::SkipEmptyParts ) )
	{
		const int index = keys.indexOf( QRegExp( QRegExp::escape( part ), Qt::CaseInsensitive ) );
		
		if ( index != -1 )
		{
			foreach ( const QString& suffixe, suffixes[ keys.at( index ) ] )
			{
				if ( !mProperties.mask.contains( suffixe ) )
				{
					mProperties.mask << suffixe;
				}
			}
		}
		else
		{
			mProperties.mask << part;
		}
	}
	
	// update options
	foreach ( const SearchAndReplaceV2::Option& option, mOptionActions.keys() )
	{
		QAction* action = mOptionActions[ option ];
		
		if ( action->isChecked() )
		{
			mProperties.options |= option;
		}
	}
	
	// update opened files
	foreach ( pAbstractChild* document, MonkeyCore::workspace()->documents() )
	{
		mProperties.openedFiles[ document->filePath() ] = document->fileBuffer();
	}
	
	// update project
	mProperties.project = mProperties.project ? mProperties.project->topLevelProject() : 0;
	
	// update sources files
	mProperties.sourcesFiles = mProperties.project ? mProperties.project->topLevelProjectSourceFiles() : QStringList();
}

void SearchWidget::showMessage( const QString& status )
{
	if ( status.isEmpty() )
	{
		MonkeyCore::mainWindow()->statusBar()->clearMessage();
	}
	else
	{
		MonkeyCore::mainWindow()->statusBar()->showMessage( status, 30000 );
	}
}

void SearchWidget::setState( SearchWidget::InputField field, SearchWidget::State state )
{
	QWidget* widget = 0;
	QColor color = QColor( Qt::white );
	
	switch ( field )
	{
		case SearchWidget::Search:
			widget = cbSearch->lineEdit();
			break;
		case SearchWidget::Replace:
			widget = cbReplace->lineEdit();
			break;
	}
	
	switch ( state )
	{
		case SearchWidget::Normal:
			color = QColor( Qt::white );
			break;
		case SearchWidget::Good:
			color = QColor( Qt::green );
			break;
		case SearchWidget::Bad:
			color = QColor( Qt::red );
			break;
	}
	
	QPalette pal = widget->palette();
	pal.setColor( widget->backgroundRole(), color );
	widget->setPalette( pal );
}

bool SearchWidget::searchFile( bool forward )
{
	pAbstractChild* document = MonkeyCore::workspace()->currentDocument();
	pChild* child = document ? static_cast<pChild*>( document ) : 0;
	pEditor* editor = child ? child->editor() : 0;
	
	if ( !editor )
	{
		setState( SearchWidget::Search, SearchWidget::Bad );
		showMessage( tr( "No active editor" ) );
		return false;
	}

	// get cursor position
	int x, y;
	editor->getCursorPosition( &y, &x );

	if ( !forward )
	{
		int temp;
		editor->getSelection( &y, &x, &temp, &temp );
	}

	// search
	const bool found = editor->findFirst( mProperties.searchText, mProperties.options & SearchAndReplaceV2::OptionRegularExpression, mProperties.options & SearchAndReplaceV2::OptionCaseSensitive, mProperties.options & SearchAndReplaceV2::OptionWholeWord, mProperties.options & SearchAndReplaceV2::OptionWrap, forward, y, x );

	// change background acording to found or not
	setState( SearchWidget::Search, found ? SearchWidget::Good : SearchWidget::Bad );
	
	// show message if needed
	showMessage( found ? QString::null : tr( "Not Found" ) );

	// return found state
	return found;
}

bool SearchWidget::replaceFile( bool all )
{
	pAbstractChild* document = MonkeyCore::workspace()->currentDocument();
	pChild* child = document ? static_cast<pChild*>( document ) : 0;
	pEditor* editor = child ? child->editor() : 0;
	
	if ( !editor )
	{
		setState( SearchWidget::Search, SearchWidget::Bad );
		showMessage( tr( "No active editor" ) );
		return false;
	}

	int count = 0;
	
	if ( all )
	{
		int x, y;
		
		editor->getCursorPosition( &y, &x );
		
		if ( mProperties.options & SearchAndReplaceV2::OptionWrap )
		{
			editor->setCursorPosition( 0, 0 );
		}
		
		while ( searchFile( true/*, false, false*/ ) ) // search next
		{
			editor->replace( mProperties.replaceText );
			count++;
		}
		
		editor->setCursorPosition( y, x ); // restore cursor position
	}
	else
	{
		int x, y, temp;
		
		editor->getSelection( &y, &x, &temp, &temp );
		editor->setCursorPosition( y, x );

		if ( searchFile( true/*, false, true*/ ) )
		{
			editor->replace( mProperties.replaceText );
			count++;
			pbNext->click(); // move selection to next item
		}
	}
	
	showMessage( tr( "%1 occurrence(s) replaced." ).arg( count ) );
	
	return true;
}

void SearchWidget::searchThread_stateChanged()
{
	pbSearchStop->setVisible( mSearchThread->isRunning() );
	updateWidgets();
}

void SearchWidget::replaceThread_stateChanged()
{
	pbReplaceCheckedStop->setVisible( mReplaceThread->isRunning() );
	updateWidgets();
}

void SearchWidget::replaceThread_openedFileHandled( const QString& fileName, const QString& content, const QString& codec )
{
	pAbstractChild* document = MonkeyCore::fileManager()->openFile( fileName, codec );
	pEditor* editor = document->editor();
	
	Q_ASSERT( editor );
	
	editor->beginUndoAction();
	editor->selectAll();
	editor->removeSelectedText();
	editor->insert( content );
	editor->endUndoAction();
}

void SearchWidget::replaceThread_error( const QString& error )
{
	MonkeyCore::messageManager()->appendMessage( error, 0 );
}

void SearchWidget::groupMode_triggered( QAction* action )
{
	setMode( mModeActions.key( action ) );
}

void SearchWidget::on_pbPrevious_clicked()
{
	initializeProperties();
	searchFile( false );
}

void SearchWidget::on_pbNext_clicked()
{
	initializeProperties();
	searchFile( true );
}

void SearchWidget::on_pbSearch_clicked()
{
	setState( SearchWidget::Search, SearchWidget::Normal );
	initializeProperties();
	mSearchThread->search( mProperties );
}

void SearchWidget::on_pbSearchStop_clicked()
{
	mSearchThread->stop();
}

void SearchWidget::on_pbReplace_clicked()
{
	initializeProperties();
	replaceFile( false );
}

void SearchWidget::on_pbReplaceAll_clicked()
{
	initializeProperties();
	replaceFile( true );
}

void SearchWidget::on_pbReplaceChecked_clicked()
{
	QHash<QString, SearchResultsModel::ResultList> items;
	SearchResultsModel* model = mDock ? mDock->model() : 0;
	
	Q_ASSERT( model );
	
	initializeProperties();
	
	foreach ( const SearchResultsModel::ResultList& results, model->results() )
	{
		foreach ( SearchResultsModel::Result* result, results )
		{
			if ( result->enabled && result->checkState == Qt::Checked )
			{
				items[ result->fileName ] << result;
			}
			else
			{
				const QModelIndex index = mDock->model()->index( result );
				mDock->model()->setData( index, false, SearchResultsModel::EnabledRole );
			}
		}
	}
	
	mReplaceThread->replace( mProperties, items );
}

void SearchWidget::on_pbReplaceCheckedStop_clicked()
{
	mReplaceThread->stop();
}

void SearchWidget::on_pbBrowse_clicked()
{
	const QString path = QFileDialog::getExistingDirectory( this, tr( "Search path" ), cbPath->currentText() );
	
	if ( !path.isEmpty() )
	{
		cbPath->setEditText( path );
	}
}
