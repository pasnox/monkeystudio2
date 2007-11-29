#include "pCommandParser.h"

pCommandParser::~pCommandParser()
{
}

bool pCommandParser::processParsing(QString* buf)
{
    foreach ( Pattern p, patterns)
        if (p.regExp.indexIn(*buf) != -1)
        {
            pConsoleManager::Step m;
            m.mFileName = replaceWithMatch(p.regExp,p.FileName);
            m.mPosition = QPoint( replaceWithMatch(p.regExp,p.row).toInt(), replaceWithMatch(p.regExp,p.col).toInt());
            m.mType = p.Type;
            m.mText = replaceWithMatch(p.regExp,p.Text);
            m.mFullText = replaceWithMatch(p.regExp,p.FullText);
            // emit signal
            emit newStepAvailable( m );
            buf->remove (0, p.regExp.pos()+p.regExp.cap().size());
            return true;
        }
    return false;
}

QString pCommandParser::name() const
{
    return mName;
}

QString pCommandParser::replaceWithMatch(QRegExp rex, QString s)
{
    int i = 0;
    while ( (i = s.indexOf("%")) != -1)
        s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
    return s;
}
