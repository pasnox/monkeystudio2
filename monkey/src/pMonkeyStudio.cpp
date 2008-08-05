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
#include "pMonkeyStudio.h"
#include "maininterface/ui/UITranslator.h"
#include "pluginsmanager/PluginsManager.h"
#include "coremanager/MonkeyCore.h"
#include "queuedstatusbar/QueuedStatusBar.h"

#include "workspace/pAbstractChild.h"
#include "qscintillamanager/pEditor.h"

#include <QTextCodec>
#include <QImageReader>
#include <QFileDialog>
#include <QFileInfo>
#include <QHash>
#include <QLibraryInfo>
#include <QTranslator>
#include <QLocale>

QList<QTranslator*> mTranslators;
QHash<QString,QsciLexer*> mGlobalsLexers;
QHash<QString,QsciAPIs*> mGlobalsAPIs;

/*!
	\details Add a translator to translators list
	\param translator The translator to install
*/
void pMonkeyStudio::addTranslator( QTranslator* translator )
{
	qApp->installTranslator( translator );
	if ( !mTranslators.contains( translator ) )
		mTranslators << translator;
}

/*!
	\details Loads all needed translations
*/
void pMonkeyStudio::loadTranslations()
{
	// clear all already installed translations
	foreach ( QTranslator* t, mTranslators )
		qApp->removeTranslator( t );
	qDeleteAll( mTranslators );
	mTranslators.clear();
	
	// get user translation setted
	QString mLanguage = MonkeyCore::settings()->value( "Translations/Language", "english" ).toString();
	QLocale l( mLanguage );
	// qt translation path
	QString resourceDir = QLibraryInfo::location( QLibraryInfo::TranslationsPath );
	
	// setting qt translation
	QTranslator* qtTranslator = new QTranslator( qApp );
	if ( qtTranslator->load( QString( "qt_" ) + l.name(), resourceDir ) )
		addTranslator( qtTranslator );
	
	// setting assistant translation
	QTranslator* assistantTranslator = new QTranslator( qApp );
	if ( assistantTranslator->load( QString( "assistant_" ) + l.name(), resourceDir ) )
		addTranslator( assistantTranslator );
	
	// setting designer translation
	QTranslator* designerTranslator = new QTranslator( qApp );
	if ( designerTranslator->load( QString( "designer_" ) + l.name(), resourceDir ) )
		addTranslator( designerTranslator );
	
	if ( mLanguage != "english" )
	{
		QTranslator* t = new QTranslator( qApp );
		foreach ( const QString& path, MonkeyCore::settings()->value( "Translations/Path" ).toStringList() )
		{
			QString fn = QString( "%1/monkey_%2" ).arg( path, mLanguage );
			if ( QFileInfo( fn ).isRelative() )
				fn.prepend( QString( "%1/" ).arg( QApplication::applicationDirPath() ) );
			if ( t->load( fn ) )
			{
				addTranslator( t );
				return;
			}
		}
	}
}

/*!
	\details A short for QMessageBox::warning static method
	\param c The window title
	\param s The text to show
	\param w The parent widget defaulted to the current active window
*/
void pMonkeyStudio::warning( const QString& c, const QString& s, QWidget* w )
{ QMessageBox::warning( w, c, s ); }

/*!
	\details A short for QMessageBox::information static method
	\param c The window title
	\param s The text to show
	\param w The parent widget defaulted to the current active window
*/
void pMonkeyStudio::information( const QString& c, const QString& s, QWidget* w )
{ QMessageBox::information( w, c, s ); }

/*!
	\details A short for QMessageBox::question static method thant return true if user accept or false if he reject
	\param c The window title
	\param s The text to show
	\param w The parent widget defaulted to the current active window
	\param b The shown buttons
	\param d The default button
*/
bool pMonkeyStudio::question( const QString& c, const QString& s, QWidget* w, QMessageBox::StandardButtons b, QMessageBox::StandardButton d )
{ return QMessageBox::question( w, c, s, b, d ) == QMessageBox::Yes; }

/*!
	\details Return true if files point to same file, usefull when files are symbolic link, or windows link
	\param left The left file
	\param right The right file
*/
bool pMonkeyStudio::isSameFile( const QString& left, const QString& right )
{
	// get file info
	QFileInfo fif( left );
	QFileInfo fio( right );
	
	// check files exists
	if ( fif.exists() != fio.exists() )
		return false;
	
	// check simlink
	if ( fif.isSymLink() )
		fif.setFile( fif.symLinkTarget() );
	if ( fio.isSymLink() )
		fio.setFile( fio.symLinkTarget() );
	
	// check canonical file path
	return fif.canonicalFilePath() == fio.canonicalFilePath();
}

/*!
	\details Return a list of all know text codecs
*/
const QStringList pMonkeyStudio::availableTextCodecs()
{
	static QStringList l;
	if ( l.isEmpty() )
	{
		foreach ( QByteArray a, QTextCodec::availableCodecs() )
			l << a;
		l.sort();
	}
	return l;
}

/*!
	\details Return a list of all know image formats
*/
const QStringList pMonkeyStudio::availableImageFormats()
{
	static QStringList l;
	if ( l.isEmpty() )
	{
		foreach ( QByteArray a, QImageReader::supportedImageFormats() )
			l << a;
		l.sort();
	}
	return l;
}

/*!
	\details Return a lsit of all know languages
*/
const QStringList pMonkeyStudio::availableLanguages()
{
	static QStringList l = QStringList() << "Bash" << "Batch" << "C#" << "C++" << "CMake" << "CSS"
		<< "D" << "Diff" << "HTML" << "IDL" << "Java" << "JavaScript" << "Lua"
		<< "Makefile" << "POV" << "Perl" << "Properties" << "Python" << "Ruby"
		<< "SQL"
#if QSCINTILLA_VERSION > 0x020200
		<< "TCL"
#endif
		<< "TeX" << "VHDL";
	return l;
}

/*!
	\details Return a QFileInfoList containing folders
	\param fromDir The folder from witch to get contents
	\param filters The filters to apply for validate folders name
	\param recursive If true, the folders are scanned recursively, else not
*/
const QFileInfoList pMonkeyStudio::getFolders( QDir fromDir, const QStringList& filters, bool recursive )
{
	QFileInfoList files;
	foreach ( QFileInfo file, fromDir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name ) )
	{
		if ( filters.isEmpty() || QDir::match( filters, file.fileName() ) )
			files << file;
		if ( recursive )
		{
			fromDir.cd( file.filePath() );
			files << getFolders( fromDir, filters, recursive );
			fromDir.cdUp();
		}
	}
	return files;
}

/*!
	\details Return a QFileInfoList containing files
	\param fromDir The folder from witch to get contents
	\param filters The filters to apply for validate files name
	\param recursive If true, the folders are scanned recursively, else not
*/
const QFileInfoList pMonkeyStudio::getFiles( QDir fromDir, const QStringList& filters, bool recursive )
{
	QFileInfoList files;
	foreach ( QFileInfo file, fromDir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name ) )
	{
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) )
			files << file;
		else if ( file.isDir() && recursive )
		{
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters );
			fromDir.cdUp();
		}
	}
	return files;
}

/*!
	\details Return a QFileInfoList containing files
	\param fromDir The folder from witch to get contents
	\param filters The filters to apply for validate files name
	\param recursive If true, the folders are scanned recursively, else not
*/
const QFileInfoList pMonkeyStudio::getFiles( QDir fromDir, const QString& filter, bool recursive )
{ return getFiles( fromDir, filter.isEmpty() ? QStringList() : QStringList( filter ), recursive ); }

QFileDialog* getOpenDialog( QFileDialog::FileMode fileMode, const QString& caption, const QString& fileName, const QString& filter, QWidget* parent, QFileDialog::AcceptMode acceptMode = QFileDialog::AcceptOpen )
{
	// create dialg
	QFileDialog* dlg = new QFileDialog( parent, caption, fileName, filter );
	// set file accept mode
	dlg->setAcceptMode( acceptMode );
	// set file mode
	dlg->setFileMode( fileMode );
	// return dialog
	return dlg;
}

/*!
	\details A dialog that return a list of files name choosen from available know image formats
	\param caption The window title
	\param fileName The default filename to select ( or path )
	\param parent The parent widget
*/
const QStringList pMonkeyStudio::getImageFileNames( const QString& caption, const QString& fileName, QWidget* parent )
{
	// get image filters
	QStringList filters;
	foreach ( QString filter, availableImageFormats() )
		filters << QObject::tr( "%1 Files (*.%2)" ).arg( filter.toUpper() ).arg( filter );
	// add all format as one filter at begining
	if ( !filters.isEmpty() )
		filters.prepend( QObject::tr( "All Image Files (%1)" ).arg( QStringList( availableImageFormats() ).replaceInStrings( QRegExp( "^(.*)$" ), "*.\\1" ).join( " " ) ) );
	// create dialog
	QFileDialog* dlg = getOpenDialog( QFileDialog::ExistingFiles, caption.isEmpty() ? QObject::tr( "Select image(s)" ) : caption, fileName, filters.join( ";;" ), parent );
	// choose last used filter if available
	if ( !filters.isEmpty() )
		dlg->selectFilter( MonkeyCore::settings()->value( "Recents/ImageFilter" ).toString() );
	// execute dialog
	if ( dlg->exec() )
	{
		// remember last filter if available
		if ( !filters.isEmpty() )
			MonkeyCore::settings()->setValue( "Recents/ImageFilter", dlg->selectedFilter() );
		// remember selected files
		filters = dlg->selectedFiles();
		// delete dialog
		delete dlg;
		// return selected files
		return filters;
	}
	// delete dialog
	delete dlg;
	// return empty list
	return QStringList();
}

/*!
	\details A dialog that return a file name choosen from available know image formats
	\param caption The window title
	\param fileName The default filename to select ( or path )
	\param parent The parent widget
*/
const QString pMonkeyStudio::getImageFileName( const QString& caption, const QString& fileName, QWidget* parent )
{ return getImageFileNames( caption, fileName, parent ).value( 0 ); }

/*!
	\details Return a QStringList of files name
	\param caption The window title
	\param fileName The default filename to select ( or path )
	\param filter The filter to apply
	\param parent The parent widget
*/
const QStringList pMonkeyStudio::getOpenFileNames( const QString& caption, const QString& fileName, const QString& filter, QWidget* parent )
{
	// create dialg
	QFileDialog* dlg = getOpenDialog( QFileDialog::ExistingFiles, caption.isEmpty() ? QObject::tr( "Select file(s)" ) : caption, fileName, filter, parent );
	// choose last used filter if available
	if ( !filter.isEmpty() )
		dlg->selectFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
	// execute dialog
	if ( dlg->exec() )
	{
		// remember last filter if available
		if ( !filter.isEmpty() )
			MonkeyCore::settings()->setValue( "Recents/FileFilter", dlg->selectedFilter() );
		// remember selected files
		QStringList files = dlg->selectedFiles();
		// delete dialog
		delete dlg;
		// return selected files
		return files;
	}
	// delete dialog
	delete dlg;
	// return empty list
	return QStringList();
}

/*!
	\details Return a QString file name
	\param caption The window title
	\param fileName The default filename to select ( or path )
	\param filter The filter to apply
	\param parent The parent widget
*/
const QString pMonkeyStudio::getOpenFileName( const QString& caption, const QString& fileName, const QString& filter, QWidget* parent )
{ return getOpenFileNames( caption, fileName, filter, parent ).value( 0 ); }

/*!
	\details Return a QString file name
	\param caption The window title
	\param fileName The default filename to select ( or path )
	\param filter The filter to apply
	\param parent The parent widget
*/
const QString pMonkeyStudio::getSaveFileName( const QString& caption, const QString& fileName, const QString& filter, QWidget* parent )
{
	// create dialg
	QFileDialog* dlg = getOpenDialog( QFileDialog::AnyFile, caption.isEmpty() ? QObject::tr( "Choose a filename" ) : caption, fileName, filter, parent, QFileDialog::AcceptSave );
	// choose last used filter if available
	if ( !filter.isEmpty() )
		dlg->selectFilter( MonkeyCore::settings()->value( "Recents/FileFilter" ).toString() );
	// execute dialog
	if ( dlg->exec() )
	{
		// remember last filter if available
		if ( !filter.isEmpty() )
			MonkeyCore::settings()->setValue( "Recents/FileFilter", dlg->selectedFilter() );
		// remember selected files
		QStringList files = dlg->selectedFiles();
		// delete dialog
		delete dlg;
		// return selected files
		return files.value( 0 );
	}
	// delete dialog
	delete dlg;
	// return empty list
	return QString();
}

/*!
	\details Convenient function for get an existing folder
	\param caption The window title
	\param fileName The default path to shown
	\param parent The parent widget
*/
const QString pMonkeyStudio::getExistingDirectory( const QString& caption, const QString& fileName, QWidget* parent )
{ return QFileDialog::getExistingDirectory( parent, caption.isEmpty() ? QObject::tr( "Select a folder" ) : caption, fileName ); }

/*!
	\details Return a tokenized string, ie: the token $HOME$ is replaced by the home path of the user
	\param string The string to tokenize
*/
const QString pMonkeyStudio::tokenizeHome( const QString& string )
{ return QString( string ).replace( QDir::homePath(), "$HOME$" ); }

/*!
	\details Return an untokenized string, ie: the home path is replaced by $HOME$
	\param string The string to untokenize
*/
const QString pMonkeyStudio::unTokenizeHome( const QString& string )
{ return QString( string ).replace( "$HOME$", QDir::homePath() ); }

/*!
	\details Return all default languages suffixes
*/
const QHash<QString, QStringList> pMonkeyStudio::defaultLanguagesSuffixes()
{
	// suffixes list
	QHash<QString, QStringList> l;
	// Bash
	l["Bash"] << "*.sh";
	// Batch
	l["Batch"] << "*.bat";
	l["Batch"] << "*.cmd";
	// C#
	l["C#"] << "*.cs";
	// C++
	l["C++"] << "*.c";
	l["C++"] << "*.cc";
	l["C++"] << "*.cpp";
	l["C++"] << "*.cxx";
	l["C++"] << "*.c++";
	l["C++"] << "*.h";
	l["C++"] << "*.hh";
	l["C++"] << "*.hpp";
	l["C++"] << "*.hxx";
	l["C++"] << "*.h++";
	// CMake
	l["CMake"] << "*.cmake";
	l["CMake"] << "CMake.txt";
	// CSS
	l["CSS"] << "*.css";
	// D
	l["D"] << "*.d";
	// Diff
	l["Diff"] << "*.diff";
	l["Diff"] << "*.patch";
	// HTML
	l["HTML"] << "*.asp";
	l["HTML"] << "*.xml";
	l["HTML"] << "*.xsd";
	l["HTML"] << "*.xsl";
	l["HTML"] << "*.xslt";
	l["HTML"] << "*.docbook";
	l["HTML"] << "*.dtd";
	l["HTML"] << "*.htm";
	l["HTML"] << "*.html";
	l["HTML"] << "*.php";
	l["HTML"] << "*.php3";
	l["HTML"] << "*.php4";
	l["HTML"] << "*.php5";
	l["HTML"] << "*.phtml";
	l["HTML"] << "*.rdf";
	l["HTML"] << "*.svg";
	l["HTML"] << "*.shtml";
	// IDL
	l["IDL"] << "*.idl";
	// Java
	l["Java"] << "*.java";
	// JavaScript
	l["JavaScript"] << "*.js";
	// Lua
	l["Lua"] << "*.lua";
	// Makefile
	l["Makefile"] << "*.mak";
	l["Makefile"] << "*makefile";
	l["Makefile"] << "Makefile*";
	// POV
	l["POV"] << "*.pov";
	// Perl
	l["Perl"] << "*.ph";
	l["Perl"] << "*.pl";
	l["Perl"] << "*.pm";
	// Properties
	l["Properties"] << "*.cfg";
	l["Properties"] << "*.cnf";
	l["Properties"] << "*.inf";
	l["Properties"] << "*.ini";
	l["Properties"] << "*.properties";
	l["Properties"] << "*.rc";
	l["Properties"] << "*.reg";
	// Python
	l["Python"] << "*.ptl";
	l["Python"] << "*.py";
	l["Python"] << "*.pyw";
	l["Python"] << "*.pyx";
	// Ruby
	l["Ruby"] << "*.rb";
	l["Ruby"] << "*.rbw";
	// SQL
	l["SQL"] << "*.sql";
#if QSCINTILLA_VERSION > 0x020200
	// TCL
	l["TCL"] << "*.tcl";
#endif
	// TeX
	l["TeX"] << "*.aux";
	l["TeX"] << "*.idx";
	l["TeX"] << "*.sty";
	l["TeX"] << "*.toc";
	// VHDL
	l["VHDL"] << "*.vhdl";
	// return list
	return l;
}

/*!
	\details Return all available languages suffixes
*/
const QHash<QString, QStringList> pMonkeyStudio::availableLanguagesSuffixes()
{
	// suffixes list
	QHash<QString, QStringList> l;
	// get settings
	pSettings* s = MonkeyCore::settings();
	// get associations from settings
	s->beginGroup( "LexersAssociations" );
	foreach ( QString k, s->childKeys() )
		l[s->value( k ).toString()] << k;
	s->endGroup();
	// if empty get default suffixes
	if ( l.isEmpty() )
		l = defaultLanguagesSuffixes();
	// return list
	return l;
}

/*!
	\details Return all available files suffixes
*/
const QHash<QString, QStringList> pMonkeyStudio::availableFilesSuffixes()
{
	// get language suffixes
	QHash<QString, QStringList> l = availableLanguagesSuffixes();
	// add child plugins suffixes
	QHash<QString, QStringList> ps = MonkeyCore::pluginsManager()->childSuffixes();
	foreach ( QString k, ps.keys() )
		foreach ( QString s, ps[k] )
			if ( !l[k].contains( s ) )
				l[k] << s;
	// return list
	return l;
}

/*!
	\details Return all available languages filters
*/
const QString pMonkeyStudio::availableLanguagesFilters()
{
	QString f;
	// get suffixes
	QHash<QString, QStringList> sl = availableLanguagesSuffixes();
	//
	foreach ( QString k, sl.keys() )
		f += QString( "%1 Files (%2);;" ).arg( k ).arg( sl.value( k ).join( " " ) );
	// remove trailing ;;
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );
	// return filters list
	return f;
}

/*!
	\details Return all available files filters
*/
const QString pMonkeyStudio::availableFilesFilters()
{
	QString f;
	// get suffixes
	QHash<QString, QStringList> sl = availableFilesSuffixes();
	//
	foreach ( QString k, sl.keys() )
		f += QString( "%1 Files (%2);;" ).arg( k ).arg( sl.value( k ).join( " " ) );
	// remove trailing ;;
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );
	// return filters list
	return f;
}

/*!
	\details Return the base settings path used by all option in this namespace
*/
const QString pMonkeyStudio::settingsPath()
{
	return "/Settings";
}

/*!
	\details Return the scintilla settigns path
*/
const QString pMonkeyStudio::scintillaSettingsPath()
{
	return "/Scintilla";
}

/*!
	\details Reload all available api files
*/
void pMonkeyStudio::prepareAPIs()
{
	// check lexers & apis
	if ( mGlobalsLexers.isEmpty() || mGlobalsAPIs.isEmpty() )
	{
		// free hashes
		qDeleteAll( mGlobalsLexers );
		mGlobalsLexers.clear();
		qDeleteAll( mGlobalsAPIs );
		mGlobalsAPIs.clear();
	}
	// get monkey status bar
	QueuedStatusBar* sbar = MonkeyCore::statusBar();
	// iterate each language
	foreach ( QString ln, availableLanguages() )
	{
		QsciLexer* l = lexerForLanguage( ln );
		QsciAPIs* a = apisForLexer( l );
		// clear raw api
		a->clear();
		// load prepared files
		foreach ( QString f, MonkeyCore::settings()->value( QString( "SourceAPIs/" ).append( ln ) ).toStringList() )
		{
			if ( !a->load( QDir::isRelativePath( f ) ? qApp->applicationDirPath().append( "/%1" ).arg( f ) : f ) )
				sbar->appendMessage( QObject::tr( "Can't load api file: '%1'" ).arg( QFileInfo( f ).fileName() ), 2000 );
		}
		// start prepare for apis
		a->prepare();
	}
}

/*!
	\details Return a QsciAPIs for the given lexer
	\param lexer The lexer to get apis object
*/
QsciAPIs* pMonkeyStudio::apisForLexer( QsciLexer* lexer )
{
	// cancel if no lexer
	if ( !lexer )
		return 0;
	// check if apis already exists
	if ( !mGlobalsAPIs.contains( lexer->language() ) )
	{
		// create apis
		QsciAPIs* apis = new QsciAPIs( lexer );
		// store global apis
		mGlobalsAPIs[lexer->language()] = apis;
	}
	// return apis
	return mGlobalsAPIs.value( lexer->language() );
}

/*!
	\details Return the language of a file name
	\param fileName The fil name to get language from
*/
QString pMonkeyStudio::languageForFileName( const QString& fileName )
{
	QsciLexer* lexer = lexerForFileName( fileName );
	return lexer ? QString( lexer->language() ) : QString();
}

/*!
	\details Return a QsciLexer for the given file name
	\param fileName The filenae to get lexer from
*/
QsciLexer* pMonkeyStudio::lexerForFileName( const QString& fileName )
{
	// get suffixes
	QHash<QString, QStringList> l = availableFilesSuffixes();
	// check suffixe
	foreach ( QString k, l.keys() )
		if ( QDir::match( l.value( k ), fileName ) )
			return lexerForLanguage( k );
	return 0;
}

/*!
	\details Return a QsciLexer for the given language
	\param language The language to get lexer from
*/
QsciLexer* pMonkeyStudio::lexerForLanguage( const QString& language )
{
	if ( mGlobalsLexers.keys().contains( language ) )
		return mGlobalsLexers.value( language );
	// get language
	const QString ln = language.toLower();
	// lexer
	QsciLexer* l = 0;
	// init lexer
	if ( ln == "bash" )
		l = new QsciLexerBash( QApplication::instance() );
	else if ( ln == "batch" )
		l = new QsciLexerBatch( QApplication::instance() );
	else if ( ln == "c#" )
		l = new QsciLexerCSharp( QApplication::instance() );
	else if ( ln == "c++" )
		l = new QsciLexerCPP( QApplication::instance() );
	else if ( ln == "cmake" )
		l = new QsciLexerCMake( QApplication::instance() );
	else if ( ln == "css" )
		l = new QsciLexerCSS( QApplication::instance() );
	else if ( ln == "d" )
		l = new QsciLexerD( QApplication::instance() );
	else if ( ln == "diff" )
		l = new QsciLexerDiff( QApplication::instance() );
	else if ( ln == "html" )
		l = new QsciLexerHTML( QApplication::instance() );
	else if ( ln == "idl" )
		l = new QsciLexerIDL( QApplication::instance() );
	else if ( ln == "java" )
		l = new QsciLexerJava( QApplication::instance() );
	else if ( ln == "javascript" )
		l = new QsciLexerJavaScript( QApplication::instance() );
	else if ( ln == "lua" )
		l = new QsciLexerLua( QApplication::instance() );
	else if ( ln == "makefile" )
		l = new QsciLexerMakefile( QApplication::instance() );
	else if ( ln == "pov" )
		l = new QsciLexerPOV( QApplication::instance() );
	else if ( ln == "perl" )
		l = new QsciLexerPerl( QApplication::instance() );
	else if ( ln == "properties" )
		l = new QsciLexerProperties( QApplication::instance() );
	else if ( ln == "python" )
		l = new QsciLexerPython( QApplication::instance() );
	else if ( ln == "ruby" )
		l = new QsciLexerRuby( QApplication::instance() );
	else if ( ln == "sql" )
		l = new QsciLexerSQL( QApplication::instance() );
#if QSCINTILLA_VERSION > 0x020200
	else if ( ln == "tcl" )
		l = new QsciLexerTCL( QApplication::instance() );
#endif
	else if ( ln == "tex" )
		l = new QsciLexerTeX( QApplication::instance() );
	else if ( ln == "vhdl" )
		l = new QsciLexerVHDL( QApplication::instance() );
	// init lexer settings
	if ( l )
	{
		// add lexer to global lexers hash
		mGlobalsLexers[l->language()] = l;
		// read settings
		pSettings* ss = MonkeyCore::settings();
		l->readSettings( *ss, qPrintable( scintillaSettingsPath() ) );
		// set apis
		l->setAPIs( apisForLexer( l ) );
	}
	// return lexer
	return l;
}

/*!
	\details Return true if can set \c property for \c lexer to \c value else return false
	\param property The property name
	\param lexer The lexer to modify
	\param value The value to set
*/
bool pMonkeyStudio::setLexerProperty( const QString& property, QsciLexer* lexer, const QVariant& value )
{
	// cancel no property, no lexer or if variant is not valid
	if ( !lexer || property.trimmed().isEmpty() || !value.isValid() )
		return false;
	// if bool
	if ( value.type() == QVariant::Bool )
		return QMetaObject::invokeMethod( lexer, qPrintable( property ), Q_ARG( bool, value.toBool() ) );
	// if int
	else if ( value.type() == QVariant::Int )
		return QMetaObject::invokeMethod( lexer, qPrintable( property ), Q_ARG( QsciLexerPython::IndentationWarning, (QsciLexerPython::IndentationWarning)value.toInt() ) );
	// return default value
	return false;
}

/*!
	\details Return a lexer property value
	\param property The property to query
	\param lexer The lexer to query property from
*/
const QVariant pMonkeyStudio::lexerProperty( const QString& property, QsciLexer* lexer )
{
	// if no member or lexer return null variant
	if ( !lexer || property.isEmpty() )
		return QVariant();
	// get language
	const QString lng = QString( lexer->language() );
	// checking property
	if ( property == "foldComments" )
	{
		if ( lng == "Bash" )
			return qobject_cast<QsciLexerBash*>( lexer )->foldComments();
		else if ( lng == "CSS" )
			return qobject_cast<QsciLexerCSS*>( lexer )->foldComments();
		else if ( lng == "D" )
			return qobject_cast<QsciLexerD*>( lexer )->foldComments();
		else if ( lng == "Perl" )
			return qobject_cast<QsciLexerPerl*>( lexer )->foldComments();
		else if ( lng == "POV" )
			return qobject_cast<QsciLexerPOV*>( lexer )->foldComments();
		else if ( lng == "Python" )
			return qobject_cast<QsciLexerPython*>( lexer )->foldComments();
		else if ( lng == "SQL" )
			return qobject_cast<QsciLexerSQL*>( lexer )->foldComments();
		else if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( lexer )->foldComments();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( lexer )->foldComments();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( lexer )->foldComments();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( lexer )->foldComments();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( lexer )->foldComments();
	}
	else if ( property == "foldCompact" )
	{
		if ( lng == "Bash" )
			return qobject_cast<QsciLexerBash*>( lexer )->foldCompact();
		else if ( lng == "CSS" )
			return qobject_cast<QsciLexerCSS*>( lexer )->foldCompact();
		else if ( lng == "D" )
			return qobject_cast<QsciLexerD*>( lexer )->foldCompact();
		else if ( lng == "HTML" )
			return qobject_cast<QsciLexerHTML*>( lexer )->foldCompact();
		else if ( lng == "Lua" )
			return qobject_cast<QsciLexerLua*>( lexer )->foldCompact();
		else if ( lng == "Perl" )
			return qobject_cast<QsciLexerPerl*>( lexer )->foldCompact();
		else if ( lng == "POV" )
			return qobject_cast<QsciLexerPOV*>( lexer )->foldCompact();
		else if ( lng == "Properties" )
			return qobject_cast<QsciLexerProperties*>( lexer )->foldCompact();
		else if ( lng == "SQL" )
			return qobject_cast<QsciLexerSQL*>( lexer )->foldCompact();
		else if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( lexer )->foldCompact();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( lexer )->foldCompact();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( lexer )->foldCompact();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( lexer )->foldCompact();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( lexer )->foldCompact();
#if QSCINTILLA_VERSION > 0x020200
		else if ( lng == "TCL" )
			return qobject_cast<QsciLexerTCL*>( lexer )->foldCompact();
#endif
	}
	else if ( property == "foldQuotes" )
	{
		if ( lng == "Python" )
			return qobject_cast<QsciLexerPython*>( lexer )->foldQuotes();
	}
	else if ( property == "foldDirectives" )
	{
		if ( lng == "POV" )
			return qobject_cast<QsciLexerPOV*>( lexer )->foldDirectives();
	}
	else if ( property == "foldAtBegin" )
	{
		if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( lexer )->foldAtBegin();
	}
	else if ( property == "foldAtParenthesis" )
	{
		if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( lexer )->foldAtParenthesis();
	}
	else if ( property == "foldAtElse" )
	{
		if ( lng == "CMake" )
			return qobject_cast<QsciLexerCMake*>( lexer )->foldAtElse();
		else if ( lng == "D" )
			return qobject_cast<QsciLexerD*>( lexer )->foldAtElse();
		else if ( lng == "VHDL" )
			return qobject_cast<QsciLexerVHDL*>( lexer )->foldAtElse();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( lexer )->foldAtElse();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( lexer )->foldAtElse();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( lexer )->foldAtElse();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( lexer )->foldAtElse();
	}
	else if ( property == "foldPreprocessor" )
	{
		if ( lng == "HTML" )
			return qobject_cast<QsciLexerHTML*>( lexer )->foldPreprocessor();
		else if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( lexer )->foldPreprocessor();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( lexer )->foldPreprocessor();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( lexer )->foldPreprocessor();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( lexer )->foldPreprocessor();
	}
	else if ( property == "stylePreprocessor" )
	{
		if ( lng == "JavaScript" )
			return qobject_cast<QsciLexerJavaScript*>( lexer )->stylePreprocessor();
		else if ( lng == "Java" )
			return qobject_cast<QsciLexerJava*>( lexer )->stylePreprocessor();
		else if ( lng == "C#" )
			return qobject_cast<QsciLexerCSharp*>( lexer )->stylePreprocessor();
		else if ( lng == "C++" )
			return qobject_cast<QsciLexerCPP*>( lexer )->stylePreprocessor();
	}
	else if ( property == "caseSensitiveTags" )
	{
		if ( lng == "HTML" )
			return qobject_cast<QsciLexerHTML*>( lexer )->caseSensitiveTags();
	}
	else if ( property == "backslashEscapes" )
	{
		if ( lng == "SQL" )
			return qobject_cast<QsciLexerSQL*>( lexer )->backslashEscapes();
	}
	else if ( property == "indentationWarning" )
	{
		if ( lng == "Python" )
			return qobject_cast<QsciLexerPython*>( lexer )->indentationWarning();
	}
	// default return value
	return QVariant();
}

/*!
	\details Reset the properties of a lexer
	\param lexer The lexer to reset
*/
void pMonkeyStudio::resetLexer( QsciLexer* lexer )
{
	// cancel if no lexer
	if ( !lexer )
		return;
	// get settings pointer
	pSettings* settings = MonkeyCore::settings();
	// remove lexer entry
	settings->remove( QString( "%1/%2" ).arg( scintillaSettingsPath() ).arg( lexer->language() ) );
	// set default styles
	for ( int i = 0; i < 128; ++i )
	{
		if ( !lexer->description( i ).isEmpty() )
		{
			lexer->setColor( lexer->defaultColor( i ), i );
			lexer->setEolFill( lexer->defaultEolFill( i ), i );
			lexer->setFont( lexer->defaultFont( i ), i );
			lexer->setPaper( lexer->defaultPaper( i ), i );
		}
	}
	// re read properties
	lexer->readSettings( *settings, qPrintable( scintillaSettingsPath() ) );
}

/*!
	\details Apply the settings ( after having pressed apply/ok in the settings dialog )
*/
void pMonkeyStudio::applyProperties()
{
	// apply editor properties
	foreach ( pAbstractChild* c, MonkeyCore::workspace()->children() )
		foreach ( pEditor* e, c->findChildren<pEditor*>() )
			setEditorProperties( e );
	// apply lexers properties
	pSettings* ss = MonkeyCore::settings();
	foreach ( QsciLexer* l, mGlobalsLexers.values() )
	{
		// refresh properties
		l->readSettings( *ss, qPrintable( scintillaSettingsPath() ) );
		// refresh default pen/paper if needed
		if ( defaultDocumentColours() )
		{
			l->setDefaultColor( defaultDocumentPen() );
			l->setDefaultPaper( defaultDocumentPaper() );
		}
	}
	// reloads apis
	prepareAPIs();
}

/*!
	\details Apply properties to the given \c editor
	\param editor The editor to set properties
*/
void pMonkeyStudio::setEditorProperties( pEditor* editor )
{
	if ( !editor )
		return;
	// apply settings from UISettings
	// General
	editor->setSelectionBackgroundColor( selectionBackgroundColor() );
	editor->setSelectionForegroundColor( selectionForegroundColor() );
	if ( defaultDocumentColours() )
	{
		// set scintilla default colors
		editor->setColor( defaultDocumentPen() );
		editor->setPaper( defaultDocumentPaper() );
	}
	// Auto Completion
	editor->setAutoCompletionCaseSensitivity( autoCompletionCaseSensitivity() );
	editor->setAutoCompletionReplaceWord( autoCompletionReplaceWord() );
	editor->setAutoCompletionShowSingle( autoCompletionShowSingle() );
	editor->setAutoCompletionSource( autoCompletionSource() );
	editor->setAutoCompletionThreshold( autoCompletionThreshold() );
	// CallTips
	editor->setCallTipsBackgroundColor( callTipsBackgroundColor() );
	editor->setCallTipsForegroundColor( callTipsForegroundColor() );
	editor->setCallTipsHighlightColor( callTipsHighlightColor() );
	editor->setCallTipsStyle( callTipsStyle() );
	editor->setCallTipsVisible( callTipsVisible() );
	// Indentation
	editor->setAutoIndent( autoIndent() );
	editor->setBackspaceUnindents( backspaceUnindents() );
	editor->setIndentationGuides( indentationGuides() );
	editor->setIndentationsUseTabs( indentationsUseTabs() );
	editor->setIndentationWidth( indentationWidth() );
	editor->setTabIndents( tabIndents() );
	editor->setTabWidth( tabWidth() );
	editor->setIndentationGuidesBackgroundColor( indentationGuidesBackgroundColor() );
	editor->setIndentationGuidesForegroundColor( indentationGuidesForegroundColor() );
	// Brace Matching
	editor->setBraceMatching( braceMatching() );
	editor->setMatchedBraceBackgroundColor( matchedBraceBackgroundColor() );
	editor->setMatchedBraceForegroundColor( matchedBraceForegroundColor() );
	editor->setUnmatchedBraceBackgroundColor( unmatchedBraceBackgroundColor() );
	editor->setUnmatchedBraceForegroundColor( unmatchedBraceForegroundColor() );
	// Edge Mode
	editor->setEdgeMode( edgeMode() );
	editor->setEdgeColor( edgeColor() );
	editor->setEdgeColumn( edgeColumn() );
	// Caret
	editor->setCaretLineVisible( caretLineVisible() );
	editor->setCaretLineBackgroundColor( caretLineBackgroundColor() );
	editor->setCaretForegroundColor( caretForegroundColor() );
	editor->setCaretWidth( caretWidth() );
	// Margins
	if ( marginsEnabled() )
	{
		editor->setMarginsBackgroundColor( marginsBackgroundColor() );
		editor->setMarginsForegroundColor( marginsForegroundColor() );
		editor->setMarginsFont( marginsFont() );
	}
	editor->setLineNumbersMarginEnabled( lineNumbersMarginEnabled() );
	editor->setLineNumbersMarginWidth( lineNumbersMarginWidth() );
	editor->setLineNumbersMarginAutoWidth( lineNumbersMarginAutoWidth() );
	editor->setFolding( folding() );
	editor->setFoldMarginColors( foldMarginForegroundColor(), foldMarginBackgroundColor() );
	// Special Characters
	editor->setEolMode( eolMode() );
	editor->setEolVisibility( eolVisibility() );
	editor->setWhitespaceVisibility( whitespaceVisibility() );
	editor->setWrapMode( wrapMode() );
	editor->setWrapVisualFlags( endWrapVisualFlag(), startWrapVisualFlag(), wrappedLineIndentWidth() );
}

/*!
	\details Restore projects at startup
	\param restore True to restore, else false
*/
void pMonkeyStudio::setRestoreProjectsOnStartup( bool restore )
{ MonkeyCore::settings()->setValue( settingsPath() +"/RestoreProjectsOnStartup", restore ); }

/*!
	\details Return true if projects are restored at startup, else false
*/
const bool pMonkeyStudio::restoreProjectsOnStartup()
{ return MonkeyCore::settings()->value( settingsPath() +"/RestoreProjectsOnStartup", true ).toBool(); }

/*!
	\details Save projects on custom actions triggered ( builder, compiler, debugger, interpreter )
	\param save True to save, else false
*/
void pMonkeyStudio::setSaveProjectsOnCustomAction( bool save )
{ MonkeyCore::settings()->setValue( settingsPath() +"/SaveProjectsOnCustomAction", save ); }

/*!
	\details Return true if projects are saved on custom actions triggered, else false
*/
const bool pMonkeyStudio::saveProjectsOnCustomAction()
{ return MonkeyCore::settings()->value( settingsPath() +"/SaveProjectsOnCustomAction", false ).toBool(); }

/*!
	\details Save files on custom actions triggered ( builder, compiler, debugger, interpreter )
	\param save True to save, else false
*/
void pMonkeyStudio::setSaveFilesOnCustomAction( bool save )
{ MonkeyCore::settings()->setValue( settingsPath() +"/SaveFilesOnCustomAction", save ); }

/*!
	\details Return true if files are saved on custom actions triggered, else false
*/
const bool pMonkeyStudio::saveFilesOnCustomAction()
{ return MonkeyCore::settings()->value( settingsPath() +"/SaveFilesOnCustomAction", false ).toBool(); }

/*!
	\details Set the default project path
	\param path The default path
*/
void pMonkeyStudio::setDefaultProjectsDirectory( const QString& path )
{ MonkeyCore::settings()->setValue( settingsPath() +"/DefaultProjectsDirectory", path ); }

/*!
	\details Return the default project path
*/
const QString pMonkeyStudio::defaultProjectsDirectory()
{ return MonkeyCore::settings()->value( settingsPath() +"/DefaultProjectsDirectory", "$HOME$/.Monkey Studio/Projects" ).toString(); }

/*!
	\details Set if tabs have close button
	\param have True to have button, else false
*/
void pMonkeyStudio::setTabsHaveCloseButton( bool have )
{ MonkeyCore::settings()->setValue( settingsPath() +"/TabsHaveCloseButton", have ); }

/*!
	\details Return if tabs have  close button
*/
const bool pMonkeyStudio::tabsHaveCloseButton()
{ return MonkeyCore::settings()->value( settingsPath() +"/TabsHaveCloseButton", false ).toBool(); }

/*!
	\details Set tabs have shortcut
	\param have True for shortcut, else false
*/
void pMonkeyStudio::setTabsHaveShortcut( bool have )
{ MonkeyCore::settings()->setValue( settingsPath() +"/TabsHaveShortcut", have ); }

/*!
	\details Return true if tabs have shortcut, else false
*/
const bool pMonkeyStudio::tabsHaveShortcut()
{ return MonkeyCore::settings()->value( settingsPath() +"/TabsHaveShortcut", false ).toBool(); }

/*!
	\details Set tabs text are elided
	\param elided True for elided text, else false
*/
void pMonkeyStudio::setTabsElided( bool have )
{ MonkeyCore::settings()->setValue( settingsPath() +"/TabsElided", have ); }

const bool pMonkeyStudio::tabsElided()
{ return MonkeyCore::settings()->value( settingsPath() +"/TabsElided", false ).toBool(); }

void pMonkeyStudio::setTabsTextColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/TabsTextColor", c ); }

const QColor pMonkeyStudio::tabsTextColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/TabsTextColor", QColor( Qt::black ) ).value<QColor>(); }

void pMonkeyStudio::setCurrentTabTextColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CurrentTabTextColor", c ); }

const QColor pMonkeyStudio::currentTabTextColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/CurrentTabTextColor", QColor( Qt::blue ) ).value<QColor>(); }

/*
void pMonkeyStudio::setTabMode( pTabbedWorkspace::TabMode m )
{ MonkeyCore::settings()->setValue( settingsPath() +"/TabMode", m ); }

const pTabbedWorkspace::TabMode pMonkeyStudio::tabMode()
{ return (pTabbedWorkspace::TabMode)MonkeyCore::settings()->value( settingsPath() +"/TabMode", pTabbedWorkspace::tmMDI ).toInt(); }
*/

void pMonkeyStudio::setDocMode( pExtendedWorkspace::DocumentMode m )
{ MonkeyCore::settings()->setValue( settingsPath() +"/DocMode", m ); }

const pExtendedWorkspace::DocumentMode pMonkeyStudio::docMode()
{ return (pExtendedWorkspace::DocumentMode)MonkeyCore::settings()->value( settingsPath() +"/DocMode", pExtendedWorkspace::dmSDI ).toInt(); }

void pMonkeyStudio::setExternalChanges( pMonkeyStudio::ExternalChangesMode e )
{ MonkeyCore::settings()->setValue( settingsPath() +"/ExternalChanges", e ); }

const pMonkeyStudio::ExternalChangesMode pMonkeyStudio::externalchanges()
{ return (pMonkeyStudio::ExternalChangesMode)MonkeyCore::settings()->value( settingsPath() +"/ExternalChanges", pMonkeyStudio::ecmAlert ).toInt(); }

void pMonkeyStudio::setSaveSessionOnClose( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/SaveSessionOnClose", b ); }

const bool pMonkeyStudio::saveSessionOnClose()
{ return MonkeyCore::settings()->value( settingsPath() +"/SaveSessionOnClose", true ).toBool(); }

void pMonkeyStudio::setRestoreSessionOnStartup( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/RestoreSessionOnStartup", b ); }

const bool pMonkeyStudio::restoreSessionOnStartup()
{ return MonkeyCore::settings()->value( settingsPath() +"/RestoreSessionOnStartup", true ).toBool(); }

void pMonkeyStudio::setAutoSyntaxCheck( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoSyntaxCheck", b ); }

const bool pMonkeyStudio::autoSyntaxCheck()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoSyntaxCheck", false ).toBool(); }

void pMonkeyStudio::setConvertTabsUponOpen( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/ConvertTabsUponOpen", b ); }

const bool pMonkeyStudio::convertTabsUponOpen()
{ return MonkeyCore::settings()->value( settingsPath() +"/ConvertTabsUponOpen", false ).toBool(); }

void pMonkeyStudio::setCreateBackupUponOpen( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CreateBackupUponOpen", b ); }

const bool pMonkeyStudio::createBackupUponOpen()
{ return MonkeyCore::settings()->value( settingsPath() +"/CreateBackupUponOpen", false ).toBool(); }

void pMonkeyStudio::setAutoEolConversion( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoEolConversion", b ); }

const bool pMonkeyStudio::autoEolConversion()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoEolConversion", true ).toBool(); }

void pMonkeyStudio::setDefaultEncoding( const QString& s )
{ MonkeyCore::settings()->setValue( settingsPath() +"/DefaultEncoding", s ); }

const QString pMonkeyStudio::defaultEncoding()
{ return MonkeyCore::settings()->value( settingsPath() +"/DefaultEncoding", "UTF-8" ).toString(); }

void pMonkeyStudio::setSelectionBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/SelectionBackgroundColor", c ); }

const QColor pMonkeyStudio::selectionBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/SelectionBackgroundColor", QColor( "#bdff9b" ) ).value<QColor>(); }

void pMonkeyStudio::setSelectionForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/SelectionForegroundColor", c ); }

const QColor pMonkeyStudio::selectionForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/SelectionForegroundColor", QColor( "#000000" ) ).value<QColor>(); }

void pMonkeyStudio::setDefaultDocumentColours( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/DefaultDocumentColours", b ); }

const bool pMonkeyStudio::defaultDocumentColours()
{ return MonkeyCore::settings()->value( settingsPath() +"/DefaultDocumentColours", false ).toBool(); }

void pMonkeyStudio::setDefaultDocumentPen( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/DefaultDocumentPen", c ); }

const QColor pMonkeyStudio::defaultDocumentPen()
{ return MonkeyCore::settings()->value( settingsPath() +"/DefaultDocumentPen", QColor( Qt::black ) ).value<QColor>(); }

void pMonkeyStudio::setDefaultDocumentPaper( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/DefaultDocumentPaper", c ); }

const QColor pMonkeyStudio::defaultDocumentPaper()
{ return MonkeyCore::settings()->value( settingsPath() +"/DefaultDocumentPaper", QColor( Qt::white ) ).value<QColor>(); }

void pMonkeyStudio::setAutoCompletionCaseSensitivity( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoCompletionCaseSensitivity", b ); }

const bool pMonkeyStudio::autoCompletionCaseSensitivity()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoCompletionCaseSensitivity", true ).toBool(); }

void pMonkeyStudio::setAutoCompletionReplaceWord( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoCompletionReplaceWord", b ); }

const bool pMonkeyStudio::autoCompletionReplaceWord()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoCompletionReplaceWord", true ).toBool(); }

void pMonkeyStudio::setAutoCompletionShowSingle( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoCompletionShowSingle", b ); }

const bool pMonkeyStudio::autoCompletionShowSingle()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoCompletionShowSingle", false ).toBool(); }

void pMonkeyStudio::setAutoCompletionSource( QsciScintilla::AutoCompletionSource a )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoCompletionSource", a ); }

const QsciScintilla::AutoCompletionSource pMonkeyStudio::autoCompletionSource()
{ return (QsciScintilla::AutoCompletionSource)MonkeyCore::settings()->value( settingsPath() +"/AutoCompletionSource", (int)QsciScintilla::AcsAll ).toInt(); }

void pMonkeyStudio::setAutoCompletionThreshold( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoCompletionThreshold", i ); }

const int pMonkeyStudio::autoCompletionThreshold()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoCompletionThreshold", 3 ).toInt(); }

void pMonkeyStudio::setCallTipsBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CallTipsBackgroundColor", c ); }

const QColor pMonkeyStudio::callTipsBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/CallTipsBackgroundColor", QColor( "#ffff9b" ) ).value<QColor>(); }

void pMonkeyStudio::setCallTipsForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CallTipsForegroundColor", c ); }

const QColor pMonkeyStudio::callTipsForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/CallTipsForegroundColor", QColor( "#000000" ) ).value<QColor>(); }

void pMonkeyStudio::setCallTipsHighlightColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CallTipsHighlightColor", c ); }

const QColor pMonkeyStudio::callTipsHighlightColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/CallTipsHighlightColor", QColor( "#ff0000" ) ).value<QColor>(); }

void pMonkeyStudio::setCallTipsStyle( QsciScintilla::CallTipsStyle s )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CallTipsStyle", s ); }

const QsciScintilla::CallTipsStyle pMonkeyStudio::callTipsStyle()
{ return (QsciScintilla::CallTipsStyle)MonkeyCore::settings()->value( settingsPath() +"/CallTipsStyle", (int)QsciScintilla::CallTipsContext ).toInt(); }

void pMonkeyStudio::setCallTipsVisible( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CallTipsVisible", i ); }

const int pMonkeyStudio::callTipsVisible()
{ return MonkeyCore::settings()->value( settingsPath() +"/CallTipsVisible", -1 ).toInt(); }

void pMonkeyStudio::setAutoIndent( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoIndent", b ); }

const bool pMonkeyStudio::autoIndent()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoIndent", true ).toBool(); }

void pMonkeyStudio::setBackspaceUnindents( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/BackspaceUnindents", b ); }

const bool pMonkeyStudio::backspaceUnindents()
{ return MonkeyCore::settings()->value( settingsPath() +"/BackspaceUnindents", true ).toBool(); }

void pMonkeyStudio::setIndentationGuides( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/IndentationGuides", b ); }

const bool pMonkeyStudio::indentationGuides()
{ return MonkeyCore::settings()->value( settingsPath() +"/IndentationGuides", true ).toBool(); }

void pMonkeyStudio::setIndentationsUseTabs( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/IndentationsUseTabs", b ); }

const bool pMonkeyStudio::indentationsUseTabs()
{ return MonkeyCore::settings()->value( settingsPath() +"/IndentationsUseTabs", true ).toBool(); }

void pMonkeyStudio::setAutoDetectIndent( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/AutoDetectIndent", b ); }

const bool pMonkeyStudio::autoDetectIndent()
{ return MonkeyCore::settings()->value( settingsPath() +"/AutoDetectIndent", false ).toBool(); }

void pMonkeyStudio::setIndentationWidth( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/IndentationWidth", i ); }

const int pMonkeyStudio::indentationWidth()
{ return MonkeyCore::settings()->value( settingsPath() +"/IndentationWidth", 4 ).toInt(); }

void pMonkeyStudio::setTabIndents( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/TabIndents", b ); }

const bool pMonkeyStudio::tabIndents()
{ return MonkeyCore::settings()->value( settingsPath() +"/TabIndents", true ).toBool(); }

void pMonkeyStudio::setTabWidth( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/TabWidth", i ); }

const int pMonkeyStudio::tabWidth()
{ return MonkeyCore::settings()->value( settingsPath() +"/TabWidth", 4 ).toInt(); }

void pMonkeyStudio::setIndentationGuidesBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/IndentationGuidesBackgroundColor", c ); }

const QColor pMonkeyStudio::indentationGuidesBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/IndentationGuidesBackgroundColor", QColor( "#0000ff" ) ).value<QColor>(); }

void pMonkeyStudio::setIndentationGuidesForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/IndentationGuidesForegroundColor", c ); }

const QColor pMonkeyStudio::indentationGuidesForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/IndentationGuidesForegroundColor", QColor( "#0000ff" ) ).value<QColor>(); }

void pMonkeyStudio::setBraceMatching( QsciScintilla::BraceMatch b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/BraceMatching", b ); }

const QsciScintilla::BraceMatch pMonkeyStudio::braceMatching()
{ return (QsciScintilla::BraceMatch)MonkeyCore::settings()->value( settingsPath() +"/BraceMatching", (int)QsciScintilla::SloppyBraceMatch ).toInt(); }

void pMonkeyStudio::setMatchedBraceBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/MatchedBraceBackgroundColor", c ); }

const QColor pMonkeyStudio::matchedBraceBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/MatchedBraceBackgroundColor", QColor( "#ffff7f" ) ).value<QColor>(); }

void pMonkeyStudio::setMatchedBraceForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/MatchedBraceForegroundColor", c ); }

const QColor pMonkeyStudio::matchedBraceForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/MatchedBraceForegroundColor", QColor( "#ff0000" ) ).value<QColor>(); }

void pMonkeyStudio::setUnmatchedBraceBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/UnmatchedBraceBackgroundColor", c ); }

const QColor pMonkeyStudio::unmatchedBraceBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/UnmatchedBraceBackgroundColor", QColor( "#ff0000" ) ).value<QColor>(); }

void pMonkeyStudio::setUnmatchedBraceForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/UnmatchedBraceForegroundColor", c ); }

const QColor pMonkeyStudio::unmatchedBraceForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/UnmatchedBraceForegroundColor", QColor( "#ffffff" ) ).value<QColor>(); }

void pMonkeyStudio::setEdgeMode( QsciScintilla::EdgeMode m )
{ MonkeyCore::settings()->setValue( settingsPath() +"/EdgeMode", m ); }

const QsciScintilla::EdgeMode pMonkeyStudio::edgeMode()
{ return (QsciScintilla::EdgeMode)MonkeyCore::settings()->value( settingsPath() +"/EdgeMode", (int)QsciScintilla::EdgeNone ).toInt(); }

void pMonkeyStudio::setEdgeColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/EdgeColor", c ); }

const QColor pMonkeyStudio::edgeColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/EdgeColor", QColor( "#ff0000" ) ).value<QColor>(); }

void pMonkeyStudio::setEdgeColumn( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/EdgeColumn", i ); }

const int pMonkeyStudio::edgeColumn()
{ return MonkeyCore::settings()->value( settingsPath() +"/EdgeColumn", 80 ).toInt(); }

void pMonkeyStudio::setCaretLineVisible( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CaretLineVisible", b ); }

const bool pMonkeyStudio::caretLineVisible()
{ return MonkeyCore::settings()->value( settingsPath() +"/CaretLineVisible", true ).toBool(); }

void pMonkeyStudio::setCaretLineBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CaretLineBackgroundColor", c ); }

const QColor pMonkeyStudio::caretLineBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/CaretLineBackgroundColor", QColor( "#aaaaff" ) ).value<QColor>(); }

void pMonkeyStudio::setCaretForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CaretForegroundColor", c ); }

const QColor pMonkeyStudio::caretForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/CaretForegroundColor", QColor( "#000000" ) ).value<QColor>(); }

void pMonkeyStudio::setCaretWidth( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/CaretWidth", i ); }

const int pMonkeyStudio::caretWidth()
{ return MonkeyCore::settings()->value( settingsPath() +"/CaretWidth", 1 ).toInt(); }

void pMonkeyStudio::setLineNumbersMarginEnabled( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/LineNumbersMarginEnabled", b ); }

const bool pMonkeyStudio::lineNumbersMarginEnabled()
{ return MonkeyCore::settings()->value( settingsPath() +"/LineNumbersMarginEnabled", true ).toBool(); }

void pMonkeyStudio::setLineNumbersMarginWidth( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/LineNumbersMarginWidth", i ); }

const int pMonkeyStudio::lineNumbersMarginWidth()
{ return MonkeyCore::settings()->value( settingsPath() +"/LineNumbersMarginWidth", 4 ).toInt(); }

void pMonkeyStudio::setLineNumbersMarginAutoWidth( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/LineNumbersMarginAutoWidth", b ); }

const bool pMonkeyStudio::lineNumbersMarginAutoWidth()
{ return MonkeyCore::settings()->value( settingsPath() +"/LineNumbersMarginAutoWidth", true ).toBool(); }

void pMonkeyStudio::setFolding( QsciScintilla::FoldStyle f )
{ MonkeyCore::settings()->setValue( settingsPath() +"/Folding", f ); }

const QsciScintilla::FoldStyle pMonkeyStudio::folding()
{ return (QsciScintilla::FoldStyle)MonkeyCore::settings()->value( settingsPath() +"/Folding", (int)QsciScintilla::BoxedTreeFoldStyle ).toInt(); }

void pMonkeyStudio::setFoldMarginBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/FoldMarginBackgroundColor", c ); }

const QColor pMonkeyStudio::foldMarginBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/FoldMarginBackgroundColor", QColor( "#c0c0c0" ) ).value<QColor>(); }

void pMonkeyStudio::setFoldMarginForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/FoldMarginForegroundColor", c ); }

const QColor pMonkeyStudio::foldMarginForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/FoldMarginForegroundColor", QColor( "#ffffff" ) ).value<QColor>(); }

void pMonkeyStudio::setMarginsEnabled( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/MarginsEnabled", b ); }

const bool pMonkeyStudio::marginsEnabled()
{ return MonkeyCore::settings()->value( settingsPath() +"/MarginsEnabled", false ).toBool(); }

void pMonkeyStudio::setMarginsBackgroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/MarginsBackgroundColor", c ); }

const QColor pMonkeyStudio::marginsBackgroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/MarginsBackgroundColor", QColor( "#c0c0c0" ) ).value<QColor>(); }

void pMonkeyStudio::setMarginsForegroundColor( const QColor& c )
{ MonkeyCore::settings()->setValue( settingsPath() +"/MarginsForegroundColor", c ); }

const QColor pMonkeyStudio::marginsForegroundColor()
{ return MonkeyCore::settings()->value( settingsPath() +"/MarginsForegroundColor", QColor( "#ffffff" ) ).value<QColor>(); }

void pMonkeyStudio::setMarginsFont( const QFont& f )
{ MonkeyCore::settings()->setValue( settingsPath() +"/MarginsFont", f.toString() ); }

const QFont pMonkeyStudio::marginsFont()
{
	QFont f;
	f.fromString( MonkeyCore::settings()->value( settingsPath() +"/MarginsFont", f.toString() ).toString() );
	return f;
}

void pMonkeyStudio::setEolMode( QsciScintilla::EolMode e )
{ MonkeyCore::settings()->setValue( settingsPath() +"/EolMode", e ); }

const QsciScintilla::EolMode pMonkeyStudio::eolMode()
{
#if defined( Q_WS_WIN )
	int i = QsciScintilla::EolWindows;
#elif defined( Q_WS_MAC )
	int i = QsciScintilla::EolMac;
#else
	int i = QsciScintilla::EolUnix;
#endif
	return (QsciScintilla::EolMode)MonkeyCore::settings()->value( settingsPath() +"/EolMode", i ).toInt();
}

QString pMonkeyStudio::getEol( QsciScintilla::EolMode e )
{
	switch ( e )
	{
		case QsciScintilla::EolWindows:
			return QString( "\r\n" );
			break;
		case QsciScintilla::EolUnix:
			return QString( "\n" );
			break;
		case QsciScintilla::EolMac:
			return QString( "\n" );
			break;
		default:
			return QString( "\r\n" );
	}
}

void pMonkeyStudio::setEolVisibility( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/EolVisibility", b ); }

const bool pMonkeyStudio::eolVisibility()
{ return MonkeyCore::settings()->value( settingsPath() +"/EolVisibility", false ).toBool(); }

void pMonkeyStudio::setWhitespaceVisibility( QsciScintilla::WhitespaceVisibility w )
{ MonkeyCore::settings()->setValue( settingsPath() +"/WhitespaceVisibility", w ); }

const QsciScintilla::WhitespaceVisibility pMonkeyStudio::whitespaceVisibility()
{ return (QsciScintilla::WhitespaceVisibility)MonkeyCore::settings()->value( settingsPath() +"/WhitespaceVisibility", (int)QsciScintilla::WsInvisible ).toInt(); }

void pMonkeyStudio::setWrapMode( QsciScintilla::WrapMode w )
{ MonkeyCore::settings()->setValue( settingsPath() +"/WrapMode", w ); }

const QsciScintilla::WrapMode pMonkeyStudio::wrapMode()
{ return (QsciScintilla::WrapMode)MonkeyCore::settings()->value( settingsPath() +"/WrapMode", (int)QsciScintilla::WrapNone ).toInt(); }

void pMonkeyStudio::setWrapVisualFlagsEnabled( bool b )
{ MonkeyCore::settings()->setValue( settingsPath() +"/WrapVisualFlagsEnabled", b ); }

const bool pMonkeyStudio::wrapVisualFlagsEnabled()
{ return MonkeyCore::settings()->value( settingsPath() +"/WrapVisualFlagsEnabled", false ).toBool(); }

void pMonkeyStudio::setStartWrapVisualFlag( QsciScintilla::WrapVisualFlag f )
{ MonkeyCore::settings()->setValue( settingsPath() +"/StartWrapVisualFlag", f ); }

const QsciScintilla::WrapVisualFlag pMonkeyStudio::startWrapVisualFlag()
{ return (QsciScintilla::WrapVisualFlag)MonkeyCore::settings()->value( settingsPath() +"/StartWrapVisualFlag", (int)QsciScintilla::WrapFlagNone ).toInt(); }

void pMonkeyStudio::setEndWrapVisualFlag( QsciScintilla::WrapVisualFlag f )
{ MonkeyCore::settings()->setValue( settingsPath() +"/EndWrapVisualFlag", f ); }

const QsciScintilla::WrapVisualFlag pMonkeyStudio::endWrapVisualFlag()
{ return (QsciScintilla::WrapVisualFlag)MonkeyCore::settings()->value( settingsPath() +"/EndWrapVisualFlag", (int)QsciScintilla::WrapFlagNone ).toInt(); }

void pMonkeyStudio::setWrappedLineIndentWidth( int i )
{ MonkeyCore::settings()->setValue( settingsPath() +"/WrappedLineIndentWidth", i ); }

const int pMonkeyStudio::wrappedLineIndentWidth()
{ return MonkeyCore::settings()->value( settingsPath() +"/WrappedLineIndentWidth", 0 ).toInt(); }
