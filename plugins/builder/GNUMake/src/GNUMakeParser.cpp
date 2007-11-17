#include "GNUMakeParser.h"

#include <QTextCodec>

GNUMakeParser::GNUMakeParser()
{
	Pattern ps[] = 
	{
		{
			QRegExp("^make: \\*\\*\\* (No rule to make target `.+', needed by `.+')\\.  Stop\\.$"), //reg exp
			"No rule for make target", //desctiption
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%1", //text
			"%0" //full text
		},
        {
            QRegExp("make\\[\\d\\]: Entering directory\\s`([^\\n]*)'"), //reg exp
            "Entering dirrectory", //desctiption
            "", //file name
            "0", //column
            "0", //row
            pConsoleManager::stCompiling, //type
            "make: Building %1", //text
            "%0" //full text
        },
		{QRegExp(), "", "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
	};
	for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
		patterns.append (ps[i]);
	mName = PLUGIN_NAME;

}

GNUMakeParser::~GNUMakeParser()
{
}

bool GNUMakeParser::processParsing( const QByteArray& arr )
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

QString GNUMakeParser::name() const
{ return PLUGIN_NAME; }

QString GNUMakeParser::replaceWithMatch(QRegExp rex, QString s)
{
	int i = 0;
	while ( (i = s.indexOf("%")) != -1)
		s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
	return s;
}
