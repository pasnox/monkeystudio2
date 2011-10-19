/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

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
#ifndef QTVERSIONMANAGER_H
#define QTVERSIONMANAGER_H

#include <pSettings.h>

#include <QFile>
#include <QDomDocument>

class MkSShellInterpreter;

struct QtVersion
{
    QtVersion( const QString& version = QString::null) { Version = version; Default = false; HasQt4Suffix = false; }
    QtVersion( const QString& version, const QString& path, bool def, const QString& qmakeSpec, const QString& qmakeParams, bool haveSuffixe )
    { Version = version; Path = path; Default = def; QMakeSpec = qmakeSpec; QMakeParameters = qmakeParams; HasQt4Suffix = haveSuffixe; }

    quint32 hash() const
    {
            return Path.isEmpty()
                ? qHash( QString( "%1/%2" ).arg( Version ).arg( QMakeSpec ) )
                : qHash( QString( "%1/%2" ).arg( Path ).arg( QMakeSpec )/*.arg( QMakeParameters ).arg( HasQt4Suffix )*/ )
            ;
    }

    bool isValid() const
    {
        return !Version.isEmpty() &&
            (
                ( !Path.isEmpty() && QFile::exists( Path ) )
                || Path.isEmpty()
            )
        ;
    }

    bool isSystem() const
    { return Version.startsWith( "Qt System", Qt::CaseInsensitive ); }

    QString qmake() const
    { return mkPath( "qmake" ); }
    QString lupdate() const
    { return mkPath( "lupdate" ); }
    QString lrelease() const
    { return mkPath( "lrelease" ); }
    QString designer() const
    { return mkPath( "designer" ); }
    QString assistant() const
    { return mkPath( "assistant" ); }
    QString linguist() const
    { return mkPath( "linguist" ); }

    QString qmakeSpec() const
    { return ( QMakeSpec != "default" && !QMakeSpec.isEmpty() ) ? QString( "-spec %1" ).arg( QMakeSpec ) : QString(); }
    QString qmakeParameters() const
    { return qmakeSpec().append( " " +QMakeParameters ); }

    QString toXml() const
    {
        QDomDocument document( "Qt Version Definition" );
        QDomElement rootElement = document.createElement( "QtVersion" );
        QDomElement versionElement = document.createElement( "Version" );
        QDomElement pathElement = document.createElement( "Path" );
        QDomElement defaultElement = document.createElement( "Default" );
        QDomElement qmakeSpecElement = document.createElement( "QMakeSpec" );
        QDomElement qmakeParametersElement = document.createElement( "QMakeParameters" );
        QDomElement hasQt4SuffixElement = document.createElement( "HasQt4Suffix" );

        versionElement.setAttribute( "value", Version );
        pathElement.setAttribute( "value", Path );
        defaultElement.setAttribute( "value", Default );
        qmakeSpecElement.setAttribute( "value", QMakeSpec );
        qmakeParametersElement.setAttribute( "value", QMakeParameters );
        hasQt4SuffixElement.setAttribute( "value", HasQt4Suffix );

        document.appendChild( rootElement );
        rootElement.appendChild( versionElement );
        rootElement.appendChild( pathElement );
        rootElement.appendChild( defaultElement );
        rootElement.appendChild( qmakeSpecElement );
        rootElement.appendChild( qmakeParametersElement );
        rootElement.appendChild( hasQt4SuffixElement );

        return document.toString( 4 );
    }

    bool operator==( const QtVersion& other ) const
    {
        return Version == other.Version && Path == other.Path
            && QMakeSpec == other.QMakeSpec && QMakeParameters == other.QMakeParameters
            && HasQt4Suffix == other.HasQt4Suffix;
    }

    bool operator!=( const QtVersion& other ) const
    { return !operator==( other ); }

    QString mkPath( const QString& binary ) const
    {
        return Path.isEmpty()
            ? QString( "%1%2" ).arg( binary ).arg( HasQt4Suffix ? QString( "-qt4" ) : QString::null )
            : QString( "%1/bin/%2%3" ).arg( Path ).arg( binary ).arg( HasQt4Suffix ? QString( "-qt4" ) : QString::null )
        ;
    }

    QString Version;
    QString Path;
    bool Default;
    QString QMakeSpec;
    QString QMakeParameters;
    bool HasQt4Suffix;
};

struct QtItem
{
    QtItem( const QString& t = QString::null ) : Text( t ) {}
    QtItem( const QString& t, const QString& v, const QString& s, const QString& h = QString::null )
        : Text( t ), Value( v ), Variable( s ), Help( h ) {}

    bool operator==( const QtItem& o ) const { return Text == o.Text && Value == o.Value && Variable == o.Variable && Help == o.Help; }
    bool operator!=( const QtItem& o ) const { return !operator==( o ); }

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
    ~QtVersionManager();

    QtVersionList versions() const;
    void setVersions( const QtVersionList& versions );

    QtVersion defaultVersion() const;
    QtVersion version( const QString& versionString ) const;

    QtItemList defaultModules() const;
    QtItemList modules() const;
    void setModules( const QtItemList& modules );

    QtItemList defaultConfigurations() const;
    QtItemList configurations() const;
    void setConfigurations( const QtItemList& configurations );

protected:
    static const QString mQtVersionKey;
    static const QString mQtModuleKey;
    static const QString mQtConfigurationKey;
    static const QRegExp mQtVersionRegExp;
    static const QRegExp mQtQMakeRegExp;
    static const QRegExp mQtUninstallRegExp;

    QStringList possibleQtPaths() const;
    QtVersionList getQtVersions( const QStringList& paths ) const;
    void synchronizeVersions();

    // interpreter handling
    void initializeInterpreterCommands( bool initialize );
    static QString commandInterpreter( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter, void* data );
};

#endif // QTVERSIONMANAGER_H
