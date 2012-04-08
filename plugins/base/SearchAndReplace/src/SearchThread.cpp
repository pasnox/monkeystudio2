#include "SearchThread.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QDir>
#include <QTextCodec>

/*! Iterator for get list of all files in the some dirrectory, using minimum memory/time.
    Every time, when called, will read from file system and return next file name
*/
class DirWalkIterator
{
protected:
    QList<QStringList> mDirStack;
    QStringList mFilesList;
    QStringList mDirList;
    QString currentDir;
    
    void enterDirrectory (QString dir)
    {
        QStringList dirs = QDir(dir).entryList (QDir::Dirs | QDir::NoDotAndDotDot);
        currentDir = dir;
        for (int i = 0; i < dirs.size (); i++)
            dirs[i] = currentDir +'/'+ dirs[i];
        mDirStack.prepend (dirs);
        mFilesList = QDir(dir).entryList (QDir::Files);
    }
    
    bool findDirWithFiles ()
    {
        while (!mDirStack.isEmpty() && mFilesList.isEmpty())
        {
            if (!mDirStack.first().isEmpty()) // 1 level down
            {
                enterDirrectory (mDirStack.first().takeFirst());
            }
            else // one level up. Will enter next dir on next WHILE iteration
                mDirStack.takeFirst ();
        }
        return !mFilesList.isEmpty();
    }

public:
    DirWalkIterator (QString dir)
    {
        enterDirrectory (dir);
    }
    
    QString next()
    {
        // Find dirrectory with files, return "", if finished search
        if (!findDirWithFiles ())
        {
            return QString::null;
        }
        
        return currentDir + '/' + mFilesList.takeFirst();
    }
};

SearchThread::SearchThread(Mode _mode, const QString &_dir, QString &_mask, const QString &_search, const QString& _replace, bool _caseSensetive, bool _isReg, const QString& _codec, QObject* parent)
    : QThread(parent), mTerm(false)
{
    mMode = _mode;
    mDir = _dir;
    mMask = _mask;
    mSearch = _search;
    mReplace = _replace;
    mIsReg = _isReg;
    mCaseSensetive = _caseSensetive;
    mCodec = _codec;
    connect (&mReadPleaseResultsTimer, SIGNAL (timeout()), this, SIGNAL (readPleaseResults()));
}

SearchThread::~SearchThread()
{
}

void SearchThread::run()
{
    setPriority (QThread::LowestPriority);
    DirWalkIterator dirWalker (mDir);
    mProcessedFilesCount = 0;
    mOccurencesFound = 0;
    QString fileName = dirWalker.next();
    
    /* Prepare masks list */
    QStringList masks = mMask.split (' ');
    QList <QRegExp> maskRexps;
    foreach (QString m, masks)
        maskRexps << QRegExp (m.trimmed (), Qt::CaseInsensitive, QRegExp::Wildcard);
    
    mReadPleaseResultsTimer.start(200);
    
    while (!fileName.isNull())
    {
        if (!mMask.isEmpty())
        {   // Check file for mask
            QString name = QFileInfo (fileName).fileName(); // Just name, no path
            bool matching = false;
            foreach (QRegExp maskRe, maskRexps)
            {
                if (maskRe.exactMatch (name))
                {
                    matching = true;
                    break;
                }
            }
            if (!matching)
            {
                fileName = dirWalker.next();            
                continue;  // Ignore this file, search in the next
            }
        }
        lockResultsAccessMutex ();
        mProcessedFilesCount++;
        unlockResultsAccessMutex ();
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) 
        {
            if (mMode == SEARCH)
            {
                search (file);
            }
            else
            {
                replace (file);
            }
        } //if open
        if (mTerm)
        {
            quit();
            break;
        }
        fileName = dirWalker.next();
    } //while has file
    
    mReadPleaseResultsTimer.stop();
    
    lockResultsAccessMutex();
    bool empty = mNewFoundOccurences.isEmpty();
    unlockResultsAccessMutex();
    emit readPleaseResults ();
    while (! empty) // wait until all data will be readed
    {
        msleep (20); // It's better to use semaphore, but lazy to write good code
        lockResultsAccessMutex();
        empty = mNewFoundOccurences.isEmpty();
        unlockResultsAccessMutex();
    }
}

void SearchThread::lockResultsAccessMutex ()
{
    mResultsAccessMutex.lock();
}

void SearchThread::unlockResultsAccessMutex ()
{
    mResultsAccessMutex.unlock();
}

QList<SearchAndReplace::Occurence> SearchThread::newFoundOccurences()
{
    return mNewFoundOccurences;
}

void SearchThread::clearNewFoundOccurences()
{
    mNewFoundOccurences.clear();
}

int SearchThread::processedFilesCount()
{
    return mProcessedFilesCount;
}

int SearchThread::foundOccurencesCount()
{
    return mOccurencesFound;
}

/*! 
    Check, if file is binary. Now we not support binary files, and it will be skipped
    Heuristics: if first 1k of file contains '\\0' - file is binary
    NOTE: procedure moving current pos in the file
*/
bool SearchThread::isBinary (QFile& file)
{
    char data [1024];
    int count = file.read (data, 1024);
    while (count--)
        if (data[count] == '\0')
            return true;
    return false;
}

/*! 
    Process file in SEARCH_DIRECTORY mode (search for occurences, store it to buffer
*/
void SearchThread::search (QFile& file)
{
    if (!isBinary (file)) // Currently we don't support binary files
    {
        file.seek (0);
        QString line;
        QTextStream in(&file);
        in.setCodec( QTextCodec::codecForName( mCodec.toLocal8Bit() ) );
        
        int i = 0;
        Qt::CaseSensitivity cs = mCaseSensetive ? Qt::CaseSensitive : Qt::CaseInsensitive;
        QRegExp rex (mSearch, cs);
        while (!in.atEnd() && !mTerm) 
        {
            ++i;
            line = in.readLine();
            
            bool ifContains = false;
            if (mIsReg)
                ifContains = line.contains(rex);
            else
            {
                ifContains = line.contains(mSearch, cs);
            }
            if (ifContains) 
            {
                SearchAndReplace::Occurence step;
                step.mode = SearchAndReplace::SEARCH_DIRECTORY;
                step.fileName = file.fileName();
                step.codec = mCodec;
                step.position = QPoint (0,i);
                step.text = QString("%1[%2]: %3").arg (QFileInfo(file.fileName()).fileName()).arg(i).arg(line.simplified());
                step.fullText= file.fileName();
                lockResultsAccessMutex ();
                mNewFoundOccurences.append (step);
                mOccurencesFound ++;
                unlockResultsAccessMutex ();
            }
        }
    } //if not binary
}

/*! 
    Process file in REPLACE_DIRECTORY mode (search for occurences, store it to buffer
*/
void SearchThread::replace (QFile& file)
{
    if (!isBinary (file)) // Currently we not supporting binary files
    {
        file.seek (0);
        QString line;
        QTextStream in(&file);
        in.setCodec( QTextCodec::codecForName( mCodec.toLocal8Bit() ) );
        
        int i = 0;
        Qt::CaseSensitivity cs = mCaseSensetive ? Qt::CaseSensitive : Qt::CaseInsensitive;
        QRegExp rex (mSearch, cs);
        while (!in.atEnd() && !mTerm) 
        {
            ++i;
            line = in.readLine();
            
            bool ifContains = false;
            if (mIsReg)
                ifContains = line.contains(rex);
            else
            {
                ifContains = line.contains(mSearch, cs);
            }
            if (ifContains) 
            {
                SearchAndReplace::Occurence step;
                step.mode = SearchAndReplace::REPLACE_DIRECTORY;
                step.fileName = file.fileName();
                step.position = QPoint (0,i);
                step.codec = mCodec;
                step.text = QString("%1[%2]: %3").arg (QFileInfo(file.fileName()).fileName()).arg(i).arg(line.simplified());
                step.fullText= file.fileName();
                step.searchText = mSearch;
                step.isRegExp = mIsReg;
                step.isCaseSensetive = mCaseSensetive;
                
                lockResultsAccessMutex ();
                mNewFoundOccurences.append (step);
                mOccurencesFound ++;
                unlockResultsAccessMutex ();
            }
        }
    } //if not binary
}
