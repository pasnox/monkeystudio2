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
#ifndef PACTIONSMANAGER_H
#define PACTIONSMANAGER_H

#include "MonkeyExport.h"

#include <QObject>
#include <QList>
#include <QAction>
#include <QPointer>

typedef QList<QAction*> QActionList;
typedef QMap<QString, QString> QMapStringString;
class QSettings;

class Q_MONKEY_EXPORT pActionsManager : public QObject
{
    Q_OBJECT
    
public:
    enum PropertyType { ActionsManager = 0, ActionPath, DefaultShortcut };
    
    pActionsManager( const QString& name, QObject* parent = 0 );
    virtual ~pActionsManager();
    
    void setSettings( QSettings* settings );
    QSettings* settings() const;
    
    QString name() const;
    void reloadSettings() const;
    QActionList actions( const QString& path = QString::null ) const;
    QAction* action( const QString& path, const QString& name ) const;
    QAction* newAction( const QString& path, const QKeySequence& defaultShortcut, const QString& name );
    QString lastError() const;
    
    static QString fixedPath( const QString& path );
    static bool setShortcut( QAction* action, const QKeySequence& shortcut );
    
    static pActionsManager* actionsManager( QAction* action );
    static void setActionsManager( QAction* action, pActionsManager* manager );
    
    static QString actionPath( QAction* action );
    static void setActionPath( QAction* action, const QString& path );
    
    static QKeySequence defaultShortcut( QAction* action );
    static void setDefaultShortcut( QAction* action, const QKeySequence& shortcut );
    
    static QString pathPartTranslation( const QString& part );
    static QString pathTranslation( const QString& path, const QString& separator = QString( " > " ) );
    static QString pathTranslation( QAction* action, const QString& separator = QString( " > " ) );
    static void setPathPartTranslation( const QString& part, const QString& translation );

protected:
    QString mName;
    QActionList mActions;
    QString mLastError;
    QPointer<QSettings> mSettings;
    static QMapStringString mPathPartTranslations; // one part of path, translation
    static const QString mSettingsScope;
    static int mUnknowActionCount;
    
    void updateShortcut( QAction* action ) const;

protected slots:
    void actionDestroyed( QObject* object );

public slots:
    void editActionsShortcuts();
};

Q_DECLARE_METATYPE( QAction* );
Q_DECLARE_METATYPE( QActionList );
Q_DECLARE_METATYPE( pActionsManager* );

#endif // PACTIONSMANAGER_H
