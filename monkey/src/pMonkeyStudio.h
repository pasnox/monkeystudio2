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
/*!
	\file pMonkeyStudio.cpp
	\date 2008-01-14T00:37:22
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A global namespace for various options and usefull functions
*/
#ifndef PMONKEYSTUDIO_H
#define PMONKEYSTUDIO_H

#include <fresh.h>
#include <qscintilla.h>

#include "workspace/pWorkspace.h"

#include <QApplication>
#include <QMessageBox>
#include <QDir>

class QTranslator;
class pEditor;

namespace pMonkeyStudio
{
	enum ExternalChangesMode { ecmNothing = 0, ecmAlert, ecmReload };
	
	Q_MONKEY_EXPORT void addTranslator( QTranslator* translator );
	Q_MONKEY_EXPORT void loadTranslations();

	Q_MONKEY_EXPORT void warning( const QString& title, const QString& message, QWidget* parent = QApplication::activeWindow() );
	Q_MONKEY_EXPORT void information( const QString& title, const QString& message, QWidget* parent = QApplication::activeWindow() );
	Q_MONKEY_EXPORT bool question( const QString& title, const QString& message, QWidget* parent = QApplication::activeWindow(), QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No, QMessageBox::StandardButton defaultButton = QMessageBox::No );
	
	Q_MONKEY_EXPORT bool isSameFile( const QString& left, const QString& right );

	Q_MONKEY_EXPORT const QStringList availableTextCodecs();
	Q_MONKEY_EXPORT const QStringList availableImageFormats();
	Q_MONKEY_EXPORT const QStringList availableLanguages();

	Q_MONKEY_EXPORT const QFileInfoList getFolders( QDir fromDir, const QStringList& filters, bool recursive = true );
	Q_MONKEY_EXPORT const QFileInfoList getFiles( QDir fromDir, const QStringList& filters, bool recursive = true );
	Q_MONKEY_EXPORT const QFileInfoList getFiles( QDir fromDir, const QString& filters = QString::null, bool recursive = true );

	Q_MONKEY_EXPORT const QStringList getImageFileNames( const QString& title, const QString& filename, QWidget* parent = QApplication::activeWindow() );
	Q_MONKEY_EXPORT const QString getImageFileName( const QString& title, const QString& filename, QWidget* parent = QApplication::activeWindow() );

	Q_MONKEY_EXPORT const QStringList getOpenFileNames( const QString& title, const QString& filename, const QString& filters = QString(), QWidget* parent = QApplication::activeWindow() );
	Q_MONKEY_EXPORT const QString getOpenFileName( const QString& title, const QString& filename, const QString& filters = QString(), QWidget* parent = QApplication::activeWindow() );
	
	Q_MONKEY_EXPORT const QString getSaveFileName( const QString& title, const QString& filename, const QString& filters = QString(), QWidget* parent = QApplication::activeWindow() );

	Q_MONKEY_EXPORT const QString getExistingDirectory( const QString& title, const QString& path, QWidget* parent = QApplication::activeWindow() );

	Q_MONKEY_EXPORT const QString tokenizeHome( const QString& string );
	Q_MONKEY_EXPORT const QString unTokenizeHome( const QString& string );

	Q_MONKEY_EXPORT const QHash<QString, QStringList> defaultLanguagesSuffixes();
	Q_MONKEY_EXPORT const QHash<QString, QStringList> availableLanguagesSuffixes();
	Q_MONKEY_EXPORT const QHash<QString, QStringList> availableFilesSuffixes();
	Q_MONKEY_EXPORT const QString availableLanguagesFilters();
	Q_MONKEY_EXPORT const QString availableFilesFilters();

	Q_MONKEY_EXPORT const QString settingsPath();
	Q_MONKEY_EXPORT const QString scintillaSettingsPath();
	Q_MONKEY_EXPORT void prepareAPIs();
	Q_MONKEY_EXPORT QsciAPIs* apisForLexer( QsciLexer* lexer );
	Q_MONKEY_EXPORT QString languageForFileName( const QString& fileName );
	Q_MONKEY_EXPORT QsciLexer* lexerForFileName( const QString& fileName );
	Q_MONKEY_EXPORT QsciLexer* lexerForLanguage( const QString& language );
	Q_MONKEY_EXPORT bool setLexerProperty( const QString& property, QsciLexer* lexer, const QVariant& value );
	Q_MONKEY_EXPORT const QVariant lexerProperty( const QString& property, QsciLexer* lexer );
	Q_MONKEY_EXPORT void resetLexer( QsciLexer* lexer );
	Q_MONKEY_EXPORT void applyProperties();
	Q_MONKEY_EXPORT void setEditorProperties( pEditor* editor );
	
	/***** GENERAL *****/
	Q_MONKEY_EXPORT void setRestoreProjectsOnStartup( bool restore );
	Q_MONKEY_EXPORT const bool restoreProjectsOnStartup();
	Q_MONKEY_EXPORT void setSaveProjectsOnCustomAction( bool save );
	Q_MONKEY_EXPORT const bool saveProjectsOnCustomAction();
	Q_MONKEY_EXPORT void setSaveFilesOnCustomAction( bool save );
	Q_MONKEY_EXPORT const bool saveFilesOnCustomAction();
	Q_MONKEY_EXPORT void setDefaultProjectsDirectory( const QString& dirName );
	Q_MONKEY_EXPORT const QString defaultProjectsDirectory();
	Q_MONKEY_EXPORT void setTabsHaveCloseButton( bool have );
	Q_MONKEY_EXPORT const bool tabsHaveCloseButton();
	Q_MONKEY_EXPORT void setTabsHaveShortcut( bool have );
	Q_MONKEY_EXPORT const bool tabsHaveShortcut();
	Q_MONKEY_EXPORT void setTabsElided( bool elided );
	Q_MONKEY_EXPORT const bool tabsElided();
	Q_MONKEY_EXPORT void setTabsTextColor( const QColor& color );
	Q_MONKEY_EXPORT const QColor tabsTextColor();
	Q_MONKEY_EXPORT void setCurrentTabTextColor( const QColor& color );
	Q_MONKEY_EXPORT const QColor currentTabTextColor();
	/*
	Q_MONKEY_EXPORT void setTabMode( pTabbedWorkspace::TabMode );
	Q_MONKEY_EXPORT const pTabbedWorkspace::TabMode tabMode();
	*/
	void setDocMode( pExtendedWorkspace::DocumentMode mode );
	const pExtendedWorkspace::DocumentMode docMode();
	Q_MONKEY_EXPORT void setExternalChanges( pMonkeyStudio::ExternalChangesMode mode );
	Q_MONKEY_EXPORT const pMonkeyStudio::ExternalChangesMode externalchanges();
	Q_MONKEY_EXPORT void setSaveSessionOnClose( bool save );
	Q_MONKEY_EXPORT const bool saveSessionOnClose();
	Q_MONKEY_EXPORT void setRestoreSessionOnStartup( bool restore );
	Q_MONKEY_EXPORT const bool restoreSessionOnStartup();
	
	/******	EDITOR ******/
	// General
	Q_MONKEY_EXPORT void setAutoSyntaxCheck( bool check );
	Q_MONKEY_EXPORT const bool autoSyntaxCheck();
	Q_MONKEY_EXPORT void setConvertTabsUponOpen( bool convert );
	Q_MONKEY_EXPORT const bool convertTabsUponOpen();
	Q_MONKEY_EXPORT void setCreateBackupUponOpen( bool backup );
	Q_MONKEY_EXPORT const bool createBackupUponOpen();
	Q_MONKEY_EXPORT void setAutoEolConversion( bool convert );
	Q_MONKEY_EXPORT const bool autoEolConversion();
	Q_MONKEY_EXPORT void setDefaultEncoding( const QString& encoding );
	Q_MONKEY_EXPORT const QString defaultEncoding();
	Q_MONKEY_EXPORT void setSelectionBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor selectionBackgroundColor();
	Q_MONKEY_EXPORT void setSelectionForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor selectionForegroundColor();
	Q_MONKEY_EXPORT void setDefaultDocumentColours( bool defaultColors );
	Q_MONKEY_EXPORT const bool defaultDocumentColours();
	Q_MONKEY_EXPORT void setDefaultDocumentPen( const QColor& defaultPen );
	Q_MONKEY_EXPORT const QColor defaultDocumentPen();
	Q_MONKEY_EXPORT void setDefaultDocumentPaper( const QColor& defaultPaper );
	Q_MONKEY_EXPORT const QColor defaultDocumentPaper();
	// Auto Completion
	Q_MONKEY_EXPORT void setAutoCompletionCaseSensitivity( bool caseSensitive );
	Q_MONKEY_EXPORT const bool autoCompletionCaseSensitivity();
	Q_MONKEY_EXPORT void setAutoCompletionReplaceWord( bool replace );
	Q_MONKEY_EXPORT const bool autoCompletionReplaceWord();
	Q_MONKEY_EXPORT void setAutoCompletionShowSingle( bool showSingle );
	Q_MONKEY_EXPORT const bool autoCompletionShowSingle();
	Q_MONKEY_EXPORT void setAutoCompletionSource( QsciScintilla::AutoCompletionSource source );
	Q_MONKEY_EXPORT const QsciScintilla::AutoCompletionSource autoCompletionSource();
	Q_MONKEY_EXPORT void setAutoCompletionThreshold( int count );
	Q_MONKEY_EXPORT const int autoCompletionThreshold();
	// CallTips
	Q_MONKEY_EXPORT void setCallTipsBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor callTipsBackgroundColor();
	Q_MONKEY_EXPORT void setCallTipsForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor callTipsForegroundColor();
	Q_MONKEY_EXPORT void setCallTipsHighlightColor( const QColor& highlight );
	Q_MONKEY_EXPORT const QColor callTipsHighlightColor();
	Q_MONKEY_EXPORT void setCallTipsStyle( QsciScintilla::CallTipsStyle style );
	Q_MONKEY_EXPORT const QsciScintilla::CallTipsStyle callTipsStyle();
	Q_MONKEY_EXPORT void setCallTipsVisible( int count );
	Q_MONKEY_EXPORT const int callTipsVisible();
	// Indentation
	Q_MONKEY_EXPORT void setAutoIndent( bool indent );
	Q_MONKEY_EXPORT const bool autoIndent();
	Q_MONKEY_EXPORT void setBackspaceUnindents( bool unindents );
	Q_MONKEY_EXPORT const bool backspaceUnindents();
	Q_MONKEY_EXPORT void setIndentationGuides( bool guides );
	Q_MONKEY_EXPORT const bool indentationGuides();
	Q_MONKEY_EXPORT void setIndentationsUseTabs( bool tabs );
	Q_MONKEY_EXPORT const bool indentationsUseTabs();
	Q_MONKEY_EXPORT void setAutoDetectIndent( bool detect );
	Q_MONKEY_EXPORT const bool autoDetectIndent();
	Q_MONKEY_EXPORT void setIndentationWidth( int width );
	Q_MONKEY_EXPORT const int indentationWidth();
	Q_MONKEY_EXPORT void setTabIndents( bool indents );
	Q_MONKEY_EXPORT const bool tabIndents();
	Q_MONKEY_EXPORT void setTabWidth( int width );
	Q_MONKEY_EXPORT const int tabWidth();
	Q_MONKEY_EXPORT void setIndentationGuidesBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor indentationGuidesBackgroundColor();
	Q_MONKEY_EXPORT void setIndentationGuidesForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor indentationGuidesForegroundColor();
	// Brace Matching
	Q_MONKEY_EXPORT void setBraceMatching( QsciScintilla::BraceMatch mode );
	Q_MONKEY_EXPORT const QsciScintilla::BraceMatch braceMatching();
	Q_MONKEY_EXPORT void setMatchedBraceBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor matchedBraceBackgroundColor();
	Q_MONKEY_EXPORT void setMatchedBraceForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor matchedBraceForegroundColor();
	Q_MONKEY_EXPORT void setUnmatchedBraceBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor unmatchedBraceBackgroundColor();
	Q_MONKEY_EXPORT void setUnmatchedBraceForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor unmatchedBraceForegroundColor();
	// Edge Mode
	Q_MONKEY_EXPORT void setEdgeMode( QsciScintilla::EdgeMode mode );
	Q_MONKEY_EXPORT const QsciScintilla::EdgeMode edgeMode();
	Q_MONKEY_EXPORT void setEdgeColor( const QColor& color );
	Q_MONKEY_EXPORT const QColor edgeColor();
	Q_MONKEY_EXPORT void setEdgeColumn( int columns );
	Q_MONKEY_EXPORT const int edgeColumn();
	// Caret
	Q_MONKEY_EXPORT void setCaretLineVisible( bool visible );
	Q_MONKEY_EXPORT const bool caretLineVisible();
	Q_MONKEY_EXPORT void setCaretLineBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor caretLineBackgroundColor();
	Q_MONKEY_EXPORT void setCaretForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor caretForegroundColor();
	Q_MONKEY_EXPORT void setCaretWidth( int width );
	Q_MONKEY_EXPORT const int caretWidth();
	// Margins
	Q_MONKEY_EXPORT void setLineNumbersMarginEnabled( bool enabled );
	Q_MONKEY_EXPORT const bool lineNumbersMarginEnabled();
	Q_MONKEY_EXPORT void setLineNumbersMarginWidth( int width );
	Q_MONKEY_EXPORT const int lineNumbersMarginWidth();
	Q_MONKEY_EXPORT void setLineNumbersMarginAutoWidth( bool width );
	Q_MONKEY_EXPORT const bool lineNumbersMarginAutoWidth();
	Q_MONKEY_EXPORT void setFolding( QsciScintilla::FoldStyle style );
	Q_MONKEY_EXPORT const QsciScintilla::FoldStyle folding();
	Q_MONKEY_EXPORT void setFoldMarginBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor foldMarginBackgroundColor();
	Q_MONKEY_EXPORT void setFoldMarginForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor foldMarginForegroundColor();
	Q_MONKEY_EXPORT void setMarginsEnabled( bool enabled );
	Q_MONKEY_EXPORT const bool marginsEnabled();
	Q_MONKEY_EXPORT void setMarginsBackgroundColor( const QColor& background );
	Q_MONKEY_EXPORT const QColor marginsBackgroundColor();
	Q_MONKEY_EXPORT void setMarginsForegroundColor( const QColor& foreground );
	Q_MONKEY_EXPORT const QColor marginsForegroundColor();
	Q_MONKEY_EXPORT void setMarginsFont( const QFont& font );
	Q_MONKEY_EXPORT const QFont marginsFont();
	// Special Characters
	Q_MONKEY_EXPORT void setEolMode( QsciScintilla::EolMode mode );
	Q_MONKEY_EXPORT const QsciScintilla::EolMode eolMode();
	Q_MONKEY_EXPORT QString getEol( QsciScintilla::EolMode mode = pMonkeyStudio::eolMode() );
	Q_MONKEY_EXPORT void setEolVisibility( bool visible );
	Q_MONKEY_EXPORT const bool eolVisibility();
	Q_MONKEY_EXPORT void setAutoDetectEol( bool detect );
	Q_MONKEY_EXPORT const bool autoDetectEol();
	Q_MONKEY_EXPORT void setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility visibility );
	Q_MONKEY_EXPORT const QsciScintilla::WhitespaceVisibility whitespaceVisibility();
	Q_MONKEY_EXPORT void setWrapMode( QsciScintilla::WrapMode mode );
	Q_MONKEY_EXPORT const QsciScintilla::WrapMode wrapMode();
	Q_MONKEY_EXPORT void setWrapVisualFlagsEnabled( bool enabled );
	Q_MONKEY_EXPORT const bool wrapVisualFlagsEnabled();
	Q_MONKEY_EXPORT void setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag flag );
	Q_MONKEY_EXPORT const QsciScintilla::WrapVisualFlag startWrapVisualFlag();
	Q_MONKEY_EXPORT void setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag flag );
	Q_MONKEY_EXPORT const QsciScintilla::WrapVisualFlag endWrapVisualFlag();
	Q_MONKEY_EXPORT void setWrappedLineIndentWidth( int witdh );
	Q_MONKEY_EXPORT const int wrappedLineIndentWidth();
};

#endif // PMONKEYSTUDIO_H
