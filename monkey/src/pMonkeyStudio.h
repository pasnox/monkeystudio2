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

	void warning( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	void information( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	bool question( const QString&, const QString&, QWidget* = QApplication::activeWindow(), QMessageBox::StandardButtons = QMessageBox::Yes | QMessageBox::No, QMessageBox::StandardButton = QMessageBox::No );

	const QStringList availableTextCodecs();
	const QStringList availableImageFormats();
	const QStringList availableLanguages();

	const QFileInfoList getFiles( QDir d, const QString& = QString::null, bool = true );

	const QStringList getImageFileNames( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	const QString getImageFileName( const QString&, const QString&, QWidget* = QApplication::activeWindow() );

	const QStringList getOpenFileNames( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );
	const QString getOpenFileName( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );

	const QString getExistingDirectory( const QString&, const QString&, QWidget* = QApplication::activeWindow() );

	const QString tokenizeHome( const QString& );
	const QString unTokenizeHome( const QString& );

	const QHash<QString, QStringList> defaultSuffixes();
	const QHash<QString, QStringList> availableSuffixes();
	const QString availableLanguagesFilters();

	const QString settingsPath();
	const QString scintillaSettingsPath();
	QsciAPIs* apisForLexer( QsciLexer* );
	QsciLexer* lexerForFilename( const QString& );
	QsciLexer* lexerForLanguage( const QString& );
	bool setLexerProperty( const QString&, QsciLexer*, const QVariant& );
	const QVariant lexerProperty( const QString&, QsciLexer* );
	void resetLexer( QsciLexer* l );
	void applyProperties();
	void setEditorProperties( pEditor* );
	
	/***** GENERAL *****/
	void setRestoreProjectsOnStartup( bool );
	const bool restoreProjectsOnStartup();
	void setDefaultProjectsDirectory( const QString& );
	const QString defaultProjectsDirectory();
	void setUIDesignerMode( pMonkeyStudio::UIDesignerMode );
	const  pMonkeyStudio::UIDesignerMode uiDesignerMode();
	void setExternalChanges( pMonkeyStudio::ExternalChangesMode );
	const pMonkeyStudio::ExternalChangesMode externalchanges();
	void setSaveSessionOnClose( bool );
	const bool saveSessionOnClose();
	void setRestoreSessionOnStartup( bool );
	const bool restoreSessionOnStartup();
	
	/******	EDITOR ******/
	// General
	void setAutoSyntaxCheck( bool );
	const bool autoSyntaxCheck();
	void setConvertTabsUponOpen( bool );
	const bool convertTabsUponOpen();
	void setCreateBackupUponOpen( bool );
	const bool createBackupUponOpen();
	void setAutoEolConversion( bool );
	const bool autoEolConversion();
	void setDefaultEncoding( const QString& );
	const QString defaultEncoding();
	void setSelectionBackgroundColor( const QColor& );
	const QColor selectionBackgroundColor();
	void setSelectionForegroundColor( const QColor& );
	const QColor selectionForegroundColor();
	void setDefaultDocumentColours( bool );
	const bool defaultDocumentColours();
	void setDefaultDocumentPen( const QColor& );
	const QColor defaultDocumentPen();
	void setDefaultDocumentPaper( const QColor& );
	const QColor defaultDocumentPaper();
	// Auto Completion
	void setAutoCompletionCaseSensitivity( bool );
	const bool autoCompletionCaseSensitivity();
	void setAutoCompletionReplaceWord( bool );
	const bool autoCompletionReplaceWord();
	void setAutoCompletionShowSingle( bool );
	const bool autoCompletionShowSingle();
	void setAutoCompletionSource( QsciScintilla::AutoCompletionSource );
	const QsciScintilla::AutoCompletionSource autoCompletionSource();
	void setAutoCompletionThreshold( int );
	const int autoCompletionThreshold();
	// CallTips
	void setCallTipsBackgroundColor( const QColor& );
	const QColor callTipsBackgroundColor();
	void setCallTipsForegroundColor( const QColor& );
	const QColor callTipsForegroundColor();
	void setCallTipsHighlightColor( const QColor& );
	const QColor callTipsHighlightColor();
	void setCallTipsStyle( QsciScintilla::CallTipsStyle );
	const QsciScintilla::CallTipsStyle callTipsStyle();
	void setCallTipsVisible( int );
	const int callTipsVisible();
	// Indentation
	void setAutoIndent( bool );
	const bool autoIndent();
	void setBackspaceUnindents( bool );
	const bool backspaceUnindents();
	void setIndentationGuides( bool );
	const bool indentationGuides();
	void setIndentationsUseTabs( bool );
	const bool indentationsUseTabs();
	void setIndentationWidth( int );
	const int indentationWidth();
	void setTabIndents( bool );
	const bool tabIndents();
	void setTabWidth( int );
	const int tabWidth();
	void setIndentationGuidesBackgroundColor( const QColor& );
	const QColor indentationGuidesBackgroundColor();
	void setIndentationGuidesForegroundColor( const QColor& );
	const QColor indentationGuidesForegroundColor();
	// Brace Matching
	void setBraceMatching( QsciScintilla::BraceMatch );
	const QsciScintilla::BraceMatch braceMatching();
	void setMatchedBraceBackgroundColor( const QColor& );
	const QColor matchedBraceBackgroundColor();
	void setMatchedBraceForegroundColor( const QColor& );
	const QColor matchedBraceForegroundColor();
	void setUnmatchedBraceBackgroundColor( const QColor& );
	const QColor unmatchedBraceBackgroundColor();
	void setUnmatchedBraceForegroundColor( const QColor& );
	const QColor unmatchedBraceForegroundColor();
	// Edge Mode
	void setEdgeMode( QsciScintilla::EdgeMode );
	const QsciScintilla::EdgeMode edgeMode();
	void setEdgeColor( const QColor& );
	const QColor edgeColor();
	void setEdgeColumn( int );
	const int edgeColumn();
	// Caret
	void setCaretLineVisible( bool );
	const bool caretLineVisible();
	void setCaretLineBackgroundColor( const QColor& );
	const QColor caretLineBackgroundColor();
	void setCaretForegroundColor( const QColor& );
	const QColor caretForegroundColor();
	void setCaretWidth( int );
	const int caretWidth();
	// Margins
	void setLineNumbersMarginEnabled( bool );
	const bool lineNumbersMarginEnabled();
	void setLineNumbersMarginWidth( int );
	const int lineNumbersMarginWidth();
	void setLineNumbersMarginAutoWidth( bool );
	const bool lineNumbersMarginAutoWidth();
	void setFolding( QsciScintilla::FoldStyle );
	const QsciScintilla::FoldStyle folding();
	void setFoldMarginBackgroundColor( const QColor& );
	const QColor foldMarginBackgroundColor();
	void setFoldMarginForegroundColor( const QColor& );
	const QColor foldMarginForegroundColor();
	void setMarginsEnabled( bool );
	const bool marginsEnabled();
	void setMarginsBackgroundColor( const QColor& );
	const QColor marginsBackgroundColor();
	void setMarginsForegroundColor( const QColor& );
	const QColor marginsForegroundColor();
	void setMarginsFont( const QFont& );
	const QFont marginsFont();
	// Special Characters
	void setEolMode( QsciScintilla::EolMode );
	const QsciScintilla::EolMode eolMode();
	void setEolVisibility( bool );
	const bool eolVisibility();
	void setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility );
	const QsciScintilla::WhitespaceVisibility whitespaceVisibility();
	void setWrapMode( QsciScintilla::WrapMode );
	const QsciScintilla::WrapMode wrapMode();
	void setWrapVisualFlagsEnabled( bool );
	const bool wrapVisualFlagsEnabled();
	void setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag );
	const QsciScintilla::WrapVisualFlag startWrapVisualFlag();
	void setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag );
	const QsciScintilla::WrapVisualFlag endWrapVisualFlag();
	void setWrappedLineIndentWidth( int );
	const int wrappedLineIndentWidth();

};

#endif // PMONKEYSTUDIO_H
