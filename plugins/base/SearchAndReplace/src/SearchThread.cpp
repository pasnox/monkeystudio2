#include "SearchThread.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QDir>

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

SearchThread::SearchThread(Mode _mode, const QString &_dir, QString &_mask, const QString &_search, const QString& _replace, bool _caseSensetive, bool _isReg, QObject* parent)
    : QThread(parent), mTerm(false)
{
	mMode = _mode;
    mDir = _dir;
    mMask = _mask;
    mSearch = _search;
    mReplace = _replace;
    mIsReg = _isReg;
	mCaseSensetive = _caseSensetive;
}

SearchThread::~SearchThread()
{
}

void SearchThread::run()
{
	setPriority (QThread::LowestPriority);
	DirWalkIterator dirWalker (mDir);
    int files_count = 0;
    QString fileName = dirWalker.next();
	
	/* Prepare masks list */
	QStringList masks = mMask.split (' ');
	QList <QRegExp> maskRexps;
	foreach (QString m, masks)
		maskRexps << QRegExp (m.trimmed (), Qt::CaseInsensitive, QRegExp::Wildcard);
	
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
        emit changeProgress(++files_count);
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
}

bool SearchThread::isBinary (QFile& file)
{
	char data [1024];
	int count = file.read (data, 1024);
	while (count--)
		if (data[count] == '\0')
			return true;
	return false;
}

void SearchThread::search (QFile& file)
{
	if (!isBinary (file)) // Currently we don't support binary files
	{
		file.seek (0);
		QString line;
		QTextStream in(&file);
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
				step.mode = SearchAndReplace::SEARCH_DIRRECTORY;
				step.fileName = file.fileName();
				step.position = QPoint (0,i);
				step.text = QString("%1[%2]: %3").arg (QFileInfo(file.fileName()).fileName()).arg(i).arg(line.simplified());
				step.fullText= file.fileName();
				emit appendSearchResult (step);
			}
		}
	} //if not binary
}

void SearchThread::replace (QFile& file)
{
	if (!isBinary (file)) // Currently we not supporting binary files
	{
		file.seek (0);
		QString line;
		QTextStream in(&file);
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
				step.mode = SearchAndReplace::REPLACE_DIRRECTORY;
				step.fileName = file.fileName();
				step.position = QPoint (0,i);
				step.text = QString("%1[%2]: %3").arg (QFileInfo(file.fileName()).fileName()).arg(i).arg(line.simplified());
				step.fullText= file.fileName();
				step.searchText = mSearch;
				step.isRegExp = mIsReg;
				step.isCaseSensetive = mCaseSensetive;
				step.replaceText = mReplace;
				emit appendSearchResult (step);
			}
		}
	} //if not binary
}
