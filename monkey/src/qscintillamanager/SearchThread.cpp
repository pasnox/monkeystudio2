#include "SearchThread.h"
#include <QFile>
#include <QTextStream>

#include <QDebug>

SearchThread::SearchThread(const QStringList &_files, const QString &_text, bool _isWhole, bool _isMatch, bool _isReg, QObject* parent)
	: QThread(parent)/*, mTerm(false)*/
{
	files = _files;
	text = _text;
	isWhole = _isWhole;
	isMatch = _isMatch;
	isReg = _isReg;
	qWarning () << "Created thread with files list " << files;
	qWarning () << "Searching " << text;
}

SearchThread::~SearchThread()
{
}

void SearchThread::run()
{
	qWarning () << "Thread started";
	QStringList::iterator it = files.begin();
	int files_count = 0;
	while ( it != files.end() )
	{
		emit changeProgress(++files_count);
		QFile file(*it);
		qWarning () << "Processing file " << *it;
		if (file.open(QIODevice::ReadOnly)) 
		{
			QString line;
			QTextStream in(&file);
			int i = 0;
			while (!in.atEnd() /*&& !isTermEnabled()*/) 
			{
				++i;
				line = in.readLine();
				qWarning () << "Readed line " << line;
				
				bool ifContains = false;
				if (isReg)
					ifContains = line.contains(QRegExp(text));
				else
				{
					if (!isWhole)
					{
						Qt::CaseSensitivity cs = isMatch ? Qt::CaseSensitive : Qt::CaseInsensitive;
						ifContains = line.contains(text, cs);
					}
					else
						ifContains = line.contains(QRegExp("\\b"+text+"\\b"));
				}
				if (ifContains) 
				{
					pConsoleManager::Step step;
					step.mFileName = file.fileName();
					step.mPosition = QPoint (0,i);
					step.mText = line.simplified();
					emit appendSearchResult (step);
				}
			}
		}
		if (isTermEnabled())
		{
			quit();
			break;
		}
		++it;
	}
	qWarning () << "Thread finished";
}
