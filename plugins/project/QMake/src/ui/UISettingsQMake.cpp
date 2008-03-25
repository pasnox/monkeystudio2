/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : UISettingsQMake.cpp
** Date      : 2008-01-14T00:54:09
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
#include "UISettingsQMake.h"

#include <monkey.h>
#include <coremanager.h>

#include <QPushButton>
#include <QWhatsThis>
#include <QInputDialog>

using namespace pMonkeyStudio;

UISettingsQMake::UISettingsQMake( QWidget* p )
	: QWidget( p )
{
	// set up dialog
	setupUi( this );
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QIcon( ":/help/icons/help/keyword.png" ) );
	dbbButtons->button( QDialogButtonBox::Save )->setIcon( QIcon( ":/file/icons/file/save.png" ) );
	// set audo delete
	setAttribute( Qt::WA_DeleteOnClose );
	// load settings
	loadSettings();
	// connect
	connect( lwQtVersions, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwQtModules, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	connect( lwSettings, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbAdd*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbAdd_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbRemove*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbRemove_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbClear*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbClear_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbUp*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbUp_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbDown*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbDown_clicked() ) );
	foreach ( QToolButton* tb, findChildren<QToolButton*>( QRegExp( "tbQtVersion*" ) ) )
		connect( tb, SIGNAL( clicked() ), this, SLOT( tbQtVersion_clicked() ) );
}

QStringList UISettingsQMake::defaultFilters()
{
	return QStringList() 
	<< "FORMS" << "FORMS3" << "HEADERS" << "SOURCES" << "RESOURCES" << "IMAGES" << "TRANSLATIONS"
	<< "TEXTS" << "UNKNOW_FILES" << "OPENED_FILES";
}

QStringList UISettingsQMake::readFilters()
{
	QStringList l = MonkeyCore::settings()->value( QString( "Plugins/%1/Filters" ).arg( PLUGIN_NAME ) ).toStringList();
	return l.isEmpty() ? defaultFilters() : l;
}

QStringList UISettingsQMake::defaultFiltersToolTips()
{
	return QStringList()
	<< tr( "Forms Files" ) << tr( "Forms 3 Files" ) << tr( "Headers Files" ) << tr( "Sources Files" ) << tr( "Resources Files" ) 
	<< tr( "Images Files" ) << tr( "Translations Files" ) << tr( "Texts Files" ) << tr( "Unknow Files" ) << tr( "Opened Files" );
}

QStringList UISettingsQMake::readFiltersToolTips()
{
	QStringList l = MonkeyCore::settings()->value( QString( "Plugins/%1/FiltersToolTips" ).arg( PLUGIN_NAME ) ).toStringList();
	return l.isEmpty() ? defaultFiltersToolTips() : l;
}

QStringList UISettingsQMake::defaultScopes()
{
	return QStringList()
	<< QString::null << "win32" << "unix" << "linux" << "mac" << "macx" << "aix-g++" << "aix-g++-64"
	<< "aix-xlc" << "aix-xlc-64" << "darwin-g++" << "freebsd-g++" << "freebsd-g++34" << "freebsd-g++40" << "freebsd-icc"
	<< "hpux-acc" << "hpux-acc-64" << "hpux-acc-o64" << "hpux-g++" << "hpux-g++-64" << "hpuxi-acc" << "hpuxi-acc-64"
	<< "hurd-g++" << "irix-cc" << "irix-cc-64" << "irix-g++" << "irix-g++-64" << "linux-cxx" << "linux-ecc-64"
	<< "linux-g++" << "linux-g++-32" << "linux-g++-64" << "linux-icc" << "linux-kcc" << "linux-lsb" << "linux-pgcc"
	<< "lynxos-g++" << "macx-g++" << "macx-icc" << "macx-pbuilder" << "macx-xcode" << "macx-xlc" << "netbsd-g++"
	<< "openbsd-g++" << "sco-cc" << "sco-g++" << "solaris-cc" << "solaris-cc-64" << "solaris-g++" << "solaris-g++-64"
	<< "tru64-cxx" << "tru64-g++" << "unixware-cc" << "unixware-g++" << "win32-g++" << "win32-x-g++" << "default"
	<< "freebsd-generic-g++" << "linux-arm-g++" << "linux-cellon-g++" << "linux-generic-g++" << "linux-generic-g++-32"
	<< "linux-ipaq-g++" << "linux-mips-g++" << "linux-sharp-g++" << "linux-x86_64-g++" << "linux-x86-g++"
	<< "linux-zylonite-g++" << "macx-generic-g++" << "solaris-generic-g++";
}

QStringList UISettingsQMake::readScopes()
{
	QStringList l = MonkeyCore::settings()->value( QString( "Plugins/%1/Scopes" ).arg( PLUGIN_NAME ) ).toStringList();
	return l.isEmpty() ? defaultScopes() : l;
}

QStringList UISettingsQMake::defaultPathFiles()
{
	return QStringList()
	<< "FORMS" << "FORMS3" << "HEADERS" << "SOURCES" << "RESOURCES" << "IMAGES" << "TRANSLATIONS" << "DEF_FILE"
	<< "DEPENDPATH" << "DESTDIR" << "DESTDIR_TARGET" << "DLLDESTDIR" << "DISTFILES" << "INCLUDEPATH" << "MOC_DIR"
	<< "OBJECTS_DIR" << "RC_FILE" << "RCC_DIR" << "RES_FILE" << "TARGET" << "UI_DIR" << "VPATH" << "YACCSOURCES"
	<< "LEXSOURCES" << "SUBDIRS" << "UNKNOW_FILES" << "OPENED_FILES" << "APP_ICON" << "APP_HELP_FILE";
}

QStringList UISettingsQMake::readPathFiles()
{
	QStringList l = MonkeyCore::settings()->value( QString( "Plugins/%1/PathFiles" ).arg( PLUGIN_NAME ) ).toStringList();
	return l.isEmpty() ? defaultPathFiles() : l;
}

QtItemList UISettingsQMake::defaultQtModules()
{
	return QtItemList()
	<< QtItem( "QtCore", "core", "QT", "Add support for Qt Core classes" )
	<< QtItem( "QtGui", "gui", "QT", "Add support for Qt Gui classes" )
	<< QtItem( "QtNetwork", "network", "QT", "Add support for Qt Network classes" )
	<< QtItem( "QtOpenGL", "opengl", "QT", "Add support for Qt OpenGL classes" )
	<< QtItem( "QtSql", "sql", "QT", "Add support for Qt Sql classes" )
	<< QtItem( "QtSvg", "svg", "QT", "Add support for Qt Svg classes" )
	<< QtItem( "QtXml", "xml", "QT", "Add support for Qt Xml classes" )
	<< QtItem( "Qt3Support", "qt3support", "QT", "Add support for Qt Qt3Support classes" )
	<< QtItem( "QtDesigner", "designer", "CONFIG", "Add support for Qt Designer classes" )
	<< QtItem( "QtUiTools", "uitools", "CONFIG", "Add support for Qt UiTools classes" )
	<< QtItem( "QtAssistant", "assistant", "CONFIG", "Add support for Qt Assistant classes" )
	<< QtItem( "QtTest", "qtestlib", "CONFIG", "Add support for Qt Test classes" )
	<< QtItem( "QAxContainer", "qaxcontainer", "CONFIG", "Add support for QAxContainer classes" )
	<< QtItem( "QAxServer", "qaxserver", "CONFIG", "Add support for QAxServer classes" )
	<< QtItem( "QtDBus", "qdbus", "CONFIG", "Add support for Qt DBus classes" );
}

QtItemList UISettingsQMake::readQtModules()
{
	QtItemList l;
	pSettings* ps = MonkeyCore::settings();
	int s = ps->beginReadArray( QString( "Plugins/%1/QtModules" ).arg( PLUGIN_NAME ) );
	for ( int i = 0; i < s; i++ )
	{
		ps->setArrayIndex( i );
		l << QtItem( ps->value( "Text" ).toString(), ps->value( "Value" ).toString(), ps->value( "Variable" ).toString(), ps->value( "Help" ).toString() );
	}
	ps->endArray();
	return l.isEmpty() ? defaultQtModules() : l;
}

QtItemList UISettingsQMake::defaultSettings()
{
	return QtItemList()
	<< QtItem( "rtti", "rtti", "CONFIG", "RTTI support is enabled." )
	<< QtItem( "stl", "stl", "CONFIG", "STL support is enabled." )
	<< QtItem( "exceptions", "execptions", "CONFIG", "Exception support is enabled." )
	<< QtItem( "thread", "thread", "CONFIG", "The target is a multi-threaded application or library. The proper defines and compiler flags will automatically be added to the project." )
	<< QtItem( "no_lflags_merge", "no_lflags_merge", "CONFIG", "Ensures that the list of libraries stored in the LIBS variable is not reduced to a list of unique values before it is used." )
	<< QtItem( "qt", "qt", "CONFIG", "The target is a Qt application/library and requires the Qt library and header files. The proper include and library paths for the Qt library will automatically be added to the project. This is defined by default, and can be fine-tuned with the \\l{#qt}{QT} variable." )
	<< QtItem( "resources", "resources", "CONFIG", "Configures qmake to run rcc on the content of RESOURCES if defined." )
	<< QtItem( "uic3", "uic3", "CONFIG", "Configures qmake to run uic3 on the content of FORMS3 if defined; otherwise the contents of FORMS will be processed instead." )
	<< QtItem( "LIB ONLY", QString::null, QString::null, "Options for LIB template only" )
	<< QtItem( "dll", "dll", "CONFIG", "The target is a shared object/DLL.The proper include paths, compiler flags and libraries will automatically be added to the project." )
	<< QtItem( "staticlib", "staticlib", "CONFIG", "The target is a static library (lib only). The proper compiler flags will automatically be added to the project." )
	<< QtItem( "plugin", "plugin", "CONFIG", "The target is a plugin (lib only). This enables dll as well." )
	<< QtItem( "X11 ONLY", QString::null, QString::null, "Options for X11 only" )
	<< QtItem( "x11", "x11", "CONFIG", "The target is a X11 application or library. The proper include paths and libraries will automatically be added to the project." )
	<< QtItem( "MAC OS X ONLY", QString::null, QString::null, "Options for Mac OS X only" )
	<< QtItem( "ppc", "ppc", "CONFIG", "Builds a PowerPC binary." )
	<< QtItem( "x86", "x86", "CONFIG", "Builds an i386 compatible binary." )
	<< QtItem( "app_bundle", "app_bundle", "CONFIG", "Puts the executable into a bundle (this is the default)." )
	<< QtItem( "lib_bundle", "lib_bundle", "CONFIG", "Puts the library into a library bundle." )
	<< QtItem( "WINDOWS ONLY", QString::null, QString::null, "Options for Windows only" )
	<< QtItem( "windows", "windows", "CONFIG", "The target is a Win32 window application (app only). The proper include paths,compiler flags and libraries will automatically be added to the project." )
	<< QtItem( "console", "console", "CONFIG", "The target is a Win32 console application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project." )
	<< QtItem( "flat", "flat", "CONFIG", "When using the vcapp template this will put all the source files into the source group and the header files into the header group regardless of what directory they reside in. Turning this option off will group the files within the source/header group depending on the directory they reside. This is turned on by default." )
	<< QtItem( "embed_manifest_dll", "embed_manifest_dll", "CONFIG", "Embeds a manifest file in the DLL created as part of an application/library project." )
	<< QtItem( "ACTIVEQT ONLY", QString::null, QString::null, "Option for Windows/Active Qt only" )
	<< QtItem( "qaxserver_no_postlink", "qaxserver_no_postlink", "CONFIG", "No help available" );
}

QtItemList UISettingsQMake::readSettings()
{
	QtItemList l;
	pSettings* ps = MonkeyCore::settings();
	int s = ps->beginReadArray( QString( "Plugins/%1/Settings" ).arg( PLUGIN_NAME ) );
	for ( int i = 0; i < s; i++ )
	{
		ps->setArrayIndex( i );
		l << QtItem( ps->value( "Text" ).toString(), ps->value( "Value" ).toString(), ps->value( "Variable" ).toString(), ps->value( "Help" ).toString() );
	}
	ps->endArray();
	return l.isEmpty() ? defaultSettings() : l;
}

QtVersionList UISettingsQMake::defaultVersions()
{
	return QtVersionList()
#ifdef Q_OS_MAC
	<< QtVersion( "Qt 4", "/usr/local/Trolltech/Qt-4.3.3", "qmake", "lupdate", "lrelease", "/usr/local/Trolltech/Qt-4.3.3/doc/html", true )
	<< QtVersion( "Qt 3", "/Developer/qt", "qmake", "lupdate", "lrelease", "/Developer/qt/doc/html", false );
#elif defined Q_OS_WIN
	<< QtVersion( "Qt 4", "C:\\Qt\\4.3.2", "qmake", "lupdate", "lrelease", "C:\\Qt\\4.3.2\\doc\\html", true )
	<< QtVersion( "Qt 3", "C:\\Qt\\3.3.7", "qmake", "lupdate", "lrelease", "C:\\Qt\\3.3.7\\doc\\html", false );
#else
	<< QtVersion( "Qt 4", "/usr/share/qt4", "qmake-qt4", "lupdate-qt4", "lrelease-qt4", "/usr/share/qt4/doc/html", true )
	<< QtVersion( "Qt 3", "/usr/share/qt3", "qmake-qt3", "lupdate-qt3", "lrelease-qt3", "/usr/share/qt3/doc/html", false );
#endif
}

QtVersionList UISettingsQMake::readVersions()
{
	QtVersionList l;
	pSettings* ps = MonkeyCore::settings();
	int s = ps->beginReadArray( QString( "Plugins/%1/Versions" ).arg( PLUGIN_NAME ) );
	for ( int i = 0; i < s; i++ )
	{
		ps->setArrayIndex( i );
		l << QtVersion( ps->value( "Text" ).toString(), ps->value( "Path" ).toString(), ps->value( "QMake" ).toString(), ps->value( "lupdate" ).toString(), ps->value( "lrelease" ).toString(), ps->value( "DocsPath" ).toString(), ps->value( "Default" ).toBool() );
	}
	ps->endArray();
	return l.isEmpty() ? defaultVersions() : l;
}

QtVersion UISettingsQMake::defaultVersion( const QString& s )
{
	// return default qmake
	foreach ( QtVersion v, readVersions() )
	{
		if ( s.isEmpty() && v.Default )
			return v;
		else if ( !s.isEmpty() && v.Text == s )
			return v;
	}
	// return hard coded if no version are default
	foreach ( QtVersion v, defaultVersions() )
		if ( v.Default )
			return v;
	// return null version
	return QtVersion();
}

void UISettingsQMake::loadSettings()
{
	// general
	foreach ( QtVersion v, readVersions() )
	{
		QListWidgetItem* it = new QListWidgetItem( v.Text, lwQtVersions );
		it->setData( QtVersion::PathRole, v.Path );
		it->setData( QtVersion::QMakeRole, v.QMake );
		it->setData( QtVersion::lupdateRole, v.lupdate );
		it->setData( QtVersion::lreleaseRole, v.lrelease );
		it->setData( QtVersion::DocsPathRole, v.DocsPath );
		it->setData( QtVersion::DefaultRole, v.Default );
		if ( v.Default )
			it->setBackground( QBrush( Qt::red ) );
	}
	// filters & scopes
	lwFilters->addItems( readFilters() );
	lwScopes->addItems( readScopes() );
	lwPathFiles->addItems( readPathFiles() );
	// set filters tooltips
	QStringList ft = readFiltersToolTips();
	for ( int i = 0; i < ft.count(); i++ )
		if ( QListWidgetItem* it = lwFilters->item( i ) )
			it->setToolTip( ft.at( i ) );
	// set items editable
	QList<QListWidgetItem*> items = QList<QListWidgetItem*> () 
	<< lwFilters->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive )
	<< lwScopes->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive )
	<< lwPathFiles->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
	foreach ( QListWidgetItem* it, items )
		it->setFlags( it->flags() | Qt::ItemIsEditable );
	items.clear();
	// qt modules
	foreach ( QtItem i, readQtModules() )
	{
		QListWidgetItem* it = new QListWidgetItem( i.Text, lwQtModules );
		it->setData( QtItem::ValueRole, i.Value );
		it->setData( QtItem::VariableRole, i.Variable );
		it->setData( QtItem::HelpRole, i.Help );
	}
	// configuration
	foreach ( QtItem i, readSettings() )
	{
		QListWidgetItem* it = new QListWidgetItem( i.Text, lwSettings );
		it->setData( QtItem::ValueRole, i.Value );
		it->setData( QtItem::VariableRole, i.Variable );
		it->setData( QtItem::HelpRole, i.Help );
	}
}

void UISettingsQMake::lw_currentItemChanged( QListWidgetItem* c, QListWidgetItem* p )
{
	if ( p )
	{
		if ( p->listWidget() == lwQtVersions )
		{
			p->setText( leQtVersionVersion->text() );
			p->setData( QtVersion::PathRole, leQtVersionPath->text() );
			p->setData( QtVersion::QMakeRole, leQtVersionQMake->text() );
			p->setData( QtVersion::lupdateRole, leQtVersionlupdate->text() );
			p->setData( QtVersion::lreleaseRole, leQtVersionlrelease->text() );
			p->setData( QtVersion::DocsPathRole, leQtVersionDocsPath->text() );
		}
		else if ( p->listWidget() == lwQtModules )
		{
			p->setText( leCaptionQtModule->text() );
			p->setData( QtItem::ValueRole, leValueQtModule->text() );
			p->setData( QtItem::VariableRole, leVariableQtModule->text().toUpper() );
			p->setData( QtItem::HelpRole, teHelpQtModule->toPlainText() );
		}
		else
		{
			p->setText( leCaptionSetting->text() );
			p->setData( QtItem::ValueRole, leValueSetting->text() );
			p->setData( QtItem::VariableRole, leVariableSetting->text().toUpper() );
			p->setData( QtItem::HelpRole, teHelpSetting->toPlainText() );
		}
	}
	if ( c )
	{
		if ( c->listWidget() == lwQtVersions )
		{
			leQtVersionVersion->setText( c->text() );
			leQtVersionPath->setText( c->data( QtVersion::PathRole ).toString() );
			leQtVersionQMake->setText( c->data( QtVersion::QMakeRole ).toString() );
			leQtVersionlupdate->setText( c->data( QtVersion::lupdateRole ).toString() );
			leQtVersionlrelease->setText( c->data( QtVersion::lreleaseRole ).toString() );
			leQtVersionDocsPath->setText( c->data( QtVersion::DocsPathRole ).toString() );
		}
		else if ( c->listWidget() == lwQtModules )
		{
			leCaptionQtModule->setText( c->text() );
			leValueQtModule->setText( c->data( QtItem::ValueRole ).toString() );
			leVariableQtModule->setText( c->data( QtItem::VariableRole ).toString() );
			teHelpQtModule->setPlainText( c->data( QtItem::HelpRole ).toString() );
		}
		else
		{
			leCaptionSetting->setText( c->text() );
			leValueSetting->setText( c->data( QtItem::ValueRole ).toString() );
			leVariableSetting->setText( c->data( QtItem::VariableRole ).toString() );
			teHelpSetting->setPlainText( c->data( QtItem::HelpRole ).toString() );
		}
	}
	else if ( sender() == lwQtVersions )
	{
		leQtVersionVersion->clear();
		leQtVersionPath->clear();
		leQtVersionQMake->clear();
		leQtVersionlupdate->clear();
		leQtVersionlrelease->clear();
		leQtVersionDocsPath->clear();
	}
	else if ( sender() == lwQtModules )
	{
		leCaptionQtModule->clear();
		leValueQtModule->clear();
		leVariableQtModule->clear();
		teHelpQtModule->clear();
	}
	else if ( sender() == lwSettings )
	{
		leCaptionSetting->clear();
		leValueSetting->clear();
		leVariableSetting->clear();
		teHelpSetting->clear();
	}
}

void UISettingsQMake::tbQtVersion_clicked()
{
	// need path
	bool b = sender() == tbQtVersionPath || sender() == tbQtVersionDocsPath;
	// get dialog text
	QString s;
	QLineEdit* le = 0;
	if ( sender() == tbQtVersionPath )
	{
		s = tr( "Locate your qt installation directory" );
		le = leQtVersionPath;
	}
	else if ( sender() == tbQtVersionQMake )
	{
		s = tr( "Locate the QMake tool associated with this version of qt" );
		le = leQtVersionQMake;
	}
	else if ( sender() == tbQtVersionlupdate )
	{
		s = tr( "Locate the lupdate tool associated with this version of qt" );
		le = leQtVersionlupdate;
	}
	else if ( sender() == tbQtVersionlrelease )
	{
		s = tr( "Locate the lrelease tool associated with this version of qt" );
		le = leQtVersionlrelease;
	}
	else if ( sender() == tbQtVersionDocsPath )
	{
		s = tr( "Locate the doc/html directory associated this with version of qt" );
		le = leQtVersionDocsPath;
	}
	else
		return;
	// get path/file
	const QString f = b ? getExistingDirectory( s, le->text(), window() ) : getOpenFileName( s, le->text(), QString::null, window() );
	// update dialog if needed
	if ( !f.isNull() )
		le->setText( f );
}

void UISettingsQMake::tbAdd_clicked()
{
	QListWidget* lw = 0;
	if ( sender() == tbAddQtVersion )
		lw = lwQtVersions;
	else if ( sender() == tbAddFilter )
		lw = lwFilters;
	else if ( sender() == tbAddScope )
		lw = lwScopes;
	else if ( sender() == tbAddPathFile )
		lw = lwPathFiles;
	else if ( sender() == tbAddQtModule )
		lw = lwQtModules;
	else if ( sender() == tbAddSetting )
		lw = lwSettings;
	if ( lw )
	{
		lw->addItem( tr( "New value" ) );
		lw->setCurrentItem( lw->item( lw->count() -1 ) );
		lw->scrollToItem( lw->item( lw->count() -1 ) );
		lw->currentItem()->setFlags( lw->currentItem()->flags() | Qt::ItemIsEditable );
	}
}

void UISettingsQMake::tbRemove_clicked()
{
	if ( sender() == tbRemoveQtVersion )
		delete lwQtVersions->selectedItems().value( 0 );
	else if ( sender() == tbRemoveFilter )
		delete lwFilters->selectedItems().value( 0 );
	else if ( sender() == tbRemoveScope )
		delete lwScopes->selectedItems().value( 0 );
	else if ( sender() == tbRemovePathFile )
		delete lwPathFiles->selectedItems().value( 0 );
	else if ( sender() == tbRemoveQtModule )
		delete lwQtModules->selectedItems().value( 0 );
	else if ( sender() == tbRemoveSetting )
		delete lwSettings->selectedItems().value( 0 );
}

void UISettingsQMake::tbClear_clicked()
{
	if ( sender() == tbClearQtVersions )
		lwQtVersions->clear();
	else if ( sender() == tbClearFilters )
		lwFilters->clear();
	else if ( sender() == tbClearScopes )
		lwScopes->clear();
	else if ( sender() == tbClearPathFiles )
		lwPathFiles->clear();
	else if ( sender() == tbClearQtModules )
		lwQtModules->clear();
	else if ( sender() == tbClearSettings )
		lwSettings->clear();
}

void UISettingsQMake::tbUp_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QListWidget* lw = 0;
	if ( tb == tbUpQtVersion )
		lw = lwQtVersions;
	else if ( tb == tbUpFilter )
		lw = lwFilters;
	else if ( tb == tbUpScope )
		lw = lwScopes;
	else if ( tb == tbUpPathFile )
		lw = lwPathFiles;
	else if ( tb == tbUpQtModule )
		lw = lwQtModules;
	else if ( tb == tbUpSetting )
		lw = lwSettings;
	if ( !lw )
		return;
	if ( QListWidgetItem* it = lw->selectedItems().value( 0 ) )
	{
		int i = lw->row( it );
		if ( i != 0 )
			lw->insertItem( i -1, lw->takeItem( i ) );
		lw->setCurrentItem( it );
	}
}

void UISettingsQMake::tbDown_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QListWidget* lw = 0;
	if ( tb == tbDownQtVersion )
		lw = lwQtVersions;
	else if ( tb == tbDownFilter )
		lw = lwFilters;
	else if ( tb == tbDownScope )
		lw = lwScopes;
	else if ( tb == tbDownPathFile )
		lw = lwPathFiles;
	else if ( tb == tbDownQtModule )
		lw = lwQtModules;
	else if ( tb == tbDownSetting )
		lw = lwSettings;
	if ( !lw )
		return;
	if ( QListWidgetItem* it = lw->selectedItems().value( 0 ) )
	{
		int i = lw->row( it );
		if ( i != lw->count() -1 )
			lw->insertItem( i +1, lw->takeItem( i ) );
		lw->setCurrentItem( it );
	}
}

void UISettingsQMake::on_tbDefaultQtVersion_clicked()
{
	if ( QListWidgetItem* it = lwQtVersions->selectedItems().value( 0 ) )
	{
		for ( int i = 0; i < lwQtVersions->count(); i++ )
		{
			QListWidgetItem* cit = lwQtVersions->item( i );
			cit->setData( QtVersion::DefaultRole, false );
			cit->setBackground( QBrush( Qt::transparent ) );
		}
		it->setData( QtVersion::DefaultRole, true );
		it->setBackground( QBrush( Qt::red ) );
	}
}

void UISettingsQMake::on_tbToolTipFilter_clicked()
{
	// get current item
	QListWidgetItem* it = lwFilters->currentItem();
	// update tooltip if needed
	if ( it )
	{
		bool b;
		QString s = QInputDialog::getText( window(), tr( "Filter ToolTip..." ), tr( "Enter a tooltip for filter '%1' :" ).arg( it->text() ), QLineEdit::Normal, it->toolTip(), &b );
		if ( b )
		{
			if ( s.isEmpty() )
				s = it->text();
			it->setToolTip( s );
		}
	}
}

void UISettingsQMake::on_dbbButtons_helpRequested()
{
	QString s;
	switch ( twQMake->currentIndex() )
	{
		case 0:
			s = tr( "Here you can configure the path/filename for the differents Qt tools, generate Qt api for auto completion." );
			break;
		case 1:
			s = tr( "<b>Filters</b>: Are the variables names that are shown when the project view is filtered.<br/>"
				"<b>Scopes</b>: scopes are used in projects settings so you can configure variable for differents scopes easily.<br/>"
				"<b>Path/Files</b>: This list is used to let plugin know about witch variable names are based on path or files for there contents." 	);
			break;
		case 2:
			s = tr( "<b>Qt Modules</b>: This list contains differents modules available with Qt, you can add your own module if you want, so you can easily check them in project settings." );
			break;
		case 3:
			s = tr( "<b>Settings</b>: This list contains differents settings available with Qt, you can add your own if you want, so you can easily check them in project settings." );
			break;
	}
	if ( !s.isEmpty() )
		QWhatsThis::showText( mapToGlobal( rect().center() ), s );
}

void UISettingsQMake::on_dbbButtons_clicked( QAbstractButton* b )
{
	if ( dbbButtons->standardButton( b )  != QDialogButtonBox::Save )
		return;
	QStringList l;
	pSettings* ps = MonkeyCore::settings();
	// general
	lw_currentItemChanged( 0, lwQtVersions->selectedItems().value( 0 ) );
	ps->beginWriteArray( QString( "Plugins/%1/Versions" ).arg( PLUGIN_NAME ) );
	for ( int i = 0; i < lwQtVersions->count(); i++ )
	{
		QListWidgetItem* it = lwQtVersions->item( i );
		ps->setArrayIndex( i );
		ps->setValue( "Text", it->text() );
		ps->setValue( "Path", it->data( QtVersion::PathRole ).toString() );
		ps->setValue( "QMake", it->data( QtVersion::QMakeRole ).toString() );
		ps->setValue( "lupdate", it->data( QtVersion::lupdateRole ).toString() );
		ps->setValue( "lrelease", it->data( QtVersion::lreleaseRole ).toString() );
		ps->setValue( "DocsPath", it->data( QtVersion::DocsPathRole ).toString() );
		ps->setValue( "Default", it->data( QtVersion::DefaultRole ).toBool() );
	}
	ps->endArray();
	// filters
	l.clear();
	foreach ( QListWidgetItem* it, lwFilters->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	ps->setValue( QString( "Plugins/%1/Filters" ).arg( PLUGIN_NAME ), l );
	// scopes
	l.clear();
	foreach ( QListWidgetItem* it, lwScopes->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	ps->setValue( QString( "Plugins/%1/Scopes" ).arg( PLUGIN_NAME ), l );
	// pathfiles
	l.clear();
	foreach ( QListWidgetItem* it, lwPathFiles->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	ps->setValue( QString( "Plugins/%1/PathFiles" ).arg( PLUGIN_NAME ), l );
	// filters tooltips
	l.clear();
	foreach ( QListWidgetItem* it, lwFilters->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->toolTip();
	ps->setValue( QString( "Plugins/%1/FiltersToolTips" ).arg( PLUGIN_NAME ), l );
	// qt modules
	lw_currentItemChanged( 0, lwQtModules->selectedItems().value( 0 ) );
	ps->beginWriteArray( QString( "Plugins/%1/QtModules" ).arg( PLUGIN_NAME ) );
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* it = lwQtModules->item( i );
		ps->setArrayIndex( i );
		ps->setValue( "Text", it->text() );
		ps->setValue( "Value", it->data( QtItem::ValueRole ).toString() );
		ps->setValue( "Variable", it->data( QtItem::VariableRole ).toString() );
		ps->setValue( "Help", it->data( QtItem::HelpRole ).toString() );
	}
	ps->endArray();
	// qt modules
	lw_currentItemChanged( 0, lwSettings->selectedItems().value( 0 ) );
	ps->beginWriteArray( QString( "Plugins/%1/Settings" ).arg( PLUGIN_NAME ) );
	for ( int i = 0; i < lwSettings->count(); i++ )
	{
		QListWidgetItem* it = lwSettings->item( i );
		ps->setArrayIndex( i );
		ps->setValue( "Text", it->text() );
		ps->setValue( "Value", it->data( QtItem::ValueRole ).toString() );
		ps->setValue( "Variable", it->data( QtItem::VariableRole ).toString() );
		ps->setValue( "Help", it->data( QtItem::HelpRole ).toString() );
	}
	ps->endArray();
}
