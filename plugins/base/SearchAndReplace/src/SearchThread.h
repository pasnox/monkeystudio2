/****************************************************************************
**
**      Created using Monkey Studio v1.8.1.0
** Authors   : Andrei KOPATS aka hlamer, <hlamer at tut by>
** Project   : Monkey Studio Base Plugins
** FileName  : SearchThread.h
** Date      : 
** License   : GPL
** Comment   : 
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
**
****************************************************************************/
/*!
    \file SearchThread.h
    \date 
    \author Andrei KOPATS
    \brief Header file for SearchThread class
*/

#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include "SearchAndReplace.h"

#include <QThread>
#include <QStringList>
#include <QMutex>
#include <QTimer>

class QFile;

/*! 
    Search thread used for search occurences in the dirrectory in SEARCH_DIRRECTORY and REPLACE_DIRRECTORY modes.
    Results stored in the buffer, which will be readed, when thread emits readPleaseResults () signal.
    Signal emited by timer.
*/
class SearchThread : public QThread
{
    Q_OBJECT
public:
    enum Mode
    {
        SEARCH,
        REPLACE
    };
    
    SearchThread(Mode mode, const QString &dir, QString &mask, const QString &text, const QString& replace, bool caseSensitive, bool regExp, const QString& codec, QObject* parent);
    ~SearchThread();
    void run();
    void setTermEnabled(bool t) {mTerm = t;};
    bool isTermEnabled() {return mTerm;};
    
    void lockResultsAccessMutex ();
    void unlockResultsAccessMutex ();
    // NOTE it's nessesary to lock mutex before use this methods
    QList<SearchAndReplace::Occurence> newFoundOccurences();
    void clearNewFoundOccurences();
    int processedFilesCount();
    int foundOccurencesCount();
    
private:
    QString mDir;
    QString mMask;
    QString mSearch;
    QString mReplace;
    bool mIsWhole;
    bool mIsMatch;
    bool mIsReg;
    bool mCaseSensetive;
    QString mCodec;
    
    Mode mMode;
    
    bool mTerm;
    
    // results
    QMutex mResultsAccessMutex;
    int mProcessedFilesCount;
    int mOccurencesFound;
    QList<SearchAndReplace::Occurence> mNewFoundOccurences;
    
    QTimer mReadPleaseResultsTimer;
    
    bool isBinary (QFile& file);
    
    void search (QFile& file);
    void replace (QFile& file);
    
signals:
    void readPleaseResults ();
};

#endif //SEARCHTHREAD_H
