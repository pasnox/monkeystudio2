#include "SearchWidget.h"

#include <MonkeyCore.h>
#include <pFileManager.h>
#include <XUPProjectItem.h>
#include <pWorkspace.h>
#include <pAbstractChild.h>

#include <QTextCodec>

SearchWidget::SearchWidget( QWidget* parent )
	: QFrame( parent )
{
	setupUi( this );
	
	// mode actions
	QMenu* menuMode = new QMenu( pbMode );
	QActionGroup* groupMode = new QActionGroup( menuMode );
	
	mModeActions[ SearchAndReplaceV2::ModeSearch ] = menuMode->addAction( tr( "&Search in File" ) );
	mModeActions[ SearchAndReplaceV2::ModeReplace ] = menuMode->addAction( tr( "&Replace in File" ) );
	mModeActions[ SearchAndReplaceV2::ModeSearchDirectory ] = menuMode->addAction( tr( "Search in &Directory" ) );
	mModeActions[ SearchAndReplaceV2::ModeReplaceDirectory ] = menuMode->addAction( tr( "Repla&ce in Directory" ) );
	mModeActions[ SearchAndReplaceV2::ModeSearchProject ] = menuMode->addAction( tr( "Search in &Project" ) );
	mModeActions[ SearchAndReplaceV2::ModeReplaceProject ] = menuMode->addAction( tr( "R&eplace in Project" ) );
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
	mOptionActions[ SearchAndReplaceV2::OptionRegularExpression ] = menuOptions->addAction( tr( "&Regular Expression" ) );
	
	foreach ( QAction* action, menuOptions->actions() )
	{
		action->setCheckable( true );
	}
	
	pbOptions->setMenu( menuOptions );
	
	// codecs
	QStringList codecs;
	foreach ( const QString& codec, QTextCodec::availableCodecs() )
	{
		codecs << codec;
	}
	codecs.sort();
	cbCodec->addItems( codecs );
	
	// connections
	connect( groupMode, SIGNAL( triggered( QAction* ) ), this, SLOT( groupMode_triggered( QAction* ) ) );
	
	setMode( SearchAndReplaceV2::ModeSearch );
}

SearchWidget::~SearchWidget()
{
}

SearchAndReplaceV2::Mode SearchWidget::mode() const
{
	return mMode;
}

void SearchWidget::setMode( SearchAndReplaceV2::Mode mode )
{
	mMode = mode;
	mModeActions[ mMode ]->setChecked( true );
	
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
		case SearchAndReplaceV2::ModeSearchProject:
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
		case SearchAndReplaceV2::ModeReplaceProject:
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
					case SearchAndReplaceV2::ModeSearchProject:
					case SearchAndReplaceV2::ModeSearchOpenedFiles:
						pbSearch->click();
						break;
					case SearchAndReplaceV2::ModeReplace:
						pbReplace->click();
						break;
					case SearchAndReplaceV2::ModeReplaceDirectory:
					case SearchAndReplaceV2::ModeReplaceProject:
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
		mProperties.mask << part;
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

void SearchWidget::groupMode_triggered( QAction* action )
{
	setMode( mModeActions.key( action ) );
}

void SearchWidget::on_pbPrevious_clicked()
{
	initializeProperties();
}

void SearchWidget::on_pbNext_clicked()
{
	initializeProperties();
}

void SearchWidget::on_pbSearch_clicked()
{
	initializeProperties();
}

void SearchWidget::on_pbReplace_clicked()
{
	initializeProperties();
}

void SearchWidget::on_pbReplaceAll_clicked()
{
	initializeProperties();
}

void SearchWidget::on_pbReplaceChecked_clicked()
{
	initializeProperties();
}

void SearchWidget::on_pbBrowse_clicked()
{
}
