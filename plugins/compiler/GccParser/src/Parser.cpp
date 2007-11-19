#include "Parser.h"

#include <QTextCodec>

Parser::Parser()
{
	mName = PLUGIN_NAME;
	Pattern ps[] = 
	{
		{
			//Error in the file/line
			QRegExp("^((.*/)?([^:]+)):(\\d+):(\\d+:)?\\serror:\\s(.+)$"), //reg exp
			"%1", //file name
			"%5", //column
			"%6", //row
			pConsoleManager::stError, //type
			"%3:%4: %6", //text
			"%0", //full text
		},
		{
			//Warning in the file/line
			QRegExp("^((.*/)?([^:]+)):(\\d+):(\\d+:)?\\swarning:\\s(.+)$"), //reg exp
			"%1", //file name
			"%5", //column
			"%4", //row
			pConsoleManager::stWarning, //type
			"%3:%4: %6", //text
			"%0" //full text
		},
		{
			//Building file
			QRegExp("^g\\+\\+\\s+\\-c.+([^\\s]+\\.cpp)$"), //reg exp
			"%1", //file name
			"0", //column
			"0", //row
			pConsoleManager::stCompiling, //type
			"Compiling %1...", //text
			"%0" //full text
		},
		{
			//Linking file
			QRegExp("^g\\+\\+.+\\-o\\s+([^\\s]+).+"), //reg exp
			"0", //file name
			"0", //column
			"0", //row
			pConsoleManager::stLinking, //type
			"Linking %1...", //text
			"%0" //full text
		},
		{
			//Undedined reference in the function
			QRegExp("^.*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto.+)$"), //reg exp
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%1%2", //text
			"%0" //full text
		},
		{
			//Undedined reference in the module
			QRegExp("^(\\w+)\\.o:\\([^\\)]+\\): undefined reference to `(\\w+)'$"), //reg exp
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%0", //text
			"%0" //full text
		},
		{
			//LD has no permissions for write file.  Not tested with unix output.
			QRegExp("^.*(ld(\\.exe)?)(: cannot open output file [^:]+: Permission denied)$"), //reg exp
			"", //file name
			"0", //column
			"0", //row
			pConsoleManager::stError, //type
			"%1%3", //text
			"%0" //full text
		},
		{QRegExp(), "", "", "", pConsoleManager::stUnknown,"",""} //this item must be last
	};
	for ( int i = 0; !ps[i].regExp.isEmpty(); i++)
		patterns.append (ps[i]);
}

Parser::~Parser()
{
}

bool Parser::processParsing(const QByteArray& arr)
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
				m.mPosition = QPoint( replaceWithMatch(p.regExp,p.row).toInt(), replaceWithMatch(p.regExp,p.col).toInt());
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

QString Parser::name() const
{
	return PLUGIN_NAME;
}

QString Parser::replaceWithMatch(QRegExp rex, QString s)
{
	int i = 0;
	while ( (i = s.indexOf("%")) != -1)
		s.replace (i,2,rex.cap(QString(s[i+1]).toInt()));
	return s;
}
