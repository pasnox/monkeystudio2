#include "UISettings.h"
#include "pSettings.h"
#include "UIEditTemplate.h"
#include "UIAddAbbreviation.h"
#include "pFileManager.h"
#include "pTemplatesManager.h"
#include "pAbbreviationsManager.h"
#include "pMonkeyStudio.h"

#include "qscilexerbash.h"
#include "qscilexerbatch.h"
#include "qscilexercmake.h"
#include "qscilexercpp.h"
#include "qscilexercsharp.h"
#include "qscilexercss.h"
#include "qscilexerd.h"
#include "qscilexerdiff.h"
#include "qscilexerhtml.h"
#include "qscilexeridl.h"
#include "qscilexerjava.h"
#include "qscilexerjavascript.h"
#include "qscilexerlua.h"
#include "qscilexermakefile.h"
#include "qscilexerperl.h"
#include "qscilexerpov.h"
#include "qscilexerproperties.h"
#include "qscilexerpython.h"
#include "qscilexerruby.h"
#include "qscilexersql.h"
#include "qscilexertex.h"
#include "qscilexervhdl.h"

#include <QButtonGroup>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QTextCodec>
#include <QDir>

const QString SettingsPath = "Settings";

UISettings::UISettings( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	twMenu->topLevelItem( 2 )->setExpanded( true );
	twMenu->setCurrentItem( twMenu->topLevelItem( 0 ) );

	QStringList l;

	foreach ( QString s, pMonkeyStudio::availableLanguages() )
			mLexers[s] = pMonkeyStudio::lexerForLanguage( s );

	// designer
	bgUIDesigner = new QButtonGroup( gbUIDesignerIntegration );
	bgUIDesigner->addButton( rbUseEmbeddedUIDesigner, pMonkeyStudio::uidmEmbedded );
	bgUIDesigner->addButton( rbRunQtDesigner, pMonkeyStudio::uidmExternal );

	// externalchanges
	bgExternalChanges = new QButtonGroup( gbOnExternalChanges );
	bgExternalChanges->addButton( rbDoNothing, pMonkeyStudio::ecmNothing );
	bgExternalChanges->addButton( rbAlertUser, pMonkeyStudio::ecmAlert );
	bgExternalChanges->addButton( rbReloadAutomatically, pMonkeyStudio::ecmReload );

	// resize column
	twTemplatesType->setColumnWidth( 0, 100 );
	twTemplatesType->setColumnWidth( 1, 100 );

	// loads text codecs
	cbDefaultEncoding->addItems( pMonkeyStudio::availableTextCodecs() );

	// auto completion source
	bgAutoCompletionSource = new QButtonGroup( gbAutoCompletionSource );
	bgAutoCompletionSource->addButton( rbAcsDocument, QsciScintilla::AcsDocument );
	bgAutoCompletionSource->addButton( rbAcsAPIs, QsciScintilla::AcsAPIs );
	bgAutoCompletionSource->addButton( rbAcsAll, QsciScintilla::AcsAll );

	// calltips style
	bgCallTipsStyle = new QButtonGroup( gbCalltipsEnabled );
	bgCallTipsStyle->addButton( rbCallTipsNoContext, QsciScintilla::CallTipsNoContext );
	bgCallTipsStyle->addButton( rbCallTipsNoAutoCompletionContext, QsciScintilla::CallTipsNoAutoCompletionContext );
	bgCallTipsStyle->addButton( rbCallTipsContext, QsciScintilla::CallTipsContext );

	// brace match
	bgBraceMatch = new QButtonGroup( gbBraceMatchingEnabled );
	bgBraceMatch->addButton( rbStrictBraceMatch, QsciScintilla::StrictBraceMatch );
	bgBraceMatch->addButton( rbSloppyBraceMatch, QsciScintilla::SloppyBraceMatch );

	// edge mode
	bgEdgeMode = new QButtonGroup( gbEdgeModeEnabled );
	bgEdgeMode->addButton( rbEdgeLine, QsciScintilla::EdgeLine );
	bgEdgeMode->addButton( rbEdgeBackground, QsciScintilla::EdgeBackground );

	// fold style
	bgFoldStyle = new QButtonGroup( gbFoldMarginEnabled );
	bgFoldStyle->addButton( rbPlainFoldStyle, QsciScintilla::PlainFoldStyle );
	bgFoldStyle->addButton( rbCircledTreeFoldStyle, QsciScintilla::CircledTreeFoldStyle );
	bgFoldStyle->addButton( rbCircledFoldStyle, QsciScintilla::CircledFoldStyle );
	bgFoldStyle->addButton( rbBoxedFoldStyle, QsciScintilla::BoxedFoldStyle );
	bgFoldStyle->addButton( rbBoxedTreeFoldStyle, QsciScintilla::BoxedTreeFoldStyle );

	// eol mode
	bgEolMode = new QButtonGroup( gbEolMode );
	bgEolMode->addButton( rbEolUnix, QsciScintilla::EolUnix );
	bgEolMode->addButton( rbEolMac, QsciScintilla::EolMac );
	bgEolMode->addButton( rbEolWindows, QsciScintilla::EolWindows );

	// whitespace visibility
	bgWhitespaceVisibility = new QButtonGroup( gbWhitespaceVisibilityEnabled );
	bgWhitespaceVisibility->addButton( rbWsVisible, QsciScintilla::WsVisible );
	bgWhitespaceVisibility->addButton( rbWsVisibleAfterIndent, QsciScintilla::WsVisibleAfterIndent );

	// wrap mode
	bgWrapMode = new QButtonGroup( gbWrapModeEnabled );
	bgWrapMode->addButton( rbWrapWord, QsciScintilla::WrapWord );
	bgWrapMode->addButton( rbWrapCharacter, QsciScintilla::WrapCharacter );

	// wrap visual flag
	bgStartWrapVisualFlag = new QButtonGroup( wStartWrapVisualFlags );
	bgStartWrapVisualFlag->addButton( rbStartWrapFlagByText, QsciScintilla::WrapFlagByText );
	bgStartWrapVisualFlag->addButton( rbStartWrapFlagByBorder, QsciScintilla::WrapFlagByBorder );
	bgEndWrapVisualFlag = new QButtonGroup( wEndWrapVisualFlags );
	bgEndWrapVisualFlag->addButton( rbEndWrapFlagByText, QsciScintilla::WrapFlagByText );
	bgEndWrapVisualFlag->addButton( rbEndWrapFlagByBorder, QsciScintilla::WrapFlagByBorder );

	// fill lexers combo
	cbSourceAPIsLanguages->addItems( pMonkeyStudio::availableLanguages() );
	cbLexersAssociationsLanguages->addItems( pMonkeyStudio::availableLanguages() );
	cbLexersHighlightingLanguages->addItems( pMonkeyStudio::availableLanguages() );

	// resize column
	twLexersAssociations->setColumnWidth( 0, 200 );

	// python indentation warning
	cbLexersHighlightingIndentationWarning->addItem( tr( "No warning" ), QsciLexerPython::NoWarning );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Inconsistent" ), QsciLexerPython::Inconsistent );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Tabs after spaces" ), QsciLexerPython::TabsAfterSpaces );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Spaces" ), QsciLexerPython::Spaces );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Tabs" ), QsciLexerPython::Tabs );

	// resize column
	twAbbreviations->setColumnWidth( 0, 100 );
	twAbbreviations->setColumnWidth( 1, 180 );

	// read settings
	loadSettings();

	// connections
	// Colours Button
	connect( tbSelectionBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbSelectionForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbDefaultDocumentPen, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbDefaultDocumentPaper, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCalltipsBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCalltipsForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCalltipsHighlight, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbIndentationGuidesBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbIndentationGuidesForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMatchedBraceForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMatchedBraceBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbUnmatchedBraceForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbUnmatchedBraceBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbEdgeColor, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCaretLineBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCaretForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbFoldMarginForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbFoldMarginBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMarginsForeground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbMarginsBackground, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	// event filter
	// margin font
	connect( tbMarginsFont, SIGNAL( clicked() ), this, SLOT( tbFonts_clicked() ) );
	// lexer elements highlighting
	connect( pbLexersHighlightingForeground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingBackground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingFont, SIGNAL( clicked() ), this, SLOT( lexersHighlightingFont_clicked() ) );
	connect( pbLexersHighlightingAllForeground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingAllBackground, SIGNAL( clicked() ), this, SLOT( lexersHighlightingColour_clicked() ) );
	connect( pbLexersHighlightingAllFont, SIGNAL( clicked() ), this, SLOT( lexersHighlightingFont_clicked() ) );
	foreach ( QCheckBox* cb, gbLexersHighlightingElements->findChildren<QCheckBox*>() )
		if ( cb != cbLexersHighlightingFillEol )
			connect( cb, SIGNAL( clicked( bool ) ), this, SLOT( cbLexersHighlightingProperties_clicked( bool ) ) );
	// apply button
	connect( dbbButtons->button( QDialogButtonBox::Apply ), SIGNAL( clicked() ), this, SLOT( apply() ) );

	// resize to minimum size
	resize( minimumSizeHint() );
}

void UISettings::loadSettings()
{
	pSettings* s = pSettings::instance();
	QString sp;

	// General
	cbRestoreProjectsOnStartup->setChecked( pMonkeyStudio::restoreProjectsOnStartup() );
	leDefaultProjectsDirectory->setText( pMonkeyStudio::defaultProjectsDirectory() );
	bgUIDesigner->button( pMonkeyStudio::uiDesignerMode() )->setChecked( true );
	bgExternalChanges->button( pMonkeyStudio::externalchanges() )->setChecked( true );
	cbSaveSession->setChecked( pMonkeyStudio::saveSessionOnClose() );
	cbRestoreSession->setChecked( pMonkeyStudio::restoreSessionOnStartup() );

	// File Templates
	leTemplatesPath->setText( pTemplatesManager::templatesPath() );
	foreach ( pTemplate t, pTemplatesManager::availableTemplates() )
	{
		QTreeWidgetItem* it = new QTreeWidgetItem( twTemplatesType );
		it->setText( 0, t.Language );
		it->setText( 1, t.Name );
		it->setText( 2, t.Description );
		it->setData( 0, Qt::UserRole, t.Icon );
		it->setData( 0, Qt::UserRole +1, t.FileNames );
		it->setData( 0, Qt::UserRole +2, t.Type );
		it->setText( 3, pTemplate::stringForType( t.Type ) );
		it->setIcon( 0, QIcon( t.Icon ) );
	}
	teHeader->setPlainText( pTemplatesManager::templatesHeader() );

	// Editor
	//  General
	cbAutoSyntaxCheck->setChecked( pMonkeyStudio::autoSyntaxCheck() );
	cbConvertTabsUponOpen->setChecked( pMonkeyStudio::convertTabsUponOpen() );
	cbCreateBackupUponOpen->setChecked( pMonkeyStudio::createBackupUponOpen() );
	cbAutoEolConversion->setChecked( pMonkeyStudio::autoEolConversion() );
	cbDefaultEncoding->setCurrentIndex( cbDefaultEncoding->findText( pMonkeyStudio::defaultEncoding() ) );
	tbSelectionBackground->setIcon( colourizedPixmap( pMonkeyStudio::selectionBackgroundColor() ) );
	tbSelectionBackground->setToolTip( pMonkeyStudio::selectionBackgroundColor().name() );
	tbSelectionForeground->setIcon( colourizedPixmap( pMonkeyStudio::selectionForegroundColor() ) );
	tbSelectionForeground->setToolTip( pMonkeyStudio::selectionForegroundColor().name() );
	gbDefaultDocumentColours->setChecked( pMonkeyStudio::defaultDocumentColours() );
	tbDefaultDocumentPen->setIcon( colourizedPixmap( pMonkeyStudio::defaultDocumentPen() ) );
	tbDefaultDocumentPen->setToolTip( pMonkeyStudio::defaultDocumentPen().name() );
	tbDefaultDocumentPaper->setIcon( colourizedPixmap( pMonkeyStudio::defaultDocumentPaper() ) );
	tbDefaultDocumentPaper->setToolTip( pMonkeyStudio::defaultDocumentPaper().name() );
	//  Auto Completion
	gbAutoCompletionEnabled->setChecked( pMonkeyStudio::autoCompletionSource() != QsciScintilla::AcsNone );
	cbAutoCompletionCaseSensitivity->setChecked( pMonkeyStudio::autoCompletionCaseSensitivity() );
	cbAutoCompletionReplaceWord->setChecked( pMonkeyStudio::autoCompletionReplaceWord() );
	cbAutoCompletionShowSingle->setChecked( pMonkeyStudio::autoCompletionShowSingle() );
	sAutoCompletionThreshold->setValue( pMonkeyStudio::autoCompletionThreshold() );
	if ( bgAutoCompletionSource->button( pMonkeyStudio::autoCompletionSource() ) )
		bgAutoCompletionSource->button( pMonkeyStudio::autoCompletionSource() )->setChecked( true );
	//  Call Tips
	gbCalltipsEnabled->setChecked( pMonkeyStudio::callTipsStyle() != QsciScintilla::CallTipsNone );
	sCallTipsVisible->setValue( pMonkeyStudio::callTipsVisible() );
	if ( bgCallTipsStyle->button( pMonkeyStudio::callTipsStyle() ) )
		bgCallTipsStyle->button( pMonkeyStudio::callTipsStyle() )->setChecked( true );
	tbCalltipsBackground->setIcon( colourizedPixmap( pMonkeyStudio::callTipsBackgroundColor() ) );
	tbCalltipsBackground->setToolTip( pMonkeyStudio::callTipsBackgroundColor().name() );
	tbCalltipsForeground->setIcon( colourizedPixmap( pMonkeyStudio::callTipsForegroundColor() ) );
	tbCalltipsForeground->setToolTip( pMonkeyStudio::callTipsForegroundColor().name() );
	tbCalltipsHighlight->setIcon( colourizedPixmap( pMonkeyStudio::callTipsHighlightColor() ) );
	tbCalltipsHighlight->setToolTip( pMonkeyStudio::callTipsHighlightColor().name() );
	//  Indentation
	cbAutoIndent->setChecked( pMonkeyStudio::autoIndent() );
	cbBackspaceUnindents->setChecked( pMonkeyStudio::backspaceUnindents() );
	cbIndentationGuides->setChecked( pMonkeyStudio::indentationGuides() );
	cbIndentationUseTabs->setChecked( pMonkeyStudio::indentationsUseTabs() );
	cbTabIndents->setChecked( pMonkeyStudio::tabIndents() );
	sIndentationTabWidth->setValue( pMonkeyStudio::tabWidth() );
	sIndentationWidth->setValue( pMonkeyStudio::indentationWidth() );
	tbIndentationGuidesBackground->setIcon( colourizedPixmap( pMonkeyStudio::indentationGuidesBackgroundColor() ) );
	tbIndentationGuidesBackground->setToolTip( pMonkeyStudio::indentationGuidesBackgroundColor().name() );
	tbIndentationGuidesForeground->setIcon( colourizedPixmap( pMonkeyStudio::indentationGuidesForegroundColor() ) );
	tbIndentationGuidesForeground->setToolTip( pMonkeyStudio::indentationGuidesForegroundColor().name() );
	//  Brace Matching
	gbBraceMatchingEnabled->setChecked( pMonkeyStudio::braceMatching() != QsciScintilla::NoBraceMatch );
	if ( bgBraceMatch->button( pMonkeyStudio::braceMatching() ) )
		bgBraceMatch->button( pMonkeyStudio::braceMatching() )->setChecked( true );
	tbMatchedBraceForeground->setIcon( colourizedPixmap( pMonkeyStudio::matchedBraceForegroundColor() ) );
	tbMatchedBraceForeground->setToolTip( pMonkeyStudio::matchedBraceForegroundColor().name() );
	tbMatchedBraceBackground->setIcon( colourizedPixmap( pMonkeyStudio::matchedBraceBackgroundColor() ) );
	tbMatchedBraceBackground->setToolTip( pMonkeyStudio::matchedBraceBackgroundColor().name() );
	tbUnmatchedBraceBackground->setIcon( colourizedPixmap( pMonkeyStudio::unmatchedBraceBackgroundColor() ) );
	tbUnmatchedBraceBackground->setToolTip( pMonkeyStudio::unmatchedBraceBackgroundColor().name() );
	tbUnmatchedBraceForeground->setIcon( colourizedPixmap( pMonkeyStudio::unmatchedBraceForegroundColor() ) );
	tbUnmatchedBraceForeground->setToolTip( pMonkeyStudio::unmatchedBraceForegroundColor().name() );
	//  Edge Mode
	gbEdgeModeEnabled->setChecked( pMonkeyStudio::edgeMode() != QsciScintilla::EdgeNone );
	if ( bgEdgeMode->button( pMonkeyStudio::edgeMode() ) )
		bgEdgeMode->button( pMonkeyStudio::edgeMode() )->setChecked( true );
	sEdgeColumnNumber->setValue( pMonkeyStudio::edgeColumn() );
	tbEdgeColor->setIcon( colourizedPixmap( pMonkeyStudio::edgeColor() ) );
	tbEdgeColor->setToolTip( pMonkeyStudio::edgeColor().name() );
	//  Caret
	gbCaretLineVisible->setChecked( pMonkeyStudio::caretLineVisible() );
	tbCaretLineBackground->setIcon( colourizedPixmap( pMonkeyStudio::caretLineBackgroundColor() ) );
	tbCaretLineBackground->setToolTip( pMonkeyStudio::caretLineBackgroundColor().name() );
	tbCaretForeground->setIcon( colourizedPixmap( pMonkeyStudio::caretForegroundColor() ) );
	tbCaretForeground->setToolTip( pMonkeyStudio::caretForegroundColor().name() );
	sCaretWidth->setValue( pMonkeyStudio::caretWidth() );
	//  Margins
	gbLineNumbersMarginEnabled->setChecked( pMonkeyStudio::lineNumbersMarginEnabled() );
	sLineNumbersMarginWidth->setValue( pMonkeyStudio::lineNumbersMarginWidth() );
	cbLineNumbersMarginAutoWidth->setChecked( pMonkeyStudio::lineNumbersMarginAutoWidth() );
	gbFoldMarginEnabled->setChecked( pMonkeyStudio::folding() != QsciScintilla::NoFoldStyle );
	if ( bgFoldStyle->button( pMonkeyStudio::folding() ) )
		bgFoldStyle->button( pMonkeyStudio::folding() )->setChecked( true );
	tbFoldMarginForeground->setIcon( colourizedPixmap( pMonkeyStudio::foldMarginForegroundColor() ) );
	tbFoldMarginForeground->setToolTip( pMonkeyStudio::foldMarginForegroundColor().name() );
	tbFoldMarginBackground->setIcon( colourizedPixmap( pMonkeyStudio::foldMarginBackgroundColor() ) );
	tbFoldMarginBackground->setToolTip( pMonkeyStudio::foldMarginBackgroundColor().name() );
	gbMarginsEnabled->setChecked( pMonkeyStudio::marginsEnabled() );
	tbMarginsForeground->setIcon( colourizedPixmap( pMonkeyStudio::marginsForegroundColor() ) );
	tbMarginsForeground->setToolTip( pMonkeyStudio::marginsForegroundColor().name() );
	tbMarginsBackground->setIcon( colourizedPixmap( pMonkeyStudio::marginsBackgroundColor() ) );
	tbMarginsBackground->setToolTip( pMonkeyStudio::marginsBackgroundColor().name() );
	tbMarginsFont->setFont( pMonkeyStudio::marginsFont() );
	//  Special Characters
	bgEolMode->button( pMonkeyStudio::eolMode() )->setChecked( true );
	cbEolVisibility->setChecked( pMonkeyStudio::eolVisibility() );
	gbWhitespaceVisibilityEnabled->setChecked( pMonkeyStudio::whitespaceVisibility() != QsciScintilla::WsInvisible );
	if ( bgWhitespaceVisibility->button( pMonkeyStudio::whitespaceVisibility() ) )
		bgWhitespaceVisibility->button( pMonkeyStudio::whitespaceVisibility() )->setChecked( true );
	gbWrapModeEnabled->setChecked( pMonkeyStudio::wrapMode() != QsciScintilla::WrapNone );
	if ( bgWrapMode->button( pMonkeyStudio::wrapMode() ) )
		bgWrapMode->button( pMonkeyStudio::wrapMode() )->setChecked( true );
	gbWrapVisualFlagsEnabled->setChecked( pMonkeyStudio::wrapVisualFlagsEnabled() );
	if ( bgStartWrapVisualFlag->button( pMonkeyStudio::startWrapVisualFlag() ) )
		bgStartWrapVisualFlag->button( pMonkeyStudio::startWrapVisualFlag() )->setChecked( true );
	if ( bgEndWrapVisualFlag->button( pMonkeyStudio::endWrapVisualFlag() ) )
		bgEndWrapVisualFlag->button( pMonkeyStudio::endWrapVisualFlag() )->setChecked( true );
	sWrappedLineIndentWidth->setValue( pMonkeyStudio::wrappedLineIndentWidth() );
	// Source APIs
	for ( int i = 0; i < cbSourceAPIsLanguages->count(); i++ )
		cbSourceAPIsLanguages->setItemData( i, s->value( "SourceAPIs/" +cbSourceAPIsLanguages->itemText( i ) ).toStringList() );
	if ( cbSourceAPIsLanguages->count() > 0 )
		cbSourceAPIsLanguages->setCurrentIndex( 0 );
	//  Lexers Associations
	QHash<QString, QStringList> l = pMonkeyStudio::availableSuffixes();
	foreach ( QString k, l.keys() )
	{
		foreach ( QString e, l.value( k ) )
		{
			QTreeWidgetItem* it = new QTreeWidgetItem( twLexersAssociations );
			it->setText( 0, e );
			it->setText( 1, k );
		}
	}
	//  Lexers Highlighting
	foreach ( QsciLexer* l, mLexers )
		l->readSettings( *pSettings::instance(), qPrintable( pMonkeyStudio::scintillaSettingsPath() ) );
	
	if ( cbLexersHighlightingLanguages->count() )
		on_cbLexersHighlightingLanguages_currentIndexChanged( cbLexersHighlightingLanguages->itemText( 0 ) );

	//  Abbreviations
	foreach ( pAbbreviation a, pAbbreviationsManager::availableAbbreviations() )
	{
		QTreeWidgetItem* it = new QTreeWidgetItem( twAbbreviations );
		it->setText( 0, a.Template );
		it->setText( 1, a.Description );
		it->setText( 2, a.Language );
		it->setData( 0, Qt::UserRole, a.Code );
	}

	// Tools
}

void UISettings::saveSettings()
{
	pSettings* s = pSettings::instance();
	QString sp;

	// General
	pMonkeyStudio::setRestoreProjectsOnStartup( cbRestoreProjectsOnStartup->isChecked() );
	pMonkeyStudio::setDefaultProjectsDirectory( leDefaultProjectsDirectory->text() );
	pMonkeyStudio::setUIDesignerMode( (pMonkeyStudio::UIDesignerMode)bgUIDesigner->checkedId() );
	pMonkeyStudio::setExternalChanges( (pMonkeyStudio::ExternalChangesMode)bgExternalChanges->checkedId() );
	pMonkeyStudio::setSaveSessionOnClose( cbSaveSession->isChecked() );
	pMonkeyStudio::setRestoreSessionOnStartup( cbRestoreSession->isChecked() );

	// File Templates
	sp = "Templates";
	// remove key
	s->remove( sp );
	// default templates path
	pTemplatesManager::setTemplatesPath( leTemplatesPath->text() );
	// tokenize items, need do separate as qsettings is not thread safe
	for ( int i = 0; i < twTemplatesType->topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* it = twTemplatesType->topLevelItem( i );
		it->setData( 0, Qt::UserRole, pTemplatesManager::tokenize( it->data( 0, Qt::UserRole ).toString() ) );
		it->setData( 0, Qt::UserRole +1, pTemplatesManager::tokenize( it->data( 0, Qt::UserRole +1 ).toStringList() ) );
	}
	// write new ones
	s->beginWriteArray( sp );
	for ( int i = 0; i < twTemplatesType->topLevelItemCount(); i++ )
	{
		s->setArrayIndex( i );
		QTreeWidgetItem* it = twTemplatesType->topLevelItem( i );
		s->setValue( "Language", it->text( 0 ) );
		s->setValue( "Name", it->text( 1 ) );
		s->setValue( "Description", it->text( 2 ) );
		s->setValue( "Icon", it->data( 0, Qt::UserRole ).toString() );
		s->setValue( "FileNames", it->data( 0, Qt::UserRole +1 ).toStringList() );
		s->setValue( "Type", it->data( 0, Qt::UserRole +2 ).toInt() );
	}
	s->endArray();
	// templates header
	pTemplatesManager::setTemplatesHeader( teHeader->toPlainText() );

	// Editor
	//  General
	pMonkeyStudio::setAutoSyntaxCheck( cbAutoSyntaxCheck->isChecked() );
	pMonkeyStudio::setConvertTabsUponOpen( cbConvertTabsUponOpen->isChecked() );
	pMonkeyStudio::setCreateBackupUponOpen( cbCreateBackupUponOpen->isChecked() ) ;
	pMonkeyStudio::setAutoEolConversion( cbAutoEolConversion->isChecked() );
	pMonkeyStudio::setDefaultEncoding( cbDefaultEncoding->currentText() );
	pMonkeyStudio::setSelectionBackgroundColor( iconBackgroundColor( tbSelectionBackground->icon() ) );
	pMonkeyStudio::setSelectionForegroundColor( iconBackgroundColor( tbSelectionForeground->icon() ) );
	pMonkeyStudio::setDefaultDocumentColours( gbDefaultDocumentColours->isChecked() );
	pMonkeyStudio::setDefaultDocumentPen( iconBackgroundColor( tbDefaultDocumentPen->icon() ) );
	pMonkeyStudio::setDefaultDocumentPaper( iconBackgroundColor( tbDefaultDocumentPaper->icon() ) );
	//  Auto Completion
	pMonkeyStudio::setAutoCompletionSource( QsciScintilla::AcsNone );
	if ( gbAutoCompletionEnabled->isChecked() )
		pMonkeyStudio::setAutoCompletionSource( (QsciScintilla::AutoCompletionSource)bgAutoCompletionSource->checkedId() );
	pMonkeyStudio::setAutoCompletionCaseSensitivity( cbAutoCompletionCaseSensitivity->isChecked() );
	pMonkeyStudio::setAutoCompletionReplaceWord( cbAutoCompletionReplaceWord->isChecked() );
	pMonkeyStudio::setAutoCompletionShowSingle( cbAutoCompletionShowSingle->isChecked() );
	pMonkeyStudio::setAutoCompletionThreshold( sAutoCompletionThreshold->value() );
	//  Call Tips
	pMonkeyStudio::setCallTipsStyle( QsciScintilla::CallTipsNone );
	if ( gbCalltipsEnabled->isChecked() )
		pMonkeyStudio::setCallTipsStyle( (QsciScintilla::CallTipsStyle)bgCallTipsStyle->checkedId() );
	pMonkeyStudio::setCallTipsVisible( sCallTipsVisible->value() );
	pMonkeyStudio::setCallTipsBackgroundColor( iconBackgroundColor( tbCalltipsBackground->icon() ) );
	pMonkeyStudio::setCallTipsForegroundColor( iconBackgroundColor( tbCalltipsForeground->icon() ) );
	pMonkeyStudio::setCallTipsHighlightColor( iconBackgroundColor( tbCalltipsHighlight->icon() ) );
	//  Indentation
	pMonkeyStudio::setAutoIndent( cbAutoIndent->isChecked()  );
	pMonkeyStudio::setBackspaceUnindents( cbBackspaceUnindents->isChecked() );
	pMonkeyStudio::setIndentationGuides( cbIndentationGuides->isChecked() );
	pMonkeyStudio::setIndentationsUseTabs( cbIndentationUseTabs->isChecked() );
	pMonkeyStudio::setTabIndents( cbTabIndents->isChecked() );
	pMonkeyStudio::setTabWidth( sIndentationTabWidth->value() );
	pMonkeyStudio::setIndentationWidth( sIndentationWidth->value() );
	pMonkeyStudio::setIndentationGuidesBackgroundColor( iconBackgroundColor( tbIndentationGuidesBackground->icon() ) );
	pMonkeyStudio::setIndentationGuidesForegroundColor( iconBackgroundColor( tbIndentationGuidesForeground->icon() ) );
	//  Brace Matching
	pMonkeyStudio::setBraceMatching( QsciScintilla::NoBraceMatch );
	if ( gbBraceMatchingEnabled->isChecked() )
		pMonkeyStudio::setBraceMatching( (QsciScintilla::BraceMatch)bgBraceMatch->checkedId() );
	pMonkeyStudio::setMatchedBraceBackgroundColor( iconBackgroundColor( tbMatchedBraceBackground->icon() ) );
	pMonkeyStudio::setMatchedBraceForegroundColor( iconBackgroundColor( tbMatchedBraceForeground->icon() ) );
	pMonkeyStudio::setUnmatchedBraceBackgroundColor( iconBackgroundColor( tbUnmatchedBraceBackground->icon() ) );
	pMonkeyStudio::setUnmatchedBraceForegroundColor( iconBackgroundColor( tbUnmatchedBraceForeground->icon() ) );
	//  Edge Mode
	pMonkeyStudio::setEdgeMode( QsciScintilla::EdgeNone );
	if ( gbEdgeModeEnabled->isChecked() )
		pMonkeyStudio::setEdgeMode( (QsciScintilla::EdgeMode)bgEdgeMode->checkedId() );
	pMonkeyStudio::setEdgeColumn( sEdgeColumnNumber->value() );
	pMonkeyStudio::setEdgeColor( iconBackgroundColor( tbEdgeColor->icon() ) );
	//  Caret
	pMonkeyStudio::setCaretLineVisible( gbCaretLineVisible->isChecked() );
	pMonkeyStudio::setCaretLineBackgroundColor( iconBackgroundColor( tbCaretLineBackground->icon() ) );
	pMonkeyStudio::setCaretForegroundColor( iconBackgroundColor( tbCaretForeground->icon() ) );
	pMonkeyStudio::setCaretWidth( sCaretWidth->value() );
	//  Margins
	pMonkeyStudio::setLineNumbersMarginEnabled( gbLineNumbersMarginEnabled->isChecked() );
	pMonkeyStudio::setLineNumbersMarginWidth( sLineNumbersMarginWidth->value() );
	pMonkeyStudio::setLineNumbersMarginAutoWidth( cbLineNumbersMarginAutoWidth->isChecked() );
	pMonkeyStudio::setFolding( QsciScintilla::NoFoldStyle );
	if ( gbFoldMarginEnabled->isChecked() )
		pMonkeyStudio::setFolding( (QsciScintilla::FoldStyle)bgFoldStyle->checkedId() );
	pMonkeyStudio::setFoldMarginForegroundColor( iconBackgroundColor( tbFoldMarginForeground->icon() ) );
	pMonkeyStudio::setFoldMarginBackgroundColor( iconBackgroundColor( tbFoldMarginBackground->icon() ) );
	pMonkeyStudio::setMarginsEnabled( gbMarginsEnabled->isChecked() );
	pMonkeyStudio::setMarginsForegroundColor( iconBackgroundColor( tbMarginsForeground->icon() ) );
	pMonkeyStudio::setMarginsBackgroundColor( iconBackgroundColor( tbMarginsBackground->icon() ) );
	pMonkeyStudio::setMarginsFont( tbMarginsFont->font() );
	//  Special Characters
	pMonkeyStudio::setEolMode( (QsciScintilla::EolMode)bgEolMode->checkedId() );
	pMonkeyStudio::setEolVisibility( cbEolVisibility->isChecked() );
	pMonkeyStudio::setWhitespaceVisibility( QsciScintilla::WsInvisible );
	if ( gbWhitespaceVisibilityEnabled->isChecked() )
		pMonkeyStudio::setWhitespaceVisibility( (QsciScintilla::WhitespaceVisibility)bgWhitespaceVisibility->checkedId() );
	pMonkeyStudio::setWrapMode( QsciScintilla::WrapNone );
	if ( gbWrapModeEnabled->isChecked() )
		pMonkeyStudio::setWrapMode( (QsciScintilla::WrapMode)bgWrapMode->checkedId() );
	pMonkeyStudio::setWrapVisualFlagsEnabled( gbWrapVisualFlagsEnabled->isChecked() );
	pMonkeyStudio::setStartWrapVisualFlag( QsciScintilla::WrapFlagNone );
	if ( gbWrapVisualFlagsEnabled->isChecked() )
		pMonkeyStudio::setStartWrapVisualFlag( (QsciScintilla::WrapVisualFlag)bgStartWrapVisualFlag->checkedId() );
	pMonkeyStudio::setEndWrapVisualFlag( QsciScintilla::WrapFlagNone );
	if ( gbWrapVisualFlagsEnabled->isChecked() )
		pMonkeyStudio::setEndWrapVisualFlag( (QsciScintilla::WrapVisualFlag)bgEndWrapVisualFlag->checkedId() );
	pMonkeyStudio::setWrappedLineIndentWidth( sWrappedLineIndentWidth->value() );
	// Source APIs
	sp = "SourceAPIs/";
	for ( int i = 0; i < cbSourceAPIsLanguages->count(); i++ )
		s->setValue( sp +cbSourceAPIsLanguages->itemText( i ), cbSourceAPIsLanguages->itemData( i ).toStringList() );
	//  Lexers Associations
	sp = "LexersAssociations/";
	// remove old associations
	s->remove( sp );
	// write new ones
	for ( int i = 0; i < twLexersAssociations->topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* it = twLexersAssociations->topLevelItem( i );
		s->setValue( sp +it->text( 0 ), it->text( 1 ) );
	}
	//  Lexers Highlighting
	foreach ( QsciLexer* l, mLexers )
	{
		l->setDefaultPaper( iconBackgroundColor( tbDefaultDocumentPaper->icon() ) );
		l->setDefaultColor( iconBackgroundColor( tbDefaultDocumentPen->icon() ) );
		l->writeSettings( *pSettings::instance(), qPrintable( pMonkeyStudio::scintillaSettingsPath() ) );
	}

	//  Abbreviations
	sp = "Abbreviations";
	// remove key
	s->remove( sp );
	// write new ones
	s->beginWriteArray( sp );
	for ( int i = 0; i < twAbbreviations->topLevelItemCount(); i++ )
	{
		s->setArrayIndex( i );
		QTreeWidgetItem* it = twAbbreviations->topLevelItem( i );

		s->setValue( "Template", it->text( 0 ).trimmed() );
		s->setValue( "Description", it->text( 1 ).trimmed() );
		s->setValue( "Language", it->text( 2 ) );
		s->setValue( "Code", it->data( 0, Qt::UserRole ).toString() );
	}
	s->endArray();

	// Tools
}

QPixmap UISettings::colourizedPixmap( const QColor& c ) const
{
	QPixmap p( 48, 16 );
	p.fill( c );
	return p;
}

QColor UISettings::iconBackgroundColor( const QIcon& i ) const
{
	return QColor( i.pixmap( QSize( 5, 5 ) ).toImage().pixel( QPoint( 0, 0 ) ) );
}

void UISettings::on_twMenu_itemSelectionChanged()
{
	// get item
	QTreeWidgetItem* it = twMenu->selectedItems().value( 0 );

	if ( it )
	{
		lInformations->setText( it->text( 0 ) );
		int i = twMenu->indexOfTopLevelItem( it );
		if ( !it->parent() )
		{
			switch ( i )
			{
				case 0:
				case 1:
				case 2:
					swPages->setCurrentIndex( i );
					break;
				default:
					swPages->setCurrentIndex( i +11 );
					break;
			}
		}
		else
			swPages->setCurrentIndex( it->parent()->indexOfChild( it ) +2 );
	}
}

void UISettings::on_tbDefaultProjectsDirectory_clicked()
{
	QString s = QFileDialog::getExistingDirectory( window(), tr( "Select default projects directory" ), leDefaultProjectsDirectory->text() );
	if ( !s.isNull() )
		leDefaultProjectsDirectory->setText( s );
}

void UISettings::on_tbTemplatesPath_clicked()
{
	const QString d = pMonkeyStudio::unTokenizeHome( leTemplatesPath->text() );
	QString s = pMonkeyStudio::getExistingDirectory( tr( "Select default templates directory" ), d, window() );
	if ( !s.isNull() )
	{
		if ( s.endsWith( "/" ) )
			s.chop( 1 );
		leTemplatesPath->setText( pMonkeyStudio::tokenizeHome( s ) );
		// update existing items
		for ( int i = 0; i < twTemplatesType->topLevelItemCount(); i++ )
		{
			QTreeWidgetItem* it = twTemplatesType->topLevelItem( i );
			// icon file
			it->setData( 0, Qt::UserRole, it->data( 0, Qt::UserRole ).toString().replace( d, s ) );
			it->setData( 0, Qt::UserRole +1, it->data( 0, Qt::UserRole +1 ).toStringList().replaceInStrings( d, s ) );
		}
	}
}

void UISettings::on_pbAddTemplateType_clicked()
{
	UIEditTemplate::edit( twTemplatesType, 0 );
}

void UISettings::on_pbEditTemplateType_clicked()
{
	UIEditTemplate::edit( twTemplatesType, twTemplatesType->selectedItems().value( 0 ) );
}

void UISettings::on_pbRemoveTemplateType_clicked()
{
	delete twTemplatesType->selectedItems().value( 0 );
}

void UISettings::on_pbEditTemplate_clicked()
{
	// get item
	QTreeWidgetItem* it = twTemplatesType->selectedItems().value( 0 );
	// open template file
	if ( it )
		foreach ( QString s, it->data( 0, Qt::UserRole +1 ).toStringList() )
			pFileManager::instance()->openFile( s );
}

void UISettings::tbColours_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	QColor c = QColorDialog::getColor( iconBackgroundColor( tb->icon() ), window() );
	if ( c.isValid() )
	{
		tb->setIcon( colourizedPixmap( c ) );
		tb->setToolTip( c.name() );
	}
}

void UISettings::tbFonts_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	bool b;
	QFont f = QFontDialog::getFont( &b, tb->font(), window() );
	if ( b )
		tb->setFont( f );
}

void UISettings::cbSourceAPIsLanguages_beforeChanged( int i )
{
	if ( i == cbSourceAPIsLanguages->currentIndex() )
	{
		QStringList l;
		for ( int j = 0; j < lwSourceAPIs->count(); j++ )
			l << lwSourceAPIs->item( j )->text();
		cbSourceAPIsLanguages->setItemData( i, l );
	}
}

void UISettings::on_cbSourceAPIsLanguages_currentIndexChanged( int i )
{
	lwSourceAPIs->clear();
	lwSourceAPIs->addItems( cbSourceAPIsLanguages->itemData( i ).toStringList() );
}

void UISettings::on_pbSourceAPIsDelete_clicked()
{
	// get selected item
	QListWidgetItem* it = lwSourceAPIs->selectedItems().value( 0 );
	if ( it )
	{
		delete it;
		cbSourceAPIsLanguages_beforeChanged( cbSourceAPIsLanguages->currentIndex() );
	}
}

void UISettings::on_pbSourceAPIsAdd_clicked()
{
	QString s = leSourceAPIs->text();
	if ( !s.isEmpty() && lwSourceAPIs->findItems( s, Qt::MatchFixedString ).count() == 0 )
	{
		lwSourceAPIs->addItem( s );
		leSourceAPIs->clear();
		cbSourceAPIsLanguages_beforeChanged( cbSourceAPIsLanguages->currentIndex() );
	}
}

void UISettings::on_pbSourceAPIsBrowse_clicked()
{
	QString s = QFileDialog::getOpenFileName( window(), tr( "Select API file" ), QString::null, tr( "API Files (*.api);;All Files (*)" ) );
	if ( !s.isNull() )
		leSourceAPIs->setText( s );
}

void UISettings::on_twLexersAssociations_itemSelectionChanged()
{
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value( 0 );
	if ( it )
	{
		leLexersAssociationsFilenamePattern->setText( it->text( 0 ) );
		cbLexersAssociationsLanguages->setCurrentIndex( cbLexersAssociationsLanguages->findText( it->text( 1 ) ) );
	}
}

void UISettings::on_pbLexersAssociationsAddChange_clicked()
{
	QString f = leLexersAssociationsFilenamePattern->text();
	QString l = cbLexersAssociationsLanguages->currentText();
	if ( f.isEmpty() || l.isEmpty() )
		return;
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value( 0 );
	if ( !it || it->text( 0 ) != f )
	{
		// check if item with same parameters already exists
		QList<QTreeWidgetItem*> l = twLexersAssociations->findItems( f, Qt::MatchFixedString );
		if ( l.count() )
			it = l.at( 0 );
		else
			it = new QTreeWidgetItem( twLexersAssociations );
	}
	it->setText( 0, f );
	it->setText( 1, l );
	twLexersAssociations->setCurrentItem( 0 );
	twLexersAssociations->selectionModel()->clear();
	leLexersAssociationsFilenamePattern->clear();
	cbLexersAssociationsLanguages->setCurrentIndex( -1 );
}

void UISettings::on_pbLexersAssociationsDelete_clicked()
{
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value( 0 );
	if ( it )
	{
		delete it;
		twLexersAssociations->setCurrentItem( 0 );
		twLexersAssociations->selectionModel()->clear();
		leLexersAssociationsFilenamePattern->clear();
		cbLexersAssociationsLanguages->setCurrentIndex( -1 );
	}
}

void UISettings::on_cbLexersHighlightingLanguages_currentIndexChanged( const QString& s )
{
	QsciLexer* l = mLexers.value( s );
	lwLexersHighlightingElements->clear();
	for ( int i = 0; i < 128; i++ )
	{
		QString n = l->description( i );
		if ( !n.isEmpty() )
		{
			QListWidgetItem* it = new QListWidgetItem( lwLexersHighlightingElements );
			it->setText( n );
			it->setForeground( l->color( i ) );
			it->setBackground( l->paper( i ) );
			it->setFont( l->font( i ) );
			it->setData( Qt::UserRole, i );
		}
	}
	// value
	QVariant v;
	// fold comments
	v = pMonkeyStudio::lexerProperty( "foldComments", l );
	cbLexersHighlightingFoldComments->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldComments->setChecked( v.toBool() );
	// fold compact
	v = pMonkeyStudio::lexerProperty( "foldCompact", l );
	cbLexersHighlightingFoldCompact->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldCompact->setChecked( v.toBool() );
	// fold quotes
	v = pMonkeyStudio::lexerProperty( "foldQuotes", l );
	cbLexersHighlightingFoldQuotes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldQuotes->setChecked( v.toBool() );
	// fold directives
	v = pMonkeyStudio::lexerProperty( "foldDirectives", l );
	cbLexersHighlightingFoldDirectives->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldDirectives->setChecked( v.toBool() );
	// fold at begin
	v = pMonkeyStudio::lexerProperty( "foldAtBegin", l );
	cbLexersHighlightingFoldAtBegin->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtBegin->setChecked( v.toBool() );
	// fold at parenthesis
	v = pMonkeyStudio::lexerProperty( "foldAtParenthesis", l );
	cbLexersHighlightingFoldAtParenthesis->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtParenthesis->setChecked( v.toBool() );
	// fold at else
	v = pMonkeyStudio::lexerProperty( "foldAtElse", l );
	cbLexersHighlightingFoldAtElse->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtElse->setChecked( v.toBool() );
	// fold preprocessor
	v = pMonkeyStudio::lexerProperty( "foldPreprocessor", l );
	cbLexersHighlightingFoldPreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldPreprocessor->setChecked( v.toBool() );
	// style preprocessor
	v = pMonkeyStudio::lexerProperty( "stylePreprocessor", l );
	cbLexersHighlightingStylePreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingStylePreprocessor->setChecked( v.toBool() );
	// indent opening brace
	cbLexersHighlightingIndentOpeningBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiOpening );
	// indent closing brace
	cbLexersHighlightingIndentClosingBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiClosing );
	// case sensitive tags
	v = pMonkeyStudio::lexerProperty( "caseSensitiveTags", l );
	cbLexersHighlightingCaseSensitiveTags->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingCaseSensitiveTags->setChecked( v.toBool() );
	// backslash escapes
	v = pMonkeyStudio::lexerProperty( "backslashEscapes", l );
	cbLexersHighlightingBackslashEscapes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingBackslashEscapes->setChecked( v.toBool() );
	// indentation warning
	v = pMonkeyStudio::lexerProperty( "indentationWarning", l );
	lLexersHighlightingIndentationWarning->setVisible( v.isValid() );
	cbLexersHighlightingIndentationWarning->setVisible( lLexersHighlightingIndentationWarning->isVisible() );
	if ( v.isValid() )
		cbLexersHighlightingIndentationWarning->setCurrentIndex( cbLexersHighlightingIndentationWarning->findData( v.toInt() ) );
}

void UISettings::on_lwLexersHighlightingElements_itemSelectionChanged()
{
	QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );
	if ( it )
		cbLexersHighlightingFillEol->setChecked( mLexers.value( cbLexersHighlightingLanguages->currentText() )->eolFill( it->data( Qt::UserRole ).toInt() ) );
}

void UISettings::lexersHighlightingColour_clicked()
{
	// get sender
	QObject* o = sender();
	// color
	QColor c;
	// element colour
	if ( o == pbLexersHighlightingForeground || o == pbLexersHighlightingBackground )
	{
		// get item
		QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );
		// cancel if no item
		if ( !it )
			return;
		// get color
		c = QColorDialog::getColor( o == pbLexersHighlightingForeground ? it->foreground().color() : it->background().color(), window() );
		// apply color
		if ( c.isValid() )
		{
			if ( o == pbLexersHighlightingForeground )
			{
				it->setForeground( c );
				mLexers.value( cbLexersHighlightingLanguages->currentText() )->setColor( c, it->data( Qt::UserRole ).toInt() );
			}
			else if ( o == pbLexersHighlightingBackground )
			{
				it->setBackground( c );
				mLexers.value( cbLexersHighlightingLanguages->currentText() )->setPaper( c, it->data( Qt::UserRole ).toInt() );
			}
		}
	}
	// gobal color
	else if ( o == pbLexersHighlightingAllForeground || o == pbLexersHighlightingAllBackground )
	{
		// get lexer
		QsciLexer* l = mLexers.value( cbLexersHighlightingLanguages->currentText() );
		// get color
		c = QColorDialog::getColor( o == pbLexersHighlightingAllForeground ? l->color( -1 ) : l->paper( -1 ), window() );
		// apply
		if ( c.isValid() )
		{
			if ( o == pbLexersHighlightingAllForeground )
				l->setColor( c, -1 );
			else if ( o == pbLexersHighlightingAllBackground )
				l->setPaper( c, -1 );
			// refresh
			on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
		}
	}
}

void UISettings::lexersHighlightingFont_clicked()
{
	// get sender
	QObject* o = sender();
	// values
	bool b;
	QFont f;
	// element font
	if ( o == pbLexersHighlightingFont )
	{
		// get item
		QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );
		// cancel if no item
		if ( !it )
			return;
		// get font
		f = QFontDialog::getFont( &b, it->font(), window() );
		// apply
		if ( b )
		{
			it->setFont( f );
			mLexers.value( cbLexersHighlightingLanguages->currentText() )->setFont( f, it->data( Qt::UserRole ).toInt() );
		}
	}
	// global font
	else if ( o == pbLexersHighlightingAllFont )
	{
		// get lexer
		QsciLexer* l = mLexers.value( cbLexersHighlightingLanguages->currentText() );
		// get font
		f = QFontDialog::getFont( &b, l->font( -1 ), window() );
		// apply
		if ( b )
		{
			l->setFont( f, -1 );
			on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
		}
	}
}

void UISettings::on_cbLexersHighlightingFillEol_clicked( bool b )
{
	QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );
	if ( it )
		mLexers.value( cbLexersHighlightingLanguages->currentText() )->setEolFill( b, it->data( Qt::UserRole ).toInt() );
}

void UISettings::cbLexersHighlightingProperties_clicked( bool b )
{
	// get check box
	QCheckBox* cb = qobject_cast<QCheckBox*>( sender() );
	if ( !cb )
		return;
	// get lexer
	QsciLexer* l = mLexers.value( cbLexersHighlightingLanguages->currentText() );
	// set lexer properties
	if ( cb == cbLexersHighlightingIndentOpeningBrace || cb == cbLexersHighlightingIndentClosingBrace )
	{
		if ( cbLexersHighlightingIndentOpeningBrace->isChecked() && cbLexersHighlightingIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening | QsciScintilla::AiClosing );
		else if ( cbLexersHighlightingIndentOpeningBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening );
		else if ( cbLexersHighlightingIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiClosing );
		else
			l->setAutoIndentStyle( QsciScintilla::AiMaintain );
	}
	else
		pMonkeyStudio::setLexerProperty( cb->statusTip(), l, b );
}

void UISettings::on_cbLexersHighlightingIndentationWarning_currentIndexChanged( int i )
{
	// get lexer
	QsciLexer* l = mLexers.value( cbLexersHighlightingLanguages->currentText() );
	// set lexer properties
	pMonkeyStudio::setLexerProperty( cbLexersHighlightingIndentationWarning->statusTip(), l, cbLexersHighlightingIndentationWarning->itemData( i ) );
}

void UISettings::on_pbLexersHighlightingReset_clicked()
{
	// get lexer
	QsciLexer* l = mLexers.value( cbLexersHighlightingLanguages->currentText() );
	// reset and refresh
	if ( l )
	{
		pMonkeyStudio::resetLexer( l );
		on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
	}
}

void UISettings::on_twAbbreviations_itemSelectionChanged()
{
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );
	if ( it )
		teAbbreviationsCode->setPlainText( it->data( 0, Qt::UserRole ).toString() );
	// enable/disable according to selection
	teAbbreviationsCode->setEnabled( it );
}

void UISettings::on_pbAbbreviationsAdd_clicked()
{
	UIAddAbbreviation::edit( twAbbreviations );
}

void UISettings::on_pbAbbreviationsRemove_clicked()
{
	delete twAbbreviations->selectedItems().value( 0 );
	teAbbreviationsCode->clear();
}

void UISettings::on_teAbbreviationsCode_textChanged()
{
	// get item
	QTreeWidgetItem* it = twAbbreviations->selectedItems().value( 0 );
	if ( it )
		it->setData( 0, Qt::UserRole, teAbbreviationsCode->toPlainText() );
}

void UISettings::accept()
{
	apply();
	QDialog::accept();
}

void UISettings::apply()
{
	saveSettings();
	pMonkeyStudio::applyProperties();
}