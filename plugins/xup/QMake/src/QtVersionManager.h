#ifndef QTVERSIONMANAGER_H
#define QTVERSIONMANAGER_H

#include <pSettings.h>

struct QtVersion
{
	QtVersion() { Default = false; }
	QtVersion( const QString& version, const QString& path, bool def, const QString& qmakeSpec, const QString& qmakeParams ) { Version = version; Path = path; Default = def; QMakeSpec = qmakeSpec; QMakeParameters = qmakeParams; }
	
	bool isValid() const { return !( Version.isEmpty() && Path.isEmpty() ); }
	
	QString qmake() const { return QString( "%1/bin/qmake" ).arg( Path ); }
	QString qmakeSpec() const { return ( QMakeSpec != "default" && !QMakeSpec.isEmpty() ) ? QString( "-spec %1" ).arg( QMakeSpec ) : QString(); }
	QString qmakeParameters() const { return qmakeSpec().append( " " +QMakeParameters ); }
	QString lupdate() const { return QString( "%1/bin/lupdate" ).arg( Path ); }
	QString lrelease() const { return QString( "%1/bin/lrelease" ).arg( Path ); }
	QString designer() const { return QString( "%1/bin/designer" ).arg( Path ); }
	QString assistant() const { return QString( "%1/bin/assistant" ).arg( Path ); }
	QString linguist() const { return QString( "%1/bin/linguist" ).arg( Path ); }
	
	bool operator==( const QtVersion& o ) const { return Version == o.Version && Path == o.Path && QMakeSpec == o.QMakeSpec && QMakeParameters == o.QMakeParameters; }
	bool operator!=( const QtVersion& o ) const { return !operator==( o ); }
	
	QString Version;
	QString Path;
	bool Default;
	QString QMakeSpec;
	QString QMakeParameters;
};

struct QtItem
{
	QtItem() {}
	QtItem( const QString& t, const QString& v, const QString& s, const QString& h = QString::null )
		: Text( t ), Value( v ), Variable( s ), Help( h ) {}
	
	bool operator==( const QtItem& o ) { return Text == o.Text && Value == o.Value && Variable == o.Variable && Help == o.Help; }
	bool operator!=( const QtItem& o ) { return !operator==( o ); }
	
	QString Text;
	QString Value;
	QString Variable;
	QString Help;
};

typedef QList<QtVersion> QtVersionList;
typedef QList<QtItem> QtItemList;

Q_DECLARE_METATYPE( QtVersion );
Q_DECLARE_METATYPE( QtVersionList );
Q_DECLARE_METATYPE( QtItem );
Q_DECLARE_METATYPE( QtItemList );

class QtVersionManager : public pSettings
{
	Q_OBJECT
	
public:
	QtVersionManager( QObject* owner = 0 );

	QtVersionList versions();
	void setVersions( const QtVersionList& versions );
	QtVersion defaultVersion();
	QtVersion version( const QString& version );

	QtItemList defaultModules();
	QtItemList modules();
	void setModules( const QtItemList& modules );

	QtItemList defaultConfigurations();
	QtItemList configurations();
	void setConfigurations( const QtItemList& configurations );
};

#endif // QTVERSIONMANAGER_H
