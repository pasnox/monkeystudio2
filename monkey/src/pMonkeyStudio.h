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

	Q_MONKEY_EXPORT void warning( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	Q_MONKEY_EXPORT void information( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	Q_MONKEY_EXPORT bool question( const QString&, const QString&, QWidget* = QApplication::activeWindow(), QMessageBox::StandardButtons = QMessageBox::Yes | QMessageBox::No, QMessageBox::StandardButton = QMessageBox::No );
	
	Q_MONKEY_EXPORT bool isSameFile( const QString&, const QString& );

	Q_MONKEY_EXPORT const QStringList availableTextCodecs();
	Q_MONKEY_EXPORT const QStringList availableImageFormats();
	Q_MONKEY_EXPORT const QStringList availableLanguages();
	Q_MONKEY_EXPORT const QStringList availableProjects();

	Q_MONKEY_EXPORT const QFileInfoList getFiles( QDir d, const QStringList&, bool = true );
	Q_MONKEY_EXPORT const QFileInfoList getFiles( QDir d, const QString& = QString::null, bool = true );

	Q_MONKEY_EXPORT const QStringList getImageFileNames( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	Q_MONKEY_EXPORT const QString getImageFileName( const QString&, const QString&, QWidget* = QApplication::activeWindow() );

	Q_MONKEY_EXPORT const QStringList getOpenFileNames( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );
	Q_MONKEY_EXPORT const QString getOpenFileName( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );
	
	Q_MONKEY_EXPORT const QString getSaveFileName( const QString&, const QString&, const QString& = QString(), QWidget* = QApplication::activeWindow() );

	Q_MONKEY_EXPORT const QString getExistingDirectory( const QString&, const QString&, QWidget* = QApplication::activeWindow() );

	Q_MONKEY_EXPORT const QString tokenizeHome( const QString& );
	Q_MONKEY_EXPORT const QString unTokenizeHome( const QString& );

	Q_MONKEY_EXPORT const QHash<QString, QStringList> defaultSuffixes();
	Q_MONKEY_EXPORT const QHash<QString, QStringList> availableSuffixes();
	Q_MONKEY_EXPORT const QHash<QString, QStringList> availableProjectsSuffixes();
	Q_MONKEY_EXPORT const QString availableLanguagesFilters();
	Q_MONKEY_EXPORT const QString availableProjectsFilters();

	Q_MONKEY_EXPORT const QString settingsPath();
	Q_MONKEY_EXPORT const QString scintillaSettingsPath();
	Q_MONKEY_EXPORT void prepareAPIs();
	Q_MONKEY_EXPORT QsciAPIs* apisForLexer( QsciLexer* );
	Q_MONKEY_EXPORT QsciLexer* lexerForFilename( const QString& );
	Q_MONKEY_EXPORT QsciLexer* lexerForLanguage( const QString& );
	Q_MONKEY_EXPORT bool setLexerProperty( const QString&, QsciLexer*, const QVariant& );
	Q_MONKEY_EXPORT const QVariant lexerProperty( const QString&, QsciLexer* );
	Q_MONKEY_EXPORT void resetLexer( QsciLexer* l );
	Q_MONKEY_EXPORT void applyProperties();
	Q_MONKEY_EXPORT void setEditorProperties( pEditor* );
	
	/***** GENERAL *****/
	Q_MONKEY_EXPORT void setRestoreProjectsOnStartup( bool );
	Q_MONKEY_EXPORT const bool restoreProjectsOnStartup();
	Q_MONKEY_EXPORT void setDefaultProjectsDirectory( const QString& );
	Q_MONKEY_EXPORT const QString defaultProjectsDirectory();
	Q_MONKEY_EXPORT void setUIDesignerMode( pMonkeyStudio::UIDesignerMode );
	Q_MONKEY_EXPORT const  pMonkeyStudio::UIDesignerMode uiDesignerMode();
	Q_MONKEY_EXPORT void setExternalChanges( pMonkeyStudio::ExternalChangesMode );
	Q_MONKEY_EXPORT const pMonkeyStudio::ExternalChangesMode externalchanges();
	Q_MONKEY_EXPORT void setSaveSessionOnClose( bool );
	Q_MONKEY_EXPORT const bool saveSessionOnClose();
	Q_MONKEY_EXPORT void setRestoreSessionOnStartup( bool );
	Q_MONKEY_EXPORT const bool restoreSessionOnStartup();
	
	/******	EDITOR ******/
	// General
	Q_MONKEY_EXPORT void setAutoSyntaxCheck( bool );
	Q_MONKEY_EXPORT const bool autoSyntaxCheck();
	Q_MONKEY_EXPORT void setConvertTabsUponOpen( bool );
	Q_MONKEY_EXPORT const bool convertTabsUponOpen();
	Q_MONKEY_EXPORT void setCreateBackupUponOpen( bool );
	Q_MONKEY_EXPORT const bool createBackupUponOpen();
	Q_MONKEY_EXPORT void setAutoEolConversion( bool );
	Q_MONKEY_EXPORT const bool autoEolConversion();
	Q_MONKEY_EXPORT void setDefaultEncoding( const QString& );
	Q_MONKEY_EXPORT const QString defaultEncoding();
	Q_MONKEY_EXPORT void setSelectionBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor selectionBackgroundColor();
	Q_MONKEY_EXPORT void setSelectionForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor selectionForegroundColor();
	Q_MONKEY_EXPORT void setDefaultDocumentColours( bool );
	Q_MONKEY_EXPORT const bool defaultDocumentColours();
	Q_MONKEY_EXPORT void setDefaultDocumentPen( const QColor& );
	Q_MONKEY_EXPORT const QColor defaultDocumentPen();
	Q_MONKEY_EXPORT void setDefaultDocumentPaper( const QColor& );
	Q_MONKEY_EXPORT const QColor defaultDocumentPaper();
	// Auto Completion
	Q_MONKEY_EXPORT void setAutoCompletionCaseSensitivity( bool );
	Q_MONKEY_EXPORT const bool autoCompletionCaseSensitivity();
	Q_MONKEY_EXPORT void setAutoCompletionReplaceWord( bool );
	Q_MONKEY_EXPORT const bool autoCompletionReplaceWord();
	Q_MONKEY_EXPORT void setAutoCompletionShowSingle( bool );
	Q_MONKEY_EXPORT const bool autoCompletionShowSingle();
	Q_MONKEY_EXPORT void setAutoCompletionSource( QsciScintilla::AutoCompletionSource );
	Q_MONKEY_EXPORT const QsciScintilla::AutoCompletionSource autoCompletionSource();
	Q_MONKEY_EXPORT void setAutoCompletionThreshold( int );
	Q_MONKEY_EXPORT const int autoCompletionThreshold();
	// CallTips
	Q_MONKEY_EXPORT void setCallTipsBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor callTipsBackgroundColor();
	Q_MONKEY_EXPORT void setCallTipsForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor callTipsForegroundColor();
	Q_MONKEY_EXPORT void setCallTipsHighlightColor( const QColor& );
	Q_MONKEY_EXPORT const QColor callTipsHighlightColor();
	Q_MONKEY_EXPORT void setCallTipsStyle( QsciScintilla::CallTipsStyle );
	Q_MONKEY_EXPORT const QsciScintilla::CallTipsStyle callTipsStyle();
	Q_MONKEY_EXPORT void setCallTipsVisible( int );
	Q_MONKEY_EXPORT const int callTipsVisible();
	// Indentation
	Q_MONKEY_EXPORT void setAutoIndent( bool );
	Q_MONKEY_EXPORT const bool autoIndent();
	Q_MONKEY_EXPORT void setBackspaceUnindents( bool );
	Q_MONKEY_EXPORT const bool backspaceUnindents();
	Q_MONKEY_EXPORT void setIndentationGuides( bool );
	Q_MONKEY_EXPORT const bool indentationGuides();
	Q_MONKEY_EXPORT void setIndentationsUseTabs( bool );
	Q_MONKEY_EXPORT const bool indentationsUseTabs();
	Q_MONKEY_EXPORT void setIndentationWidth( int );
	Q_MONKEY_EXPORT const int indentationWidth();
	Q_MONKEY_EXPORT void setTabIndents( bool );
	Q_MONKEY_EXPORT const bool tabIndents();
	Q_MONKEY_EXPORT void setTabWidth( int );
	Q_MONKEY_EXPORT const int tabWidth();
	Q_MONKEY_EXPORT void setIndentationGuidesBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor indentationGuidesBackgroundColor();
	Q_MONKEY_EXPORT void setIndentationGuidesForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor indentationGuidesForegroundColor();
	// Brace Matching
	Q_MONKEY_EXPORT void setBraceMatching( QsciScintilla::BraceMatch );
	Q_MONKEY_EXPORT const QsciScintilla::BraceMatch braceMatching();
	Q_MONKEY_EXPORT void setMatchedBraceBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor matchedBraceBackgroundColor();
	Q_MONKEY_EXPORT void setMatchedBraceForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor matchedBraceForegroundColor();
	Q_MONKEY_EXPORT void setUnmatchedBraceBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor unmatchedBraceBackgroundColor();
	Q_MONKEY_EXPORT void setUnmatchedBraceForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor unmatchedBraceForegroundColor();
	// Edge Mode
	Q_MONKEY_EXPORT void setEdgeMode( QsciScintilla::EdgeMode );
	Q_MONKEY_EXPORT const QsciScintilla::EdgeMode edgeMode();
	Q_MONKEY_EXPORT void setEdgeColor( const QColor& );
	Q_MONKEY_EXPORT const QColor edgeColor();
	Q_MONKEY_EXPORT void setEdgeColumn( int );
	Q_MONKEY_EXPORT const int edgeColumn();
	// Caret
	Q_MONKEY_EXPORT void setCaretLineVisible( bool );
	Q_MONKEY_EXPORT const bool caretLineVisible();
	Q_MONKEY_EXPORT void setCaretLineBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor caretLineBackgroundColor();
	Q_MONKEY_EXPORT void setCaretForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor caretForegroundColor();
	Q_MONKEY_EXPORT void setCaretWidth( int );
	Q_MONKEY_EXPORT const int caretWidth();
	// Margins
	Q_MONKEY_EXPORT void setLineNumbersMarginEnabled( bool );
	Q_MONKEY_EXPORT const bool lineNumbersMarginEnabled();
	Q_MONKEY_EXPORT void setLineNumbersMarginWidth( int );
	Q_MONKEY_EXPORT const int lineNumbersMarginWidth();
	Q_MONKEY_EXPORT void setLineNumbersMarginAutoWidth( bool );
	Q_MONKEY_EXPORT const bool lineNumbersMarginAutoWidth();
	Q_MONKEY_EXPORT void setFolding( QsciScintilla::FoldStyle );
	Q_MONKEY_EXPORT const QsciScintilla::FoldStyle folding();
	Q_MONKEY_EXPORT void setFoldMarginBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor foldMarginBackgroundColor();
	Q_MONKEY_EXPORT void setFoldMarginForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor foldMarginForegroundColor();
	Q_MONKEY_EXPORT void setMarginsEnabled( bool );
	Q_MONKEY_EXPORT const bool marginsEnabled();
	Q_MONKEY_EXPORT void setMarginsBackgroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor marginsBackgroundColor();
	Q_MONKEY_EXPORT void setMarginsForegroundColor( const QColor& );
	Q_MONKEY_EXPORT const QColor marginsForegroundColor();
	Q_MONKEY_EXPORT void setMarginsFont( const QFont& );
	Q_MONKEY_EXPORT const QFont marginsFont();
	// Special Characters
	Q_MONKEY_EXPORT void setEolMode( QsciScintilla::EolMode );
	Q_MONKEY_EXPORT const QsciScintilla::EolMode eolMode();
	Q_MONKEY_EXPORT void setEolVisibility( bool );
	Q_MONKEY_EXPORT const bool eolVisibility();
	Q_MONKEY_EXPORT void setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility );
	Q_MONKEY_EXPORT const QsciScintilla::WhitespaceVisibility whitespaceVisibility();
	Q_MONKEY_EXPORT void setWrapMode( QsciScintilla::WrapMode );
	Q_MONKEY_EXPORT const QsciScintilla::WrapMode wrapMode();
	Q_MONKEY_EXPORT void setWrapVisualFlagsEnabled( bool );
	Q_MONKEY_EXPORT const bool wrapVisualFlagsEnabled();
	Q_MONKEY_EXPORT void setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag );
	Q_MONKEY_EXPORT const QsciScintilla::WrapVisualFlag startWrapVisualFlag();
	Q_MONKEY_EXPORT void setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag );
	Q_MONKEY_EXPORT const QsciScintilla::WrapVisualFlag endWrapVisualFlag();
	Q_MONKEY_EXPORT void setWrappedLineIndentWidth( int );
	Q_MONKEY_EXPORT const int wrappedLineIndentWidth();

};

#endif // PMONKEYSTUDIO_H
