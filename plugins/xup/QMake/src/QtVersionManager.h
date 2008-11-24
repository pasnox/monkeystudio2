#ifndef QTVERSIONMANAGER_H
#define QTVERSIONMANAGER_H

#include <pSettings.h>

struct QtVersion
{
	QtVersion() { Default = false; HaveQt4Suffixe = false; }
	QtVersion( const QString& version, const QString& path, bool def, const QString& qmakeSpec, const QString& qmakeParams, bool haveSuffixe )
	{ Version = version; Path = path; Default = def; QMakeSpec = qmakeSpec; QMakeParameters = qmakeParams; HaveQt4Suffixe = haveSuffixe; }
	
	bool isValid() const
	{ return !( Version.isEmpty() && Path.isEmpty() ); }
	
	QString qmake() const
	{ return QString( "%1/bin/qmake%2" ).arg( Path ).arg( binarySuffixe() ); }
	QString qmakeSpec() const
	{ return ( QMakeSpec != "default" && !QMakeSpec.isEmpty() ) ? QString( "-spec %1" ).arg( QMakeSpec ) : QString(); }
	QString qmakeParameters() const
	{ return qmakeSpec().append( " " +QMakeParameters ); }
	QString lupdate() const
	{ return QString( "%1/bin/lupdate%2" ).arg( Path ).arg( binarySuffixe() ); }
	QString lrelease() const
	{ return QString( "%1/bin/lrelease%2" ).arg( Path ).arg( binarySuffixe() ); }
	QString designer() const
	{ return QString( "%1/bin/designer%2" ).arg( Path ).arg( binarySuffixe() ); }
	QString assistant() const
	{ return QString( "%1/bin/assistant%2" ).arg( Path ).arg( binarySuffixe() ); }
	QString linguist() const
	{ return QString( "%1/bin/linguist%2" ).arg( Path ).arg( binarySuffixe() ); }
	QString binarySuffixe() const
	{ return HaveQt4Suffixe ? QString( "-qt4" ) : QString::null; }
	
	bool operator==( const QtVersion& o ) const
	{ return Version == o.Version && Path == o.Path && QMakeSpec == o.QMakeSpec && QMakeParameters == o.QMakeParameters && HaveQt4Suffixe == o.HaveQt4Suffixe; }
	bool operator!=( const QtVersion& o ) const
	{ return !operator==( o ); }
	
	QString Version;
	QString Path;
	bool Default;
	QString QMakeSpec;
	QString QMakeParameters;
	bool HaveQt4Suffixe;
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

	void checkForSystemVersion();
	QtVersion systemVersion() const;
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

protected:
	static const QString mQtVersionKey;
	static const QString mQtModuleKey;
	static const QString mQtConfigurationKey;
};

#endif // QTVERSIONMANAGER_H
