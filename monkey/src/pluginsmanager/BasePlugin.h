#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H
//
#include <QtPlugin>
#include <QLabel>
#ifdef __COVERAGESCANNER__
#include <QCoreApplication>
#include <QDir>
#endif
//
#include "Workspace.h"
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT BasePlugin : public QObject
{
	Q_OBJECT
	Q_ENUMS( Type )
	//
public:
	// plugin type enums
	enum Type
	{	iAll = -1,
		iBase,
		iChild,
		iWorkspace,
		iCompiler,
		iDebugger,
		iProject,
		iLast };
	// plugin info structure
	struct Q_MONKEY_EXPORT PluginInfos
	{
		QString Caption; // the string to show as caption
		QString Description; // the plugin description
		BasePlugin::Type Type; // the plugin type
		QString Name; // the plugin name for version control
		QString Version; // the plugin version for version control
		bool Installed; // to know if this plugin is installed
	};
	//
	BasePlugin()
	{ mPluginInfos.Installed = false; mWorkspace = 0; }
	virtual ~BasePlugin() {}
	//
	virtual void initialize( Workspace* w )
	{ Q_ASSERT( w != 0 ); mWorkspace = w; }
	//
	virtual PluginInfos infos() const
	{ return mPluginInfos; }
	//
	virtual QWidget* settingsWidget()
	{ return new QLabel( QObject::tr( "This plugin can't be configured" ) ); }
	//
	virtual bool isInstalled() const
	{ return mPluginInfos.Installed; }
	// coverage support members
#ifdef __COVERAGESCANNER__
	virtual void saveCodeCoverage( const QString& n, const QString& s )
	{
		__coveragescanner_filename( codeCoverageFile().toAscii() );
		__coveragescanner_teststate( s.toAscii() );
		__coveragescanner_testname( QString( "%1/%2" ).arg( n, infos().Name ).toAscii() );
		__coveragescanner_save();
	}
	//
	static QString codeCoverageFile()
	{
#ifdef Q_OS_WIN
		QString s = QCoreApplication::applicationDirPath();
#else
		QString s = QDir::homePath();
#endif
		s.append( "/monkeystudio_tests" );
		QDir d( s );
		if ( !d.exists() )
			d.mkdir( s );
		return QDir::toNativeSeparators( s.append( "/monkey_cov" ) ); 
	}
#endif
	// NEED REIMPLEMENTATION
	virtual bool install() = 0;
	virtual bool uninstall() = 0;
	//
protected:
	PluginInfos mPluginInfos;
	Workspace* mWorkspace;
	//
};
//
Q_DECLARE_INTERFACE( BasePlugin, "org.monkeystudio.MonkeyStudio.BasePlugin/1.0" )
//
#endif // BASEPLUGIN_H
