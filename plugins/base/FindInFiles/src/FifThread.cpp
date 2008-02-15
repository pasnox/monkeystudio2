#include "FifThread.h"
#include <QFile>
#include <QTextStream>

FifThread::FifThread(const QStringList &_files, const QString &_text, bool _isWhole, bool _isMatch, bool _isReg, QObject* parent)
	: QThread(parent), mTerm(false)
{
	files = _files;
	text = _text;
	isWhole = _isWhole;
	isMatch = _isMatch;
	isReg = _isReg;
}

FifThread::~FifThread()
{
}

void FifThread::run()
{
	QStringList::iterator it = files.begin();
	int files_count = 0;
	while ( it != files.end() )
	{
		emit sig_changeProgress(++files_count);
		QFile file(*it);
		if (file.open(QIODevice::ReadOnly)) {
			QString line;
			QTextStream in(&file);
			int i = 0;
			while (!in.atEnd() && !isTermEnabled()) {
				++i;
				line = in.readLine();
				
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
				if (ifContains) {
					emit sig_updateResult(i, file.fileName(), line.simplified());
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
}
