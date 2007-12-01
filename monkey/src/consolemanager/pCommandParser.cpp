#include "pCommandParser.h"

#define PARSERS_DEBUG 1

pCommandParser::~pCommandParser()
{
}

#if PARSERS_DEBUG
 #include <QDebug>
 #include <QTime>
#endif
				 
bool pCommandParser::processParsing(QString& buf)
{
#if PARSERS_DEBUG
	static int allTime;
	static int total;
#endif
    foreach ( Pattern p, patterns)
	{
#if PARSERS_DEBUG
		QTime time1;
		time1.start();
#endif
		int pos = p.regExp.indexIn(buf);
#if PARSERS_DEBUG
		allTime += time1.elapsed ();
		qDebug () << "All time" <<allTime;
		qDebug () << "Total count" <<total++;
		qDebug () << "parser " << name();
		qDebug () << "parsing  " << buf << "with" << p.regExp.pattern();
#endif
        if (pos != -1)
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
            return true;
        }
	}
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
	{
		QString cap = rex.cap(QString(s[i+1]).toInt());
		if (cap.startsWith ("\n"))
			cap.remove (0,1);
        s.replace (i,2,cap);
	}
    return s;
}
