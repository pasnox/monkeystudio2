#include "QtVersionManager.h"

const QString mQtVersionKey = "Versions";
const QString mQtModuleKey = "Modules";
const QString mQtConfigurationKey = "Configurations";

QtVersionManager::QtVersionManager( QObject* owner )
	: pSettings( owner, "QtVersions", "1.0.0" )
{}

QtVersionList QtVersionManager::versions()
{
	QtVersionList l;
	int s = beginReadArray( mQtVersionKey );
	for ( int i = 0; i < s; i++ )
	{
		setArrayIndex( i );
		l << QtVersion( value( "Version" ).toString(), value( "Path" ).toString(), value( "Default" ).toBool(), value( "QMakeSpec" ).toString(), value( "QMakeParameters" ).toString() );
	}
	endArray();
	return l;
}

void QtVersionManager::setVersions( const QtVersionList& versions )
{
	beginWriteArray( mQtVersionKey );
	foreach ( const QtVersion& v, versions )
	{
		setArrayIndex( versions.indexOf( v ) );
		setValue( "Version", v.Version );
		setValue( "Path", v.Path );
		setValue( "Default", v.Default );
		setValue( "QMakeSpec", v.QMakeSpec );
		setValue( "QMakeParameters", v.QMakeParameters );
	}
	endArray();
}

QtVersion QtVersionManager::defaultVersion()
{
	foreach ( const QtVersion& v, versions() )
		if ( v.Default )
			return v;
	return versions().value( 0 );
}

QtVersion QtVersionManager::version( const QString& version )
{
	foreach ( const QtVersion& v, versions() )
		if ( v.Version == version )
			return v;
	return defaultVersion();
}

QtItemList QtVersionManager::defaultModules()
{
	return QtItemList()
	<< QtItem( "QtCore", "core", "QT", "Add support for Qt Core classes" )
	<< QtItem( "QtGui", "gui", "QT", "Add support for Qt Gui classes" )
	<< QtItem( "QtNetwork", "network", "QT", "Add support for Qt Network classes" )
	<< QtItem( "QtOpenGL", "opengl", "QT", "Add support for Qt OpenGL classes" )
	<< QtItem( "QtSql", "sql", "QT", "Add support for Qt Sql classes" )
	<< QtItem( "QtSvg", "svg", "QT", "Add support for Qt Svg classes" )
	<< QtItem( "QtXml", "xml", "QT", "Add support for Qt Xml classes" )
	<< QtItem( "Qt3Support", "qt3support", "QT", "Add support for Qt Qt3Support classes" );
}

QtItemList QtVersionManager::modules()
{
	QtItemList l = defaultModules();
	int s = beginReadArray( mQtModuleKey );
	for ( int i = 0; i < s; i++ )
	{
		setArrayIndex( i );
		QtItem it( value( "Text" ).toString(), value( "Value" ).toString(), value( "Variable" ).toString(), value( "Help" ).toString() );
		if ( !l.contains( it ) )
			l << it;
	}
	endArray();
	return l;
}

void QtVersionManager::setModules( const QtItemList& modules )
{
	beginWriteArray( mQtModuleKey );
	foreach ( const QtItem& it, modules )
	{
		setArrayIndex( modules.indexOf( it ) );
		setValue( "Text", it.Text );
		setValue( "Value", it.Value );
		setValue( "Variable", it.Variable );
		setValue( "Help", it.Help );
	}
	endArray();
}

QtItemList QtVersionManager::defaultConfigurations()
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
	<< QtItem( "qaxserver_no_postlink", "qaxserver_no_postlink", "CONFIG", "No help available" )
	<< QtItem( "Qt ONLY", QString::null, QString::null, "Options for Qt only" )
	<< QtItem( "designer", "designer", "CONFIG", "Add support for Qt Designer classes" )
	<< QtItem( "uitools", "uitools", "CONFIG", "Add support for Qt UiTools classes" )
	<< QtItem( "assistant", "assistant", "CONFIG", "Add support for Qt Assistant classes" )
	<< QtItem( "qtestlib", "qtestlib", "CONFIG", "Add support for Qt Test classes" )
	<< QtItem( "qaxcontainer", "qaxcontainer", "CONFIG", "Add support for QAxContainer classes" )
	<< QtItem( "qaxserver", "qaxserver", "CONFIG", "Add support for QAxServer classes" )
	<< QtItem( "qdbus", "qdbus", "CONFIG", "Add support for Qt DBus classes" );
}

QtItemList QtVersionManager::configurations()
{
	QtItemList l = defaultConfigurations();
	int s = beginReadArray( mQtConfigurationKey );
	for ( int i = 0; i < s; i++ )
	{
		setArrayIndex( i );
		QtItem it( value( "Text" ).toString(), value( "Value" ).toString(), value( "Variable" ).toString(), value( "Help" ).toString() );
		if ( !l.contains( it ) )
			l << it;
	}
	endArray();
	return l;
}

void QtVersionManager::setConfigurations( const QtItemList& configurations )
{
	beginWriteArray( mQtConfigurationKey );
	foreach ( const QtItem& it, configurations )
	{
		setArrayIndex( configurations.indexOf( it ) );
		setValue( "Text", it.Text );
		setValue( "Value", it.Value );
		setValue( "Variable", it.Variable );
		setValue( "Help", it.Help );
	}
	endArray();
}
