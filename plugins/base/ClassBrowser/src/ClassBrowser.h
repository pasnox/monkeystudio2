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
#ifndef CLASSBROWSER_H
#define CLASSBROWSER_H

#include <pluginsmanager/BasePlugin.h>
#include <qCtagsSense.h>

#include <QPointer>

class pDockClassBrowser;
class pAbstractChild;
class XUPProjectItem;
struct qCtagsSenseEntry;

class ClassBrowser : public QObject, public BasePlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA( IID "org.monkeystudio.MonkeyStudio.BasePlugin/1.0" /*FILE PLUGIN_JSON*/ )
#endif
    Q_INTERFACES( BasePlugin )

    void fillPluginInfos();
    virtual bool install();
    virtual bool uninstall();
public:
    enum IntegrationMode { imDock, imCombo, imBoth };
    
    virtual QWidget* settingsWidget() const;
    
    qCtagsSenseProperties properties() const;
    ClassBrowser::IntegrationMode integrationMode() const;
    
    static QString defaultDatabase();

public slots:
    void setProperties( const qCtagsSenseProperties& properties );
    void setIntegrationMode( ClassBrowser::IntegrationMode mode );

protected:
    QPointer<pDockClassBrowser> mDock;

protected slots:
    void documentOpened( pAbstractChild* document );
    void currentDocumentChanged( pAbstractChild* document );
    void opened( XUPProjectItem* project );
    void buffersChanged( const QMap<QString, QString>& entries );
    void entryActivated( const qCtagsSenseEntry& entry );
    void fileNameActivated( const QString& fileName );

signals:
    void propertiesChanged( const qCtagsSenseProperties& properties );
    void integrationModeChanged( ClassBrowser::IntegrationMode mode );
};

#endif // CLASSBROWSER_H
