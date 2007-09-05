#include "UISettingsQMake.h"
#include "pSettings.h"

// qscintilla include
#include <qscilexercpp.h>
#include <qsciapis.h>

#include <QFileDialog>
#include <QFileInfo>
#include <QWhatsThis>

UISettingsQMake::UISettingsQMake( QWidget* p )
	: QWidget( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	pbProgress->hide();
	
	// create items
	// qt4
	cbKeywords->addItem( QIcon( ":/icons/icons/folder.png" ), "Qt4 Path", PathType );
	cbKeywords->addItem( QIcon( ":/icons/icons/misc.png" ), "QMake4", BinaryType );
	cbKeywords->addItem( QIcon( ":/icons/icons/misc.png" ), "lupdate4", BinaryType );
	cbKeywords->addItem( QIcon( ":/icons/icons/misc.png" ), "lrelease4", BinaryType );
	// qt3
	cbKeywords->addItem( QIcon( ":/icons/icons/folder.png" ), "Qt3 Path", PathType );
	cbKeywords->addItem( QIcon( ":/icons/icons/misc.png" ), "QMake3", BinaryType );
	cbKeywords->addItem( QIcon( ":/icons/icons/misc.png" ), "lupdate3", BinaryType );
	cbKeywords->addItem( QIcon( ":/icons/icons/misc.png" ), "lrelease3", BinaryType );
	// load settings
	loadSettings();
	// connect
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
}

void UISettingsQMake::recursiveFiles( QDir d )
{
	// looking fodlers to load
	foreach ( QString s, d.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) )
	{
		d.cd( s );
		recursiveFiles( d );
		d.cdUp();
	}
	// looking files to load
	foreach ( QString s, d.entryList( QDir::Files ) )
	{
		lInformations->setText( tr( "Loading file to prepare: %1" ).arg( s ) );
		qApp->processEvents( QEventLoop::ExcludeUserInputEvents );
		mAPI->load( d.absoluteFilePath( s ) );
	}
}

void UISettingsQMake::generateApi( const QString& s1, const QString& s2 ) // qtversion, include path
{
	// disable window
	window()->setEnabled( false );
	// show progress bar
	pbProgress->show();
	pbProgress->setValue( 0 );
	// create api objects
	mLexer = new QsciLexerCPP( this );
	mAPI = new QsciAPIs( mLexer );
	// load prepared files
	mFile = s1 +".api";
	if ( mAPI->isPrepared( mFile ) )
		QFile::remove( mFile );
	// add header files to apis
	recursiveFiles( QDir( s2 ) );
	// connect
	connect( mAPI, SIGNAL( apiPreparationFinished() ), this, SLOT( generationFinished() ) );
	// prepare apis files
	lInformations->setText( tr( "Preparing files, please wait..." ) );
	mAPI->prepare();
}

QStringList UISettingsQMake::defaultOperators()
{
	return QStringList()
	<< "=" << "-=" << "+=" << "*=" << "~=" << ":" << "|";
}

QStringList UISettingsQMake::readOperators()
{
	QStringList l = pSettings::instance()->value( "Plugins/QMake/Operators" ).toStringList();
	return l.isEmpty() ? defaultOperators() : l;
}

QStringList UISettingsQMake::defaultFilters()
{
	return QStringList()
	<< "FORMS" << "FORMS3" << "HEADERS" << "SOURCES" << "RESOURCES" << "IMAGES" << "TRANSLATIONS";
}

QStringList UISettingsQMake::readFilters()
{
	QStringList l = pSettings::instance()->value( "Plugins/QMake/Filters" ).toStringList();
	return l.isEmpty() ? defaultFilters() : l;
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
	QStringList l = pSettings::instance()->value( "Plugins/QMake/Scopes" ).toStringList();
	return l.isEmpty() ? defaultScopes() : l;
}

QStringList UISettingsQMake::defaultPathFiles()
{
	return QStringList()
	<< "FORMS" << "FORMS3" << "HEADERS" << "SOURCES" << "RESOURCES" << "IMAGES" << "TRANSLATIONS" << "DEF_FILE"
	<< "DEPENDPATH" << "DESTDIR" << "DESTDIR_TARGET" << "DLLDESTDIR" << "DISTFILES" << "INCLUDEPATH" << "MOC_DIR"
	<< "OBJECTS_DIR" << "RC_FILE" << "RCC_DIR" << "RES_FILE" << "TARGET" << "UI_DIR" << "VPATH" << "YACCSOURCES"
	<< "LEXSOURCES" << "SUBDIRS";
}

QStringList UISettingsQMake::readPathFiles()
{
	QStringList l = pSettings::instance()->value( "Plugins/QMake/PathFiles" ).toStringList();
	return l.isEmpty() ? defaultPathFiles() : l;
}

QtItemList UISettingsQMake::defaultQtModules()
{
	return QtItemList()
	<< new QtItem( "QtCore", "core", "QT", "Add support for Qt Core classes" )
	<< new QtItem( "QtGui", "gui", "QT", "Add support for Qt Gui classes" )
	<< new QtItem( "QtNetwork", "network", "QT", "Add support for Qt Network classes" )
	<< new QtItem( "QtOpenGL", "opengl", "QT", "Add support for Qt OpenGL classes" )
	<< new QtItem( "QtSql", "sql", "QT", "Add support for Qt Sql classes" )
	<< new QtItem( "QtSvg", "svg", "QT", "Add support for Qt Svg classes" )
	<< new QtItem( "QtXml", "xml", "QT", "Add support for Qt Xml classes" )
	<< new QtItem( "Qt3Support", "qt3support", "QT", "Add support for Qt Qt3Support classes" )
	<< new QtItem( "QtDesigner", "designer", "CONFIG", "Add support for Qt Designer classes" )
	<< new QtItem( "QtUiTools", "uitools", "CONFIG", "Add support for Qt UiTools classes" )
	<< new QtItem( "QtAssistant", "assistant", "CONFIG", "Add support for Qt Assistant classes" )
	<< new QtItem( "QtTest", "qtestlib", "CONFIG", "Add support for Qt Test classes" )
	<< new QtItem( "QAxContainer", "qaxcontainer", "CONFIG", "Add support for QAxContainer classes" )
	<< new QtItem( "QAxServer", "qaxserver", "CONFIG", "Add support for QAxServer classes" )
	<< new QtItem( "QtDBus", "qdbus", "CONFIG", "Add support for Qt DBus classes" );
}

QtItemList UISettingsQMake::readQtModules()
{
	QtItemList l;
	int s = pSettings::instance()->beginReadArray( "Plugins/QMake/QtModules" );
	for ( int i = 0; i < s; i++ )
	{
		pSettings::instance()->setArrayIndex( i );
		l << new QtItem( pSettings::instance()->value( "Text" ).toString(), pSettings::instance()->value( "Value" ).toString(), pSettings::instance()->value( "Variable" ).toString(), pSettings::instance()->value( "Help" ).toString() );
	}
	pSettings::instance()->endArray();
	return l.isEmpty() ? defaultQtModules() : l;
}

QtItemList UISettingsQMake::defaultSettings()
{
	return QtItemList()
	<< new QtItem( "rtti", "rtti", "CONFIG", "RTTI support is enabled." )
	<< new QtItem( "stl", "stl", "CONFIG", "STL support is enabled." )
	<< new QtItem( "exceptions", "execptions", "CONFIG", "Exception support is enabled." )
	<< new QtItem( "thread", "thread", "CONFIG", "The target is a multi-threaded application or library. The proper defines and compiler flags will automatically be added to the project." )
	<< new QtItem( "no_lflags_merge", "no_lflags_merge", "CONFIG", "Ensures that the list of libraries stored in the LIBS variable is not reduced to a list of unique values before it is used." )
	<< new QtItem( "qt", "qt", "CONFIG", "The target is a Qt application/library and requires the Qt library and header files. The proper include and library paths for the Qt library will automatically be added to the project. This is defined by default, and can be fine-tuned with the \\l{#qt}{QT} variable." )
	<< new QtItem( "resources", "resources", "CONFIG", "Configures qmake to run rcc on the content of RESOURCES if defined." )
	<< new QtItem( "uic3", "uic3", "CONFIG", "Configures qmake to run uic3 on the content of FORMS3 if defined; otherwise the contents of FORMS will be processed instead." )
	<< new QtItem( "LIB ONLY", QString::null, QString::null, "Options for LIB template only" )
	<< new QtItem( "dll", "dll", "CONFIG", "The target is a shared object/DLL.The proper include paths, compiler flags and libraries will automatically be added to the project." )
	<< new QtItem( "staticlib", "staticlib", "CONFIG", "The target is a static library (lib only). The proper compiler flags will automatically be added to the project." )
	<< new QtItem( "plugin", "plugin", "CONFIG", "The target is a plugin (lib only). This enables dll as well." )
	<< new QtItem( "X11 ONLY", QString::null, QString::null, "Options for X11 only" )
	<< new QtItem( "x11", "x11", "CONFIG", "The target is a X11 application or library. The proper include paths and libraries will automatically be added to the project." )
	<< new QtItem( "MAC OS X ONLY", QString::null, QString::null, "Options for Mac OS X only" )
	<< new QtItem( "ppc", "ppc", "CONFIG", "Builds a PowerPC binary." )
	<< new QtItem( "x86", "x86", "CONFIG", "Builds an i386 compatible binary." )
	<< new QtItem( "app_bundle", "app_bundle", "CONFIG", "Puts the executable into a bundle (this is the default)." )
	<< new QtItem( "lib_bundle", "lib_bundle", "CONFIG", "Puts the library into a library bundle." )
	<< new QtItem( "WINDOWS ONLY", QString::null, QString::null, "Options for Windows only" )
	<< new QtItem( "windows", "windows", "CONFIG", "The target is a Win32 window application (app only). The proper include paths,compiler flags and libraries will automatically be added to the project." )
	<< new QtItem( "console", "console", "CONFIG", "The target is a Win32 console application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project." )
	<< new QtItem( "flat", "flat", "CONFIG", "When using the vcapp template this will put all the source files into the source group and the header files into the header group regardless of what directory they reside in. Turning this option off will group the files within the source/header group depending on the directory they reside. This is turned on by default." )
	<< new QtItem( "embed_manifest_dll", "embed_manifest_dll", "CONFIG", "Embeds a manifest file in the DLL created as part of an application/library project." )
	<< new QtItem( "ACTIVEQT ONLY", QString::null, QString::null, "Option for Windows/Active Qt only" )
	<< new QtItem( "qaxserver_no_postlink", "qaxserver_no_postlink", "CONFIG", "No help available" );
}

QtItemList UISettingsQMake::readSettings()
{
	QtItemList l;
	int s = pSettings::instance()->beginReadArray( "Plugins/QMake/Settings" );
	for ( int i = 0; i < s; i++ )
	{
		pSettings::instance()->setArrayIndex( i );
		l << new QtItem( pSettings::instance()->value( "Text" ).toString(), pSettings::instance()->value( "Value" ).toString(), pSettings::instance()->value( "Variable" ).toString(), pSettings::instance()->value( "Help" ).toString() );
	}
	pSettings::instance()->endArray();
	return l.isEmpty() ? defaultSettings() : l;
}

void UISettingsQMake::generationFinished()
{
	// save prepared files
	mAPI->savePrepared( mFile );
	// delete api objects
	delete mLexer;
	// hide progress bar
	pbProgress->setValue( -1 );
	pbProgress->hide();
	// finish message
	lInformations->setText( tr( "Prepared files finished." ) );
	// enable window
	window()->setEnabled( true );
}

void UISettingsQMake::loadSettings()
{
	// general
	for ( int i = 0; i < cbKeywords->count(); i++ )
		cbKeywords->setItemData( i, pSettings::instance()->value( QString( "Plugins/QMake/%1" ).arg( cbKeywords->itemText( i ) ) ).toString(), DataRole );
	if ( cbKeywords->count() )
		on_cbKeywords_currentIndexChanged( 0 );
	lwOperators->addItems( readOperators() );
	// filters & scopes
	lwFilters->addItems( readFilters() );
	lwScopes->addItems( readScopes() );
	lwPathFiles->addItems( readPathFiles() );
	// set items editable
	QList<QListWidgetItem*> items = QList<QListWidgetItem*> () 
	<< lwOperators->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive )
	<< lwFilters->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive )
	<< lwScopes->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive )
	<< lwPathFiles->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive );
	foreach ( QListWidgetItem* it, items )
		it->setFlags( it->flags() | Qt::ItemIsEditable );
	items.clear();
	//
	QtItemList l;
	// qt modules
	l = readQtModules();
	foreach ( QtItem* i, l )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwQtModules );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
	}
	qDeleteAll( l );
	l.clear();
	// settings
	l = readSettings();
	foreach ( QtItem* i, l )
	{
		QListWidgetItem* it = new QListWidgetItem( i->Text, lwSettings );
		it->setData( QtItem::ValueRole, i->Value );
		it->setData( QtItem::VariableRole, i->Variable );
		it->setData( QtItem::HelpRole, i->Help );
	}
	qDeleteAll( l );
	l.clear();
}

void UISettingsQMake::on_cbKeywords_currentIndexChanged( int i )
{
	lePath->setText( cbKeywords->itemData( i, DataRole ).toString() );
}

void UISettingsQMake::on_lePath_editingFinished()
{
	cbKeywords->setItemData( cbKeywords->currentIndex(), lePath->text(), DataRole );
}

void UISettingsQMake::on_tbBrowse_clicked()
{
	QString s = cbKeywords->currentText();
	switch ( cbKeywords->itemData( cbKeywords->currentIndex(), TypeRole ).toInt() )
	{
	case BinaryType:
		s = QFileDialog::getOpenFileName( this, tr( "Choose your tool binary" ), s );
		if ( !s.isNull() )
			lePath->setText( s );
		break;
	case PathType:
		s = QFileDialog::getExistingDirectory( this, tr( "Locate your tool path" ), s );
		if ( !s.isNull() )
			lePath->setText( s );
		break;
	}
}

void UISettingsQMake::on_pbGenerate_clicked()
{
	// apply settings in case of user set path but not yet click on apply
	on_bbDialog_clicked( bbDialog->button( QDialogButtonBox::Apply ) );
	// clear label
	lInformations->clear();
	// if no checked box cancel
	if ( !cbQt4->isChecked() && !cbQt3->isChecked() )
	{
		lInformations->setText( tr( "You need to check the boxes for the Qt version you want to generate api." ) );
		return;
	}
	//
	QString s;
	// if qt4 and no qt4 path configured
	if ( cbQt4->isChecked() )
	{
		s = pSettings::instance()->value( "Plugins/QMake/Qt4 Path" ).toString().append( "/include" );
		s = QFileDialog::getExistingDirectory( this, tr( "Locate your Qt4 include path" ), s );
		if ( !s.isNull() )
			generateApi( "Qt4", s );
	}
	// if qt3 and no qt3 path configured
	if ( cbQt3->isChecked() )
	{
		s = pSettings::instance()->value( "Plugins/QMake/Qt3 Path" ).toString().append( "/include" );
		s = QFileDialog::getExistingDirectory( this, tr( "Locate your Qt3 include path" ), s );
		if ( !s.isNull() )
			generateApi( "Qt3", s );
	}
}

void UISettingsQMake::lw_currentItemChanged( QListWidgetItem* c, QListWidgetItem* p )
{
	if ( p )
	{
		if ( p->listWidget() == lwQtModules )
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
		if ( c->listWidget() == lwQtModules )
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

void UISettingsQMake::tbAdd_clicked()
{
	QListWidget* lw = 0;
	if ( sender() == tbAddOperator )
		lw = lwOperators;
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
	if ( sender() == tbRemoveOperator )
		delete lwOperators->currentItem();
	else if ( sender() == tbRemoveFilter )
		delete lwFilters->currentItem();
	else if ( sender() == tbRemoveScope )
		delete lwScopes->currentItem();
	else if ( sender() == tbRemovePathFile )
		delete lwPathFiles->currentItem();
	else if ( sender() == tbRemoveQtModule )
		delete lwQtModules->currentItem();
	else if ( sender() == tbRemoveSetting )
		delete lwSettings->currentItem();
}

void UISettingsQMake::tbClear_clicked()
{
	if ( sender() == tbClearOperators )
		lwOperators->clear();
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
	if ( tb == tbUpOperator )
		lw = lwOperators;
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
	QListWidgetItem* it = lw->currentItem();
	int i = lw->row( it );
	if ( !it )
		return;
	if ( i != 0 )
		lw->insertItem( i -1, lw->takeItem( i ) );
	lw->setCurrentItem( it );
}

void UISettingsQMake::tbDown_clicked()
{
	QToolButton* tb = qobject_cast<QToolButton*>( sender() );
	if ( !tb )
		return;
	QListWidget* lw = 0;
	if ( tb == tbDownOperator )
		lw = lwOperators;
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
	QListWidgetItem* it = lw->currentItem();
	int i = lw->row( it );
	if ( !it )
		return;
	if ( i != lw->count() -1 )
		lw->insertItem( i +1, lw->takeItem( i ) );
	lw->setCurrentItem( it );
}

void UISettingsQMake::on_bbDialog_helpRequested()
{
	QString s;
	switch ( twQMake->currentIndex() )
	{
		case 0:
			s = tr( "Here you can configure the path/filename for the differents Qt tools, generate Qt api for auto completion.<br>"
				"<b>Operators</b>: The operators are used in project settings, so you can easily configure your project with differents operators." );
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
		QWhatsThis::showText( bbDialog->button( QDialogButtonBox::Help )->mapToGlobal( QPoint( 0, 0 ) ), s );
}

void UISettingsQMake::on_bbDialog_clicked( QAbstractButton* b )
{
	if ( bbDialog->standardButton( b )  != QDialogButtonBox::Apply )
		return;
	QStringList l;
	// general
	for ( int i = 0; i < cbKeywords->count(); i++ )
		pSettings::instance()->setValue( QString( "Plugins/QMake/%1" ).arg( cbKeywords->itemText( i ) ), cbKeywords->itemData( i, DataRole ).toString() );
	foreach ( QListWidgetItem* it, lwOperators->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	pSettings::instance()->setValue( "Plugins/QMake/Operators", l );
	// filters & scopes
	l.clear();
	foreach ( QListWidgetItem* it, lwFilters->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	pSettings::instance()->setValue( "Plugins/QMake/Filters", l );
	l.clear();
	foreach ( QListWidgetItem* it, lwScopes->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	pSettings::instance()->setValue( "Plugins/QMake/Scopes", l );
	l.clear();
	foreach ( QListWidgetItem* it, lwPathFiles->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		l << it->text();
	pSettings::instance()->setValue( "Plugins/QMake/PathFiles", l );
	// qt modules
	lw_currentItemChanged( 0, lwQtModules->currentItem() );
	pSettings::instance()->beginWriteArray( "Plugins/QMake/QtModules" );
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* it = lwQtModules->item( i );
		pSettings::instance()->setArrayIndex( i );
		pSettings::instance()->setValue( "Text", it->text() );
		pSettings::instance()->setValue( "Value", it->data( QtItem::ValueRole ).toString() );
		pSettings::instance()->setValue( "Variable", it->data( QtItem::VariableRole ).toString() );
		pSettings::instance()->setValue( "Help", it->data( QtItem::HelpRole ).toString() );
	}
	pSettings::instance()->endArray();
	// qt modules
	lw_currentItemChanged( 0, lwSettings->currentItem() );
	pSettings::instance()->beginWriteArray( "Plugins/QMake/Settings" );
	for ( int i = 0; i < lwSettings->count(); i++ )
	{
		QListWidgetItem* it = lwSettings->item( i );
		pSettings::instance()->setArrayIndex( i );
		pSettings::instance()->setValue( "Text", it->text() );
		pSettings::instance()->setValue( "Value", it->data( QtItem::ValueRole ).toString() );
		pSettings::instance()->setValue( "Variable", it->data( QtItem::VariableRole ).toString() );
		pSettings::instance()->setValue( "Help", it->data( QtItem::HelpRole ).toString() );
	}
	pSettings::instance()->endArray();
}
