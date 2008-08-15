/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UISettings.cpp
** Date      : 2008-01-14T00:36:54
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
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
**
****************************************************************************/
#include "UISettings.h"
#include "../../abbreviationsmanager/ui/UIAddAbbreviation.h"
#include "../../workspace/pFileManager.h"
#include "../../templatesmanager/pTemplatesManager.h"
#include "../../abbreviationsmanager/pAbbreviationsManager.h"
#include "../../pMonkeyStudio.h"
#include "../../workspace/pWorkspace.h"
#include "../../coremanager/MonkeyCore.h"
#include "../../settingsmanager/Settings.h"

#include <qscintilla.h>

#include <QButtonGroup>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QTextCodec>
#include <QDir>

using namespace pMonkeyStudio;

const QString SettingsPath = "Settings";

UISettings::UISettings( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	twMenu->topLevelItem( 2 )->setExpanded( true );
	twMenu->setCurrentItem( twMenu->topLevelItem( 0 ) );

	QStringList l;

	foreach ( QString s, availableLanguages() )
		mLexers[s] = lexerForLanguage( s );

	// externalchanges
	bgExternalChanges = new QButtonGroup( gbOnExternalChanges );
	bgExternalChanges->addButton( rbDoNothing, pMonkeyStudio::ecmNothing );
	bgExternalChanges->addButton( rbAlertUser, pMonkeyStudio::ecmAlert );
	bgExternalChanges->addButton( rbReloadAutomatically, pMonkeyStudio::ecmReload );
	
	// tab mode
	cbTabModes->addItem( tr( "SDI" ), pExtendedWorkspace::dmSDI );
	cbTabModes->addItem( tr( "MDI" ), pExtendedWorkspace::dmMDI );
	cbTabModes->addItem( tr( "Top Level" ), pExtendedWorkspace::dmTopLevel );

	// loads text codecs
	cbDefaultEncoding->addItems( availableTextCodecs() );

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
	cbSourceAPIsLanguages->addItems( availableLanguages() );
	cbLexersAssociationsLanguages->addItems( availableLanguages() );
	cbLexersHighlightingLanguages->addItems( availableLanguages() );

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
	connect( tbTabsTextColor, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
	connect( tbCurrentTabTextColor, SIGNAL( clicked() ), this, SLOT( tbColours_clicked() ) );
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
	pSettings* s = MonkeyCore::settings();
	QString sp;
	
	// General
	cbRestoreProjectsOnStartup->setChecked( restoreProjectsOnStartup() );
	cbSaveProjects->setChecked( saveProjectsOnCustomAction() );
	cbSaveFiles->setChecked( saveFilesOnCustomAction() );
	leDefaultProjectsDirectory->setText( defaultProjectsDirectory() );
	cbTabsHaveCloseButton->setChecked( tabsHaveCloseButton() );
	cbTabsHaveShortcut->setChecked( tabsHaveShortcut() );
	cbTabsElided->setChecked( tabsElided() );
	tbTabsTextColor->setIcon( colourizedPixmap( tabsTextColor() ) );
	tbTabsTextColor->setToolTip( tabsTextColor().name() );
	tbCurrentTabTextColor->setIcon( colourizedPixmap( currentTabTextColor() ) );
	tbCurrentTabTextColor->setToolTip( currentTabTextColor().name() );
	cbTabModes->setCurrentIndex( cbTabModes->findData( docMode() ) );
	bgExternalChanges->button( externalchanges() )->setChecked( true );
	cbSaveSession->setChecked( saveSessionOnClose() );
	cbRestoreSession->setChecked( restoreSessionOnStartup() );

	// Paths
	pleTemplatesPaths->setValues( pTemplatesManager::instance()->templatesPath() );
	pleTranslationsPaths->setValues( s->value( "Translations/Path" ).toStringList() );
	plePluginsPaths->setValues( s->value( "Plugins/Path" ).toStringList() );

	// Editor
	//  General
	cbAutoSyntaxCheck->setChecked( autoSyntaxCheck() );
	cbConvertTabsUponOpen->setChecked( convertTabsUponOpen() );
	cbCreateBackupUponOpen->setChecked( createBackupUponOpen() );
	cbAutoEolConversion->setChecked( autoEolConversion() );
	cbDefaultEncoding->setCurrentIndex( cbDefaultEncoding->findText( defaultEncoding() ) );
	tbSelectionBackground->setIcon( colourizedPixmap( selectionBackgroundColor() ) );
	tbSelectionBackground->setToolTip( selectionBackgroundColor().name() );
	tbSelectionForeground->setIcon( colourizedPixmap( selectionForegroundColor() ) );
	tbSelectionForeground->setToolTip( selectionForegroundColor().name() );
	gbDefaultDocumentColours->setChecked( defaultDocumentColours() );
	tbDefaultDocumentPen->setIcon( colourizedPixmap( defaultDocumentPen() ) );
	tbDefaultDocumentPen->setToolTip( defaultDocumentPen().name() );
	tbDefaultDocumentPaper->setIcon( colourizedPixmap( defaultDocumentPaper() ) );
	tbDefaultDocumentPaper->setToolTip( defaultDocumentPaper().name() );
	//  Auto Completion
	gbAutoCompletionEnabled->setChecked( autoCompletionSource() != QsciScintilla::AcsNone );
	cbAutoCompletionCaseSensitivity->setChecked( autoCompletionCaseSensitivity() );
	cbAutoCompletionReplaceWord->setChecked( autoCompletionReplaceWord() );
	cbAutoCompletionShowSingle->setChecked( autoCompletionShowSingle() );
	sAutoCompletionThreshold->setValue( autoCompletionThreshold() );
	if ( bgAutoCompletionSource->button( autoCompletionSource() ) )
		bgAutoCompletionSource->button( autoCompletionSource() )->setChecked( true );
	//  Call Tips
	gbCalltipsEnabled->setChecked( callTipsStyle() != QsciScintilla::CallTipsNone );
	sCallTipsVisible->setValue( callTipsVisible() );
	if ( bgCallTipsStyle->button( callTipsStyle() ) )
		bgCallTipsStyle->button( callTipsStyle() )->setChecked( true );
	tbCalltipsBackground->setIcon( colourizedPixmap( callTipsBackgroundColor() ) );
	tbCalltipsBackground->setToolTip( callTipsBackgroundColor().name() );
	tbCalltipsForeground->setIcon( colourizedPixmap( callTipsForegroundColor() ) );
	tbCalltipsForeground->setToolTip( callTipsForegroundColor().name() );
	tbCalltipsHighlight->setIcon( colourizedPixmap( callTipsHighlightColor() ) );
	tbCalltipsHighlight->setToolTip( callTipsHighlightColor().name() );
	//  Indentation
	cbAutoIndent->setChecked( autoIndent() );
	cbBackspaceUnindents->setChecked( backspaceUnindents() );
	cbIndentationGuides->setChecked( indentationGuides() );
	cbIndentationUseTabs->setChecked( indentationsUseTabs() );
	cbAutodetectIndent->setChecked( autoDetectIndent() );
	cbTabIndents->setChecked( tabIndents() );
	sIndentationTabWidth->setValue( tabWidth() );
	sIndentationWidth->setValue( indentationWidth() );
	tbIndentationGuidesBackground->setIcon( colourizedPixmap( indentationGuidesBackgroundColor() ) );
	tbIndentationGuidesBackground->setToolTip( indentationGuidesBackgroundColor().name() );
	tbIndentationGuidesForeground->setIcon( colourizedPixmap( indentationGuidesForegroundColor() ) );
	tbIndentationGuidesForeground->setToolTip( indentationGuidesForegroundColor().name() );
	//  Brace Matching
	gbBraceMatchingEnabled->setChecked( braceMatching() != QsciScintilla::NoBraceMatch );
	if ( bgBraceMatch->button( braceMatching() ) )
		bgBraceMatch->button( braceMatching() )->setChecked( true );
	tbMatchedBraceForeground->setIcon( colourizedPixmap( matchedBraceForegroundColor() ) );
	tbMatchedBraceForeground->setToolTip( matchedBraceForegroundColor().name() );
	tbMatchedBraceBackground->setIcon( colourizedPixmap( matchedBraceBackgroundColor() ) );
	tbMatchedBraceBackground->setToolTip( matchedBraceBackgroundColor().name() );
	tbUnmatchedBraceBackground->setIcon( colourizedPixmap( unmatchedBraceBackgroundColor() ) );
	tbUnmatchedBraceBackground->setToolTip( unmatchedBraceBackgroundColor().name() );
	tbUnmatchedBraceForeground->setIcon( colourizedPixmap( unmatchedBraceForegroundColor() ) );
	tbUnmatchedBraceForeground->setToolTip( unmatchedBraceForegroundColor().name() );
	//  Edge Mode
	gbEdgeModeEnabled->setChecked( edgeMode() != QsciScintilla::EdgeNone );
	if ( bgEdgeMode->button( edgeMode() ) )
		bgEdgeMode->button( edgeMode() )->setChecked( true );
	sEdgeColumnNumber->setValue( edgeColumn() );
	tbEdgeColor->setIcon( colourizedPixmap( edgeColor() ) );
	tbEdgeColor->setToolTip( edgeColor().name() );
	//  Caret
	gbCaretLineVisible->setChecked( caretLineVisible() );
	tbCaretLineBackground->setIcon( colourizedPixmap( caretLineBackgroundColor() ) );
	tbCaretLineBackground->setToolTip( caretLineBackgroundColor().name() );
	tbCaretForeground->setIcon( colourizedPixmap( caretForegroundColor() ) );
	tbCaretForeground->setToolTip( caretForegroundColor().name() );
	sCaretWidth->setValue( caretWidth() );
	//  Margins
	gbLineNumbersMarginEnabled->setChecked( lineNumbersMarginEnabled() );
	sLineNumbersMarginWidth->setValue( lineNumbersMarginWidth() );
	cbLineNumbersMarginAutoWidth->setChecked( lineNumbersMarginAutoWidth() );
	gbFoldMarginEnabled->setChecked( folding() != QsciScintilla::NoFoldStyle );
	if ( bgFoldStyle->button( folding() ) )
		bgFoldStyle->button( folding() )->setChecked( true );
	tbFoldMarginForeground->setIcon( colourizedPixmap( foldMarginForegroundColor() ) );
	tbFoldMarginForeground->setToolTip( foldMarginForegroundColor().name() );
	tbFoldMarginBackground->setIcon( colourizedPixmap( foldMarginBackgroundColor() ) );
	tbFoldMarginBackground->setToolTip( foldMarginBackgroundColor().name() );
	gbMarginsEnabled->setChecked( marginsEnabled() );
	tbMarginsForeground->setIcon( colourizedPixmap( marginsForegroundColor() ) );
	tbMarginsForeground->setToolTip( marginsForegroundColor().name() );
	tbMarginsBackground->setIcon( colourizedPixmap( marginsBackgroundColor() ) );
	tbMarginsBackground->setToolTip( marginsBackgroundColor().name() );
	tbMarginsFont->setFont( marginsFont() );
	//  Special Characters
	bgEolMode->button( eolMode() )->setChecked( true );
	cbEolVisibility->setChecked( eolVisibility() );
	cbAutoDetectEol->setChecked( autoDetectEol() );
	gbWhitespaceVisibilityEnabled->setChecked( whitespaceVisibility() != QsciScintilla::WsInvisible );
	if ( bgWhitespaceVisibility->button( whitespaceVisibility() ) )
		bgWhitespaceVisibility->button( whitespaceVisibility() )->setChecked( true );
	gbWrapModeEnabled->setChecked( wrapMode() != QsciScintilla::WrapNone );
	if ( bgWrapMode->button( wrapMode() ) )
		bgWrapMode->button( wrapMode() )->setChecked( true );
	gbWrapVisualFlagsEnabled->setChecked( wrapVisualFlagsEnabled() );
	if ( bgStartWrapVisualFlag->button( startWrapVisualFlag() ) )
		bgStartWrapVisualFlag->button( startWrapVisualFlag() )->setChecked( true );
	if ( bgEndWrapVisualFlag->button( endWrapVisualFlag() ) )
		bgEndWrapVisualFlag->button( endWrapVisualFlag() )->setChecked( true );
	sWrappedLineIndentWidth->setValue( wrappedLineIndentWidth() );
	// Source APIs
	for ( int i = 0; i < cbSourceAPIsLanguages->count(); i++ )
		cbSourceAPIsLanguages->setItemData( i, s->value( "SourceAPIs/" +cbSourceAPIsLanguages->itemText( i ) ).toStringList() );
	if ( cbSourceAPIsLanguages->count() > 0 )
		cbSourceAPIsLanguages->setCurrentIndex( 0 );
	//  Lexers Associations
	QHash<QString, QStringList> l = availableLanguagesSuffixes();
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
		l->readSettings( *s, qPrintable( scintillaSettingsPath() ) );
	
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
}

void UISettings::saveSettings()
{
	pSettings* s = MonkeyCore::settings();
	QString sp;

	// General
	setRestoreProjectsOnStartup( cbRestoreProjectsOnStartup->isChecked() );
	setSaveProjectsOnCustomAction( cbSaveProjects->isChecked() );
	setSaveFilesOnCustomAction( cbSaveFiles->isChecked() );
	setDefaultProjectsDirectory( leDefaultProjectsDirectory->text() );
	setTabsHaveCloseButton( cbTabsHaveCloseButton->isChecked() );
	setTabsHaveShortcut( cbTabsHaveShortcut->isChecked() );
	setTabsElided( cbTabsElided->isChecked() );
	setTabsTextColor( QColor( tbTabsTextColor->toolTip() ) );
	setCurrentTabTextColor( QColor( tbCurrentTabTextColor->toolTip() ) );
	setDocMode( (pExtendedWorkspace::DocumentMode)cbTabModes->itemData( cbTabModes->currentIndex() ).toInt() );
	setExternalChanges( (pMonkeyStudio::ExternalChangesMode)bgExternalChanges->checkedId() );
	setSaveSessionOnClose( cbSaveSession->isChecked() );
	setRestoreSessionOnStartup( cbRestoreSession->isChecked() );

	// Paths
	pTemplatesManager::instance()->setTemplatesPath( pleTemplatesPaths->values() );
	s->setValue( "Translations/Path", pleTranslationsPaths->values() );
	s->setValue( "Plugins/Path", plePluginsPaths->values() );

	// Editor
	//  General
	setAutoSyntaxCheck( cbAutoSyntaxCheck->isChecked() );
	setConvertTabsUponOpen( cbConvertTabsUponOpen->isChecked() );
	setCreateBackupUponOpen( cbCreateBackupUponOpen->isChecked() ) ;
	setAutoEolConversion( cbAutoEolConversion->isChecked() );
	setAutoDetectEol( cbAutoDetectEol->isChecked() );
	setDefaultEncoding( cbDefaultEncoding->currentText() );
	setSelectionBackgroundColor( QColor( tbSelectionBackground->toolTip() ) );
	setSelectionForegroundColor( QColor( tbSelectionForeground->toolTip() ) );
	setDefaultDocumentColours( gbDefaultDocumentColours->isChecked() );
	setDefaultDocumentPen( QColor( tbDefaultDocumentPen->toolTip() ) );
	setDefaultDocumentPaper( QColor( tbDefaultDocumentPaper->toolTip() ) );
	//  Auto Completion
	setAutoCompletionSource( QsciScintilla::AcsNone );
	if ( gbAutoCompletionEnabled->isChecked() )
		setAutoCompletionSource( (QsciScintilla::AutoCompletionSource)bgAutoCompletionSource->checkedId() );
	setAutoCompletionCaseSensitivity( cbAutoCompletionCaseSensitivity->isChecked() );
	setAutoCompletionReplaceWord( cbAutoCompletionReplaceWord->isChecked() );
	setAutoCompletionShowSingle( cbAutoCompletionShowSingle->isChecked() );
	setAutoCompletionThreshold( sAutoCompletionThreshold->value() );
	//  Call Tips
	setCallTipsStyle( QsciScintilla::CallTipsNone );
	if ( gbCalltipsEnabled->isChecked() )
		setCallTipsStyle( (QsciScintilla::CallTipsStyle)bgCallTipsStyle->checkedId() );
	setCallTipsVisible( sCallTipsVisible->value() );
	setCallTipsBackgroundColor( QColor( tbCalltipsBackground->toolTip() ) );
	setCallTipsForegroundColor( QColor( tbCalltipsForeground->toolTip() ) );
	setCallTipsHighlightColor( QColor( tbCalltipsHighlight->toolTip() ) );
	//  Indentation
	setAutoIndent( cbAutoIndent->isChecked()  );
	setBackspaceUnindents( cbBackspaceUnindents->isChecked() );
	setIndentationGuides( cbIndentationGuides->isChecked() );
	setIndentationsUseTabs( cbIndentationUseTabs->isChecked() );
	setTabIndents( cbTabIndents->isChecked() );
	setAutoDetectIndent( cbAutodetectIndent->isChecked() );
	setTabWidth( sIndentationTabWidth->value() );
	setIndentationWidth( sIndentationWidth->value() );
	setIndentationGuidesBackgroundColor( QColor( tbIndentationGuidesBackground->toolTip() ) );
	setIndentationGuidesForegroundColor( QColor( tbIndentationGuidesForeground->toolTip() ) );
	//  Brace Matching
	setBraceMatching( QsciScintilla::NoBraceMatch );
	if ( gbBraceMatchingEnabled->isChecked() )
		setBraceMatching( (QsciScintilla::BraceMatch)bgBraceMatch->checkedId() );
	setMatchedBraceBackgroundColor( QColor( tbMatchedBraceBackground->toolTip() ) );
	setMatchedBraceForegroundColor( QColor( tbMatchedBraceForeground->toolTip() ) );
	setUnmatchedBraceBackgroundColor( QColor( tbUnmatchedBraceBackground->toolTip() ) );
	setUnmatchedBraceForegroundColor( QColor( tbUnmatchedBraceForeground->toolTip() ) );
	//  Edge Mode
	setEdgeMode( QsciScintilla::EdgeNone );
	if ( gbEdgeModeEnabled->isChecked() )
		setEdgeMode( (QsciScintilla::EdgeMode)bgEdgeMode->checkedId() );
	setEdgeColumn( sEdgeColumnNumber->value() );
	setEdgeColor( QColor( tbEdgeColor->toolTip() ) );
	//  Caret
	setCaretLineVisible( gbCaretLineVisible->isChecked() );
	setCaretLineBackgroundColor( QColor( tbCaretLineBackground->toolTip() ) );
	setCaretForegroundColor( QColor( tbCaretForeground->toolTip() ) );
	setCaretWidth( sCaretWidth->value() );
	//  Margins
	setLineNumbersMarginEnabled( gbLineNumbersMarginEnabled->isChecked() );
	setLineNumbersMarginWidth( sLineNumbersMarginWidth->value() );
	setLineNumbersMarginAutoWidth( cbLineNumbersMarginAutoWidth->isChecked() );
	setFolding( QsciScintilla::NoFoldStyle );
	if ( gbFoldMarginEnabled->isChecked() )
		setFolding( (QsciScintilla::FoldStyle)bgFoldStyle->checkedId() );
	setFoldMarginForegroundColor( QColor( tbFoldMarginForeground->toolTip() ) );
	setFoldMarginBackgroundColor( QColor( tbFoldMarginBackground->toolTip() ) );
	setMarginsEnabled( gbMarginsEnabled->isChecked() );
	setMarginsForegroundColor( QColor( tbMarginsForeground->toolTip() ) );
	setMarginsBackgroundColor( QColor( tbMarginsBackground->toolTip() ) );
	setMarginsFont( tbMarginsFont->font() );
	//  Special Characters
	setEolMode( (QsciScintilla::EolMode)bgEolMode->checkedId() );
	setEolVisibility( cbEolVisibility->isChecked() );
	setAutoDetectEol( cbAutoDetectEol->isChecked() );
	setWhitespaceVisibility( QsciScintilla::WsInvisible );
	if ( gbWhitespaceVisibilityEnabled->isChecked() )
		setWhitespaceVisibility( (QsciScintilla::WhitespaceVisibility)bgWhitespaceVisibility->checkedId() );
	setWrapMode( QsciScintilla::WrapNone );
	if ( gbWrapModeEnabled->isChecked() )
		setWrapMode( (QsciScintilla::WrapMode)bgWrapMode->checkedId() );
	setWrapVisualFlagsEnabled( gbWrapVisualFlagsEnabled->isChecked() );
	setStartWrapVisualFlag( QsciScintilla::WrapFlagNone );
	if ( gbWrapVisualFlagsEnabled->isChecked() )
		setStartWrapVisualFlag( (QsciScintilla::WrapVisualFlag)bgStartWrapVisualFlag->checkedId() );
	setEndWrapVisualFlag( QsciScintilla::WrapFlagNone );
	if ( gbWrapVisualFlagsEnabled->isChecked() )
		setEndWrapVisualFlag( (QsciScintilla::WrapVisualFlag)bgEndWrapVisualFlag->checkedId() );
	setWrappedLineIndentWidth( sWrappedLineIndentWidth->value() );
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
		l->setDefaultPaper( QColor( tbDefaultDocumentPaper->toolTip() ) );
		l->setDefaultColor( QColor( tbDefaultDocumentPen->toolTip() ) );
		l->writeSettings( *s, qPrintable( scintillaSettingsPath() ) );
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
}

QPixmap UISettings::colourizedPixmap( const QColor& c ) const
{
	QPixmap p( 48, 16 );
	p.fill( c );
	return p;
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

void UISettings::tbColours_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	QColor c = QColorDialog::getColor( QColor( tb->toolTip() ) , window() );
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
	// get files
	QStringList files = leSourceAPIs->text().split( ";", QString::SkipEmptyParts );
	// add them recursively
	foreach ( const QString& fn, files )
		if ( lwSourceAPIs->findItems( fn, Qt::MatchFixedString ).count() == 0 )
			lwSourceAPIs->addItem( fn );
	// clear input
	leSourceAPIs->clear();
	// save datas
	cbSourceAPIsLanguages_beforeChanged( cbSourceAPIsLanguages->currentIndex() );
}

void UISettings::on_pbSourceAPIsBrowse_clicked()
{
	QStringList files = QFileDialog::getOpenFileNames( window(), tr( "Select API files" ), QString::null, tr( "API Files (*.api);;All Files (*)" ) );
	if ( !files.isEmpty() )
		leSourceAPIs->setText( files.join( ";" ) );
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
	v = lexerProperty( "foldComments", l );
	cbLexersHighlightingFoldComments->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldComments->setChecked( v.toBool() );
	// fold compact
	v = lexerProperty( "foldCompact", l );
	cbLexersHighlightingFoldCompact->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldCompact->setChecked( v.toBool() );
	// fold quotes
	v = lexerProperty( "foldQuotes", l );
	cbLexersHighlightingFoldQuotes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldQuotes->setChecked( v.toBool() );
	// fold directives
	v = lexerProperty( "foldDirectives", l );
	cbLexersHighlightingFoldDirectives->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldDirectives->setChecked( v.toBool() );
	// fold at begin
	v = lexerProperty( "foldAtBegin", l );
	cbLexersHighlightingFoldAtBegin->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtBegin->setChecked( v.toBool() );
	// fold at parenthesis
	v = lexerProperty( "foldAtParenthesis", l );
	cbLexersHighlightingFoldAtParenthesis->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtParenthesis->setChecked( v.toBool() );
	// fold at else
	v = lexerProperty( "foldAtElse", l );
	cbLexersHighlightingFoldAtElse->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtElse->setChecked( v.toBool() );
	// fold preprocessor
	v = lexerProperty( "foldPreprocessor", l );
	cbLexersHighlightingFoldPreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldPreprocessor->setChecked( v.toBool() );
	// style preprocessor
	v = lexerProperty( "stylePreprocessor", l );
	cbLexersHighlightingStylePreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingStylePreprocessor->setChecked( v.toBool() );
	// indent opening brace
	cbLexersHighlightingIndentOpeningBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiOpening );
	// indent closing brace
	cbLexersHighlightingIndentClosingBrace->setChecked( l->autoIndentStyle() & QsciScintilla::AiClosing );
	// case sensitive tags
	v = lexerProperty( "caseSensitiveTags", l );
	cbLexersHighlightingCaseSensitiveTags->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingCaseSensitiveTags->setChecked( v.toBool() );
	// backslash escapes
	v = lexerProperty( "backslashEscapes", l );
	cbLexersHighlightingBackslashEscapes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingBackslashEscapes->setChecked( v.toBool() );
	// indentation warning
	v = lexerProperty( "indentationWarning", l );
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
		setLexerProperty( cb->statusTip(), l, b );
}

void UISettings::on_cbLexersHighlightingIndentationWarning_currentIndexChanged( int i )
{
	// get lexer
	QsciLexer* l = mLexers.value( cbLexersHighlightingLanguages->currentText() );
	// set lexer properties
	setLexerProperty( cbLexersHighlightingIndentationWarning->statusTip(), l, cbLexersHighlightingIndentationWarning->itemData( i ) );
}

void UISettings::on_pbLexersHighlightingReset_clicked()
{
	// get lexer
	QsciLexer* l = mLexers.value( cbLexersHighlightingLanguages->currentText() );
	// reset and refresh
	if ( l )
	{
		resetLexer( l );
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
{ UIAddAbbreviation::edit( twAbbreviations ); }

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
	applyProperties();
	MonkeyCore::workspace()->loadSettings();
}
