#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include "MonkeyExport.h"
#include "pSettings.h"

#include <QtPlugin>
#include <QLabel>

#ifdef __COVERAGESCANNER__
#include <QCoreApplication>
#include <QDir>
#endif

class Q_MONKEY_EXPORT BasePlugin : public QObject
{
	Q_OBJECT
	Q_ENUMS( Type )
	
public:
	// plugin type enums
	enum Type
	{	iAll = -1,
		iBase,
		iChild,
		iCLITool,
		iBuilder,
		iCompiler,
		iDebugger,
		iInterpreter,
		iProject,
		iLast };
		
	// plugin info structure
	struct Q_MONKEY_EXPORT PluginInfos
	{
		QString Caption; // the string to show as caption
		QString Description; // the plugin description
		QString Author; // the plugin author
		BasePlugin::Type Type; // the plugin type
		QString Name; // the plugin name for version control
		QString Version; // the plugin version for version control
		bool Enabled; // to know if this plugin is enabled
	};
	
	BasePlugin()
	{ mPluginInfos.Enabled = false; }
	virtual ~BasePlugin()
	{ if ( isEnabled() ) setEnabled( false ); }
	
	virtual PluginInfos infos() const
	{ return mPluginInfos; }
	
	virtual QWidget* settingsWidget()
	{ return new QLabel( QObject::tr( "This plugin can't be configured" ) ); }
	
	virtual bool isEnabled() const
	{ return mPluginInfos.Enabled; }
	
	virtual bool setEnabled( bool )
	{ return false; }
	
	virtual QString settingsKey( const QString& k ) const
	{ return QString( "Plugins/%1/%2" ).arg( mPluginInfos.Name ).arg(  k ); }
	
	virtual QSettings* settings() const
	{ return pSettings::instance(); }
	
	virtual QVariant settingsValue( const QString& k, const QVariant& v = QVariant() ) const
	{ return pSettings::instance()->value( settingsKey( k ), v ); }
	
	virtual void setSettingsValue( const QString& k, const QVariant& v ) const
	{ pSettings::instance()->setValue( settingsKey( k ), v ); }
	
	// coverage support members
#ifdef __COVERAGESCANNER__
	virtual void saveCodeCoverage( const QString& n, const QString& s )
	{
		// set path
		QString s = QCoreApplication::applicationDirPath();
#ifndef Q_OS_WIN
		s = QDir::homePath();
#endif
		s.append( "/monkeystudio_tests" );
		
		// create path if it not exists
		QDir d( s );
		if ( !d.exists() )
			d.mkdir( s );
		
		// set os specific filename
		s = QDir::toNativeSeparators( s.append( "/monkey_cov" ) ); 
		
		// deal with coverage meter
		__coveragescanner_filename( qPrintable( s ) );
		__coveragescanner_teststate( qPrintable( s ) );
		__coveragescanner_testname( qPrintable( QString( "%1/%2" ).arg( n, infos().Name ) ) );
		__coveragescanner_save();
	}
#endif
	
protected:
	PluginInfos mPluginInfos;
	
};

Q_DECLARE_INTERFACE( BasePlugin, "org.monkeystudio.MonkeyStudio.BasePlugin/1.0" )

#endif // BASEPLUGIN_H
