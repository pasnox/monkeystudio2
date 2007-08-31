/*****************************************************
* AUTHOR   : Nox P@sNox <pasnox@gmail.com>
* PROJECT  : 
* FILENAME : pMonkeyStudio.h
* DATE     : 2007/07/29
* TIME     : 13:13
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#ifndef PMONKEYSTUDIO_H
#define PMONKEYSTUDIO_H

#include "MonkeyExport.h"

#include <QApplication>
#include <QMessageBox>
#include <QDir>

#include "qsciscintilla.h"

class QsciLexer;
class QsciAPIs;
class pEditor;

namespace pMonkeyStudio
{
	enum UIDesignerMode { uidmEmbedded = 0, uidmExternal };
	enum ExternalChangesMode { ecmNothing = 0, ecmAlert, ecmReload };

	void Q_MONKEY_EXPORT warning( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	void Q_MONKEY_EXPORT information( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	bool Q_MONKEY_EXPORT question( const QString&, const QString&, QWidget* = QApplication::activeWindow(), QMessageBox::StandardButtons = QMessageBox::Yes | QMessageBox::No, QMessageBox::StandardButton = QMessageBox::No );

	const QStringList Q_MONKEY_EXPORT availableTextCodecs();
	const QStringList Q_MONKEY_EXPORT availableImageFormats();
	const QStringList Q_MONKEY_EXPORT availableLanguages();

	const QFileInfoList Q_MONKEY_EXPORT getFiles( QDir d, const QString& = QString::null, bool = true );

	const QStringList Q_MONKEY_EXPORT getImageFileNames( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	const QString Q_MONKEY_EXPORT getImageFileName( const QString&, const QString&, QWidget* = QApplication::activeWindow() );

	const QStringList Q_MONKEY_EXPORT getOpenFileNames( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );
	const QString Q_MONKEY_EXPORT getOpenFileName( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );
	
	const QString Q_MONKEY_EXPORT getSaveFileName( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );

	const QString Q_MONKEY_EXPORT getExistingDirectory( const QString&, const QString&, QWidget* = QApplication::activeWindow() );

	const QString Q_MONKEY_EXPORT tokenizeHome( const QString& );
	const QString Q_MONKEY_EXPORT unTokenizeHome( const QString& );

	const QHash<QString, QStringList> Q_MONKEY_EXPORT defaultSuffixes();
	const QHash<QString, QStringList> Q_MONKEY_EXPORT availableSuffixes();
	const QString Q_MONKEY_EXPORT availableLanguagesFilters();

	const QString Q_MONKEY_EXPORT settingsPath();
	const QString Q_MONKEY_EXPORT scintillaSettingsPath();
	QsciAPIs* Q_MONKEY_EXPORT apisForLexer( QsciLexer* );
	QsciLexer* Q_MONKEY_EXPORT lexerForFilename( const QString& );
	QsciLexer* Q_MONKEY_EXPORT lexerForLanguage( const QString& );
	bool Q_MONKEY_EXPORT setLexerProperty( const QString&, QsciLexer*, const QVariant& );
	const QVariant Q_MONKEY_EXPORT lexerProperty( const QString&, QsciLexer* );
	void Q_MONKEY_EXPORT resetLexer( QsciLexer* l );
	void Q_MONKEY_EXPORT applyProperties();
	void Q_MONKEY_EXPORT setEditorProperties( pEditor* );
	
	/***** GENERAL *****/
	void Q_MONKEY_EXPORT setRestoreProjectsOnStartup( bool );
	const bool Q_MONKEY_EXPORT restoreProjectsOnStartup();
	void Q_MONKEY_EXPORT setDefaultProjectsDirectory( const QString& );
	const QString Q_MONKEY_EXPORT defaultProjectsDirectory();
	void Q_MONKEY_EXPORT setUIDesignerMode( pMonkeyStudio::UIDesignerMode );
	const  pMonkeyStudio::UIDesignerMode Q_MONKEY_EXPORT uiDesignerMode();
	void Q_MONKEY_EXPORT setExternalChanges( pMonkeyStudio::ExternalChangesMode );
	const pMonkeyStudio::ExternalChangesMode Q_MONKEY_EXPORT externalchanges();
	void Q_MONKEY_EXPORT setSaveSessionOnClose( bool );
	const bool Q_MONKEY_EXPORT saveSessionOnClose();
	void Q_MONKEY_EXPORT setRestoreSessionOnStartup( bool );
	const bool Q_MONKEY_EXPORT restoreSessionOnStartup();
	
	/******	EDITOR ******/
	// General
	void Q_MONKEY_EXPORT setAutoSyntaxCheck( bool );
	const bool Q_MONKEY_EXPORT autoSyntaxCheck();
	void Q_MONKEY_EXPORT setConvertTabsUponOpen( bool );
	const bool Q_MONKEY_EXPORT convertTabsUponOpen();
	void Q_MONKEY_EXPORT setCreateBackupUponOpen( bool );
	const bool Q_MONKEY_EXPORT createBackupUponOpen();
	void Q_MONKEY_EXPORT setAutoEolConversion( bool );
	const bool Q_MONKEY_EXPORT autoEolConversion();
	void Q_MONKEY_EXPORT setDefaultEncoding( const QString& );
	const QString Q_MONKEY_EXPORT defaultEncoding();
	void Q_MONKEY_EXPORT setSelectionBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT selectionBackgroundColor();
	void Q_MONKEY_EXPORT setSelectionForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT selectionForegroundColor();
	void Q_MONKEY_EXPORT setDefaultDocumentColours( bool );
	const bool Q_MONKEY_EXPORT defaultDocumentColours();
	void Q_MONKEY_EXPORT setDefaultDocumentPen( const QColor& );
	const QColor Q_MONKEY_EXPORT defaultDocumentPen();
	void Q_MONKEY_EXPORT setDefaultDocumentPaper( const QColor& );
	const QColor Q_MONKEY_EXPORT defaultDocumentPaper();
	// Auto Completion
	void Q_MONKEY_EXPORT setAutoCompletionCaseSensitivity( bool );
	const bool Q_MONKEY_EXPORT autoCompletionCaseSensitivity();
	void Q_MONKEY_EXPORT setAutoCompletionReplaceWord( bool );
	const bool Q_MONKEY_EXPORT autoCompletionReplaceWord();
	void Q_MONKEY_EXPORT setAutoCompletionShowSingle( bool );
	const bool Q_MONKEY_EXPORT autoCompletionShowSingle();
	void Q_MONKEY_EXPORT setAutoCompletionSource( QsciScintilla::AutoCompletionSource );
	const QsciScintilla::AutoCompletionSource Q_MONKEY_EXPORT autoCompletionSource();
	void Q_MONKEY_EXPORT setAutoCompletionThreshold( int );
	const int Q_MONKEY_EXPORT autoCompletionThreshold();
	// CallTips
	void Q_MONKEY_EXPORT setCallTipsBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT callTipsBackgroundColor();
	void Q_MONKEY_EXPORT setCallTipsForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT callTipsForegroundColor();
	void Q_MONKEY_EXPORT setCallTipsHighlightColor( const QColor& );
	const QColor Q_MONKEY_EXPORT callTipsHighlightColor();
	void Q_MONKEY_EXPORT setCallTipsStyle( QsciScintilla::CallTipsStyle );
	const QsciScintilla::CallTipsStyle Q_MONKEY_EXPORT callTipsStyle();
	void Q_MONKEY_EXPORT setCallTipsVisible( int );
	const int Q_MONKEY_EXPORT callTipsVisible();
	// Indentation
	void Q_MONKEY_EXPORT setAutoIndent( bool );
	const bool Q_MONKEY_EXPORT autoIndent();
	void Q_MONKEY_EXPORT setBackspaceUnindents( bool );
	const bool Q_MONKEY_EXPORT backspaceUnindents();
	void Q_MONKEY_EXPORT setIndentationGuides( bool );
	const bool Q_MONKEY_EXPORT indentationGuides();
	void Q_MONKEY_EXPORT setIndentationsUseTabs( bool );
	const bool Q_MONKEY_EXPORT indentationsUseTabs();
	void Q_MONKEY_EXPORT setIndentationWidth( int );
	const int Q_MONKEY_EXPORT indentationWidth();
	void Q_MONKEY_EXPORT setTabIndents( bool );
	const bool Q_MONKEY_EXPORT tabIndents();
	void Q_MONKEY_EXPORT setTabWidth( int );
	const int Q_MONKEY_EXPORT tabWidth();
	void Q_MONKEY_EXPORT setIndentationGuidesBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT indentationGuidesBackgroundColor();
	void Q_MONKEY_EXPORT setIndentationGuidesForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT indentationGuidesForegroundColor();
	// Brace Matching
	void Q_MONKEY_EXPORT setBraceMatching( QsciScintilla::BraceMatch );
	const QsciScintilla::BraceMatch Q_MONKEY_EXPORT braceMatching();
	void Q_MONKEY_EXPORT setMatchedBraceBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT matchedBraceBackgroundColor();
	void Q_MONKEY_EXPORT setMatchedBraceForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT matchedBraceForegroundColor();
	void Q_MONKEY_EXPORT setUnmatchedBraceBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT unmatchedBraceBackgroundColor();
	void Q_MONKEY_EXPORT setUnmatchedBraceForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT unmatchedBraceForegroundColor();
	// Edge Mode
	void Q_MONKEY_EXPORT setEdgeMode( QsciScintilla::EdgeMode );
	const QsciScintilla::EdgeMode Q_MONKEY_EXPORT edgeMode();
	void Q_MONKEY_EXPORT setEdgeColor( const QColor& );
	const QColor Q_MONKEY_EXPORT edgeColor();
	void Q_MONKEY_EXPORT setEdgeColumn( int );
	const int Q_MONKEY_EXPORT edgeColumn();
	// Caret
	void Q_MONKEY_EXPORT setCaretLineVisible( bool );
	const bool Q_MONKEY_EXPORT caretLineVisible();
	void Q_MONKEY_EXPORT setCaretLineBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT caretLineBackgroundColor();
	void Q_MONKEY_EXPORT setCaretForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT caretForegroundColor();
	void Q_MONKEY_EXPORT setCaretWidth( int );
	const int Q_MONKEY_EXPORT caretWidth();
	// Margins
	void Q_MONKEY_EXPORT setLineNumbersMarginEnabled( bool );
	const bool Q_MONKEY_EXPORT lineNumbersMarginEnabled();
	void Q_MONKEY_EXPORT setLineNumbersMarginWidth( int );
	const int Q_MONKEY_EXPORT lineNumbersMarginWidth();
	void Q_MONKEY_EXPORT setLineNumbersMarginAutoWidth( bool );
	const bool Q_MONKEY_EXPORT lineNumbersMarginAutoWidth();
	void Q_MONKEY_EXPORT setFolding( QsciScintilla::FoldStyle );
	const QsciScintilla::FoldStyle Q_MONKEY_EXPORT folding();
	void Q_MONKEY_EXPORT setFoldMarginBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT foldMarginBackgroundColor();
	void Q_MONKEY_EXPORT setFoldMarginForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT foldMarginForegroundColor();
	void Q_MONKEY_EXPORT setMarginsEnabled( bool );
	const bool Q_MONKEY_EXPORT marginsEnabled();
	void Q_MONKEY_EXPORT setMarginsBackgroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT marginsBackgroundColor();
	void Q_MONKEY_EXPORT setMarginsForegroundColor( const QColor& );
	const QColor Q_MONKEY_EXPORT marginsForegroundColor();
	void Q_MONKEY_EXPORT setMarginsFont( const QFont& );
	const QFont Q_MONKEY_EXPORT marginsFont();
	// Special Characters
	void Q_MONKEY_EXPORT setEolMode( QsciScintilla::EolMode );
	const QsciScintilla::EolMode Q_MONKEY_EXPORT eolMode();
	void Q_MONKEY_EXPORT setEolVisibility( bool );
	const bool Q_MONKEY_EXPORT eolVisibility();
	void Q_MONKEY_EXPORT setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility );
	const QsciScintilla::WhitespaceVisibility Q_MONKEY_EXPORT whitespaceVisibility();
	void Q_MONKEY_EXPORT setWrapMode( QsciScintilla::WrapMode );
	const QsciScintilla::WrapMode Q_MONKEY_EXPORT wrapMode();
	void Q_MONKEY_EXPORT setWrapVisualFlagsEnabled( bool );
	const bool Q_MONKEY_EXPORT wrapVisualFlagsEnabled();
	void Q_MONKEY_EXPORT setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag );
	const QsciScintilla::WrapVisualFlag Q_MONKEY_EXPORT startWrapVisualFlag();
	void Q_MONKEY_EXPORT setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag );
	const QsciScintilla::WrapVisualFlag Q_MONKEY_EXPORT endWrapVisualFlag();
	void Q_MONKEY_EXPORT setWrappedLineIndentWidth( int );
	const int Q_MONKEY_EXPORT wrappedLineIndentWidth();

};

#endif // PMONKEYSTUDIO_H
