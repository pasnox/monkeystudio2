#include "pCommandParser.h"

#define PARSERS_DEBUG 0

pCommandParser::~pCommandParser()
{
}

#if PARSERS_DEBUG
 #include <QDebug>
 #include <QTime>
#endif
				 
int pCommandParser::processParsing(QString* buf)
{
#if PARSERS_DEBUG
	static int allTime;
	static int total;
	QTime time1;
	time1.start();
#endif
    foreach ( Pattern p, patterns)
	{
		int pos = p.regExp.indexIn(*buf);
#if PARSERS_DEBUG
		qDebug () << "parser " << name();
		qDebug () << "parsing  " << *buf << "with" << p.regExp.pattern();
#endif
        if (pos != -1)
        {
            pConsoleManager::Step m;
            m.mFileName = replaceWithMatch(p.regExp,p.FileName);
            m.mPosition = QPoint( replaceWithMatch(p.regExp,p.col).toInt(), replaceWithMatch(p.regExp,p.row).toInt());
            m.mType = p.Type;
            m.mText = replaceWithMatch(p.regExp,p.Text);
            m.mFullText = replaceWithMatch(p.regExp,p.FullText);
            // emit signal
            emit newStepAvailable( m );
#if PARSERS_DEBUG
			qDebug () << "Capture :" << p.regExp.cap();
			qDebug () << "Returning " << p.regExp.cap().count ('\n');
#endif
            return p.regExp.cap().count ('\n');
        }
#if PARSERS_DEBUG
			qDebug () << "Not matching";
#endif
	}
#if PARSERS_DEBUG
	allTime += time1.elapsed ();
	qDebug () << "All time" <<allTime;
	qDebug () << "Total count" <<total++;
	
	qDebug () << "Returning false";
#endif
    return 0;
}

QString pCommandParser::name() const
{
    return mName;
}

QString pCommandParser::replaceWithMatch(QRegExp& rex, QString s)
{
	int pos = 0; 
    int i = 0;
    while ( (i = s.indexOf("%", pos)) != -1)
	{
		pos += i;
		if ( ! s[i+1].isDigit () )
			continue;
		QString cap = rex.cap(s[i+1].digitValue ());
		if (cap.endsWith ("\n"))
			cap.chop(1);
        s.replace (i,2,cap);
		pos += cap.size ();
	}
    return s;
}
