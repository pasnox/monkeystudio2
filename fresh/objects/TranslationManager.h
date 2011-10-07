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
#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QSet>
#include <QHash>
#include <QLocale>
#include <QFileInfo>

class QTranslator;

typedef QList<QTranslator*> QTranslatorList;

class TranslationManager : public QObject
{
    Q_OBJECT
    
public:
    TranslationManager( QObject* parent = 0 );
    virtual ~TranslationManager();
    
    QStringList translationsMasks() const;
    QStringList forbiddenTranslationsMasks() const;
    QLocale currentLocale() const;
    QLocale systemLocale() const;
    QStringList availableLocales() const;
    QList<QLocale> availableQLocales() const;
    QStringList translationsPaths() const;
    QStringList systemTranslationsPaths() const;
    bool isFakeCLocaleEnabled() const;

public slots:
    void reloadTranslations();
    void setTranslationsMasks( const QStringList& masks );
    void addTranslationsMask( const QString& mask );
    void removeTranslationsMask( const QString& mask );
    void setForbiddenTranslationsMasks( const QStringList& masks );
    void addForbiddenTranslationsMask( const QString& mask );
    void removeForbiddenTranslationsMask( const QString& mask );
    void setCurrentLocale( const QLocale& locale );
    void setTranslationsPaths( const QStringList& paths );
    void setSystemTranslationsPaths( const QStringList& paths );
    void setFakeCLocaleEnabled( bool enabled );
    
protected:
    QLocale mCurrentLocale;
    QSet<QString> mTranslationsMasks;
    QSet<QString> mForbiddenTranslationsMasks;
    QStringList mTranslationsPaths;
    QStringList mSystemTranslationsPaths;
    bool mFakeCLocaleEnabled;
    QSet<QString> mAvailableLocales;
    QHash<QLocale, QTranslatorList> mTranslators;
    
    bool addTranslator( const QString& filePath, const QLocale& locale );
    void clearTranslators();
};

inline uint qHash( const QLocale& locale )
{
    return qHash( locale.name() );
}

#endif // TRANSLATIONMANAGER_H
