/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pMonkeyStudio.h
** Date      : 2008-01-14T00:37:25
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
	
	Q_MONKEY_EXPORT void addTranslator( QTranslator* );
	Q_MONKEY_EXPORT void loadTranslations();

	Q_MONKEY_EXPORT void warning( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	Q_MONKEY_EXPORT void information( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	Q_MONKEY_EXPORT bool question( const QString&, const QString&, QWidget* = QApplication::activeWindow(), QMessageBox::StandardButtons = QMessageBox::Yes | QMessageBox::No, QMessageBox::StandardButton = QMessageBox::No );
	
	Q_MONKEY_EXPORT bool isSameFile( const QString&, const QString& );

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

	Q_MONKEY_EXPORT const QString tokenizeHome( const QString& );
	Q_MONKEY_EXPORT const QString unTokenizeHome( const QString& );

	Q_MONKEY_EXPORT const QHash<QString, QStringList> defaultLanguagesSuffixes();
	Q_MONKEY_EXPORT const QHash<QString, QStringList> availableLanguagesSuffixes();
	Q_MONKEY_EXPORT const QHash<QString, QStringList> availableFilesSuffixes();
	Q_MONKEY_EXPORT const QString availableLanguagesFilters();
	Q_MONKEY_EXPORT const QString availableFilesFilters();

	Q_MONKEY_EXPORT const QString settingsPath();
	Q_MONKEY_EXPORT const QString scintillaSettingsPath();
	Q_MONKEY_EXPORT void prepareAPIs();
	Q_MONKEY_EXPORT QsciAPIs* apisForLexer( QsciLexer* );
	Q_MONKEY_EXPORT QString languageForFileName( const QString& );
	Q_MONKEY_EXPORT QsciLexer* lexerForFileName( const QString& );
	Q_MONKEY_EXPORT QsciLexer* lexerForLanguage( const QString& );
	Q_MONKEY_EXPORT bool setLexerProperty( const QString&, QsciLexer*, const QVariant& );
	Q_MONKEY_EXPORT const QVariant lexerProperty( const QString&, QsciLexer* );
	Q_MONKEY_EXPORT void resetLexer( QsciLexer* l );
	Q_MONKEY_EXPORT void applyProperties();
	Q_MONKEY_EXPORT void setEditorProperties( pEditor* );
	
	/***** GENERAL *****/
	Q_MONKEY_EXPORT void setRestoreProjectsOnStartup( bool );
	Q_MONKEY_EXPORT const bool restoreProjectsOnStartup();
	Q_MONKEY_EXPORT void setSaveProjectsOnCustomAction( bool );
	Q_MONKEY_EXPORT const bool saveProjectsOnCustomAction();
	Q_MONKEY_EXPORT void setSaveFilesOnCustomAction( bool );
	Q_MONKEY_EXPORT const bool saveFilesOnCustomAction();
	Q_MONKEY_EXPORT void setDefaultProjectsDirectory( const QString& );
	Q_MONKEY_EXPORT const QString defaultProjectsDirectory();
	Q_MONKEY_EXPORT void setTabsHaveCloseButton( bool );
	Q_MONKEY_EXPORT const bool tabsHaveCloseButton();
	Q_MONKEY_EXPORT void setTabsHaveShortcut( bool );
	Q_MONKEY_EXPORT const bool tabsHaveShortcut();
	Q_MONKEY_EXPORT void setTabsElided( bool );
	Q_MONKEY_EXPORT const bool tabsElided();
	Q_MONKEY_EXPORT void setTabsTextColor( const QColor& );
	Q_MONKEY_EXPORT const QColor tabsTextColor();
	Q_MONKEY_EXPORT void setCurrentTabTextColor( const QColor& );
	Q_MONKEY_EXPORT const QColor currentTabTextColor();
	/*
	Q_MONKEY_EXPORT void setTabMode( pTabbedWorkspace::TabMode );
	Q_MONKEY_EXPORT const pTabbedWorkspace::TabMode tabMode();
	*/
	void setDocMode( pExtendedWorkspace::DocumentMode );
	const pExtendedWorkspace::DocumentMode docMode();
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
	Q_MONKEY_EXPORT void setAutoDetectIndent( bool );
	Q_MONKEY_EXPORT const bool autoDetectIndent();
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
	Q_MONKEY_EXPORT QString getEol( QsciScintilla::EolMode = pMonkeyStudio::eolMode() );
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
