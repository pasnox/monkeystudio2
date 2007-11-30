#include "pCommandParser.h"

pCommandParser::~pCommandParser()
{
}

#include <QDebug>
#include <QTime>
				 
bool pCommandParser::processParsing(QString& buf)
{
	static int allTime;
	static int total;
    foreach ( Pattern p, patterns)
	{
		QTime time1;
		time1.start();
		int i = p.regExp.indexIn(buf);
		allTime += time1.elapsed ();
		qDebug () << "All time" <<allTime;
		qDebug () << "Total count" <<total++;
		qDebug () << "parser " << name();
		qDebug () << "parsing  " << buf;
        if (i != -1)
        {
            pConsoleManager::Step m;
            m.mFileName = replaceWithMatch(p.regExp,p.FileName);
            m.mPosition = QPoint( replaceWithMatch(p.regExp,p.row).toInt(), replaceWithMatch(p.regExp,p.col).toInt());
            m.mType = p.Type;
            m.mText = replaceWithMatch(p.regExp,p.Text);
            m.mFullText = replaceWithMatch(p.regExp,p.FullText);
            // emit signal
            emit newStepAvailable( m );
            buf.remove (0, p.regExp.pos()+p.regExp.cap().size());
			qDebug () << "returning true  " << name();
            return true;
        }
	}
	qDebug () << "returning false  " << name();
    return false;
}

QString pCommandParser::name() const
{
    return mName;
}

QString pCommandParser::replaceWithMatch(QRegExp& rex, QString s)
{
    int i = 0;
    while ( (i = s.indexOf("%")) != -1)
        s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
    return s;
}
