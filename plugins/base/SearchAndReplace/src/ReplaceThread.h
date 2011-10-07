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
#ifndef REPLACETHREAD_H
#define REPLACETHREAD_H

#include <QThread>
#include <QMutex>

#include "SearchAndReplace.h"
#include "SearchResultsModel.h"

class ReplaceThread : public QThread
{
    Q_OBJECT
    
public:
    ReplaceThread( QObject* parent = 0 );
    virtual ~ReplaceThread();
    
    void replace( const SearchAndReplace::Properties& properties, const QHash<QString, SearchResultsModel::ResultList>& results );
    void stop();

protected:
    static int mMaxTime;
    SearchAndReplace::Properties mProperties;
    QHash<QString, SearchResultsModel::ResultList> mResults;
    QMutex mMutex;
    bool mReset;
    bool mExit;
    
    void saveContent( const QString& fileName, const QString& content, const QString& codec );
    QString fileContent( const QString& fileName ) const;
    void replace( const QString& fileName, QString content );
    void run();

signals:
    void resultsHandled( const QString& fileName, const SearchResultsModel::ResultList& results );
    void openedFileHandled( const QString& fileName, const QString& content, const QString& codec );
    void error( const QString& error );
};

#endif // REPLACETHREAD_H
