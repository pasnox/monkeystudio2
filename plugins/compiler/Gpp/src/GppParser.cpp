#include "GppParser.h"

#include <QTextCodec>

GppParser::GppParser()
{
    Pattern ps[] = 
    {
        {
            QRegExp("^(.*/)?([^:]+):(\\d+):((\\d+):)?\\serror:\\s(.+)$"), //reg exp
            "Error in the file/line", //desctiption
            "%2", //file name
            "%5", //column
            "%3", //row
            pConsoleManager::stError, //type
            "%2:%3: %6", //text
            "%0", //full text
        },
        {
            QRegExp("^(.*/)?([^:]+):(\\d+):((\\d+):)?\\swarning:\\s(.+)$"), //reg exp
            "Warning in the file/line", //desctiption
            "%2", //file name
            "%5", //column
            "%3", //row
            pConsoleManager::stWarning, //type
            "%2:%3: %6", //text
            "%0" //full text
        },
        {
            QRegExp("^g\\+\\+\\s+\\-c.+([^\\s]+\\.cpp)"), //reg exp
            "Building file", //desctiption
            "%1", //file name
            "0", //column
            "0", //row
            pConsoleManager::stCompiling, //type
            "Compiling %1...", //text
            "%0" //full text
        },
        {
            QRegExp("^g\\+\\+.+\\-o\\s+([^\\s]+).+"), //reg exp
            "Linking file", //desctiption
            "0", //file name
            "0", //column
            "0", //row
            pConsoleManager::stLinking, //type
            "Linking %1...", //text
            "%0" //full text
        },
        {
            QRegExp("^.*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto.+)$"), //reg exp
            "Undedined reference", //desctiption
            "", //file name
            "0", //column
            "0", //row
            pConsoleManager::stError, //type
            "%1%2", //text
            "%0" //full text
        },
        {QRegExp(), "", "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
    };
    for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
        patterns.append (ps[i]);
}

GppParser::~GppParser()
{
}

bool GppParser::processParsing(const QByteArray& arr)
{
    QStringList l = QTextCodec::codecForLocale()->toUnicode( arr ).split( '\n' );
    foreach (QString s, l)
    {
        foreach ( Pattern p, patterns)
        {
            if (p.regExp.indexIn(s) != -1)
            {
                pConsoleManager::Step m;
                m.mFileName = replaceWithMatch(p.regExp,p.FileName);
                m.mPosition = QPoint( replaceWithMatch(p.regExp,p.col).toInt(), replaceWithMatch(p.regExp,p.row).toInt());
                m.mType = p.Type;
                m.mText = replaceWithMatch(p.regExp,p.Text);
                m.mFullText = replaceWithMatch(p.regExp,p.FullText);
                // emit signal
                emit newStepAvailable( m );
                return true;
            }
            
        }
    }
return false;
}

QString GppParser::name() const
{
    return PLUGIN_NAME;
}

QString GppParser::replaceWithMatch(QRegExp rex, QString s)
{
    int i = 0;
    while ( (i = s.indexOf("%")) != -1)
        s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
    return s;
}
